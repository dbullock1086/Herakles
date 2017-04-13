/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_CorrHF_H
#define Herakles_CorrHF_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <TTree.h>
#include <TH2D.h>

namespace TD
{
  class CorrHF : public EL::Algorithm
  {
  public:
    Bool_t gains[2];
    Bool_t channels[48];
    UShort_t window[2];
    
    CorrHF ();
    ~CorrHF ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH2D *m_hfcorr; //!

    UShort_t gain; //!
    UShort_t pmt1; //!
    UShort_t pmt2; //!
    ULong64_t numX[2][48]; //!
    ULong64_t numXY[2][48][48]; //!
    Double_t xsum[2][48]; //!
    Double_t x2sum[2][48]; //!
    Double_t xysum[2][48][48]; //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!

    UShort_t sample; //!
    Float_t sval1; //!
    Float_t sval2; //!

    ClassDef(CorrHF, 1);
  };
}

#endif
