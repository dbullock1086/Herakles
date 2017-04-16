/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/PulseFit.h>

ClassImp(TD::PulseFit)

namespace TD
{
  PulseFit :: PulseFit () {}

  PulseFit :: ~PulseFit () {}

  Double_t PulseFit :: PulseShape (Double_t *x, Double_t *par)
  {
    /* par[0] is pedestal
     * par[1] is height
     * par[2] is phase
     * par[3] is width
     */

    /* the function can only accept:
     * ... positive pedestal values
     * ... positive phase values
     * ... and positive width values
     * but it CAN accept negative height (negative pulses)
     */
    if (par[0]<0) return 0;
    if (par[2]<0) return 0;
    if (par[3]<0) return 0;
    
    const Double_t pulse[100] = {
      -0.00066,  0.00194,  0.00005, -0.00236, -0.00170,  0.00044,  0.00133,
       0.00088,  0.00019, -0.00012, -0.00018, -0.00037, -0.00073, -0.00063,
       0.00148,  0.00733,  0.01792,  0.03352,  0.05468,  0.08233,  0.11697,
       0.15842,  0.20641,  0.26093,  0.32185,  0.38857,  0.46007,  0.53488,
       0.61101,  0.68571,  0.75555,  0.81753,  0.87042,  0.91497,  0.95234,
       0.98038,  0.99559,  1.00000,  0.99470,  0.97875,  0.95271,  0.91860,
       0.87812,  0.83228,  0.78239,  0.73022,  0.67699,  0.62298,  0.56822,
       0.51314,  0.45855,  0.40556,  0.35548,  0.30968,  0.26919,  0.23406,
       0.20313,  0.17517,  0.15038,  0.12972,  0.11287,  0.09823,  0.08490,
       0.07340,  0.06419,  0.05690,  0.05077,  0.04532,  0.04035,  0.03576,
       0.03148,  0.02745,  0.02361,  0.01991,  0.01634,  0.01291,  0.00965,
       0.00658,  0.00375,  0.00118, -0.00110, -0.00305, -0.00465, -0.00585,
      -0.00664, -0.00699, -0.00693, -0.00651, -0.00581, -0.00493, -0.00398,
      -0.00304, -0.00215, -0.00136, -0.00068, -0.00008,  0.00042,  0.00086,
       0.00124,  0.00158
    };

    // what is the max/min x-value that will fit this distribution
    // for the given parameters?
    // FWHM is 11.356 ...
    // pulse peak occurs at 37% of the array size
    const Double_t xMin = par[2] - 3.258*par[3];
    // array size is 8.806 times the FWHM
    const Double_t xMax = xMin + 8.806*par[3];
    // outside of this shape's range, return a flat pedestal
    if (x[0]<xMin || x[0]>xMax) return par[0];

    // the x-values should be scaled up to the size of the array
    const Double_t xScale = 11.356 / par[3];
    // value should shift left by...
    const Double_t xOff = 0.8806*par[3];

    // calculate where the current value should fit in the distribution
    const Double_t xVal = (x[0]-xMin) * xScale;
    // the array can only accept integer values
    const Int_t xInt = static_cast<int>(xVal);

    // quick return in case the x-value is already an integer
    if (xVal-xInt == 0) return par[0] + par[1]*pulse[xInt];

    // pedestal + height * yvalue (from the bin's lower edge)
    const Double_t arrValLow = par[0] + par[1]*pulse[xInt];
    // pedestal + height * yvalue (from the next bin in the array)
    Double_t arrValHigh = par[0]; //  pedestal or overflow
    if (xInt<99) arrValHigh += par[1]*pulse[xInt+1];

    // slope of linear interpolation
    const Double_t slope = arrValHigh - arrValLow;
    // intercept of linear interpolation
    const Double_t intercept = arrValLow - slope*xInt;

    // return the interpolated pulse height from the actual value
    return slope*xVal + intercept;
  }
    
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
    ntuple->tree()->Branch ("prob",     &prob,     "prob[2][48]/D");
    ntuple->tree()->Branch ("chgratio", &chgratio, "chgratio[2][48]/D");

    // initialize with inverted min-max
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

	    pedestal_min[gain][pmt] = 4096;
	    pedestal_max[gain][pmt] = -1;

	    height_min[gain][pmt] = 10e10;
	    height_max[gain][pmt] = -10e10;

	    phase_min[gain][pmt] = 129;
	    phase_max[gain][pmt] = -1;

	    width_min[gain][pmt] = 129;
	    width_max[gain][pmt] = -1;

	    prob_min[gain][pmt] = 10e10;
	    prob_max[gain][pmt] = -1;

