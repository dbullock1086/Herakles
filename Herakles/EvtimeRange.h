/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_EvtimeRange_H
#define Herakles_EvtimeRange_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class EvtimeRange : public EL::Algorithm
  {
  public:
    EvtimeRange ();
    ~EvtimeRange ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH1D *m_evtime; //!

    Int_t evtime; //!

    Int_t evtime_min; //!
    Int_t evtime_max; //!

    ClassDef(EvtimeRange, 1);
  };
}

#endif
