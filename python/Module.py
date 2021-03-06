#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

# Module.py
# This class manages a collection of channels. It also stores summary
# information about the entire mini-drawer so that you can view the status and
# results of the tests for all 48 PMTs simultaneously for both gains.

from ROOTBase import *
from Channel import Channel
from HistReader import HistReader

class Module (HistReader):
    def __init__ (self, gains, channels, window):
        #### manager of channels and summaries
        self.gains = gains
        self.channels = channels
        self.window = window
        for gain in self.gains:
            self.members[gain] = {}
            for pmt in self.channels:
                self.members[gain][pmt] = Channel (gain, pmt, self.window)
                pass
            pass
        self.algs = {}
        self.hists = {}
        self.summary = {}
        pass

    def Clear (self):
        #### clear histograms and algorithms from instances
        for gain in self.gains:
            for pmt in self.channels:
                self.members[gain][pmt].Clear ()
                pass
            pass
        for name in self.hists:
            self.hists[name].IsA().Destructor (self.hists[name])
            del self.hists[name]
            pass
        pass

    def AddMDHist (self, xvar):
        #### add a 1D histogram to each channel
        for gain in self.gains:
            for pmt in self.channels:
                xname = self.ArrName (xvar, gain, pmt)
                xbins, xmin, xmax = self.GetRange (xvar, gain, pmt)
                self.members[gain][pmt].MDHist (xname, xbins, xmin, xmax)
                pass
            pass
        pass

    def OwnMDHist (self, xvar):
        #### get a 1D histogram and assign to each channel
        for gain in self.gains:
            for pmt in self.channels:
                name = self.members[gain][pmt].NameHist (xvar)
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnHist (hist, xvar)
                pass
            pass
        pass

    def AddMDHist2D (self, xvar, yvar):
        #### add a 2D histogram to each channel
        for gain in self.gains:
            for pmt in self.channels:
                xname = self.ArrName (xvar, gain, pmt)
                xbins, xmin, xmax = self.GetRange (xvar, gain, pmt)
                yname = self.ArrName (yvar, gain, pmt)
                ybins, ymin, ymax = self.GetRange (yvar, gain, pmt)
                self.members[gain][pmt].MDHist2D (xname, xbins, xmin, xmax,
                                                  yname, ybins, ymin, ymax)
                pass
            pass
        pass

    def OwnMDHist2D (self, xvar, yvar):
        #### get a 2D histogram and assign to each channel
        for gain in self.gains:
            for pmt in self.channels:
                name = self.members[gain][pmt].NameHist2D (xvar, yvar)
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnHist (hist, xvar, yvar)
                pass
            pass
        pass

    def AddMDProfile (self, xvar, yvar):
        #### add a profile to each channel
        for gain in self.gains:
            for pmt in self.channels:
                xname = self.ArrName (xvar, gain, pmt)
                xbins, xmin, xmax = self.GetRange (xvar, gain, pmt)
                yname = self.ArrName (yvar, gain, pmt)
                ybins, ymin, ymax = self.GetRange (yvar, gain, pmt)
                self.members[gain][pmt].MDProfile (xname, xbins, xmin, xmax,
                                                   yname)
                pass
            pass
        pass

    def OwnMDProfile (self, xvar, yvar):
        #### get a profile and assign to each channel
        for gain in self.gains:
            for pmt in self.channels:
                name = self.members[gain][pmt].NameProfile (xvar, yvar)
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnHist (hist, xvar, yvar)
                pass
            pass
        pass

    def GetAlgs (self, gain, pmt):
        #### get the algorithms from each channel
        algs = self.members[gain][pmt].algs.values ()
        return algs

    def DoFit (self, key, mode):
        #### perform a particular fit (mode) on histograms (key) for each gain
        #    and PMT
        for gain in self.gains:
            for pmt in self.channels:
                self.members[gain][pmt].DoFit (key, mode)
                # if a fit is performed, add it to summary
                if not self.summary.has_key (key): self.summary[key] = {}
                pass
            pass
        pass

    def Summarize (self):
        for gain in self.gains:
            for pmt in self.channels:
                for name in self.summary:
                    if gain:
                        sbase = '%s_hi' % name
                        stitle = '%s High' % name
                        pass
                    else:
                        sbase = '%s_lo' % name
                        stitle = '%s Low' % name
                        pass

                    namesplit = name.split('_')
                    if len(namesplit) == 2:
                        xvar = namesplit[0]
                        yvar = namesplit[1]
                        cname = self.members[gain][pmt].NameProfile (xvar,yvar)
                        pass
                    else:
                        xvar = namesplit[0]
                        cname = self.members[gain][pmt].NameHist (xvar)
                        pass

                    for pname in self.members[gain][pmt].fitparams[cname]:
                        hname = '%s_fit_%s' % (sname, pname)
                        if not self.hists.has_key (hname):
                            htitle = '%s Fit %s' % (sname, pname)
                            self.hists[hname] = ROOT.TH1D (hname, htitle,
                                                           48, 0, 48)
                            self.hists[hname].SetDirectory (0)
                            pass
                        pval = self.members[gain][pmt].fitparams[cname][pname]
                        self.hists[hname].SetBinContent (pmt + 1, pval)
                        self.hists[hname].SetBinError (pmt + 1, 0)
                        pass
                    pass
                pass
            pass
        pass

    def Write (self):
        for name in self.hists: self.hists[name].Write ()
        for gain in self.gains:
            for pmt in self.channels:
                for name in self.members[gain][pmt].hists:
                    self.members[gain][pmt].hists[name].Write ()
                    pass
                pass
            pass
        pass
    pass
