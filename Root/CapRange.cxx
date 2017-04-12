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
    // initialize with inverted min-max
    cap_min = 4096;
    cap_max = -1;

    // store a previous value, count how many times it changes
    cap_prev = -1;
    cap_steps = 0;

    // histogram contains min and max in separate bins
    m_cap = new TH1D ("CapRange", "Cap Range", 2, 0, 2);
    m_cap->GetXaxis()->SetBinLabel (1, "Min");
    m_cap->GetXaxis()->SetBinLabel (2, "Max");

    // histogram contains number of steps
    m_capsteps = new TH1D ("CapSteps", "Cap Steps", 1, 0, 1);
    m_capsteps->GetXaxis()->SetBinLabel (1, "Steps");

    // add the histograms to EL output
    wk()->addOutput (m_cap);
    wk()->addOutput (m_capsteps);
    
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CapRange :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CapRange :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("cap", &cap);
    m_tree->GetEntry (wk()->treeEntry());

    // check if value exceeds range
    if (cap < cap_min) cap_min = cap;
    if (cap > cap_max) cap_max = cap;

    // check if value matches previous
    if (cap != cap_prev) cap_steps++;
    cap_prev = cap;

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CapRange :: finalize ()
  {
    // error state: min is still greater than max
    if (cap_min > cap_max)
      {
	cap_min = -1;
	cap_max = -1;
      }

    // set min-max bin contents in histogram
    m_cap->SetBinContent (1, cap_min);
    m_cap->SetBinError (1, 0);
    m_cap->SetBinContent (2, cap_max);
    m_cap->SetBinError (2, 0);

    // set steps bin contents in histogram
    m_capsteps->SetBinContent (1, cap_steps);
    m_capsteps->SetBinError (1, 0);

    return EL::StatusCode::SUCCESS;
  }
}
