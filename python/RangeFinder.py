#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *

class RangeFinder:
    def __init__ (self, fname):
        #### open the eventloop output file
        self.rfile = ROOT.TFile.Open (fname)
        pass

    def GetRange (self, tname, nevts=0):
        #### get the range for a scalar
        #    (e.g. evt or cap)
        hist = self.rfile.Get (tname)
        minval = hist.GetBinContent (1)
        maxval = hist.GetBinContent (2)
        hist.IsA().Destructor (hist)
        steps, minval, maxval = self._scale (tname, minval, maxval, nevts)
        return [steps, minval, maxval]

    def GetPMTRange (self, tname, gain, pmt, nevt=0):
        #### get the range for an array that depends on gain and/or PMT
        #    (e.g. ped_hi[48] or samples_lo[48][128])
        if gain: tname += '_hi'
        else: tname += '_lo'

        hist = self.rfile.Get (tname + '_min')
        minval = hist.GetBinContent (pmt + 1)
        hist.IsA().Destructor (hist)

        hist = self.rfile.Get (tname + '_max')
        maxval = hist.GetBinContent (pmt + 1)
        hist.IsA().Destructor (hist)

        steps, minval, maxval = self._scale (tname, minval, maxval, nevts)
        return [steps, minval, maxval]

    def _scale (self, tname, minval, maxval, nevts):
        #### determine binning
        # if nevts is specified, scale to the number of events
        if nevts: steps = int(ROOT.TMath.Sqrt (nevts))
        # otherwise, use the number of steps
        else:
            hist = self.rfile.Get (tname)
            steps = hist.GetBinContent (1)
            hist.IsA().Desctructor (hist)
            pass
        # limit the number of bins
        if steps >= 200: steps = 200
        # stretch the range to include the maximum value
        valsize = maxval - minval
        binsize = valsize / (steps - 1)
        maxval += binsize
        return [steps, minval, maxval]

    def Close (self):
        # always be tidy
        self.rfile.Close ()
        pass
    pass
