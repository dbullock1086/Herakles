/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/CorrHF.h>
#include <TMath.h>

ClassImp(TD::CorrHF)

namespace TD
{
  CorrHF :: CorrHF () {}

  CorrHF :: ~CorrHF () {}

  EL::StatusCode CorrHF :: initialize ()
  {
    char buffer[11];
    
    m_hfcorr = new TH2D ("CorrHF", "CorrHF", 96, 0, 96, 96, 0, 96);
    wk()->addOutput (m_hfcorr);
    m_hfcorr_>GetZaxis()->SetRangeUser (0, 1);

    for (gain=0; gain<2; gain++)
      {
	for (pmt1=0; pmt1<48; pmt1++)
	  {
	    sprintf (buffer, "Gain%d_PMT%d", gain, pmt);
	    m_hfcorr->GetXaxis()->SetBinLabel (pmt1 + 48*gain + 1, buffer);
	    m_hfcorr->GetYaxis()->SetBinLabel (pmt1 + 48*gain + 1, buffer);
	    numX [gain][pmt1] = 0;
	    xsum [gain][pmt1] = 0;
	    x2sum[gain][pmt1] = 0;
	    for (pmt2=0; pmt2<48; pmt2++)
	      {
		numXY[gain][pmt1][pmt2] = 0;
		xysum[gain][pmt1][pmt2] = 0;
	      }
	  }
      }

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CorrHF :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CorrHF :: execute ()
  {
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j1=0; j1<sizeof(channels); j1++)
	  {
	    pmt1 = channels[j1];
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval1 = samples_hi[pmt1][sample];
		else      sval1 = samples_lo[pmt1][sample];
		if (sval1 < 4096 && sval1 >= 0)
		  {
		    numX [gain][pmt1] ++;
		    xsum [gain][pmt1] += sval1;
		    x2sum[gain][pmt1] += sval1 * sval1;
		  }
	      } // end sample
	    for (j2=0; j2<j1; j2++)
	      {
		pmt2 = channels[j2];
		for (sample=window[0]; sample<window[1]; sample++)
		  {
		    if (gain)
		      {
			sval1 = samples_hi[pmt1][sample];
			sval2 = samples_hi[pmt2][sample];
		      }
		    else
		      {
			sval1 = samples_lo[pmt1][sample];
			sval2 = samples_lo[pmt2][sample];
		      }
		    if (sval1 < 4096 && sval1 >= 0
			&& sval2 < 4096 && sval2 >= 0)
		      {
			numXY[gain][pmt1][pmt2] ++;
			xysum[gain][pmt1][pmt2] += sval1 * sval2;
		      }
		  } // end sample
	      } // end pmt2
	  } // end pmt1
      } // end gain

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CorrHF :: finalize ()
  {
    Double xsqr, meanx, xmsqr, meanx2, stdX;
    Double ysqr, meany, ymsqr, meany2, stdY;
    Double meanxy, corr;

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j1=0; j1<sizeof(channels); j1++)
	  {
	    pmt1 = channels[j1];
	    xval = pmt1 + 48*gain;

	    xsqr = x2sum[gain][pmt1] / numX[gain][pmt1];
	    meanx = xsum[gain][pmt1] / numX[gain][pmt1];
	    xmsqr = xsum[gain][pmt1] * meanx / numX[gain][pmt1];
	    meanx2 = meanx * meanx / numX[gain][pmt1];
	    stdX = TMath::Sqrt (xsqr - 2*xmsqr + meanx2);

	    for (j2=0; j2<j1; j2++)
	      {
		pmt2 = channels[j2];
		yval = pmt2 + 48*gain;

		ysqr = x2sum[gain][pmt2] / numX[gain][pmt2];
		meany = xsum[gain][pmt2] / numX[gain][pmt2];
		ymsqr = xsum[gain][pmt2] * meany / numX[gain][pmt2];
		meany2 = meany * meany / numX[gain][pmt2];
		stdY = TMath::Sqrt (ysqr - 2*ymsqr + meany2);

		meanxy = xysum[gain][pmt2] / numX[gain][pmt2];
		corr = (meanxy - meanx*meany) / (stdX * stdY);
		m_hfcorr->Fill (xval, yval, corr);
	      }
	  }
      }

    return EL::StatusCode::SUCCESS;
  }
}