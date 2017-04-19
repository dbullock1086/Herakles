#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

# ELHandler.py
# The heavy-lifting of this software is RootCore and EventLoop. This class just
# wraps the steering macros into python methods.

from ROOTBase import *

class ELHandler:
    def __init__ (self, name, tree):
        #### define some configuration variables
        self.name  = name
        self.tree  = tree
        self.hists = {}
        self.ostream = {}
        pass

    def SH (self, scandir, sname):
        #### scan for samples
        self.sh = ROOT.SH.SampleHandler ()
        ROOT.SH.scanForTrees (self.sh, self.tree)
        print 'Scanning for samples in %s ...' % scandir
        ROOT.SH.ScanDir().sampleDepth(-1)            \
                         .samplePostfix('.root')     \
                         .filePattern(sname+'.root') \
                         .scan(self.sh, scandir)
        self.sh.printContent ()
        self.sh.setMetaString ('nc_tree', self.tree)
        pass

    def EL (self):
        #### generic job init
        self.job = ROOT.EL.Job ()
        self.job.sampleHandler (self.sh)
        pass

    def AddOutput (self, name):
	#### specify an output
	self.ostream[name] = ROOT.EL.OutputStream (name)
	self.job.outputAdd (self.ostream[name])
        pass

    def NTuple (self, branches):
        #### specify any branches to copy from raw data
	self.AddOutput ('ntuple')
        self.ntuple = ROOT.EL.NTupleSvc ('ntuple')
        for br in branches: self.ntuple.copyBranch (br)
        self.job.algsAdd (self.ntuple)
        pass

    def AddAlg (self, alg):
        #### add an algorithm to the job
        self.job.algsAdd (alg)
        pass

    def Submit (self, eldir):
        #### run it
        self.driver = ROOT.EL.DirectDriver ()
        self.driver.submit (self.job, eldir)
        pass
    pass
