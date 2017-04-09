/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/EvtimeRange.h>

ClassImp(TD::EvtimeRange)

namespace TD
{
  EvtimeRange :: EvtimeRange () {}

  EvtimeRange :: ~EvtimeRange () {}

  EL::StatusCode EvtimeRange :: initialize ()
  {
    evtime_min = 10e10;
    evtime_max = -1;
    m_evtime = new TH1S ("EvtimeRange", "EvtimeRange", 2, 0, 2);
    wk()->addOutput (m_evtime);
    m_evtime->GetXaxis()->SetBinLabel (1, "evtime_min");
    m_evtime->GetXaxis()->SetBinLabel (2, "evtime_max");

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtimeRange :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtimeRange :: execute ()
  {
    m_tree->SetBranchAddress ("evtime", &evtime);
    m_tree->GetEntry (wk()->treeEntry());

    if (evtime < evtime_min)      evtime_min = evtime;
    else if (evtime > evtime_max) evtime_max = evtime;

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtimeRange :: finalize ()
  {
    if (evtime_min > evtime_max)
      {
	evtime_min = -1;
	evtime_max = -1;
      }

    m_evtime->SetBinContent (1, evtime_min);
    m_evtime->SetBinError (1, 0);
    m_evtime->SetBinContent (2, evtime_max);
    m_evtime->SetBinError (2, 0);

    return EL::::StatusCode::SUCCESS;
  }
}
