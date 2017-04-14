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
#include <TH1D.h>
#include <TF1.h>

namespace TD
{
  class PulseFit : public EL::Algorithm
  {
  public:
    Bool_t gains[2];
    Bool_t channels[48];
    UShort_t window[2];
    
    PulseFit ();
    ~PulseFit ();
    static Double_t PulseShape (Double_t *x, Double_t *par);

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

    TH1D *m_pedestal_lo_min; //!
    TH1D *m_pedestal_lo_max; //!
    TH1D *m_pedestal_hi_min; //!
    TH1D *m_pedestal_hi_max; //!

    TH1D *m_height_lo_min; //!
    TH1D *m_height_lo_max; //!
    TH1D *m_height_hi_min; //!
    TH1D *m_height_hi_max; //!

    TH1D *m_phase_lo_min; //!
    TH1D *m_phase_lo_max; //!
    TH1D *m_phase_hi_min; //!
    TH1D *m_phase_hi_max; //!

    TH1D *m_width_lo_min; //!
    TH1D *m_width_lo_max; //!
    TH1D *m_width_hi_min; //!
    TH1D *m_width_hi_max; //!

    TH1D *m_chisqr_lo_min; //!
    TH1D *m_chisqr_lo_max; //!
    TH1D *m_chisqr_hi_min; //!
    TH1D *m_chisqr_hi_max; //!

    TH1D *m_ndf_lo_min; //!
    TH1D *m_ndf_lo_max; //!
    TH1D *m_ndf_hi_min; //!
    TH1D *m_ndf_hi_max; //!

    TH1D *m_chgratio_lo_min; //!
    TH1D *m_chgratio_lo_max; //!
    TH1D *m_chgratio_hi_min; //!
    TH1D *m_chgratio_hi_max; //!

    Float_t charge; //!
    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t ped_hi[48]; //!
    Float_t ped_lo[48]; //!
    Float_t sval; //!
    Float_t ped; //!

    UShort_t gain; //!
    UShort_t pmt; //!
    UShort_t sample; //!

    Double_t pedestal[2][48]; //!
    Double_t pedestal_min[2][48]; //!
    Double_t pedestal_max[2][48]; //!

    Double_t height[2][48]; //!
    Double_t height_min[2][48]; //!
    Double_t height_max[2][48]; //!

    Double_t width[2][48]; //!
    Double_t width_min[2][48]; //!
    Double_t width_max[2][48]; //!

    Double_t phase[2][48]; //!
    Double_t phase_min[2][48]; //!
    Double_t phase_max[2][48]; //!

    Double_t chisqr[2][48]; //!
    Double_t chisqr_min[2][48]; //!
    Double_t chisqr_max[2][48]; //!

    Short_t ndf[2][48]; //!
    Short_t ndf_min[2][48]; //!
    Short_t ndf_max[2][48]; //!

    Double_t chgratio[2][48]; //!
    Double_t chgratio_min[2][48]; //!
    Double_t chgratio_max[2][48]; //!

    UShort_t binrange;
    UShort_t binmin;
    UShort_t binmax;
    Double_t approx;
    UShort_t xbin;
    Double_t minval;
    Double_t maxval;
    Bool_t pos;

    ClassDef(PulseFit, 1);
  };
}

#endif
