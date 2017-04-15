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
                name = self.members[gain][pmt].NameHist (xvar)
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnHist (hist)
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
                name = self.members[gain][pmt].NameHist2D (xvar, yvar)
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnHist2D (hist)
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
                name = self.members[gain][pmt].NameProfile (xvar, yvar)
                hist = self.rfile.Get (name)
                self.members[gain][pmt].OwnProfile (hist)
                pass
            pass
        pass

    def GetAlgs (self, gain, pmt):
        algs = self.members[gain][pmt].GetAlgs ()
        return algs

    def DoFit (self):
        for gain in self.gains:
            for pmt in self.channels:
                self.members[gain][pmt].DoFit ()
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
