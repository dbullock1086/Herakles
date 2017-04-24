#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

# Channel.py
# A class for managing algorithms and histograms for a specific gain and PMT.
# Each instance of the Channel class receives its calls from the Module class,
# which manages the entire collection of channels.

from ROOTBase import *
from Fitter import Fitter
from Nomenclature import Nomenclature

class Channel (Fitter, Nomenclature):
    def __init__ (self, mname, gain, pmt):
        #### identify the channel with its specific gain and PMT
        self.gain  = gain
        self.pmt   = pmt
        self.name  = 'gain%i_pmt%i' % (self.gain, self.pmt)
        self.title = '%s Gain%i PMT%i' % (mname, self.gain, self.pmt)
        self.hists = {}
        self.algs  = {}
        self.summary = {}
        self.fitparams = {}
        pass

    def Clear (self):
        #### clear histograms and algorithms from the instance
        for name in self.algs:
            self.algs[name].IsA().Destructor (self.algs[name])
            del self.algs[name]
            pass
        for name in self.hists:
            self.hists[name].IsA().Destructor (self.hists[name])
            del self.hists[name]
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

    def StaticHist (self, name):
        #### add a static algorithm to fill a histogram
        pname = self.NameHist (name)
        title = self.TitleHist (name)
        self.algs[pname] = eval ('ROOT.TD.%s()' % name)
        self.algs[pname].gain = self.gain
        self.algs[pname].pmt = self.pmt
        pass

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
        name = '%s_scatter_%s_%s' % (self.name, yvar, xvar)
        return name

    def TitleHist2D (self, xvar, yvar):
        #### get a descriptive title for this histogram
        xtitle = self.VarTitle (xvar)
        ytitle = self.VarTitle (yvar)
        title = '%s: Scatter %s vs %s' % (self.title, xtitle, ytitle)
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
        name = '%s_prf_%s_%s' % (self.name, yvar, + xvar)
        return name

    def TitleProfile (self, xvar, yvar):
        #### get a descriptive title for this histogram
        xtitle = self.VarTitle (xvar)
        ytitle = self.VarTitle (yvar)
        title = '%s: Profile %s vs %s' % (self.title, xtitle, ytitle)
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
        #### save a histogram, usually for the purpose of fitting and output
        name = hist.GetName ()
        self.hists[name] = hist
        self.hists[name].SetDirectory (0)
        pass

    def DoFit (self, key, mode):
        #### perform a fit to a specified mode
        name = '%s_%s' % (self.name, key)
        self.Fit (name, mode)

        # read back the parameters of the fit
        self.fitparams[name] = {}
        fitres = self.hists[name].GetFunction (name + '_fit')
        npar = fitres.GetNumberFreeParameters ()
        for i in xrange(npar):
            pname = fitres.GetParName   (i)
            pval  = fitres.GetParameter (i)
            self.fitparams[name][pname] = pval
            pass
        self.fitparams[name]['prob'] = fitres.GetProb ()
        pass
    pass
