/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/FastFit.h>
#include <Herakles/PulseShape.h>

ClassImp(TD::FastFit)

namespace TD
{
  FastFit :: FastFit () {}

  FastFit :: ~FastFit () {}

  EL::StatusCode FastFit :: initialize ()
  {
    // label buffer
    char buffer[5];

    // new branch added to raw data
    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("fastfit", &fastfit, "fastfit[2][48]/D");
    ntuple->tree()->Branch ("fastratio", &fastratio, "fastratio[2][48]/D");

    // initialize with inverted min-max
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    fastfit_min[gain][pmt] = 4096;
	    fastfit_max[gain][pmt] = -4096;
	    fastratio_min[gain][pmt] = 4096;
	    fastratio_max[gain][pmt] = -4096;
	  }
      }

    // histogram contains min and max in separate bins
    m_fastfit_lo_min = new TH1D ("FastFit_lo_min", "FF Low", 48, 0, 48);
    m_fastfit_lo_max = new TH1D ("FastFit_lo_max", "FF Low", 48, 0, 48);
    m_fastfit_hi_min = new TH1D ("FastFit_hi_min", "FF High", 48, 0, 48);
    m_fastfit_hi_max = new TH1D ("FastFit_hi_max", "FF High", 48, 0, 48);

    m_fastfit_lo_min = new TH1D ("FastRatio_lo_min", "FF Low", 48, 0, 48);
    m_fastfit_lo_max = new TH1D ("FastRatio_lo_max", "FF Low", 48, 0, 48);
    m_fastfit_hi_min = new TH1D ("FastRatio_hi_min", "FF High", 48, 0, 48);
    m_fastfit_hi_max = new TH1D ("FastRatio_hi_max", "FF High", 48, 0, 48);

    m_fastfit_lo_min->SetYTitle ("Min");
    m_fastfit_lo_max->SetYTitle ("Max");
    m_fastfit_hi_min->SetYTitle ("Min");
    m_fastfit_hi_max->SetYTitle ("Max");

    m_fastratio_lo_min->SetYTitle ("Min");
    m_fastratio_lo_max->SetYTitle ("Max");
    m_fastratio_hi_min->SetYTitle ("Min");
    m_fastratio_hi_max->SetYTitle ("Max");

    for (pmt=1; pmt<49; pmt++) // notice number convention
      {
	sprintf (buffer, "PMT%d", pmt);

	m_fastfit_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_fastfit_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_fastfit_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_fastfit_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

	m_fastratio_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_fastratio_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_fastratio_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_fastratio_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);
      }
    
    // add the histograms to EL output
    wk()->addOutput (m_fastfit_lo_min);
    wk()->addOutput (m_fastfit_lo_max);
    wk()->addOutput (m_fastfit_hi_min);
    wk()->addOutput (m_fastfit_hi_max);

    wk()->addOutput (m_fastratio_lo_min);
    wk()->addOutput (m_fastratio_lo_max);
    wk()->addOutput (m_fastratio_hi_min);
    wk()->addOutput (m_fastratio_hi_max);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode FastFit :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode FastFit :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("charge",     &charge);
    m_tree->SetBranchAddress ("ped_hi",     &ped_hi);
    m_tree->SetBranchAddress ("ped_lo",     &ped_lo);
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

	    // default value is an error state
	    fastfit  [gain][pmt] = 0;

	    if (gain) ped = ped_hi[pmt];
	    else ped = ped_lo[pmt];

	    // fast-fit: sum (sample - pedestal)
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0)
		  {
		    fastfit[gain][pmt] += sval - ped;
		  }
	      } // end sample
	    fastratio[gain][pmt] = fastfit[gain][pmt] / charge;

            // check if value exceeds range
	    if (fastfit[gain][pmt] < fastfit_min[gain][pmt])
	      {
		fastfit_min[gain][pmt] = fastfit[gain][pmt];
	      }
	    if (fastfit[gain][pmt] > fastfit_max[gain][pmt])
	      {
		fastfit_max[gain][pmt] = fastfit[gain][pmt];
	      }

	    if (fastratio[gain][pmt] < fastratio_min[gain][pmt])
	      {
		fastratio_min[gain][pmt] = fastratio[gain][pmt];
	      }
	    if (fastratio[gain][pmt] > fastratio_max[gain][pmt])
	      {
		fastratio_max[gain][pmt] = fastratio[gain][pmt];
	      }
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode FastFit :: finalize ()
  {
    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

            // error state: min is still greater than max
	    if (fastfit_min[gain][pmt] > fastfit_max[gain][pmt])
	      {
		fastfit_min[gain][pmt] = -1;
		fastfit_max[gain][pmt] = -1;
	      }
	    if (fastratio_min[gain][pmt] > fastratio_max[gain][pmt])
	      {
		fastratio_min[gain][pmt] = -1;
		fastratio_max[gain][pmt] = -1;
	      }

            // set min-max bin contents in histograms
            if (gain)
	      {
		m_fastfit_hi_min->SetBinContent (pmt+1,
						 fastfit_min[gain][pmt]);
		m_fastfit_hi_min->SetBinError (pmt+1, 0);
		m_fastfit_hi_max->SetBinContent (pmt+1,
						 fastfit_max[gain][pmt]);
		m_fastfit_hi_max->SetBinError (pmt+1, 0);

		m_fastratio_hi_min->SetBinContent (pmt+1,
						   fastratio_min[gain][pmt]);
		m_fastratio_hi_min->SetBinError (pmt+1, 0);
		m_fastratio_hi_max->SetBinContent (pmt+1,
						   fastratio_max[gain][pmt]);
		m_fastratio_hi_max->SetBinError (pmt+1, 0);
	      }

            else
	      {
		m_fastfit_lo_min->SetBinContent (pmt+1,
						 fastfit_min[gain][pmt]);
		m_fastfit_lo_min->SetBinError (pmt+1, 0);
		m_fastfit_lo_max->SetBinContent (pmt+1,
						 fastfit_max[gain][pmt]);
		m_fastfit_lo_max->SetBinError (pmt+1, 0);

		m_fastratio_lo_min->SetBinContent (pmt+1,
						   fastratio_min[gain][pmt]);
		m_fastratio_lo_min->SetBinError (pmt+1, 0);
		m_fastratio_lo_max->SetBinContent (pmt+1,
						   fastratio_max[gain][pmt]);
		m_fastratio_lo_max->SetBinError (pmt+1, 0);
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }
}
