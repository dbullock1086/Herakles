#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class InvalidCount (Hercules):
    def __init__ (self):
        super (InvalidCount, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddTDAlg ('EventRange')
        self.AddTDAlg ('Saturation')
        self.AddTDAlg ('NullValue')
        self.CopyBranch ('evt')
        self.AddMDHist2D ('evt', 'saturation')
        self.AddMDHist2D ('evt', 'null')
        self.OwnELHist ('crc')
        pass
    pass

test = InvalidCount ()
test.Run ()
