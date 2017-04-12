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
    // label buffer
    char buffer[5];

    // new branch added to raw data
    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("pedestal", &pedestal, "pedestal[2][48]/D");
    ntuple->tree()->Branch ("height",   &height,   "height[2][48]/D");
    ntuple->tree()->Branch ("phase",    &phase,    "phase[2][48]/S");
    ntuple->tree()->Branch ("width",    &width,    "width[2][48]/D");
    ntuple->tree()->Branch ("chisqr",   &chisqr,   "chisqr[2][48]/D");
    ntuple->tree()->Branch ("ndf",      &ndf,      "ndf[2][48]/S");
    ntuple->tree()->Branch ("chgratio", &chgratio, "chgratio[2][48]/D");

    // initialize with inverted min-max
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
	  } // end pmt
      } // end gain

    // histogram contains min and max in separate bins
    m_pedestal_lo_min = new TH1D ("PulseFit_pedestal_lo_min",
				  "Pulse Pedestal Low", 48, 0, 48);
    m_pedestal_lo_max = new TH1D ("PulseFit_pedestal_lo_max",
				  "Pulse Pedestal Low", 48, 0, 48);
    m_pedestal_hi_min = new TH1D ("PulseFit_pedestal_hi_min",
				  "Pulse Pedestal High", 48, 0, 48);
    m_pedestal_hi_max = new TH1D ("PulseFit_pedestal_hi_max",
				  "Pulse Pedestal High", 48, 0, 48);

    m_height_lo_min = new TH1D ("PulseFit_height_lo_min",
				"Pulse Height Low", 48, 0, 48);
    m_height_lo_max = new TH1D ("PulseFit_height_lo_max",
				"Pulse Height Low", 48, 0, 48);
    m_height_hi_min = new TH1D ("PulseFit_height_hi_min",
				"Pulse Height High", 48, 0, 48);
    m_height_hi_max = new TH1D ("PulseFit_height_hi_max",
				"Pulse Height High", 48, 0, 48);

    m_phase_lo_min = new TH1D ("PulseFit_phase_lo_min",
			       "Pulse Phase Low", 48, 0, 48);
    m_phase_lo_max = new TH1D ("PulseFit_phase_lo_max",
			       "Pulse Phase Low", 48, 0, 48);
    m_phase_hi_min = new TH1D ("PulseFit_phase_hi_min",
			       "Pulse Phase High", 48, 0, 48);
    m_phase_hi_max = new TH1D ("PulseFit_phase_hi_max",
			       "Pulse Phase High", 48, 0, 48);

    m_width_lo_min = new TH1D ("PulseFit_width_lo_min",
			       "Pulse Width Low", 48, 0, 48);
    m_width_lo_max = new TH1D ("PulseFit_width_lo_max",
			       "Pulse Width Low", 48, 0, 48);
    m_width_hi_min = new TH1D ("PulseFit_width_hi_min",
			       "Pulse Width High", 48, 0, 48);
    m_width_hi_max = new TH1D ("PulseFit_width_hi_max",
			       "Pulse Width High", 48, 0, 48);

    m_chisqr_lo_min = new TH1D ("PulseFit_chisqr_lo_min",
				"Pulse Chi2 Low", 48, 0, 48);
    m_chisqr_lo_max = new TH1D ("PulseFit_chisqr_lo_max",
				"Pulse Chi2_Low", 48, 0, 48);
    m_chisqr_hi_min = new TH1D ("PulseFit_chisqr_hi_min",
				"Pulse Chi2 High", 48, 0, 48);
    m_chisqr_hi_max = new TH1D ("PulseFit_chisqr_hi_max",
				"Pulse Chi2 High", 48, 0, 48);

    m_ndf_lo_min = new TH1D ("PulseFit_ndf_lo_min", "Pulse NDF Low",
			     48, 0, 48);
    m_ndf_lo_max = new TH1D ("PulseFit_ndf_lo_max", "Pulse NDF Low",
			     48, 0, 48);
    m_ndf_hi_min = new TH1D ("PulseFit_ndf_hi_min", "Pulse NDF High",
			     48, 0, 48);
    m_ndf_hi_max = new TH1D ("PulseFit_ndf_hi_max", "Pulse NDF High",
			     48, 0, 48);

    m_chgratio_lo_min = new TH1D ("PulseFit_chgratio_lo_min",
				  "Pulse Height/Charge Low", 48, 0, 48);
    m_chgratio_lo_max = new TH1D ("PulseFit_chgratio_lo_max",
				  "Pulse Height/Charge Low", 48, 0, 48);
    m_chgratio_hi_min = new TH1D ("PulseFit_chgratio_hi_min",
				  "Pulse Height/Charge High", 48, 0, 48);
    m_chgratio_hi_max = new TH1D ("PulseFit_chgratio_hi_max",
				  "Pulse Height/Charge High", 48, 0, 48);

    m_pedestal_lo_min->SetYTitle ("Min");
    m_pedestal_lo_max->SetYTitle ("Max");
    m_pedestal_hi_min->SetYTitle ("Min");
    m_pedestal_hi_min->SetYTitle ("Max");

    m_height_lo_min->SetYTitle ("Min");
    m_height_lo_max->SetYTitle ("Max");
    m_height_hi_min->SetYTitle ("Min");
    m_height_hi_max->SetYTitle ("Max");

    m_phase_lo_min->SetYTitle ("Min");
    m_phase_lo_max->SetYTitle ("Max");
    m_phase_hi_min->SetYTitle ("Min");
    m_phase_hi_max->SetYTitle ("Max");

    m_width_lo_min->SetYTitle ("Min");
    m_width_lo_max->SetYTitle ("Max");
    m_width_hi_min->SetYTitle ("Min");
    m_width_hi_max->SetYTitle ("Max");

    m_chisqr_lo_min->SetYTitle ("Min");
    m_chisqr_lo_max->SetYTitle ("Max");
    m_chisqr_hi_min->SetYTitle ("Min");
    m_chisqr_hi_max->SetYTitle ("Max");

    m_ndf_lo_min->SetYTitle ("Min");
    m_ndf_lo_max->SetYTitle ("Max");
    m_ndf_hi_min->SetYTitle ("Min");
    m_ndf_hi_min->SetYTitle ("Max");

    m_chgratio_lo_min->SetYTitle ("Min");
    m_chgratio_lo_max->SetYTitle ("Max");
    m_chgratio_hi_min->SetYTitle ("Min");
    m_chgratio_hi_max->SetYTitle ("Max");

    for (pmt=1; pmt<49; pmt++) // notice number convention
      {
	sprintf (buffer, "PMT%d", pmt);

	m_pedestal_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedestal_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedestal_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedestal_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

	m_height_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_height_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_height_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_height_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

	m_phase_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_phase_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_phase_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_phase_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

	m_width_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_width_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_width_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_width_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

	m_chisqr_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chisqr_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chisqr_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chisqr_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

	m_ndf_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ndf_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ndf_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ndf_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

	m_chgratio_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chgratio_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chgratio_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_chgratio_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);
      }

    // add the histograms to EL output
    wk()->addOutput (m_pedestal_lo_min);
    wk()->addOutput (m_pedestal_lo_max);
    wk()->addOutput (m_pedestal_hi_min);
    wk()->addOutput (m_pedestal_hi_max);

    wk()->addOutput (m_height_lo_min);
    wk()->addOutput (m_height_lo_max);
    wk()->addOutput (m_height_hi_min);
    wk()->addOutput (m_height_hi_max);

    wk()->addOutput (m_phase_lo_min);
    wk()->addOutput (m_phase_lo_max);
    wk()->addOutput (m_phase_hi_min);
    wk()->addOutput (m_phase_hi_max);

    wk()->addOutput (m_width_lo_min);
    wk()->addOutput (m_width_lo_max);
    wk()->addOutput (m_width_hi_min);
    wk()->addOutput (m_width_hi_max);

    wk()->addOutput (m_chisqr_lo_min);
    wk()->addOutput (m_chisqr_lo_max);
    wk()->addOutput (m_chisqr_hi_min);
    wk()->addOutput (m_chisqr_hi_max);

    wk()->addOutput (m_ndf_lo_min);
    wk()->addOutput (m_ndf_lo_max);
    wk()->addOutput (m_ndf_hi_min);
    wk()->addOutput (m_ndf_hi_max);

    wk()->addOutput (m_chgratio_lo_min);
    wk()->addOutput (m_chgratio_lo_max);
    wk()->addOutput (m_chgratio_hi_min);
    wk()->addOutput (m_chgratio_hi_max);

    // temporary histogram and function for event-by-event fit
    hist_temp = new TH1D ("htemp", "htemp",
			  window[1]-window[0], window[0], window[1]);
    hist_temp->SetDirectory (0);

    fit_temp = new TF1 ("ftemp", PulseShape,
			window[0], window[1], 4);
    // the width should be fairly stable
    // (fit success is also sensitive to constraining this parameter)
    fit_temp->SetParLimits (3, 0.9, 1.0);
    fit_temp->SetParameter (3, 0.95);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PulseFit :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PulseFit :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("charge",     &charge);
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    // loop through gain and PMT
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];

	    // default values are invalid (or error states)
	    pedestal[gain][pmt] = -1;
	    height  [gain][pmt] =  0;
	    phase   [gain][pmt] = -1;
	    width   [gain][pmt] = -1;
	    chisqr  [gain][pmt] = -1;
	    ndf     [gain][pmt] = -1;
	    chgratio[gain][pmt] = -1;

	    // fill a histogram with samples
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

	    // fit that histogram with a pulse shape
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

		if (pos) // the pulse is positive
		  {
		    // pedestal should be in the lower half of the range
		    fit_temp->SetParLimits (0, minval, (maxval-minval)/2);
		    fit_temp->SetParameter (0, minval + (maxval-minval)/8);
		    // height can actually be larger than the range
		    fit_temp->SetParLimits (1, (maxval-minval)/2,
					    2*(maxval-minval));
		    fit_temp->SetParameter (1, maxval-minval);
		    // phase is somewhere near the maximum
		    fit_temp->SetParLimits (2, binmax - 3, binmax+1);
		    fit_temp->SetParameter (2, binmax-1);
		  }
		else // the pulse is negative
		  {
		    // pedestal should be in the upper half of the range
		    fit_temp->SetParLimits (0, (maxval-minval)/2, maxval);
		    fit_temp->SetParameter (0, maxval - (maxval-minval)/8);
		    // height can actually be larger than the range
		    fit_temp->SetParLimits (1, (minval-maxval)/2,
					    2*(minval-maxval));
		    fit_temp->SetParameter (1, minval-maxval);
		    // phase is somewhere near the minimum
		    fit_temp->SetParLimits (2, binmin - 3, binmin+1);
		    fit_temp->SetParameter (2, binmin-1);
		  }

		// get parameters from the fit
		hist_temp->Fit (fit_temp, "ww0qS");
		fitRes = hist_temp->GetFunction ("ftemp");
		pedestal[gain][pmt] = fitRes->GetParameter (0);
		height  [gain][pmt] = fitRes->GetParameter (1);
		phase   [gain][pmt] = fitRes->GetParameter (2);
		width   [gain][pmt] = fitRes->GetParameter (3);
		chisqr  [gain][pmt] = fitRes->GetChisquare ();
		ndf     [gain][pmt] = fitRes->GetNDF ();
		chgratio[gain][pmt] = height[gain][pmt] / charge;

                // check if value exceeds range
		if (pedestal[gain][pmt] < pedestal_min[gain][pmt])
		  {
		    pedestal_min[gain][pmt] = pedestal[gain][pmt];
		  }
		if (pedestal[gain][pmt] > pedestal_max[gain][pmt])
		  {
		    pedestal_max[gain][pmt] = pedestal[gain][pmt];
		  }
		if (height[gain][pmt] < height_min[gain][pmt])
		  {
		    height_min[gain][pmt] = height[gain][pmt];
		  }
		if (height[gain][pmt] > height_max[gain][pmt])
		  {
		    height_max[gain][pmt] = height[gain][pmt];
		  }
		if (phase[gain][pmt] < phase_min[gain][pmt])
		  {
		    phase_min[gain][pmt] = phase[gain][pmt];
		  }
		if (phase[gain][pmt] > phase_max[gain][pmt])
		  {
		    phase_max[gain][pmt] = phase[gain][pmt];
		  }
		if (width[gain][pmt] < width_min[gain][pmt])
		  {
		    width_min[gain][pmt] = width[gain][pmt];
		  }
		if (width[gain][pmt] > width_max[gain][pmt])
		  {
		    width_max[gain][pmt] = width[gain][pmt];
		  }
		if (chisqr[gain][pmt] < chisqr_min[gain][pmt])
		  {
		    chisqr_min[gain][pmt] = chisqr[gain][pmt];
		  }
		if (chisqr[gain][pmt] > chisqr_max[gain][pmt])
		  {
		    chisqr_max[gain][pmt] = chisqr[gain][pmt];
		  }
		if (ndf[gain][pmt] < ndf_min[gain][pmt])
		  {
		    ndf_min[gain][pmt] = ndf[gain][pmt];
		  }
		if (ndf[gain][pmt] > ndf_max[gain][pmt])
		  {
		    ndf_max[gain][pmt] = ndf[gain][pmt];
		  }
		if (chgratio[gain][pmt] < chgratio_min[gain][pmt])
		  {
		    chgratio_min[gain][pmt] = chgratio[gain][pmt];
		  }
		if (chgratio[gain][pmt] > chgratio_max[gain][pmt])
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
    // loop through gain and PMT
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];

            // error state: min is still greater than max
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

            // set min-max bin contents in histograms
            if (gain)
	      {
		m_pedestal_hi_min->SetBinContent (pmt+1,
						  pedestal_min[gain][pmt]);
		m_pedestal_hi_min->SetBinError (pmt+1, 0);
		m_pedestal_hi_max->SetBinContent (pmt+1,
						  pedestal_max[gain][pmt]);
		m_pedestal_hi_max->SetBinError (pmt+1, 0);

		m_height_hi_min->SetBinContent (pmt+1, height_min[gain][pmt]);
		m_height_hi_min->SetBinError (pmt+1, 0);
		m_height_hi_max->SetBinContent (pmt+1, height_max[gain][pmt]);
		m_height_hi_max->SetBinError (pmt+1, 0);

		m_phase_hi_min->SetBinContent (pmt+1, phase_min[gain][pmt]);
		m_phase_hi_min->SetBinError (pmt+1, 0);
		m_phase_hi_max->SetBinContent (pmt+1, phase_max[gain][pmt]);
		m_phase_hi_max->SetBinError (pmt+1, 0);

		m_width_hi_min->SetBinContent (pmt+1, width_min[gain][pmt]);
		m_width_hi_min->SetBinError (pmt+1, 0);
		m_width_hi_max->SetBinContent (pmt+1, width_max[gain][pmt]);
		m_width_hi_max->SetBinError (pmt+1, 0);

		m_chisqr_hi_min->SetBinContent (pmt+1, chisqr_min[gain][pmt]);
		m_chisqr_hi_min->SetBinError (pmt+1, 0);
		m_chisqr_hi_max->SetBinContent (pmt+1, chisqr_max[gain][pmt]);
		m_chisqr_hi_max->SetBinError (pmt+1, 0);

		m_ndf_hi_min->SetBinContent (pmt+1, ndf_min[gain][pmt]);
		m_ndf_hi_min->SetBinError (pmt+1, 0);
		m_ndf_hi_max->SetBinContent (pmt+1, ndf_max[gain][pmt]);
		m_ndf_hi_max->SetBinError (pmt+1, 0);

		m_chgratio_hi_min->SetBinContent (pmt+1,
						  chgratio_min[gain][pmt]);
		m_chgratio_hi_min->SetBinError (pmt+1, 0);
		m_chgratio_hi_max->SetBinContent (pmt+1,
						  chgratio_max[gain][pmt]);
		m_chgratio_hi_max->SetBinError (pmt+1, 0);
	      }

            else
	      {
		m_pedestal_lo_min->SetBinContent (pmt+1,
						  pedestal_min[gain][pmt]);
		m_pedestal_lo_min->SetBinError (pmt+1, 0);
		m_pedestal_lo_max->SetBinContent (pmt+1,
						  pedestal_max[gain][pmt]);
		m_pedestal_lo_max->SetBinError (pmt+1, 0);

		m_height_lo_min->SetBinContent (pmt+1, height_min[gain][pmt]);
		m_height_lo_min->SetBinError (pmt+1, 0);
		m_height_lo_max->SetBinContent (pmt+1, height_max[gain][pmt]);
		m_height_lo_max->SetBinError (pmt+1, 0);

		m_phase_lo_min->SetBinContent (pmt+1, phase_min[gain][pmt]);
		m_phase_lo_min->SetBinError (pmt+1, 0);
		m_phase_lo_max->SetBinContent (pmt+1, phase_max[gain][pmt]);
		m_phase_lo_max->SetBinError (pmt+1, 0);

		m_width_lo_min->SetBinContent (pmt+1, width_min[gain][pmt]);
		m_width_lo_min->SetBinError (pmt+1, 0);
		m_width_lo_max->SetBinContent (pmt+1, width_max[gain][pmt]);
		m_width_lo_max->SetBinError (pmt+1, 0);

		m_chisqr_lo_min->SetBinContent (pmt+1, chisqr_min[gain][pmt]);
		m_chisqr_lo_min->SetBinError (pmt+1, 0);
		m_chisqr_lo_max->SetBinContent (pmt+1, chisqr_max[gain][pmt]);
		m_chisqr_lo_max->SetBinError (pmt+1, 0);

		m_ndf_lo_min->SetBinContent (pmt+1, ndf_min[gain][pmt]);
		m_ndf_lo_min->SetBinError (pmt+1, 0);
		m_ndf_lo_max->SetBinContent (pmt+1, ndf_max[gain][pmt]);
		m_ndf_lo_max->SetBinError (pmt+1, 0);

		m_chgratio_lo_min->SetBinContent (pmt+1,
						  chgratio_min[gain][pmt]);
		m_chgratio_lo_min->SetBinError (pmt+1, 0);
		m_chgratio_lo_max->SetBinContent (pmt+1,
						  chgratio_max[gain][pmt]);
		m_chgratio_lo_max->SetBinError (pmt+1, 0);
	      }
	  }
      }
    return EL::StatusCode::SUCCESS;
  }
}
