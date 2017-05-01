#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class SampleStability (Hercules):
    def __init__ (self):
        super (SampleStability, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddMDProfile ('evt', 'samples', fit='linear')
        pass
    pass

test = SampleStability ()
test.Run ()
