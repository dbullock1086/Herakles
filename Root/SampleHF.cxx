/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/SampleHF.h>
#include <TMath.h>

ClassImp(TD::SampleHF)

namespace TD
{
  SampleHF :: SampleHF () {}

  SampleHF :: ~SampleHF () {}

  EL::StatusCode SampleHF :: initialize ()
  {
    // label buffer
    char buffer[5];

    // initialize with inverted min-max
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    hfmean_min[gain][pmt] = 4096;
	    hfmean_max[gain][pmt] = -1;
	    hfstd_min [gain][pmt] = 4096;
	    hfstd_max [gain][pmt] = -1;
	  } // end pmt
      } // end gain

    // new branch added to raw data
    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("hfmean", &hfmean, "hfmean[2][48]/D");
    ntuple->tree()->Branch ("hfstd",  &hfstd,  "hfstd[2][48]/D");

    // histogram contains min and max in separate bins
    m_hfmean_lo_min = new TH1D ("HFMean_lo_min", "Sample HF Mean Low",
				48, 0, 48);
    m_hfmean_lo_max = new TH1D ("HFMean_lo_max", "Sample HF Mean Low",
				48, 0, 48);
    m_hfmean_hi_min = new TH1D ("HFMean_hi_min", "Sample HF Mean High",
				48, 0, 48);
    m_hfmean_hi_max = new TH1D ("HFMean_hi_max", "Sample HF Mean High",
				48, 0, 48);

    m_hfstd_lo_min = new TH1D ("HFStd_lo_min", "Sample HF StdDev Low",
			       48, 0, 48);
    m_hfstd_lo_max = new TH1D ("HFStd_lo_max", "Sample HF StdDev Low",
			       48, 0, 48);
    m_hfstd_hi_min = new TH1D ("HFStd_hi_min", "Sample HF StdDev High",
			       48, 0, 48);
    m_hfstd_hi_max = new TH1D ("HFStd_hi_max", "Sample HF StdDev High",
			       48, 0, 48);

    m_hfmean_lo_min->SetYTitle ("Min");
    m_hfmean_lo_max->SetYTitle ("Max");
    m_hfmean_hi_min->SetYTitle ("Min");
    m_hfmean_hi_max->SetYTitle ("Max");

    m_hfstd_lo_min->SetYTitle ("Min");
    m_hfstd_lo_max->SetYTitle ("Max");
    m_hfstd_hi_min->SetYTitle ("Min");
    m_hfstd_hi_max->SetYTitle ("Max");

    for (pmt=1; pmt<49; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);

	m_hfmean_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfmean_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfmean_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfmean_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

	m_hfstd_lo_min ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfstd_lo_max ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfstd_hi_min ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfstd_hi_max ->GetXaxis()->SetBinLabel (pmt, buffer);
      }

    // add the histograms to EL output
    wk()->addOutput (m_hfmean_lo_min);
    wk()->addOutput (m_hfmean_lo_max);
    wk()->addOutput (m_hfmean_hi_min);
    wk()->addOutput (m_hfmean_hi_max);

    wk()->addOutput (m_hfstd_lo_min);
    wk()->addOutput (m_hfstd_lo_max);
    wk()->addOutput (m_hfstd_hi_min);
    wk()->addOutput (m_hfstd_hi_max);

    return EL::StatusCode::SUCCESS;
    }

  EL::StatusCode SampleHF :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleHF :: execute ()
  {
    // refresh the variable reference per algorithm
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
	    xval  = 0;
	    x2val = 0;
	    CRC   = 0;
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0)
		  {
		    xval  += sval;
		    x2val += sval * sval;
		  }
		else CRC++;
	      } // end sample

	    if (CRC == 128) mean = -1;
	    else            mean = xval / (128 - CRC);
	    if (CRC == 127) std = -1;
	    // (1/(n-1)) * sum(yi-muy)
	    else
	      {
		x2 = x2val       / (127 - CRC);
		xm = xval * mean / (127 - CRC);
		m2 = mean * mean / (127 - CRC);
		std = TMath::Sqrt(x2 - 2*xm + m2);
	      }
	    hfmean[gain][pmt] = mean;
	    hfstd [gain][pmt] = std;

            // check if value exceeds range
	    if (mean < hfmean_min[gain][pmt])
	      {
		hfmean_min[gain][pmt] = mean;
	      }
	    if (mean > hfmean_max[gain][pmt])
	      {
		hfmean_max[gain][pmt] = mean;
	      }
	    if (std < hfstd_min[gain][pmt])
	      {
		hfstd_min[gain][pmt] = std;
	      }
	    if (std > hfstd_max[gain][pmt])
	      {
		hfstd_max[gain][pmt] = std;
	      }
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleHF :: finalize ()
  {
    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

            // error state: min is still greater than max
	    if (hfmean_min[gain][pmt] > hfmean_max[gain][pmt])
	      {
		hfmean_min[gain][pmt] = -1;
		hfmean_max[gain][pmt] = -1;
	      }
	    if (hfstd_min[gain][pmt] > hfstd_max[gain][pmt])
	      {
		hfstd_min[gain][pmt] = -1;
		hfstd_max[gain][pmt] = -1;
	      }

	    // set min-max bin contents in histograms
	    if (gain)
	      {
		m_hfmean_hi_min->SetBinContent (pmt+1, hfmean_min[gain][pmt]);
		m_hfmean_hi_min->SetBinContent (pmt+1, 0);
		m_hfmean_hi_max->SetBinContent (pmt+1, hfmean_max[gain][pmt]);
		m_hfmean_hi_max->SetBinContent (pmt+1, 0);

		m_hfstd_hi_min->SetBinContent (pmt+1, hfstd_min[gain][pmt]);
		m_hfstd_hi_min->SetBinContent (pmt+1, 0);
		m_hfstd_hi_max->SetBinContent (pmt+1, hfstd_max[gain][pmt]);
		m_hfstd_hi_max->SetBinContent (pmt+1, 0);
	      }

	    else
	      {
		m_hfmean_lo_min->SetBinContent (pmt+1, hfmean_min[gain][pmt]);
		m_hfmean_lo_min->SetBinContent (pmt+1, 0);
		m_hfmean_lo_max->SetBinContent (pmt+1, hfmean_max[gain][pmt]);
		m_hfmean_lo_max->SetBinContent (pmt+1, 0);

		m_hfstd_lo_min->SetBinContent (pmt+1, hfstd_min[gain][pmt]);
		m_hfstd_lo_min->SetBinContent (pmt+1, 0);
		m_hfstd_lo_max->SetBinContent (pmt+1, hfstd_max[gain][pmt]);
		m_hfstd_lo_max->SetBinContent (pmt+1, 0);
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }
}
