/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/PulseFit.h>
#include <Herakles/PulseShape.h>

ClassImp(TD::PulseFit)

namespace TD
{
  PulseFit :: PulseFit () {}

  PulseFit :: ~PulseFit () {}

  EL::StatusCode PulseFit :: initialize ()
  {
    char buffer[5];

    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("pedestal", &pedestal, "pedestal[2][48]/D");
    ntuple->tree()->Branch ("height",   &height,   "height[2][48]/D");
    ntuple->tree()->Branch ("phase",    &phase,    "phase[2][48]/S");
    ntuple->tree()->Branch ("width",    &width,    "width[2][48]/D");
    ntuple->tree()->Branch ("chisqr",   &chisqr,   "chisqr[2][48]/D");
    ntuple->tree()->Branch ("ndf",      &ndf,      "ndf[2][48]/S");
    ntuple->tree()->Branch ("chgratio", &chgratio, "chgratio[2][48]/D");

    m_pedestal_lo = new TH2D ("PulseFit_pedestal_lo",
			      "PulseFit_pedestal_lo",
			      48, 0, 48, 2, 0, 2);
    m_pedestal_hi = new TH2D ("PulseFit_pedestal_hi",
			      "PulseFit_pedestal_hi",
			      48, 0, 48, 2, 0, 2);
    m_height_lo = new TH1D ("PulseFit_height_lo",
			    "PulseFit_height_lo",
			    48, 0, 48, 2, 0, 2);
    m_height_hi = new TH1D ("PulseFit_height_hi",
			    "PulseFit_height_hi",
			    48, 0, 48, 2, 0, 2);
    m_phase_lo = new TH1D ("PulseFit_phase_lo",
			   "PulseFit_phase_lo",
			   48, 0, 48, 2, 0, 2);
    m_phase_hi = new TH1D ("PulseFit_phase_hi",
			   "PulseFit_phase_hi",
			   48, 0, 48, 2, 0, 2);
    m_width_lo = new TH1D ("PulseFit_width_lo",
			   "PulseFit_width_lo",
			   48, 0, 48, 2, 0, 2);
    m_width_hi = new TH1D ("PulseFit_width_hi",
			   "PulseFit_width_hi",
			   48, 0, 48, 2, 0, 2);
    m_chisqr_lo = new TH1D ("PulseFit_chisqr_lo",
			    "PulseFit_chisqr_lo",
			    48, 0, 48, 2, 0, 2);
    m_chisqr_hi = new TH1D ("PulseFit_chisqr_hi",
			    "PulseFit_chisqr_hi",
			    48, 0, 48, 2, 0, 2);
    m_ndf_lo = new TH1S ("PulseFit_ndf_lo",
			 "PulseFit_ndf_lo",
			 48, 0, 48, 2, 0, 2);
    m_ndf_hi = new TH1S ("PulseFit_ndf_hi",
			 "PulseFit_ndf_hi",
			 48, 0, 48, 2, 0, 2);
    m_chgratio_lo = new TH1D ("PulseFit_chgratio_lo",
			      "PulseFit_chgratio_lo",
			      48, 0, 48, 2, 0, 2);
    m_chgratio_hi = new TH1D ("PulseFit_chgratio_hi",
			      "PulseFit_chgratio_hi",
			      48, 0, 48, 2, 0, 2);
    wk()->addOutput (m_pedestal_lo);
    wk()->addOutput (m_pedestal_hi);
    wk()->addOutput (m_height_lo);
    wk()->addOutput (m_height_hi);
    wk()->addOutput (m_phase_lo);
    wk()->addOutput (m_phase_hi);
    wk()->addOutput (m_width_lo);
    wk()->addOutput (m_width_hi);
    wk()->addOutput (m_chisqr_lo);
    wk()->addOutput (m_chisqr_hi);
    wk()->addOutput (m_ndf_lo);
    wk()->addOutput (m_ndf_hi);
    wk()->addOutput (m_chgratio_lo);
    wk()->addOutput (m_chgratio_hi);
    m_pedestal_lo->GetYaxis()->SetBinLabel (1, "Min");
    m_pedestal_lo->GetYaxis()->SetBinLabel (2, "Max");
    m_pedestal_hi->GetYaxis()->SetBinLabel (1, "Min");
    m_pedestal_hi->GetYaxis()->SetBinLabel (2, "Max");
    m_height_lo  ->GetYaxis()->SetBinLabel (1, "Min");
    m_height_lo  ->GetYaxis()->SetBinLabel (2, "Max");
    m_height_hi  ->GetYaxis()->SetBinLabel (1, "Min");
    m_height_hi  ->GetYaxis()->SetBinLabel (2, "Max");
    m_phase_lo   ->GetYaxis()->SetBinLabel (1, "Min");
    m_phase_lo   ->GetYaxis()->SetBinLabel (2, "Max");
    m_phase_hi   ->GetYaxis()->SetBinLabel (1, "Min");
    m_phase_hi   ->GetYaxis()->SetBinLabel (2, "Max");
    m_width_lo   ->GetYaxis()->SetBinLabel (1, "Min");
    m_width_lo   ->GetYaxis()->SetBinLabel (2, "Max");
    m_width_hi   ->GetYaxis()->SetBinLabel (1, "Min");
    m_width_hi   ->GetYaxis()->SetBinLabel (2, "Max");
    m_chisqr_lo  ->GetYaxis()->SetBinLabel (1, "Min");
    m_chisqr_lo  ->GetYaxis()->SetBinLabel (2, "Max");
    m_chisqr_hi  ->GetYaxis()->SetBinLabel (1, "Min");
    m_chisqr_hi  ->GetYaxis()->SetBinLabel (2, "Max");
    m_ndf_lo     ->GetYaxis()->SetBinLabel (1, "Min");
    m_ndf_lo     ->GetYaxis()->SetBinLabel (2, "Max");
    m_ndf_hi     ->GetYaxis()->SetBinLabel (1, "Min");
    m_ndf_hi     ->GetYaxis()->SetBinLabel (2, "Max");
    m_chgratio_lo->GetYaxis()->SetBinLabel (1, "Min");
    m_chgratio_lo->GetYaxis()->SetBinLabel (2, "Max");
    m_chgratio_hi->GetYaxis()->SetBinLabel (1, "Min");
    m_chgratio_hi->GetYaxis()->SetBinLabel (2, "Max");
    for (pmt=1; pmt<49; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);
	m_pedestal_lo->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedestal_hi->GetXaxis()->SetBinLabel (pmt, buffer);
	m_height_lo  ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_height_hi  ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_phase_lo   ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_phase_hi   ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_width_lo   ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_width_hi   ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chisqr_lo  ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chisqr_hi  ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ndf_lo     ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ndf_hi     ->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chgratio_lo->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chgratio_hi->GetXaxis()->SetBinLabel (pmt, buffer);
      }
    
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    pedestal_min[gain][pmt] = 4096;
	    pedestal_max[gain][pmt] = -1;
	    height_min[gain][pmt] = 10e10;
	    height_max[gain][pmt] = -10e10;
	    phase_min[gain][pmt] = 129;
	    phase_max[gain][pmt] = -1;
	    width_min[gain][pmt] = 129;
	    width_max[gain][pmt] = -1;
	    chisqr_min[gain][pmt] = 10e10;
	    chisqr_max[gain][pmt] = -1;
	    ndf_min[gain][pmt] = 129;
	    ndf_max[gain][pmt] = -1;
	    chgratio_min[gain][pmt] = 10e10;
	    chgratio_max[gain][pmt] = -10e10;
	  }
      }

    hist_temp = new TH1D ("htemp", "htemp",
			  highcut-lowcut, lowcut, highcut);
    hist_temp->SetDirectory (0);

    fit_temp = new TF1 ("ftemp", PulseShape,
			lowcut, highcut, 4);
    fit_temp->SetParLimits (3, 0.9, 1.0);
    fit_temp->SetParameter (3, 0.95);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PulseFit :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PulseFit :: execute ()
  {
    m_tree->SetBranchAddress ("charge",     &charge);
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    pedestal[gain][pmt] = -1;
	    height  [gain][pmt] =  0;
	    phase   [gain][pmt] = -1;
	    width   [gain][pmt] = -1;
	    chisqr  [gain][pmt] = -1;
	    ndf     [gain][pmt] = -1;
	    chgratio[gain][pmt] = -1;
	    hist_temp->Reset ("icesm");
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0)
		  {
		    hist_temp->Fill (sample, sval);
		  }
	      } // end sample
	    if (hist_temp->GetEntries())
	      {
		// is the pulse positive or negative?
		// ignore the maximum and minimum values then determine if
		// max or min is farther from the remaining average
		binrange = hist_temp->GetNbinsX ();
		binmin   = hist_temp->GetMinimumBin ();
		binmax   = hist_temp->GetMaximumBin ();
		approx   = 0;
		for (xbin=1; xbin<binrange+1; xbin++)
		  {
		    if (xbin == binmin) continue;
		    if (xbin == binmax) continue;
		    approx += hist_temp->GetBinContent (xbin);
		  }
		approx /= binrange - 2;
		minval = hist_temp->GetMinimum ();
		maxval = hist_temp->GetMaximum ();
		if (fabs(maxval-approx) > fabs(approx-minval)) pos = true;
		else pos = false;

		if (pos)
		  {
		    fit_temp->SetParLimits (0, minval, (maxval-minval)/2);
		    fit_temp->SetParameter (0, minval + (maxval-minval)/8);
		    fit_temp->SetParLimits (1, (maxval-minval)/2,
					    2*(maxval-minval));
		    fit_temp->SetParameter (1, maxval-minval);
		    fit_temp->SetParLimits (2, binmax - 3, binmax+1);
		    fit_temp->SetParameter (2, binmax-1);
		  }
		else
		  {
		    fit_temp->SetParLimits (0, (maxval-minval)/2, maxval);
		    fit_temp->SetParameter (0, maxval - (maxval-minval)/8);
		    fit_temp->SetParLimits (1, (minval-maxval)/2,
					    2*(minval-maxval));
		    fit_temp->SetParameter (1, minval-maxval);
		    fit_temp->SetParLimits (2, binmin - 3, binmin+1);
		    fit_temp->SetParameter (2, binmin-1);
		  }

		hist_temp->Fit (fit_temp, "ww0qS");
		fitRes = hist_temp->GetFunction ("ftemp");
		pedestal[gain][pmt] = fitRes->GetParameter (0);
		height  [gain][pmt] = fitRes->GetParameter (1);
		phase   [gain][pmt] = fitRes->GetParameter (2);
		width   [gain][pmt] = fitRes->GetParameter (3);
		chisqr  [gain][pmt] = fitRes->GetChiSquare ();
		ndf     [gain][pmt] = fitRes->GetNDF ();
		chgratio[gain][pmt] = height[gain][pmt] / charge;

		if (pedestal[gain][pmt] < pedestal_min[gain][pmt])
		  {
		    pedestal_min[gain][pmt] = pedestal[gain][pmt];
		  }
		else if (pedestal[gain][pmt] > pedestal_max[gain][pmt])
		  {
		    pedestal_max[gain][pmt] = pedestal[gain][pmt];
		  }
		if (height[gain][pmt] < height_min[gain][pmt])
		  {
		    height_min[gain][pmt] = height[gain][pmt];
		  }
		else if (height[gain][pmt] > height_max[gain][pmt])
		  {
		    height_max[gain][pmt] = height[gain][pmt];
		  }
		if (phase[gain][pmt] < phase_min[gain][pmt])
		  {
		    phase_min[gain][pmt] = phase[gain][pmt];
		  }
		else if (phase[gain][pmt] > phasemax[gain][pmt])
		  {
		    phase_max[gain][pmt] = phase[gain][pmt];
		  }
		if (width[gain][pmt] < width_min[gain][pmt])
		  {
		    width_min[gain][pmt] = width[gain][pmt];
		  }
		else if (width[gain][pmt] > width_max[gain][pmt])
		  {
		    width_max[gain][pmt] = width[gain][pmt];
		  }
		if (chisqr[gain][pmt] < chisqr_min[gain][pmt])
		  {
		    chisqr_min[gain][pmt] = chisqr[gain][pmt];
		  }
		else if (chisqr[gain][pmt] > chisqr_max[gain][pmt])
		  {
		    chisqr_max[gain][pmt] = chisqr[gain][pmt];
		  }
		if (ndf[gain][pmt] < ndf_min[gain][pmt])
		  {
		    ndf_min[gain][pmt] = ndf[gain][pmt];
		  }
		else if (ndf[gain][pmt] > ndf_max[gain][pmt])
		  {
		    ndf_max[gain][pmt] = ndf[gain][pmt];
		  }
		if (chgratio[gain][pmt] < chgratio_min[gain][pmt])
		  {
		    chgratio_min[gain][pmt] = chgratio[gain][pmt];
		  }
		else if (chgratio[gain][pmt] > chgratio_max[gain][pmt])
		  {
		    chgratio_max[gain][pmt] = chgratio[gain][pmt];
		  }
	      }
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PulseFit :: finalize ()
  {
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];

	    if (pedestal_min[gain][pmt] > pedestal_max[gain][pmt])
	      {
		pedestal_min[gain][pmt] = -1;
		pedestal_max[gain][pmt] = -1;
	      }
	    if (height_min[gain][pmt] > height_max[gain][pmt])
	      {
		height_min[gain][pmt] = -1;
		height_max[gain][pmt] = -1;
	      }
	    if (phase_min[gain][pmt] > phase_max[gain][pmt])
	      {
		phase_min[gain][pmt] = -1;
		phase_max[gain][pmt] = -1;
	      }
	    if (width_min[gain][pmt] > width_max[gain][pmt])
	      {
		width_min[gain][pmt] = -1;
		width_max[gain][pmt] = -1;
	      }
	    if (chisqr_min[gain][pmt] > chisqr_max[gain][pmt])
	      {
		chisqr_min[gain][pmt] = -1;
		chisqr_max[gain][pmt] = -1;
	      }
	    if (ndf_min[gain][pmt] > ndf_max[gain][pmt])
	      {
		ndf_min[gain][pmt] = -1;
		ndf_max[gain][pmt] = -1;
	      }
	    if (chgratio_min[gain][pmt] > chgratio_max[gain][pmt])
	      {
		chgratio_min[gain][pmt] = -1;
		chgratio_max[gain][pmt] = -1;
	      }

            if (gain)
	      {
		m_pedestal_hi->Fill (pmt, 0, pedestal_min[gain][pmt]);
		m_pedestal_hi->Fill (pmt, 1, pedestal_max[gain][pmt]);
		m_height_hi  ->Fill (pmt, 0, height_min[gain][pmt]);
		m_height_hi  ->Fill (pmt, 1, height_max[gain][pmt]);
		m_phase_hi   ->Fill (pmt, 0, phase_min[gain][pmt]);
		m_phase_hi   ->Fill (pmt, 1, phase_max[gain][pmt]);
		m_width_hi   ->Fill (pmt, 0, width_min[gain][pmt]);
		m_width_hi   ->Fill (pmt, 1, width_max[gain][pmt]);
		m_chisqr_hi  ->Fill (pmt, 0, chisqr_min[gain][pmt]);
		m_chisqr_hi  ->Fill (pmt, 1, chisqr_max[gain][pmt]);
		m_ndf_hi     ->Fill (pmt, 0, ndf_min[gain][pmt]);
		m_ndf_hi     ->Fill (pmt, 1, ndf_max[gain][pmt]);
		m_chgratio_hi->Fill (pmt, 0, chgratio_min[gain][pmt]);
		m_chgratio_hi->Fill (pmt, 1, chgratio_max[gain][pmt]);
	      }

            else
	      {
		m_pedestal_lo->Fill (pmt, 0, pedestal_min[gain][pmt]);
		m_pedestal_lo->Fill (pmt, 1, pedestal_max[gain][pmt]);
		m_height_lo  ->Fill (pmt, 0, height_min[gain][pmt]);
		m_height_lo  ->Fill (pmt, 1, height_max[gain][pmt]);
		m_phase_lo   ->Fill (pmt, 0, phase_min[gain][pmt]);
		m_phase_lo   ->Fill (pmt, 1, phase_max[gain][pmt]);
		m_width_lo   ->Fill (pmt, 0, width_min[gain][pmt]);
		m_width_lo   ->Fill (pmt, 1, width_max[gain][pmt]);
		m_chisqr_lo  ->Fill (pmt, 0, chisqr_min[gain][pmt]);
		m_chisqr_lo  ->Fill (pmt, 1, chisqr_max[gain][pmt]);
		m_ndf_lo     ->Fill (pmt, 0, ndf_min[gain][pmt]);
		m_ndf_lo     ->Fill (pmt, 1, ndf_max[gain][pmt]);
		m_chgratio_lo->Fill (pmt, 0, chgratio_min[gain][pmt]);
		m_chgratio_lo->Fill (pmt, 1, chgratio_max[gain][pmt]);
	      }
	  }
      }
    return EL::StatusCode::SUCCESS;
  }
}
