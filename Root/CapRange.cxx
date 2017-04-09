/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/CapRange.h>

ClassImp(TD::CapRange)

namespace TD
{
  CapRange :: CapRange () {}

  CapRange :: ~CapRange () {}

  EL::StatusCode CapRange :: initialize ()
  {
    cap_min = 4096;
    cap_max = -1;

    m_cap = new TH1S ("CapRange", "CapRange", 2, 0, 2);
    wk()->addOutput (m_cap);
    m_cap->GetXaxis()->SetBinLabel (1, "cap_min");
    m_cap->GetXaxis()->SetBinLabel (2, "cap_max");

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CapRange :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CapRange :: execute ()
  {
    m_tree->SetBranchAddress ("cap", &cap);
    m_tree->GetEntry (wk()->treeEntry());

    if (cap < cap_min)      cap_min = cap;
    else if (cap > cap_max) cap_max = cap;

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CapRange :: finalize ()
  {
    if (cap_min > cap_max)
      {
	cap_min = -1;
	cap_max = -1;
      }

    m_cap->SetBinContent (1, cap_min);
    m_cap->SetBinError (1, 0);
    m_cap->SetBinContent (2, cap_max);
    m_cap->SetBinError (2, 0);

    return EL::::StatusCode::SUCCESS;
  }
}