	    chgratio_min[gain][pmt] = 10e10;
	    chgratio_max[gain][pmt] = -10e10;
	  } // end pmt
      } // end gain

    // histogram contains min and max in separate bins
    m_pedestal_lo_min = new TH1D ("Pedestal_lo_min",
				  "Pulse Pedestal Low", 48, 0, 48);
    m_pedestal_lo_max = new TH1D ("Pedestal_lo_max",
				  "Pulse Pedestal Low", 48, 0, 48);
    m_pedestal_hi_min = new TH1D ("Pedestal_hi_min",
				  "Pulse Pedestal High", 48, 0, 48);
    m_pedestal_hi_max = new TH1D ("Pedestal_hi_max",
				  "Pulse Pedestal High", 48, 0, 48);

    m_height_lo_min = new TH1D ("Height_lo_min",
				"Pulse Height Low", 48, 0, 48);
    m_height_lo_max = new TH1D ("Height_lo_max",
				"Pulse Height Low", 48, 0, 48);
    m_height_hi_min = new TH1D ("Height_hi_min",
				"Pulse Height High", 48, 0, 48);
    m_height_hi_max = new TH1D ("Height_hi_max",
				"Pulse Height High", 48, 0, 48);

    m_phase_lo_min = new TH1D ("Phase_lo_min",
			       "Pulse Phase Low", 48, 0, 48);
    m_phase_lo_max = new TH1D ("Phase_lo_max",
			       "Pulse Phase Low", 48, 0, 48);
    m_phase_hi_min = new TH1D ("Phase_hi_min",
			       "Pulse Phase High", 48, 0, 48);
    m_phase_hi_max = new TH1D ("Phase_hi_max",
			       "Pulse Phase High", 48, 0, 48);

    m_width_lo_min = new TH1D ("Width_lo_min",
			       "Pulse Width Low", 48, 0, 48);
    m_width_lo_max = new TH1D ("Width_lo_max",
			       "Pulse Width Low", 48, 0, 48);
    m_width_hi_min = new TH1D ("Width_hi_min",
			       "Pulse Width High", 48, 0, 48);
    m_width_hi_max = new TH1D ("Width_hi_max",
			       "Pulse Width High", 48, 0, 48);

    m_prob_lo_min = new TH1D ("Prob_lo_min",
			      "Pulse Probability Low", 48, 0, 48);
    m_prob_lo_max = new TH1D ("Prob_lo_max",
			      "Pulse Probability_Low", 48, 0, 48);
    m_prob_hi_min = new TH1D ("Prob_hi_min",
			      "Pulse Probability High", 48, 0, 48);
    m_prob_hi_max = new TH1D ("Prob_hi_max",
			      "Pulse Probability High", 48, 0, 48);

    m_chgratio_lo_min = new TH1D ("ChgRatio_lo_min",
				  "Pulse Height/Charge Low", 48, 0, 48);
    m_chgratio_lo_max = new TH1D ("ChgRatio_lo_max",
				  "Pulse Height/Charge Low", 48, 0, 48);
    m_chgratio_hi_min = new TH1D ("ChgRatio_hi_min",
				  "Pulse Height/Charge High", 48, 0, 48);
    m_chgratio_hi_max = new TH1D ("ChgRatio_hi_max",
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

    m_prob_lo_min->SetYTitle ("Min");
    m_prob_lo_max->SetYTitle ("Max");
    m_prob_hi_min->SetYTitle ("Min");
    m_prob_hi_max->SetYTitle ("Max");

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

	m_prob_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_prob_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_prob_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_prob_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);

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

    wk()->addOutput (m_prob_lo_min);
    wk()->addOutput (m_prob_lo_max);
    wk()->addOutput (m_prob_hi_min);
    wk()->addOutput (m_prob_hi_max);

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
    fit_temp->SetNpx (10000);
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
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

	    // default values are invalid (or error states)
	    pedestal[gain][pmt] = -1;
	    height  [gain][pmt] =  0;
	    phase   [gain][pmt] = -1;
	    width   [gain][pmt] = -1;
	    prob    [gain][pmt] = -1;
	    chgratio[gain][pmt] = -1;

	    // fill a histogram with samples
	    hist_temp->Reset ("icesm");
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval <= 4095 && sval > 0)
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
		prob    [gain][pmt] = fitRes->GetProb ();
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
		if (prob[gain][pmt] < prob_min[gain][pmt])
		  {
		    prob_min[gain][pmt] = prob[gain][pmt];
		  }
		if (prob[gain][pmt] > prob_max[gain][pmt])
		  {
		    prob_max[gain][pmt] = prob[gain][pmt];
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
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

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
	    if (prob_min[gain][pmt] > prob_max[gain][pmt])
	      {
		prob_min[gain][pmt] = -1;
		prob_max[gain][pmt] = -1;
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

		m_prob_hi_min->SetBinContent (pmt+1, prob_min[gain][pmt]);
		m_prob_hi_min->SetBinError (pmt+1, 0);
		m_prob_hi_max->SetBinContent (pmt+1, prob_max[gain][pmt]);
		m_prob_hi_max->SetBinError (pmt+1, 0);

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

		m_prob_lo_min->SetBinContent (pmt+1, prob_min[gain][pmt]);
		m_prob_lo_min->SetBinError (pmt+1, 0);
		m_prob_lo_max->SetBinContent (pmt+1, prob_max[gain][pmt]);
		m_prob_lo_max->SetBinError (pmt+1, 0);

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
