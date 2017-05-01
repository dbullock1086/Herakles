#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *
from Branches import Branches

class HistReader (Branches):
    def OpenFile (self, fname, option='read'):
        #### open the eventloop output file
        self.rfile = ROOT.TFile.Open (fname, option)
        pass

    def InitRange (self):
        hist = self.rfile.Get ('EvtRange')
        self.evtmin = hist.GetBinContent (1)
        self.evtmax = hist.GetBinContent (2)
        self.nevts = self.evtmin - self.evtmax
        hist.IsA().Destructor (hist)
        pass

    def GetRange (self, var, gain=0, pmt=0):
        #### get the number of bins and range for a particular variable
        #    for a particular gain and channel
        name = self.VarRange (var)

        if var == 'evt':
            minval = self.evtmin
            maxval = self.evtmax
            pass
        elif self.VarChannel (var):
            if gain:
                hist = self.rfile.Get (name + '_hi_min')
                minval = hist.GetBinContent (pmt + 1)
                hist.IsA().Destructor (hist)
                hist = self.rfile.Get (name + '_hi_max')
                maxval = hist.GetBinContent (pmt + 1)
                hist.IsA().Destructor (hist)
                pass
            else:
                hist = self.rfile.Get (name + '_lo_min')
                minval = hist.GetBinContent (pmt + 1)
                hist.IsA().Destructor (hist)
                hist = self.rfile.Get (name + '_lo_max')
                maxval = hist.GetBinContent (pmt + 1)
                hist.IsA().Destructor (hist)
                pass
            pass
        else:
            hist = self.rfile.Get (name)
            minval = hist.GetBinContent (1)
            maxval = hist.GetBiNContent (2)
            hist.IsA().Destructor (hist)
            pass

        name = self.VarSteps (var)
        if name:
            if self.VarMinMax (var):
                if gain:
                    hist = self.rfile.Get (name + '_hi')
                    steps = hist.GetBinContent (pmt + 1)
                    hist.IsA().Destructor (hist)
                    pass
                else:
                    hist = self.rfile.Get (name + '_lo')
                    minval = hist.GetBinContent (pmt + 1)
                    hist.IsA().Destructor (hist)
                    pass
                pass
            else:
                hist = self.rfile.Get (name)
                steps = hist.GetBinContent (1)
                hist.IsA().Destructor (hist)
                pass
            pass
        else:
            steps = int(ROOT.TMath.Sqrt(self.nevts))
            pass

        if steps >= 200: steps = 200
        valsize = maxval - minval
        binsize = valsize / (steps - 1)
        maxval += binsize
        return [steps, minval, maxval]

    def CloseFile (self):
        #### always be tidy
        self.rfile.Flush ()
        self.rfile.Close ()
        pass
    pass
