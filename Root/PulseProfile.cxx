/*
 * Herakles
 * by Daniel Bullock, 2013-2017
 * https://github.com/dbullock1086/Herakles
 */

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <Herakles/PulseProfile.h>

ClassImp(TD::PulseProfile)

namespace TD
{
  PulseProfile :: PulseProfile () {}

  PulseProfile :: ~PulseProfile () {}

  EL::StatusCode PulseProfile :: initialize ()
  {
    // label buffer
    char buffer[9];

    // initialize profile to fill
    m_pulseprf = new TProfile (histname, histtitle, 128, 0, 128);
    m_pulseprf->SetYTitle ("Average Sample");
    for (sample=0; sample<128; sample++)
      {
	sprintf (buffer, "Sample%d", sample);
	m_pulseprf->GetXaxis()->SetBinLabel (sample + 1, buffer);
      }

    // add the histograms to EL output
    wk()->addOutput (m_pulseprf);

    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PulseProfile :: changeInput (bool firstFile)
  {
    // refresh the TTree reference
    m_tree = wk()->tree();
    m_tree->ResetBit (TTree::kForceRead);
    return EL::StatusCode::SUCCESS;
  }

  EL::StatusCode PulseProfile :: execute ()
  {
    // refresh the variable reference per algorithm
    m_tree->SetBranchAddress ("samples_hi", &samples_hi);
    m_tree->SetBranchAddress ("samples_lo", &samples_lo);
    m_tree->GetEntry (wk()->treeEntry());

    // fill the profile with samples
    for (sample=window[0]; sample<window[1]; sample++)
      {
	if (gain) sval = samples_hi[pmt][sample];
	else      sval = samples_lo[pmt][sample];
	if (sval <= 4095 && sval > 0)
	  {
	    m_pulseprf->Fill (sample, sval);
	  }
      } // end sample

    return EL::StatusCode::SUCCESS;
  }
}
