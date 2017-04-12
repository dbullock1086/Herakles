#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *
from Channel import Channel
from RangeFinder import RangeFinder

class Module:
    def __init__ (self, name, gains, channels, window):
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
        self.output = 0
        pass

    def AddHist (self, name, ylabel=''):
        self.hists[name] = ROOT.TH1D (name, name, 48, 0, 48)
        self.hists[name].SetDirectory (self.output)
        self.hists[name].SetYTitle (ylabel)
        for pmt in xrange(48):
            self.hists[name].GetXaxis().SetBinLabel (pmt+1, 'PMT%i' % pmt+1)
            pass
        pass

    def AddChannel (self, name, pmt, value):
        self.hists[name].SetBinContent (pmt + 1, value)
        self.hists[name].SetBinError (pmt + 1, 0)
        pass

    def Output (self, hfile):
        self.output = ROOT.TFile.Open (hfile, 'recreate')
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

    def Close (self):
        self.output.Close ()
        pass
    pass
