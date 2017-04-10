/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/PedRange.h>

ClassImp(TD::PedRange)

namespace TD
{
  PedRange :: PedRange () {}

  PedRange :: ~PedRange () {}

  EL::StatusCode PedRange :: initialize ()
  {
    // label buffer
    char buffer[5];

    // initialize with inverted min-max
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    ped_min[gain][pmt] = 4096;
	    ped_max[gain][pmt] = -1;
	    ped_prev[gain][pmt] = -1;
	    ped_steps[gain][pmt] = 0;
	  } // end pmt
      } // end gain

    // histogram contains min and max in separate bins
    m_ped_lo_min = new TH1D ("PedRange_lo_min", "PedRange Low", 48, 0, 48);
    m_ped_lo_max = new TH1D ("PedRange_lo_max", "PedRange Low", 48, 0, 48);
    m_ped_hi_min = new TH1D ("PedRange_hi_min", "PedRange High", 48, 0, 48);
    m_ped_hi_max = new TH1D ("PedRange_hi_max", "PedRange High", 48, 0, 48);
    m_ped_lo_min->SetYTitle ("Min");
    m_ped_lo_max->SetYTitle ("Max");
    m_ped_hi_min->SetYTitle ("Min");
    m_ped_hi_max->SetYTitle ("Max");

    m_pedsteps_lo = new TH1D ("PedSteps_lo", "Ped Steps Low", 48, 0, 48);
    m_pedsteps_hi = new TH1D ("PedSteps_hi", "Ped Steps High", 48, 0, 48);
    m_pedsteps_lo->SetYTitle ("Steps");
    m_pedsteps_hi->SetYTitle ("Steps");

    for (pmt=1; pmt<49; pmt++) // notice number convention
      {
	sprintf (buffer, "PMT%d", pmt);
	m_ped_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ped_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ped_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ped_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedsteps_lo->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedsteps_hi->GetXaxis()->SetBinLabel (pmt, buffer);
      }

    // add the histograms to EL output
    wk()->addOutput (m_ped_lo_min);
    wk()->addOutput (m_ped_lo_max);
    wk()->addOutput (m_ped_hi_min);
    wk()->addOutput (m_ped_hi_max);
    wk()->addOutput (m_pedsteps_lo);
    wk()->addOutput (m_pedsteps_hi);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRange :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRange :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("ped_hi", &ped_hi);
    m_tree->SetBranchAddress ("ped_lo", &ped_lo);
    m_tree->GetEntry (wk()->treeEntry());

    // loop through gain and PMT
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    if (gain) ped = ped_hi[pmt];
	    else ped = ped_lo[pmt];
	    // check if value exceeds range
	    if (ped < ped_min[gain][pmt]) ped_min[gain][pmt] = ped;
	    if (ped > ped_max[gain][pmt]) ped_max[gain][pmt] = ped;
	    if (ped != ped_prev[gain][pmt]) ped_steps[gain][pmt]++;
	    ped_prev[gain][pmt] = ped;
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRange :: finalize ()
  {
    // loop through gain and PMT
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[i];

	    // error state: min is still greater than max
	    if (ped_min[gain][pmt] > ped_max[gain][pmt])
	      {
		ped_min[gain][pmt] = -1;
		ped_max[gain][pmt] = -1;
	      }

	    // set min-max bin contents in histograms
	    if (gain)
	      {
		m_ped_hi_min->Fill (pmt, ped_min[gain][pmt]);
		m_ped_hi_max->Fill (pmt, ped_max[gain][pmt]);
		m_pedsteps_hi->Fill (pmt, ped_steps[gain][pmt];
	      }

	    else
	      {
		m_ped_lo_min->Fill (pmt, ped_min[gain][pmt]);
		m_ped_lo_max->Fill (pmt, ped_max[gain][pmt]);
		m_pedsteps_lo->Fill (pmt, ped_steps[gain][pmt];
	      }
	  } // end pmt
      } // end gain

    return EL::::StatusCode::SUCCESS;
  }
}
