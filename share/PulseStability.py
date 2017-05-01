#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class PulseStability (Hercules):
    def __init__ (self):
        super (PulseStability, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddMDProfile ('evt', 'pedestal', fit='linear')
        self.AddMDProfile ('evt', 'height',   fit='linear')
        self.AddMDProfile ('evt', 'phase',    fit='linear')
        self.AddMDProfile ('evt', 'width',    fit='linear')
        self.AddMDProfile ('evt', 'prob',     fit='linear')
        pass
    pass

test = PulseStability ()
test.Run ()
