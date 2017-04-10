/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_ChargeRange_H
#define Herakles_ChargeRange_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class ChargeRange : public EL::Algorithm
  {
  public:
    ChargeRange ();
    ~ChargeRange ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH1D *m_charge; //!

    Int_t charge; //!

    Int_t charge_min;
    Int_t charge_max; //!

    ClassDef(ChargeRange, 1);
  };
}

#endif
