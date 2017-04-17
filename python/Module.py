#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *
from Channel import Channel
from HistReader import HistReader

class Module (HistReader):
    def __init__ (self, name, gains, channels, window):
        #### manager of channels and summaries
        self.name = name
        self.gains = gains
        self.channels = channels
        self.window = window
        for gain in self.gains:
            self.members[gain] = {}
            for pmt in self.channels:
                self.members[gain][pmt] = Channel (self.name, gain, pmt)
                pass
            pass
        self.hists = {}
        self.summary = {}
        pass

    def Clear (self):
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
        for gain in self.gains:
            for pmt in self.channels:
                xname = self.ArrName (xvar, gain, pmt)
                xbins, xmin, xmax = self.GetRange (xvar, gain, pmt)
                self.members[gain][pmt].MDHist (xname, xbins, xmin, xmax)
                pass
            pass
        pass

    def OwnMDHist (self, xvar):
        for gain in self.gains:
            for pmt in self.channels:
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnHist (hist, xvar)
                pass
            pass
        pass

    def AddMDHist2D (self, xvar, yvar):
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
        for gain in self.gains:
            for pmt in self.channels:
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnHist (hist, xvar, yvar)
                pass
            pass
        pass

    def AddMDProfile (self, xvar, yvar):
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
        for gain in self.gains:
            for pmt in self.channels:
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnHist (hist, xvar, yvar)
                pass
            pass
        pass

    def GetAlgs (self, gain, pmt):
        algs = self.members[gain][pmt].GetAlgs ()
        return algs

    def DoFit (self, key, mode):
        for gain in self.gains:
            for pmt in self.channels:
                self.members[gain][pmt].DoFit (vargs, mode)
                if not self.summary.has_key (key): self.summary[key] = {}
                pass
            pass
        pass

    def Summarize (self):
        for gain in self.gains:
            for pmt in self.channels:
                for name in self.summary:
                    if gain:
                        sbase = '%s_%s_hi' % (self.name, name)
                        stitle = '%s %s High' % (self.name, name)
                        pass
                    else:
                        sbase = '%s_%s_lo' % (self.name, name)
                        stitle = '%s %s Low' % (self.name, name)
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
