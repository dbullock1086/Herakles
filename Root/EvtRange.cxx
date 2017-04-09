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

  EL::StatusCode EvtRange :: histInitialize ()
  {
    evt_min = 10e10;
    evt_max = -1;
    m_evt = new TH1S ("EvtRange", "EvtRange", 2, 0, 2);
    wk()->addOutput (m_evt);
    m_evt->GetXaxis()->SetBinLabel (1, "evt_min");
    m_evt->GetXaxis()->SetBinLabel (2, "evt_max");

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtRange :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtRange :: execute ()
  {
    m_tree->SetBranchAddress ("evt", &evt);
    m_tree->GetEntry (wk()->treeEntry());

    if (evt < evt_min)      evt_min = evt;
    else if (evt > evt_max) evt_max = evt;

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode EvtRange :: finalize ()
  {
    if (evt_min > evt_max)
      {
	evt_min = -1;
	evt_max = -1;
      }

    m_evt->SetBinContent (1, evt_min);
    m_evt->SetBinError (1, 0);
    m_evt->SetBinContent (2, evt_max);
    m_evt->SetBinError (2, 0);

    return EL::::StatusCode::SUCCESS;
  }
}
