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
        self.AddMDProfile ('evt', 'pedestal')
        self.AddMDProfile ('evt', 'height')
        self.AddMDProfile ('evt', 'phase')
        self.AddMDProfile ('evt', 'width')
        self.AddMDProfile ('evt', 'chisqr')
        self.AddMDProfile ('evt', 'ndf')
        self.AddMDProfile ('evt', 'chgratio')
        self.OwnHist ('pedestal')
        self.OwnHist ('height')
        self.OwnHist ('phase')
        self.OwnHist ('width')
        self.OwnHist ('chisqr')
        self.OwnHist ('ndf')
        self.OwnHist ('chgratio')
        pass
    pass

test = PulseStability ()
test.Run ()
