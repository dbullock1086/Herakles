/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#ifndef Herakles_PedRange_H
#define Herakles_PedRange_H

#include <Herakles/Global.h>
#include <EventLoop/Algorithm.h>
#include <TTree.h>
#include <TH1D.h>

namespace TD
{
  class PedRange : public EL::Algorithm
  {
  public:
    UShort_t gains[];
    UShort_t channels[];
    
    PedRange ();
    ~PedRange ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    TTree *m_tree; //!

    TH1D *m_ped_lo_min; //!
    TH1D *m_ped_lo_max; //!
    TH1D *m_ped_hi_min; //!
    TH1D *m_ped_hi_max; //!
    TH1D *m_pedsteps_lo; //!
    TH1D *m_pedsteps_hi; //!

    Float_t ped_hi[48]; //!
    Float_t ped_lo[48]; //!
    Float_t ped; //!
    Float_t ped_min[2][48]; //!
    Float_t ped_max[2][48]; //!

    UShort_t i; //!
    UShort_t j; //!
    UShort_t gain; //!
    UShort_t pmt; //!

    Float_t ped_prev[2][48]; //!
    Float_t ped_steps[2][48]; //!

    ClassDef(PedRange, 1);
  };
}

#endif
