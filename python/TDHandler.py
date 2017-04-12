#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ROOTBase import *

class TDHandler:
    def __init__ (self, gains, channels, window):
        # these might seem redundant, but this allows python to access xrange
        # while C++ reads a list rather than a python class
        self.gains = [ gain for gain in gains ]
        self.channels = [ pmt for pmt in channels ]
        self.window = [ index for index in window ]
        pass

    def GetAlg (self, name):
        #### get a configured Herakles algorithm
        if name == 'EvtRange':      alg = ROOT.TD.EvtRange ()
        elif name == 'CapRange':    alg = ROOT.TD.CapRange ()
        elif name == 'ChargeRange': alg = ROOT.TD.ChargeRange ()
        elif name == 'PedRange':
            alg = ROOT.TD.PedRange ()
            alg.gains = self.gains
            alg.channels = self.channels
            pass
        elif name == 'SampleRange':
            alg = ROOT.TD.SampleRange ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        elif name == 'CRCError':
            alg = ROOT.TD.CRCError ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        elif name == 'PedRatio':
            alg = ROOT.TD.PedRatio ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        elif name == 'SampleHF':
            alg = ROOT.TD.SampleHF ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        elif name == 'SampleLF':
            alg = ROOT.TD.SampleLF ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        elif name == 'CorrHF':
            alg = ROOT.TD.CorrHF ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        elif name == 'PulseFit':
            alg = ROOT.TD.PulseFit ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        elif name == 'FastFit':
            alg = ROOT.TD.FastFit ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        return alg
    pass
