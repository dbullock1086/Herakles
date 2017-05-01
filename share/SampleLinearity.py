#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class SampleLinearity (Hercules):
    def __init__ (self):
        super (SampleLinearity, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddMDProfile ('ped', 'samples', fit='linear')
        pass
    pass

test = SampleLinearity ()
test.Run ()
