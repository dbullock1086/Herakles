/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/PedRatio.h>
#include <TMath.h>

ClassImp(TD::PedRatio)

namespace TD
{
  PedRatio :: PedRatio () {}

  PedRatio :: ~PedRatio () {}

  EL::StatusCode PedRatio :: initialize ()
  {
    // label buffer
    char buffer[5];

    // new branch added to raw data
    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("pedratio", &pedratio, "pedratio[2][48][128]/D");

    // initialize with inverted min-max
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    pedratio_min[gain][pmt] = 4096;
	    pedratio_max[gain][pmt] = -1;
	  } // end pmt
      } // end gain

    // histogram contains min and max in separate bins
    m_pedratio_lo_min = new TH1D ("PedRatio_lo_min", "Pedestal Ratio Low",
    				  48, 0, 48);
    m_pedratio_lo_max = new TH1D ("PedRatio_lo_max", "Pedestal Ratio Low",
    				  48, 0, 48);
    m_pedratio_hi_min = new TH1D ("PedRatio_hi_min", "Pedestal Ratio High",
    				  48, 0, 48);
    m_pedratio_hi_max = new TH1D ("PedRatio_hi_max", "Pedestal Ratio High",
				  48, 0, 48);
    m_pedratio_lo_min->SetYTitle ("Min");
    m_pedratio_lo_max->SetYTitle ("Max");
    m_pedratio_hi_min->SetYTitle ("Min");
    m_pedratio_hi_max->SetYTitle ("Max");

    for (pmt=1; pmt<49; pmt++) // notice number convention
      {
    	sprintf (buffer, "PMT%d", pmt);
    	m_pedratio_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
    	m_pedratio_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
    	m_pedratio_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
    	m_pedratio_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);
      } // end pmt

    // add the histograms to EL output
    wk()->addOutput (m_pedratio_lo_min);
    wk()->addOutput (m_pedratio_lo_max);
    wk()->addOutput (m_pedratio_hi_min);
    wk()->addOutput (m_pedratio_hi_max);
    
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRatio :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRatio :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->SetBranchAddress ("ped_hi",     &ped_hi);
    m_tree->SetBranchAddress ("ped_lo",     &ped_lo);
    m_tree->GetEntry (wk()->treeEntry());

    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    if (gain) ped = ped_hi[pmt];
	    else      ped = ped_lo[pmt];
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0 && ped != 0)
		  {
		    // observed / expected
		    pedratio[gain][pmt][sample] = sval / ped;
		  }
		else pedratio[gain][pmt][sample] = -1;

		// check if value exceeds range
		if (pedratio[gain][pmt][sample] < pedratio_min[gain][pmt])
		  {
		    pedratio_min[gain][pmt] = pedratio[gain][pmt][sample];
		  }
		if (pedratio[gain][pmt][sample] > pedratio_max[gain][pmt])
		  {
		    pedratio_max[gain][pmt] = pedratio[gain][pmt][sample];
		  }
	      } // end sample
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRatio :: finalize ()
  {
    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

            // error state: min is still greater than max
	    if (pedratio_min[gain][pmt] > pedratio_max[gain][pmt])
	      {
		pedratio_min[gain][pmt] = -1;
		pedratio_max[gain][pmt] = -1;
	      }

            // set min-max bin contents in histograms
	    if (gain)
	      {
	    	m_pedratio_hi_min->SetBinContent (pmt + 1,
	    					  pedratio_min[gain][pmt]);
	    	m_pedratio_hi_min->SetBinError (pmt + 1, 0);
	    	m_pedratio_hi_max->SetBinContent (pmt + 1,
	    					  pedratio_max[gain][pmt]);
	    	m_pedratio_hi_max->SetBinError (pmt + 1, 0);
	      }
	    else
	      {
	    	m_pedratio_lo_min->SetBinContent (pmt + 1,
	    					  pedratio_min[gain][pmt]);
	    	m_pedratio_lo_min->SetBinError (pmt + 1, 0);
	    	m_pedratio_lo_max->SetBinContent (pmt + 1,
	    					  pedratio_max[gain][pmt]);
	    	m_pedratio_lo_max->SetBinError (pmt + 1, 0);
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }
}
