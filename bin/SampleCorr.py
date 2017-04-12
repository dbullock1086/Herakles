#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class SampleCorr (Hercules):
    def __init__ (self):
        super (SampleCorr, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddTDAlg ('SampleCorr')
        self.CopyBranch ('samples')
        pass
    pass

test = SampleCorr ()
test.Run ()
