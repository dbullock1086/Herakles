/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/SampleRange.h>

ClassImp(TD::SampleRange)

namespace TD
{
  SampleRange :: SampleRange () {}

  SampleRange :: ~SampleRange () {}

  EL::StatusCode SampleRange :: initialize ()
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
	    samples_min[gain][pmt] = 4096;
	    samples_max[gain][pmt] = -1;
	  } // end pmt
      } // end gain

    // histogram contains min and max in separate bins
    m_samples_lo_min = new TH1D ("SampleRange_lo_min", "Sample Range Low",
				 48, 0, 48);
    m_samples_lo_max = new TH1D ("SampleRange_lo_max", "Sample Range Low",
				 48, 0, 48);
    m_samples_hi_min = new TH1D ("SampleRange_hi_min", "Sample Range High",
				 48, 0, 48);
    m_samples_hi_max = new TH1D ("SampleRange_hi_max", "Sample Range High",
				 48, 0, 48);

    m_samples_lo_min->SetYTitle ("Min");
    m_samples_lo_max->SetYTitle ("Max");
    m_samples_hi_min->SetYTitle ("Min");
    m_samples_hi_max->SetYTitle ("Max");

    for (pmt=1; pmt<49; pmt++) // notice number convention
      {
	sprintf (buffer, "PMT%d", pmt);
	m_samples_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_samples_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_samples_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_samples_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);
      } // end pmt
    
    // add the histograms to EL output
    wk()->addOutput (m_samples_lo_min);
    wk()->addOutput (m_samples_lo_max);
    wk()->addOutput (m_samples_hi_min);
    wk()->addOutput (m_samples_hi_max);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleRange :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleRange :: execute ()
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
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0)
		  {
		    // check if value exceeds range
		    if (sval < samples_min[gain][pmt])
		      {
			samples_min[gain][pmt] = sval;
		      }
		    if (sval > samples_max[gain][pmt])
		      {
			samples_max[gain][pmt] = sval;
		      }
		  }
	      } // end sample
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleRange :: finalize ()
  {
    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

            // error state: min is still greater than max
	    if (samples_min[gain][pmt] > samples_max[gain][pmt])
	      {
		samples_min[gain][pmt] = -1;
		samples_max[gain][pmt] = -1;
	      }

            // set min-max bin contents in histograms
            if (gain)
	      {
		m_samples_hi_min->Fill (pmt, samples_min[gain][pmt]);
		m_samples_hi_max->Fill (pmt, samples_max[gain][pmt]);
	      }

	    else
	      {
		m_samples_lo_min->Fill (pmt, samples_min[gain][pmt]);
		m_samples_lo_max->Fill (pmt, samples_max[gain][pmt]);
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }
}
