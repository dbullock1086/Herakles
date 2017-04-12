/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_PedRatio_H
#define Herakles_PedRatio_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class PedRatio : public EL::Algorithm
  {
  public:
    UShort_t gains[];
    UShort_t channels[];
    UShort_t window[2];
    
    PedRatio ();
    ~PedRatio ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    EL::NTupleSvc *ntuple; //!
    TTree *m_tree; //!

    TH1D *m_pedratio_lo_min; //!
    TH1D *m_pedratio_lo_max; //!
    TH1D *m_pedratio_hi_min; //!
    TH1D *m_pedratio_hi_max; //!

    Float_t ped_hi[48]; //!
    Float_t ped_lo[48]; //!
    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t ped; //!
    Float_t sval; //!

    Double_t pedratio[2][48]; //!
    Double_t pedratio_min[2][48]; //!
    Double_t pedratio_max[2][48]; //!

    UShort_t i; //!
    UShort_t j; //!
    UShort_t gain; //!
    UShort_t pmt; //!
    UShort_t sample; //!

    ClassDef(PedRatio, 1);
  };
}

#endif
