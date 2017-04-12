#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *
from Fitter import Fitter

class Channel (Fitter):
    def __init__ (self, gain, pmt):
        self.gain  = gain
        self.pmt   = pmt
        self.name  = 'Gain%i_PMT%i' % (self.gain, self.pmt)
        self.hists = {}
        self.algs  = {}
        pass

    def OwnHist (self, hist):
        name = hist.GetName ()
        self.hists[name] = hist
        pass

    def MDHist (self, testname,
                xvar, xbins, xmin, xmax):
        #### add a MultiDraw algorithm to fill a 1D histogram
        name = testname + '_' + self.name
        self.hists[name] = ROOT.TH1D (name, name, xbins, xmin, xmax)
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar)
        pass

    def MDHist2D (self, testname,
                  xvar, xbins, xmin, xmax,
                  yvar, ybins, ymin, ymax):
        #### add a MultiDraw algorithm to fill a 2D histogram
        name = testname + '_' + self.name
        self.hists[name] = ROOT.TH2D (name, name, xbins, xmin, xmax)
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar, yvar)
        pass

    def MDProfile (self, testname,
                   xvar, xbins, xmin, xmax,
                   yvar):
        #### add a MultiDraw algorithm to fill a Profile histogram
        name = testname + '_' + self.name
        self.hists[name] = ROOT.TProfile (name, name, xbins, xmin, xmax)
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar, yvar)
        pass
    pass
