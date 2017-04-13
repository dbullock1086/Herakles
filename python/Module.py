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
        self.owned = {}
        pass

    def _arrayloop (self, var, gain, pmt):
        name = var
        if var in ['ped', 'samples']:
            if gain: name += '_hi'
            else: name += '_lo'
            name += '[%i]' % pmt
            pass
        elif var in ['crc', 'fastfit', 'fastratio', 'pedratio',
                     'pedestal', 'height', 'phase', 'width',
                     'chisqr', 'ndf', 'chgratio',
                     'hfmean', 'hfstd']:
            name += '[%i][%i]' % (gain, pmt)
            pass
        return name

    def AddMDHist (self, xvar):
        for gain in self.gains:
            for pmt in self.channels:
                xname = self._arrayloop (xvar, gain, pmt)
                #xbins, xmin, xmax = self.rf.GetRange (xvar, gain, pmt)
                self.members[gain][pmt].MDHist (xname, xbins, xmin, xmax)
                pass
            pass
        pass

    def AddMDHist2D (self, xvar, yvar):
        for gain in self.gains:
            for pmt in self.channels:
                xname = self._arrayloop (xvar, gain, pmt)
                #xbins, xmin, xmax = self.()
                yname = self._arrayloop (yvar, gain, pmt)
                #ybins, ymin, ymax = self.()
                self.members[gain][pmt].MDHist2D (xname, xbins, xmin, xmax,
                                                  yname, ybins, ymin, ymax)
                pass
            pass
        pass

    def AddMDProfile (self, xvar, yvar):
        for gain in self.gains:
            for pmt in self.channels:
                xname = self._arrayloop (xvar, gain, pmt)
                #xbins, xmin, xmax = self.()
                yname = self._arrayloop (yvar, gain, pmt)
                #ybins, ymin, ymax = self.()
                self.members[gain][pmt].MDProfile (xname, xbins, xmin, xmax,
                                                   yname)
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
