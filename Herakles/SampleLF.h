/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_SampleLF_H
#define Herakles_SampleLF_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <TTree.h>
#include <TH2D.h>

namespace TD
{
  class SampleLF : public EL::Algorithm
  {
  public:
    Bool_t gains[2];
    Bool_t channels[48];
    UShort_t window[2];
    
    SampleLF ();
    ~SampleLF ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH2D *m_lfmean_lo; //!
    TH2D *m_lfmean_hi; //!
    TH2D *m_lfstd_lo;  //!
    TH2D *m_lfstd_hi;  //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t sval; //!

    UChar_t gain; //!
    UChar_t pmt; //!
    UShort_t sample; //!

    Double_t xval[2][48][128]; //!
    Double_t x2val[2][48][128]; //!
    Double_t nonCRC[2][48][128]; //!

    ClassDef(SampleLF, 1);
  };
}

#endif
