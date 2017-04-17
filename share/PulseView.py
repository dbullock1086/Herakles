#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class PulseView (Hercules):
    def __init__ (self):
        super (PulseView, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddTDAlg ('SampleLF')
        self.OwnELHist ('lfmean')
        self.OwnELHist ('lfstd')
        pass
    pass

test = PulseView ()
test.Run ()
