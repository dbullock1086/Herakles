#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

class Algorithms:
    def __init__ (self, gains, channels, window):
        self.gains = [ (i in gains) for i in xrange(2) ]
        self.channels = [ (i in channels) for i in xrange(48) ]
        self.window = window
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
        elif name == 'Saturation':
            alg = ROOT.TD.Saturation ()
            alg.gains = self.gains
            alg.channels = self.channels
            alg.window = self.window
            pass
        elif name == 'NullValue':
            alg = ROOT.TD.NullValue ()
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
