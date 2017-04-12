/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_SampleHF_H
#define Herakles_SampleHF_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class SampleHF : public EL::Algorithm
  {
  public:
    UShort_t gains[];
    UShort_t channels[];
    UShort_t window[2];
    
    SampleHF ();
    ~SampleHF ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    EL::NTupleSvc *ntuple; //!
    TTree *m_tree; //!

    TH1D *m_hfmean_lo_min; //!
    TH1D *m_hfmean_lo_max; //!
    TH1D *m_hfmean_hi_min; //!
    TH1D *m_hfmean_hi_max; //!

    TH1D *m_hfstd_lo_min; //!
    TH1D *m_hfstd_lo_max; //!
    TH1D *m_hfstd_hi_min; //!
    TH1D *m_hfstd_hi_max; //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t sval; //!

    Double_t hfmean[2][48]; //!
    Double_t hfstd[2][48]; //!
    Double_t hfmean_min[2][48]; //!
    Double_t hfmean_max[2][48]; //!
    Double_t hfstd_min[2][48]; //!
    Double_t hfstd_max[2][48]; //!

    UShort_t i; //!
    UShort_t j; //!
    UShort_t gain; //!
    UShort_t pmt; //!
    UShort_t sample; //!
    Double_t xval; //!
    Double_t x2val; //!
    Double_t mean; //!
    Double_t std; //!
    UShort_t CRC; //!
    Double_t x2; //!
    Double_t xm; //!
    Double_t m2; //!

    ClassDef(SampleHF, 1);
  };
}

#endif
