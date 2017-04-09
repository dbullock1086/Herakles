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
    charge_min = 0;
    charge_max = 0;

    m_charge = new TH1S ("ChargeRange", "ChargeRange", 2, 0, 2);
    wk()->addOutput (m_charge);
    m_charge->GetXaxis()->SetBinLabel (1, "charge_min");
    m_charge->GetXaxis()->SetBinLabel (2, "charge_max");

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode ChargeRange :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode ChargeRange :: execute ()
  {
    m_tree->SetBranchAddress ("charge", &charge);
    m_tree->GetEntry (wk()->treeEntry());

    if (charge < charge_min)      charge_min = charge;
    else if (charge > charge_max) charge_max = charge;

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode ChargeRange :: finalize ()
  {
    if (charge_min > charge_max)
      {
	charge_min = -1;
	charge_max = -1;
      }

    m_charge->SetBinContent (1, charge_min);
    m_charge->SetBinError (1, 0);
    m_charge->SetBinContent (2, charge_max);
    m_charge->SetBinError (2, 0);

    return EL::::StatusCode::SUCCESS;
  }
}
