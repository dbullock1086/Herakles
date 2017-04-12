/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/EvtRange.h>

ClassImp(TD::EvtRange)

namespace TD
{
  EvtRange :: EvtRange () {}

  EvtRange :: ~EvtRange () {}

  EL::StatusCode EvtRange :: initialize ()
  {
    // initialize with inverted min-max
    evt_min = 2147483647;
    evt_max = -1;

    // histogram contains min and max in separate bins
    m_evt = new TH1D ("EvtRange", "Evt Range", 2, 0, 2);
    m_evt->GetXaxis()->SetBinLabel (1, "Min");
    m_evt->GetXaxis()->SetBinLabel (2, "Max");

    // add the histogram to EL output
    wk()->addOutput (m_evt);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtRange :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtRange :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("evt", &evt);
    m_tree->GetEntry (wk()->treeEntry());

    // check if value exceeds range
    if (evt < evt_min) evt_min = evt;
    if (evt > evt_max) evt_max = evt;

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtRange :: finalize ()
  {
    // error state: min is still greater than max
    if (evt_min > evt_max)
      {
	evt_min = -1;
	evt_max = -1;
      }

    // set min-max bin contents in histogram
    m_evt->SetBinContent (1, evt_min);
    m_evt->SetBinError (1, 0);
    m_evt->SetBinContent (2, evt_max);
    m_evt->SetBinError (2, 0);

    return EL::StatusCode::SUCCESS;
  }
}
