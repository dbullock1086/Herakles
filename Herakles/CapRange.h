/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_CapRange_H
#define Herakles_CapRange_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class CapRange : public EL::Algorithm
  {
  public:
    CapRange ();
    ~CapRange ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH1D *m_cap; //!
    TH1D *m_capsteps; //!

    Int_t cap; //!

    Int_t cap_min; //!
    Int_t cap_max; //!

    Int_t cap_prev; //!
    UShort_t cap_steps; //!

    ClassDef(CapRange, 1);
  };
}

#endif
