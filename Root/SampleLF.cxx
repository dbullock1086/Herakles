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
    // label buffer
    char buffer[9];

    /*
     * Variance: E[(x-mux)^2] = std[x]^2
                 = E[x^2 - 2*x*mux + mux^2]
                 = E[x^2] - 2*E[x]*mux + (mux)^2
                 = E[x^2] - (mux)^2
     * Therefore we need to calculate sum(x) and sum(x^2)
     */
    
    // histograms contain min-max of high-frequency mean and standard deviation
    m_lfmean_lo = new TProfile2D ("LFMean_lo", "LF Mean Low",
				  128, 0, 128, 48, 0, 48, "S");
    m_lfmean_hi = new TProfile2D ("LFMean_hi", "LF Mean High",
				  128, 0, 128, 48, 0, 48, "S");
    m_lfstd_lo = new TH2D ("LFStd_lo", "LF StdDev Low",
			   128, 0, 128, 48, 0, 48);
    m_lfstd_hi = new TH2D ("LFStd_hi", "LF StdDev High",
			   128, 0, 128, 48, 0, 48);

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

    // add the histograms to EL output
    wk()->addOutput (m_lfmean_lo);
    wk()->addOutput (m_lfmean_hi);
    wk()->addOutput (m_lfstd_lo);
    wk()->addOutput (m_lfstd_hi);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleLF :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleLF :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    // loop through gain and PMT
    for (gain=0; gain<48; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    // sum(x) and sum(x^2)
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0)
		  {
		    if (gain) m_lfmean_hi->Fill (sample, pmt, sval);
		    else m_lfmean_lo->Fill (sample, pmt, sval);
		  }
	      } // end sample
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleLF :: finalize ()
  {
    // readability variables;
    Int_t binnum;
    Double_t std;
    
    // loop through gain and PMT
    for (gain=0; gain<48; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain)
		  {
		    binnum = m_lfmean_hi->FindBin (sample, pmt);
		    std = m_lfmean_hi->GetBinError (binnum);
		    m_lfstd_hi->SetBinContent (binnum, std);
		    m_lfstd_hi->SetBinError (binnum, 0);
		  }
		else
		  {
		    binnum = m_lfmean_lo->FindBin (sample, pmt);
		    std = m_lfmean_lo->GetBinError (binnum);
		    m_lfstd_lo->SetBinContent (binnum, std);
		    m_lfstd_lo->SetBinError (binnum, 0);
		  }
	      }
	  }
      }
    return EL::StatusCode::SUCCESS;
  }
}
