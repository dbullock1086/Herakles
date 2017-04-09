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
    char buffer[5];

    m_sample_lo = new TH2S ("SampleRange_lo", "SampleRange_lo",
			    48, 0, 48, 2, 0, 2);
    m_sample_hi = new TH2S ("SampleRange_hi", "SampleRange_hi",
			    48, 0, 48, 2, 0, 2);
    wk()->addOutput (m_sample_lo);
    wk()->addOutput (m_sample_hi);
    m_sample_lo->GetXaxis()->SetBinLabel (1, "Min");
    m_sample_lo->GetXaxis()->SetBinLabel (2, "Max");
    m_sample_hi->GetXaxis()->SetBinLabel (1, "Min");
    m_sample_hi->GetXaxis()->SetBinLabel (2, "Max");
    for (pmt=1; pmt<49; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);
	m_sample_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_sample_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_sample_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_sample_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);
      } // end pmt
    
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    sample_min[gain][pmt] = 4096;
	    sample_max[gain][pmt] = -1;
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleRange :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode SampleRange :: execute ()
  {
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0)
		  {
		    if (sval < samples_min[gain][pmt])
		      {
			samples_min[gain][pmt] = sval;
		      }
		    else if (sval > samples_max[gain][pmt])
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
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[i];

	    if (sample_min[gain][pmt] > sample_max[gain][pmt])
	      {
		sample_min[gain][pmt] = -1;
		sample_max[gain][pmt] = -1;
	      }

            if (gain)
	      {
		m_sample_hi->Fill (pmt, 0, sample_min[gain][pmt]);
		m_sample_hi->Fill (pmt, 1, sample_max[gain][pmt]);
	      }

	    else
	      {
		m_sample_lo->Fill (pmt, 0, sample_min[gain][pmt]);
		m_sample_lo->Fill (pmt, 1, sample_max[gain][pmt]);
	      }
	  } // end pmt
      } // end gain

    return EL::::StatusCode::SUCCESS;
  }
}
