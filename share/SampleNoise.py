#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class SampleNoise (Hercules):
    def __init__ (self):
        super (SampleNoise, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddMDHist ('samples', fit='gaus')
        pass
    pass

test = SampleNoise ()
test.Run ()
