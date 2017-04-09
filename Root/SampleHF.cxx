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
    char buffer[5];

    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("hfmean", &hfmean, "hfmean[2][48]/D");
    ntuple->tree()->Branch ("hfstd",  &hfstd,  "hfstd[2][48]/D");

    m_hfmean_lo = new TH2D ("SampleHF_mean_lo", "SampleHF_mean_lo",
			    48, 0, 48, 2, 0, 2);
    m_hfmean_hi = new TH1D ("SampleHF_mean_hi", "SampleHF_mean_hi",
			    48, 0, 48, 2, 0, 2);
    m_hfstd_lo  = new TH1D ("SampleHF_std_lo", "SampleHF_std_lo",
			    48, 0, 48, 2, 0, 2);
    m_hfstd_hi  = new TH1D ("SampleHF_std_hi", "SampleHF_std_hi",
			    48, 0, 48, 2, 0, 2);
    wk()->addOutput (m_hfmean_lo);
    wk()->addOutput (m_hfmean_hi);
    wk()->addOutput (m_hfstd_lo);
    wk()->addOutput (m_hfstd_hi);
    m_hfmean_lo->GetXaxis()->SetBinLabel (1, "Min");
    m_hfmean_lo->GetXaxis()->SetBinLabel (2, "Max");
    m_hfmean_hi->GetXaxis()->SetBinLabel (1, "Min");
    m_hfmean_hi->GetXaxis()->SetBinLabel (2, "Max");
    m_hfstd_lo ->GetXaxis()->SetBinLabel (1, "Min");
    m_hfstd_lo ->GetXaxis()->SetBinLabel (2, "Max");
    m_hfstd_hi ->GetXaxis()->SetBinLabel (1, "Min");
    m_hfstd_hi ->GetXaxis()->SetBinLabel (2, "Max");
    for (pmt=1; pmt<49; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);
	m_hfmean_lo->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfmean_hi->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfstd_lo ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_hfstd_hi ->GetXaxis()->SetBinLabel (pmt, buffer);
      }

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[i];
	    hfmean_min[gain][pmt] = 4096;
	    hfmean_max[gain][pmt] = -1;
	    hfstd_min [gain][pmt] = 4096;
	    hfstd_max [gain][pmt] = -1;
	  }
      }

    return EL::StatusCode::SUCCESS;
    }

  EL::StatusCode SampleHF :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleHF :: execute ()
  {
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j]
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

	    if (mean < hfmean_min[gain][pmt])
	      {
		hfmean_min[gain][pmt] = mean;
	      }
	    else if (mean > hfmean_max[gain][pmt])
	      {
		hfmean_max[gain][pmt] = mean;
	      }
	    if (std < hfstd_min[gain][pmt])
	      {
		hfstd_min[gain][pmt] = std;
	      }
	    else if (std > hfstd_max[gain][pmt])
	      {
		hfstd_max[gain][pmt] = std;
	      }
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleHF :: finalize ()
  {
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[i];

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

	    if (gain)
	      {
		m_hfmean_hi->Fill (pmt, 0, hfmean_min[gain][pmt]);
		m_hfmean_hi->Fill (pmt, 1, hfmean_max[gain][pmt]);
		m_hfstd_hi ->Fill (pmt, 0, hfstd_min[gain][pmt]);
		m_hfstd_hi ->Fill (pmt, 1, hfstd_max[gain][pmt]);
	      }

	    else
	      {
		m_hfmean_lo->Fill (pmt, 0, hfmean_min[gain][pmt]);
		m_hfmean_lo->Fill (pmt, 1, hfmean_max[gain][pmt]);
		m_hfstd_lo ->Fill (pmt, 0, hfstd_min[gain][pmt]);
		m_hfstd_lo ->Fill (pmt, 1, hfstd_max[gain][pmt]);
	      }
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }
}
