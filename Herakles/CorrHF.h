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
    UChar_t gains = {0, 1};
    UChar_t channels = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
			40, 41, 42, 43, 44, 45, 46, 47};
    UChar_t window[2] = {0, 128};
    
    CorrHF ();
    ~CorrHF ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH2D *m_hfcorr; //!

    UChar_t gain; //!
    UChar_t pmt1; //!
    UChar_t pmt2; //!
    ULong64_t numX[2][48]; //!
    ULong64_t numXY[2][48][48]; //!
    Double_t xsum[2][48]; //!
    Double_t s2sum[2][48]; //!
    Double_t xysum[2][48][48]; //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!

    UChar_t i; //!
    UChar_t j1; //!
    UChar_t j2; //!
    UShort_t sample; //!
    Float_t sval1; //!
    Float_t sval2; //!

    ClassDef(CorrHF, 1);
  };
}

#endif
