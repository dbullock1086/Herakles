/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_PulseFit_H
#define Herakles_PulseFit_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <TTree.h>
#include <TH2S.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TF1.h>

namespace TD
{
  class PulseFit : public EL::Algorithm
  {
  public:
    UChar_t gains = {0, 1};
    UChar_t channels = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
			40, 41, 42, 43, 44, 45, 46, 47};
    UChar_t window[2] = {0, 128};
    
    PulseFit ();
    ~PulseFit ();

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

    TH2D *m_pedestal_lo; //!
    TH2D *m_pedestal_hi; //!
    TH2D *m_height_lo;   //!
    TH2D *m_height_hi;   //!
    TH2D *m_phase_lo;    //!
    TH2D *m_phase_hi;    //!
    TH2D *m_width_lo;    //!
    TH2D *m_width_hi;    //!
    TH2D *m_chisqr_lo;   //!
    TH2D *m_chisqr_hi;   //!
    TH2S *m_ndf_lo;      //!
    TH2S *m_ndf_hi;      //!
    TH2D *m_chgratio_lo; //!
    TH2D *m_chgratio_hi; //!

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

    Double_t pedestal[2][48]; //!
    Double_t height[2][48]; //!
    Double_t width[2][48]; //!
    Double_t phase[2][48]; //!
    Double_t chisqr[2][48]; //!
    Double_t chgratio[2][48]; //!
    Short_t ndf[2][48]; //!
    Double_t pedestal_min[2][48]; //!
    Double_t pedestal_max[2][48]; //!
    Double_t height_min[2][48]; //!
    Double_t height_max[2][48]; //!
    Double_t width_min[2][48]; //!
    Double_t width_max[2][48]; //!
    Double_t phase_min[2][48]; //!
    Double_t phase_max[2][48]; //!
    Double_t chisqr_min[2][48]; //!
    Double_t chisqr_max[2][48]; //!
    Short_t ndf_min[2][48]; //!
    Short_t ndf_max[2][48]; //!
    Double_t chgratio_min[2][48]; //!
    Double_t chgratio_max[2][48]; //!

    ClassDef(PulseFit, 1);
  };
}

#endif
