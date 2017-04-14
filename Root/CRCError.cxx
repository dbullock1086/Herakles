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
    // label buffer
    char buffer[5];
    
    // new branch added to raw data
    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("crc", &crc, "crc[2][48]/s");

    // initialize with inverted min-max
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    crc_min[gain][pmt] = 129;
	    crc_max[gain][pmt] = -1;
	  } // end pmt
      } // end gain

    // histogram contains min and max in separate bins
    m_crc_lo_min = new TH1D ("CRCError_lo_min", "CRC Low",
			     48, 0, 48);
    m_crc_lo_max = new TH1D ("CRCError_lo_max", "CRC Low",
			     48, 0, 48);
    m_crc_hi_min = new TH1D ("CRCError_hi_min", "CRC High",\
			     48, 0, 48);
    m_crc_hi_max = new TH1D ("CRCError_hi_max", "CRC High",\
			     48, 0, 48);

    m_crc_lo_min->SetYTitle ("Min");
    m_crc_lo_max->SetYTitle ("Max");
    m_crc_hi_min->SetYTitle ("Min");
    m_crc_hi_max->SetYTitle ("Max");

    for (pmt=1; pmt<49; pmt++) // notice number convention
      {
	sprintf (buffer, "PMT%d", pmt);

	m_crc_lo_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_crc_lo_max->GetXaxis()->SetBinLabel (pmt, buffer);
	m_crc_hi_min->GetXaxis()->SetBinLabel (pmt, buffer);
	m_crc_hi_max->GetXaxis()->SetBinLabel (pmt, buffer);
      }
    
    // add the histograms to EL output
    wk()->addOutput (m_crc_lo_min);
    wk()->addOutput (m_crc_lo_max);
    wk()->addOutput (m_crc_hi_min);
    wk()->addOutput (m_crc_hi_max);

    return EL::StatusCode::SUCCESS;
    }

  EL::StatusCode CRCError :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CRCError :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    crc[gain][pmt] = 0;
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval > 4095 || sval < 0) crc[gain][pmt]++;
	      } // end sample
	    
            // check if value exceeds range
	    if (crc[gain][pmt] < crc_min[gain][pmt])
	      {
		crc_min[gain][pmt] = crc[gain][pmt];
	      }
	    if (crc[gain][pmt] > crc_max[gain][pmt])
	      {
		crc_max[gain][pmt] = crc[gain][pmt];
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode CRCError :: finalize ()
  {
    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

            // error state: min is still greater than max
	    if (crc_min[gain][pmt] > crc_max[gain][pmt])
	      {
		crc_min[gain][pmt] = -1;
		crc_max[gain][pmt] = -1;
	      }

            // set min-max bin contents in histograms
	    if (gain)
	      {
		m_crc_hi_min->SetBinContent (pmt + 1, crc_min[gain][pmt]);
		m_crc_hi_min->SetBinError (pmt + 1, 0);
		m_crc_hi_max->SetBinContent (pmt + 1, crc_max[gain][pmt]);
		m_crc_hi_max->SetBinError (pmt + 1, 0);
	      }
	    else
	      {
		m_crc_lo_min->SetBinContent (pmt + 1, crc_min[gain][pmt]);
		m_crc_lo_min->SetBinError (pmt + 1, 0);
		m_crc_lo_max->SetBinContent (pmt + 1, crc_max[gain][pmt]);
		m_crc_lo_max->SetBinError (pmt + 1, 0);
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }
}
