/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <Herakles/SampleLF.h>
#include <TMath.h>

ClassImp(TD::SampleLF)

namespace TD
{
  SampleLF :: SampleLF () {}

  SampleLF :: ~SampleLF () {}

  EL::StatusCode SampleLF :: initialize ()
  {
    char buffer[9];

    m_lfmean_lo = new TH2D ("SampleLF_mean_lo",
			    "SampleLF_mean_lo",
			    128, 0, 128, 48, 0, 48);
    m_lfmean_hi = new TH2D ("SampleLF_mean_hi",
			    "SampleLF_mean_hi",
			    128, 0, 128, 48, 0, 48);
    m_lfstd_lo = new TH2D ("SampleLF_std_lo",
			   "SampleLF_std_lo",
			   128, 0, 128, 48, 0, 48);
    m_lfstd_hi = new TH2D ("SampleLF_std_hi",
			   "SampleLF_std_hi",
			   128, 0, 128, 48, 0, 48);

    wk()->addOutput (m_lfmean_lo);
    wk()->addOutput (m_lfmean_hi);
    wk()->addOutput (m_lfstd_lo);
    wk()->addOutput (m_lfstd_hi);

    for (pmt=1; pmt<49; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);
	m_lfmean_lo->GetYaxis()->SetBinLabel (pmt, buffer);
	m_lfmean_hi->GetYaxis()->SetBinLabel (pmt, buffer);
	m_lfstd_lo ->GetYaxis()->SetBinLabel (pmt, buffer);
	m_lfstd_hi ->GetYaxis()->SetBinLabel (pmt, buffer);
      }
    for (sample=0; sample<128; sample++)
      {
	sprintf (buffer, "Sample%d", sample);
	m_lfmean_lo->GetXaxis()->SetBinLabel (sample + 1, buffer);
	m_lfmean_hi->GetXaxis()->SetBinLabel (sample + 1, buffer);
	m_lfstd_lo ->GetXaxis()->SetBinLabel (sample + 1, buffer);
	m_lfstd_hi ->GetXaxis()->SetBinLabel (sample + 1, buffer);
      }

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		xval  [gain][pmt][sample] = 0;
		x2val [gain][pmt][sample] = 0;
		nonCRC[gain][pmt][sample] = 0;
	      }
	  }
      }

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleLF :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleLF :: execute ()
  {
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[i];
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0)
		  {
		    xval  [gain][pmt][sample] += sval;
		    x2val [gain][pmt][sample] += sval * sval;
		    nonCRC[gain][pmt][sample] += 1;
		  }
	      } // end sample
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleLF :: finalize ()
  {
    Double_t mean, std;

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (nonCRC[gain][pmt][sample])
		  {
		    mean = xval[gain][pmt][sample] / nonCRC[gain][pmt][sample];
		  }
		else mean = -1;
		if (nonCRC[gain][pmt][sample] > 1)
		  {
		    x2  = x2val[gain][pmt][sample] / \
		      (nonCRC[gain][pmt][sample] - 1);
		    xm  = xval[gain][pmt][sample] * mean / \
		      (nonCRC[gain][pmt][sample] - 1);
		    m2  = mean * mean / (nonCRC[gain][pmt][sample] - 1);
		    std = TMath::Sqrt(x2 - 2*xm + m2);
		  }
		else std = -1;

		if (gain)
		  {
		    m_lfmean_hi->Fill (sample, pmt, mean);
		    m_lfstd_hi ->Fill (sample, pmt, std);
		  }

		else
		  {
		    m_lfmean_lo->Fill (sample, pmt, mean);
		    m_lfstd_lo ->Fill (sample, pmt, std);
		  }
	      }
	  }
      }
    return EL::StatusCode::SUCCESS;
  }
}
