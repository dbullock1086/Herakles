#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class CRCCount (Hercules):
    def __init__ (self):
        super (CRCCount, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddTDAlg ('EventRange')
        self.AddTDAlg ('CRCError')
        self.CopyBranch ('evt')
        self.AddMDHist ('crc')
        self.AddMDHist2D ('evt', 'crc')
        self.OwnHist ('crc')
        pass
    pass

test = SampleStability ()
test.Run ()
