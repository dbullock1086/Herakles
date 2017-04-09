/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/CRCError.h>
#include <TMath.h>

ClassImp(TD::CRCError)

namespace TD
{
  CRCError :: CRCError () {}

  CRCError :: ~CRCError () {}

  EL::StatusCode CRCError :: initialize ()
  {
    char buffer[5];
    
    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("crc", &crc, "crc[2][48]/s");

    m_crc_lo = new TH2S ("CRCError_lo", "CRCError_lo", 48, 0, 48, 2, 0, 2);
    m_crc_hi = new TH2S ("CRCError_hi", "CRCError_hi", 48, 0, 48, 2, 0, 2);
    wk()->addOutput (m_crc_lo);
    wk()->addOutput (m_crc_hi);
    m_crc_lo->GetYaxis()->SetBinLabel (1, "Min");
    m_crc_lo->GetYaxis()->SetBinLabel (2, "Max");
    m_crc_hi->GetYaxis()->SetBinLabel (1, "Min");
    m_crc_hi->GetYaxis()->SetBinLabel (2, "Max");
    for (pmt=1; pmt<49; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);
	m_crc_lo->GetXaxis()->SetBinLabel (pmt, buffer);
	m_crc_hi->GetXaxis()->SetBinLabel (pmt, buffer);
      }
    
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    crc_min[gain][pmt] = 129;
	    crc_max[gain][pmt] = -1;
	  }
      }

    return EL::StatusCode::SUCCESS;
    }

  EL::StatusCode CRCError :: changeInput (bool firstFile)
  {
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CRCError :: execute ()
  {
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[j];
	    crc[gain][pmt] = 0;
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval > 4095 || sval < 0) crc[gain][pmt]++;
	      } // end sample
	    
	    if (crc[gain][pmt] < crc_min[gain][pmt])
	      {
		crc_min[gain][pmt] = crc[gain][pmt];
	      }
	    else if (crc[gain][pmt] > crc_max[gain][pmt])
	      {
		crc_max[gain][pmt] = crc[gain][pmt];
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CRCError :: finalize ()
  {
    for (i=0; i<sizeof(gains); i++)
      {
	gain = gains[i];
	for (j=0; j<sizeof(channels); j++)
	  {
	    pmt = channels[i];
	    if (crc_min[gain][pmt] > crc_max[gain][pmt])
	      {
		crc_min[gain][pmt] = -1;
		crc_max[gain][pmt] = -1;
	      }
	    if (gain)
	      {
		m_crc_hi->Fill (pmt, 0, crc_min[gain][pmt]);
		m_crc_hi->Fill (pmt, 1, crc_max[gain][pmt]);
	      }
	    else
	      {
		m_crc_lo->Fill (pmt, 0, crc_min[gain][pmt]);
		m_crc_lo->Fill (pmt, 1, crc_max[gain][pmt]);
	      }
	  } // end pmt
      } // end gain
    return EL::StatusCode::SUCCESS;
  }
}
