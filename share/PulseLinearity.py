#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class PulseLinearity (Hercules):
    def __init__ (self):
        super (PulseLinearity, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddMDProfile ('charge', 'pedestal', fit='linear')
        self.AddMDProfile ('charge', 'height',   fit='linear')
        self.AddMDProfile ('charge', 'phase',    fit='linear')
        self.AddMDProfile ('charge', 'width',    fit='linear')
        self.AddMDProfile ('charge', 'prob',     fit='linear')
        pass
    pass

test = PulseLinearity ()
test.Run ()
