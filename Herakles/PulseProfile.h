/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_PulseProfile_H
#define Herakles_PulseProfile_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <TTree.h>
#include <TProfile.h>

namespace TD
{
  class PulseProfile : public EL::Algorithm
  {
  public:
    std::string& histname, histtitle;
    UShort_t gain;
    UShort_t pmt;
    UShort_t window[2];
    
    PulseProfile ();
    ~PulseProfile ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();

    TTree *m_tree; //!

    TProfile *m_pulseprf; //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t sval; //!

    UShort_t sample; //!

    ClassDef(PulseProfile, 1);
  };
}

#endif
