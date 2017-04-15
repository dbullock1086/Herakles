#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class SampleStability (Hercules):
    def __init__ (self):
        super (SampleStability, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddTDAlg ('EvtRange')
        self.CopyBranch ('evt')
        self.CopyBranch ('samples')
        self.AddMDProfile ('evt', 'samples')
        self.OwnELHist ('samples')
        pass
    pass

test = SampleStability ()
test.Run ()
