#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *
from Fitter import Fitter

class Channel (Fitter):
    def __init__ (self, mname, gain, pmt):
        self.gain  = gain
        self.pmt   = pmt
        self.name  = '%s_Gain%i_PMT%i' % (mname, self.gain, self.pmt)
        self.hists = {}
        self.algs  = {}
        pass

    def MDHist (self, xvar, xbins, xmin, xmax):
        #### add a MultiDraw algorithm to fill a 1D histogram
        name = '%s_%s' % (self.name, xvar)
        self.hists[name] = ROOT.TH1D (name, name, xbins, xmin, xmax)
        self.hists[name].Sumw2 ()
        self.hists[name].SetXTitle (xvar)
        self.hists[name].SetYTitle ('Entries / Bin')
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar)
        pass

    def MDHist2D (self, xvar, xbins, xmin, xmax,
                  yvar, ybins, ymin, ymax):
        #### add a MultiDraw algorithm to fill a 2D histogram
        name = '%s_%s_vs_%s' % (self.name, yvar, xvar)
        self.hists[name] = ROOT.TH2D (name, name, xbins, xmin, xmax)
        self.hists[name].Sumw2 ()
        self.hists[name].SetXTitle (xvar)
        self.hists[name].SetYTitle (yvar)
        self.hists[name].SetZTitle ('Entries / Bin')
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar, yvar)
        pass

    def MDProfile (self, xvar, xbins, xmin, xmax,
                   yvar):
        #### add a MultiDraw algorithm to fill a Profile histogram
        name = '%s_Mean[%s]_vs_%s' % (self.name, yvar, + xvar)
        self.hists[name] = ROOT.TProfile (name, name, xbins, xmin, xmax)
        self.hists[name].Sumw2 ()
        self.hists[name].SetXTitle (xvar)
        self.hists[name].SetYTitle (yvar)
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar, yvar)
        pass
    pass
