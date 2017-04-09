/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/PedRatio.h>
#include <TMath.h>

ClassImp(TD::PedRatio)

namespace TD
{
  PedRatio :: PedRatio () {}

  PedRatio :: ~PedRatio () {}

  EL::StatusCode SampleHF :: initialize ()
  {
    char buffer[5];
    
    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("pedratio", &pedratio, "pedratio[2][48][128]/D");

    m_pedratio_lo = new TH2D ("PedRatio_lo", "PedRatio_lo",
			      48, 0, 48, 2, 0, 2);
    m_pedratio_hi = new TH2D ("PedRatio_hi", "PedRatio_hi",
			      48, 0, 48, 2, 0, 2);
    wk()->addOutput (m_pedratio_lo);
    wk()->addOutput (m_pedratio_hi);
    m_pedratio_lo->GetYaxis()->SetBinLabel (1, "Min");
    m_pedratio_lo->GetYaxis()->SetBinLabel (2, "Max");
    m_pedratio_hi->GetYaxis()->SetBinLabel (1, "Min");
    m_pedratio_hi->GetYaxis()->SetBinLabel (2, "Max");
    for (pmt=1; pmt<49; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);
	m_pedratio_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedratio_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedratio_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_pedratio_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);
      } // end pmt
    
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    ped_min[gain][pmt] = 4096;
	    ped_max[gain][pmt] = -1;
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRatio :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedRatio :: execute ()
  {
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->SetBranchAddress ("ped_hi",     &ped_hi);
    m_tree->SetBranchAddress ("ped_lo",     &ped_lo);
    m_tree->GetEntry (wk()->treeEntry());
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    if (gain) ped = ped_hi[pmt];
	    else      ped = ped_lo[pmt];
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval < 4096 && sval >= 0 && ped != 0)
		  {
		    pedratio[gain][pmt][sample] = sval / ped;
		  }
		else pedratio[gain][pmt][sample] = -1;
		if (pedratio[gain][pmt][sample] < pedratio_min[gain][pmt])
		  {
		    pedratio_min[gain][pmt] = pedratio[gain][pmt][sample];
		  }
		else if (pedratio[gain][pmt][sample] > pedratio_max[gain][pmt])
		  {
		    pedratio_max[gain][pmt] = pedratio[gain][pmt][sample];
		  }
	      } // end sample
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PedratioRatio :: finalize ()
  {
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[i];

	    if (pedratio_min[gain][pmt] > pedratio_max[gain][pmt])
	      {
		pedratio_min[gain][pmt] = -1;
		pedratio_max[gain][pmt] = -1;
	      }

	    if (gain)
	      {
		m_pedratio_hi->Fill (pmt, 0, pedratio_min[gain][pmt]);
		m_pedratio_hi->Fill (pmt, 1, pedratio_max[gain][pmt]);
	      }
	    else
	      {
		m_pedratio_lo->Fill (pmt, 0, pedratio_min[gain][pmt]);
		m_pedratio_lo->Fill (pmt, 1, pedratio_max[gain][pmt]);
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }
}
