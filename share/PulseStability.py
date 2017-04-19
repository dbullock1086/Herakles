#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class PulseStability (Hercules):
    def __init__ (self):
        super (PulseStability, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddTDAlg ('EvtRange')
        self.AddTDAlg ('PulseFit')
        self.CopyBranch ('evt')
        self.AddMDProfile ('evt', 'pedestal', fit='linear')
        self.AddMDProfile ('evt', 'height',   fit='linear')
        self.AddMDProfile ('evt', 'phase',    fit='linear')
        self.AddMDProfile ('evt', 'width',    fit='linear')
        self.AddMDProfile ('evt', 'prob',     fit='linear')
        self.OwnELHist ('pedestal')
        self.OwnELHist ('height')
        self.OwnELHist ('phase')
        self.OwnELHist ('width')
        self.OwnELHist ('prob')
        pass
    pass

test = PulseStability ()
test.Run ()
