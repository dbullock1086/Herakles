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
#include <TH2D.h>

namespace TD
{
  class PedRatio : public EL::Algorithm
  {
  public:
    UChar_t gains = {0, 1};
    UChar_t channels = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
			40, 41, 42, 43, 44, 45, 46, 47};
    UShort_t window[2] = {0, 128};
    
    PedRatio ();
    ~PedRatio ();

  private:
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode finalize ();

    EL::NTupleSvc *ntuple; //!
    TTree *m_tree; //!

    TH2D *m_pedratio_lo; //!
    TH2D *m_pedratio_hi; //!

    Float_t ped_hi[48]; //!
    Float_t ped_lo[48]; //!
    Float_t samples_hi[48][128]; //!
    Float_t samples_lo[48][128]; //!
    Float_t ped; //!
    Float_t sval; //!

    Double_t pedratio[2][48]; //!
    Double_t pedratio_min[2][48]; //!
    Double_t pedratio_max[2][48]; //!

    UChar_t i; //!
    UChar_t j; //!
    UChar_t gain; //!
    UChar_t pmt; //!
    UShort_t sample; //!

    ClassDef(PedRatio, 1);
  };
}

#endif