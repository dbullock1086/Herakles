/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_FastFit_H
#define Herakles_FastFit_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>

namespace TD
{
  class FastFit : public EL::Algorithm
  {
  public:
    UChar_t gains = {0, 1};
    UChar_t channels = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
			40, 41, 42, 43, 44, 45, 46, 47};
    UChar_t window[2] = {0, 128};
    
    FastFit ();
    ~FastFit ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    EL::NTupleSvc *ntuple; //!
    TTree *m_tree;         //!
    TH1D  *hist_temp;      //!
    TF1   *fit_temp;       //!
    TF1   *fitRes;         //!

    TH1D *m_fastfit_lo_min; //!
    TH1D *m_fastfit_lo_max; //!
    TH1D *m_fastfit_hi_min; //!
    TH1D *m_fastfit_hi_max; //!

    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t ped_hi[48]; //!
    Float_t ped_lo[48]; //!
    Float_t sval; //!
    Float_t ped; //!

    UChar_t i; //!
    UChar_t j; //!
    UChar_t gain; //!
    UChar_t pmt; //!
    UShort sample; //!

    Double_t fastfit[2][48];
    Double_t fastfit_min[2][48];
    Double_t fastfit_max[2][48];
    
    ClassDef(FastFit, 1);
  };
}

#endif
