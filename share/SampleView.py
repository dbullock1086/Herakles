#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from Hercules import Hercules

class SampleView (Hercules):
    def __init__ (self):
        super (SampleView, self).__init__ ()
        self.SetTree ('dataTree')
        self.AddTDAlg ('EvtRange')
        self.AddTDAlg ('SampleRange')
        self.CopyBranch ('evt')
        self.CopyBranch ('samples')
        self.AddMDHist2D ('evt', 'samples')
        pass
    pass

test = SampleView ()
test.Run ()
