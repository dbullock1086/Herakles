/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_Saturation_H
#define Herakles_Saturation_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class Saturation : public EL::Algorithm
  {
  public:
    Bool_t gains[2];
    Bool_t channels[48];
    UShort_t window[2];
    
    Saturation ();
    ~Saturation ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    EL::NTupleSvc *ntuple; //!
    TTree *m_tree; //!

    TH1D *m_saturation_lo_min; //!
    TH1D *m_saturation_lo_max; //!
    TH1D *m_saturation_hi_min; //!
    TH1D *m_saturation_hi_max; //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t sval; //!

    UShort_t saturation[2][48]; //!
    Short_t saturation_min[2][48]; //!
    Short_t saturation_max[2][48]; //!

    UShort_t gain; //!
    UShort_t pmt; //!
    UShort_t sample; //!

    ClassDef(Saturation, 1);
  };
}

#endif
