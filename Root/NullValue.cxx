/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/NullValue.h>
#include <TMath.h>

ClassImp(TD::NullValue)

namespace TD
{
  NullValue :: NullValue () {}

  NullValue :: ~NullValue () {}

  EL::StatusCode NullValue :: initialize ()
  {
    // label buffer
    char buffer[5];
    
    // new branch added to raw data
    ntuple = EL::getNTupleSvc (wk(), "ntuple");
    ntuple->tree()->Branch ("null", &null, "null[2][48]/s");

    // initialize with inverted min-max
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;
	    null_min[gain][pmt] = 129;
	    null_max[gain][pmt] = -1;
	  } // end pmt
      } // end gain

    // histogram contains min and max in separate bins
    m_null_lo_min = new TH1D ("Null_lo_min", "Null Low",
				    48, 0, 48);
    m_null_lo_max = new TH1D ("Null_lo_max", "Null Low",
				    48, 0, 48);
    m_null_hi_min = new TH1D ("Null_hi_min", "Null High",\
				    48, 0, 48);
    m_null_hi_max = new TH1D ("Null_hi_max", "Null High",\
				    48, 0, 48);

    m_null_lo_min->SetYTitle ("Min");
    m_null_lo_max->SetYTitle ("Max");
    m_null_hi_min->SetYTitle ("Min");
    m_null_hi_max->SetYTitle ("Max");

    for (pmt=0; pmt<48; pmt++)
      {
	sprintf (buffer, "PMT%d", pmt);
	m_null_lo_min->GetXaxis()->SetBinLabel (pmt + 1, buffer);
	m_null_lo_max->GetXaxis()->SetBinLabel (pmt + 1, buffer);
	m_null_hi_min->GetXaxis()->SetBinLabel (pmt + 1, buffer);
	m_null_hi_max->GetXaxis()->SetBinLabel (pmt + 1, buffer);
      }
    
    // add the histograms to EL output
    wk()->addOutput (m_null_lo_min);
    wk()->addOutput (m_null_lo_max);
    wk()->addOutput (m_null_hi_min);
    wk()->addOutput (m_null_hi_max);

    return EL::StatusCode::SUCCESS;
    }

  EL::StatusCode NullValue :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode NullValue :: execute ()
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
	    null[gain][pmt] = 0;
	    for (sample=window[0]; sample<window[1]; sample++)
	      {
		if (gain) sval = samples_hi[pmt][sample];
		else      sval = samples_lo[pmt][sample];
		if (sval == 0) null[gain][pmt]++;
	      } // end sample
	    
            // check if value exceeds range
	    if (null[gain][pmt] < null_min[gain][pmt])
	      {
		null_min[gain][pmt] = null[gain][pmt];
	      }
	    if (null[gain][pmt] > null_max[gain][pmt])
	      {
		null_max[gain][pmt] = null[gain][pmt];
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode NullValue :: finalize ()
  {
    // loop through gain and PMT
    for (gain=0; gain<2; gain++)
      {
	if (!gains[gain]) continue;
	for (pmt=0; pmt<48; pmt++)
	  {
	    if (!channels[pmt]) continue;

            // error state: min is still greater than max
	    if (null_min[gain][pmt] > null_max[gain][pmt])
	      {
		null_min[gain][pmt] = -1;
		null_max[gain][pmt] = -1;
	      }

            // set min-max bin contents in histograms
	    if (gain)
	      {
		m_null_hi_min->SetBinContent (pmt + 1, null_min[gain][pmt]);
		m_null_hi_min->SetBinError (pmt + 1, 0);
		m_null_hi_max->SetBinContent (pmt + 1, null_max[gain][pmt]);
		m_null_hi_max->SetBinError (pmt + 1, 0);
	      }
	    else
	      {
		m_null_lo_min->SetBinContent (pmt + 1, null_min[gain][pmt]);
		m_null_lo_min->SetBinError (pmt + 1, 0);
		m_null_lo_max->SetBinContent (pmt + 1, null_max[gain][pmt]);
		m_null_lo_max->SetBinError (pmt + 1, 0);
	      }
	  } // end pmt
      } // end gain

    return EL::StatusCode::SUCCESS;
  }
}
