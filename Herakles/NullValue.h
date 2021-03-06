/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_NullValue_H
#define Herakles_NullValue_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class NullValue : public EL::Algorithm
  {
  public:
    Bool_t gains[2];
    Bool_t channels[48];
    UShort_t window[2];
    
    NullValue ();
    ~NullValue ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    EL::NTupleSvc *ntuple; //!
    TTree *m_tree; //!

    TH1D *m_null_lo_min; //!
    TH1D *m_null_lo_max; //!
    TH1D *m_null_hi_min; //!
    TH1D *m_null_hi_max; //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t sval; //!

    UShort_t null[2][48]; //!
    Short_t null_min[2][48]; //!
    Short_t null_max[2][48]; //!

    UShort_t gain; //!
    UShort_t pmt; //!
    UShort_t sample; //!

    ClassDef(NullValue, 1);
  };
}

#endif
