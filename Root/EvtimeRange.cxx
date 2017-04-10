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
    // initialize with inverted min-max
    evtime_min = 10e10;
    evtime_max = -1;

    // histogram contains min and max in separate bins
    m_evtime = new TH1D ("EvtimeRange", "Evtime Range", 2, 0, 2);
    m_evtime->GetXaxis()->SetBinLabel (1, "Min");
    m_evtime->GetXaxis()->SetBinLabel (2, "Max");

    // add the histogram to EL output
    wk()->addOutput (m_evtime);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtimeRange :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtimeRange :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("evtime", &evtime);
    m_tree->GetEntry (wk()->treeEntry());

    // check if value exceeds range
    if (evtime < evtime_min) evtime_min = evtime;
    if (evtime > evtime_max) evtime_max = evtime;

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtimeRange :: finalize ()
  {
    // error state: min is still greater than max
    if (evtime_min > evtime_max)
      {
	evtime_min = -1;
	evtime_max = -1;
      }

    // set min-max bin contents in histogram
    m_evtime->SetBinContent (1, evtime_min);
    m_evtime->SetBinError (1, 0);
    m_evtime->SetBinContent (2, evtime_max);
    m_evtime->SetBinError (2, 0);

    return EL::::StatusCode::SUCCESS;
  }
}
