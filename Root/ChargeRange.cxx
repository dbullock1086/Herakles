/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/ChargeRange.h>

ClassImp(TD::ChargeRange)

namespace TD
{
  ChargeRange :: ChargeRange () {}

  ChargeRange :: ~ChargeRange () {}

  EL::StatusCode ChargeRange :: initialize ()
  {
    // initialize with inverted min-max
    charge_min = 4096;
    charge_max = -1;

    // store a previous value, count how many times it changes
    charge_prev = -1;
    charge_steps = 0;

    // histogram contains min and max in separate bins
    m_charge = new TH1D ("ChargeRange", "Charge Range", 2, 0, 2);
    m_charge->GetXaxis()->SetBinLabel (1, "Min");
    m_charge->GetXaxis()->SetBinLabel (2, "Max");

    // histogram contains number of steps
    m_chargesteps = new TH1D ("ChargeSteps", "Charge Steps", 1, 0, 1);
    m_chargesteps->GetXaxis()->SetBinLabel (1, "Steps");

    // add the histograms to EL output
    wk()->addOutput (m_charge);
    wk()->addOutput (m_chargesteps);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode ChargeRange :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode ChargeRange :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("charge", &charge);
    m_tree->GetEntry (wk()->treeEntry());

    // check if value exceeds range
    if (charge < charge_min) charge_min = charge;
    if (charge > charge_max) charge_max = charge;

    // check if value matches previous
    if (charge != charge_prev) charge_steps++;
    charge_prev = charge;

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode ChargeRange :: finalize ()
  {
    // error state: min is still greater than max
    if (charge_min > charge_max)
      {
	charge_min = -1;
	charge_max = -1;
      }

    // set min-max bin contents in histogram
    m_charge->SetBinContent (1, charge_min);
    m_charge->SetBinError (1, 0);
    m_charge->SetBinContent (2, charge_max);
    m_charge->SetBinError (2, 0);

    // set steps bin contents in histogram
    m_chargesteps->SetBinContent (1, charge_steps);
    m_chargesteps->SetBinError (1, 0);
    
    return EL::::StatusCode::SUCCESS;
  }
}
