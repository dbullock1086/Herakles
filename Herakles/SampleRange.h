/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_SampleRange_H
#define Herakles_SampleRange_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class SampleRange : public EL::Algorithm
  {
  public:
    UShort_t gains[];
    UShort_t channels[];
    UShort_t window[2];
    
    SampleRange ();
    ~SampleRange ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH1D *m_samples_lo_min; //!
    TH1D *m_samples_lo_max; //!
    TH1D *m_samples_hi_min; //!
    TH1D *m_samples_hi_max; //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t sval;
    Float_t samples_min[2][48]; //!
    Float_t samples_max[2][48]; //!

    UShort_t i; //!
    UShort_t j; //!
    UShort_t gain; //!
    UShort_t pmt; //!
    UShort_t sample; //!

    ClassDef(SampleRange, 1);
  };
}

#endif
