/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_EvtRange_H
#define Herakles_EvtRange_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <TTree.h>
#include <TH1S.h>

namespace TD
{
  class EvtRange : public EL::Algorithm
  {
  public:
    EvtRange ();
    ~EvtRange ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH1S *m_evt; //!

    Int_t evt; //!

    Int_t evt_min; //!
    Int_t evt_max; //!

    ClassDef(EvtRange, 1);
  };
}

#endif
