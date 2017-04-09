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
    char buffer[5];
    
    m_ped_lo = new TH2S ("PedRange_lo", "PedRange_lo", 48, 0, 48, 2, 0, 2);
    m_ped_hi = new TH2S ("PedRange_hi", "PedRange_hi", 48, 0, 48, 2, 0, 2);
    wk()->addOutput (m_ped_lo);
    wk()->addOutput (m_ped_hi);
    m_ped_lo->GetYaxis()->SetBinLabel (1, "Min");
    m_ped_lo->GetYaxis()->SetBinLabel (2, "Max");
    m_ped_hi->GetYaxis()->SetBinLabel (1, "Min");
    m_ped_hi->GetYaxis()->SetBinLabel (2, "Max");
    for (pmt=1; pmt<49; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);
	m_ped_lo->GetXaxis()->SetBinLabel (pmt, buffer);
	m_ped_hi->GetXaxis()->SetBinLabel (pmt, buffer);
      }

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    ped_min[gain][pmt] = 4096;
	    ped_max[gain][pmt] = -1;
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRange :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRange :: execute ()
  {
    m_tree->SetBranchAddress ("ped_hi", &ped_hi);
    m_tree->SetBranchAddress ("ped_lo", &ped_lo);
    m_tree->GetEntry (wk()->treeEntry());
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    if (gain) ped = ped_hi[pmt];
	    else ped = ped_lo[pmt];
	    if (ped < ped_min[gain][pmt]) ped_min[gain][pmt] = ped;
	    else if (ped > ped_max[gain][pmt]) ped_max[gain][pmt] = ped;
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRange :: finalize ()
  {
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[i];

	    if (ped_min[gain][pmt] > ped_max[gain][pmt])
	      {
		ped_min[gain][pmt] = -1;
		ped_max[gain][pmt] = -1;
	      }

	    if (gain)
	      {
		m_ped_hi->Fill (pmt, 0, ped_min[gain][pmt]);
		m_ped_hi->Fill (pmt, 1, ped_max[gain][pmt]);
	      }

	    else
	      {
		m_ped_lo->Fill (pmt, 0, ped_min[gain][pmt]);
		m_ped_lo->Fill (pmt, 1, ped_max[gain][pmt]);
	      }
	  } // end pmt
      } // end gain

    return EL::::StatusCode::SUCCESS;
  }
}
