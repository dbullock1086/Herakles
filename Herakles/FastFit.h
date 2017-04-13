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
    Bool_t gains[2];
    Bool_t channels[48];
    UShort_t window[2];
    
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

    TH1D *m_fastratio_lo_min; //!
    TH1D *m_fastratio_lo_max; //!
    TH1D *m_fastratio_hi_min; //!
    TH1D *m_fastratio_hi_max; //!

    Int_t charge; //!
    Float_t ped_hi[48]; //!
    Float_t ped_lo[48]; //!
    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t sval; //!
    Float_t ped; //!

    UShort_t gain; //!
    UShort_t pmt; //!
    UShort_t sample; //!

    Double_t fastfit[2][48];
    Double_t fastfit_min[2][48];
    Double_t fastfit_max[2][48];
    
    Double_t fastratio[2][48];
    Double_t fastratio_min[2][48];
    Double_t fastratio_max[2][48];
    
    ClassDef(FastFit, 1);
  };
}

#endif
