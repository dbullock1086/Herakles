#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *
from Fitter import Fitter
from Nomenclature import Nomenclature

class Channel (Fitter, Nomenclature):
    def __init__ (self, mname, gain, pmt):
        self.gain  = gain
        self.pmt   = pmt
        self.name  = 'gain%i_pmt%i' % (self.gain, self.pmt)
        self.title = '%s Gain%i PMT%i' % (mname, self.gain, self.pmt)
        self.hists = {}
        self.algs  = {}
        pass

    def GetAlgs (self):
        algs = self.algs.values ()
        return algs

    def Clear (self):
        for name in self.algs:
            self.algs[name].IsA().Destructor (self.algs[name])
            del self.algs[name]
            pass
        for name in self.hists:
            self.hists[name].IsA().Destructor (self.hists[name])
            del self.hists[name]
            pass
        pass

    def DoFit (self):
        for name in self.hists:
            cname = self.hists[name].Class().GetName ()
            if cname == 'TH1D':
                self.Fit (name, mode)
                pass
            elif cname == 'TProfile': self.Fit (name, 'linear')
                pass
            pass
        pass

    def NameHist (self, xvar):
        #### get a unique name for this histogram
        name = '%s_%s' % (self.name, xvar)
        return name

    def TitleHist (self, xvar):
        #### get a descriptive title for this histogram
        xtitle = self.VarTitle (xvar)
        title = '%s: %s' % (self.title, xtitle)
        return title

    def MDHist (self, xvar, xbins, xmin, xmax):
        #### add a MultiDraw algorithm to fill a 1D histogram
        name = self.NameHist (xvar)
        title = self.TitleHist (xvar)
        self.hists[name] = ROOT.TH1D (name, title, xbins, xmin, xmax)
        self.hists[name].Sumw2 ()
        self.hists[name].SetXTitle (xvar)
        self.hists[name].SetYTitle ('Entries / Bin')
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar)
        pass

    def NameHist2D (self, xvar, yvar):
        #### get a unique name for this histogram
        name = '%s_%s_vs_%s' % (self.name, yvar, xvar)
        return name

    def TitleHist2D (self, xvar, yvar):
        #### get a descriptive title for this histogram
        xtitle = self.VarTitle (xvar)
        ytitle = self.VarTitle (yvar)
        title = '%s: %s vs %s' % (self.title, xtitle, ytitle)
        return title

    def MDHist2D (self, xvar, xbins, xmin, xmax,
                  yvar, ybins, ymin, ymax):
        #### add a MultiDraw algorithm to fill a 2D histogram
        name = self.NameHist2D (xvar, yvar)
        title = self.TitleHist2D (xvar, yvar)
        self.hists[name] = ROOT.TH2D (name, title, xbins, xmin, xmax)
        self.hists[name].Sumw2 ()
        self.hists[name].SetXTitle (xvar)
        self.hists[name].SetYTitle (yvar)
        self.hists[name].SetZTitle ('Entries / Bin')
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar, yvar)
        pass

    def NameProfile (self, xvar, yvar):
        #### get a unique name for this histogram
        name = '%s_prf_%s_vs_%s' % (self.name, yvar, + xvar)
        return name

    def TitleProfile (self, xvar, yvar):
        #### get a descriptive title for this histogram
        xtitle = self.VarTitle (xvar)
        ytitle = self.VarTitle (yvar)
        title = '%s: Mean[%s] vs %s' % (self.title, xtitle, ytitle)
        return title

    def MDProfile (self, xvar, xbins, xmin, xmax,
                   yvar):
        #### add a MultiDraw algorithm to fill a Profile histogram
        name = self.NameProfile (xvar, yvar)
        title = self.TitleProfile (xvar, yvar)
        self.hists[name] = ROOT.TProfile (name, name, xbins, xmin, xmax)
        self.hists[name].Sumw2 ()
        self.hists[name].SetXTitle (xvar)
        self.hists[name].SetYTitle (yvar)
        self.algs[name] = ROOT.MD.AlgHist (self.hists[name], xvar, yvar)
        pass

    def OwnHist (self, hist):
        name = hist.GetName ()
        self.hists[name] = hist
        self.hists[name].SetDirectory (0)
        pass
    pass
