#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

from ArgParse import *

import os, subprocess, json

from ELHandler import ELHandler
from TDHandler import TDHandler
from Module import Module

WorkDir = os.environ['WorkDir']
DataDir = os.environ['DataDir']
HistDir = os.environ['HistDir']
LogDir  = os.environ['LogDir']
EOSDir  = os.environ['EOSDir']
TMPDIR  = os.environ['TMPDIR']

execfile (WorkDir + '/history.json')
if args.name not in history:
    history[args.name] = {'eventloop': 0,
                          'multidraw': 0,
                          'hist': 0}
    pass

class Hercules (object):
    def __init__ (self):
        self.elalgs = set ()
        self.mdhists    = []
        self.mdhists2d  = []
        self.mdprofiles = []
        self.cpybr = set ()
        self.tree = 'dataTree'
        pass

    def SetTree (self, tree):
        #### set the TTree name for the raw data (e.g. dataTree or dcsTree)
        self.tree = tree
        pass

    def AddTDAlg (self, alg):
        #### add a TileDemo algorithm class (string reference)
        self.elalgs.add (alg)
        pass

    def CopyBranch (self, branch):
        #### copy a branch from raw data to the derived dataset
        if branch == 'ped' or branch == 'samples':
            if 0 in args.gains: self.cpybr.add (branch + '_lo')
            if 1 in args.gains: self.cpybr.add (branch + '_hi')
            pass
        else: self.cpybr.add (branch)
        pass

    def AddMDHist (self, xvar):
        #### add a MultiDraw algorithm class (string reference)
        self.mdhists.append (xvar)
        pass

    def AddMDHist2D (self, xvar, yvar):
        #### add a MultiDraw algorithm class (string reference)
        self.mdhists2d.append ([xvar, yvar])
        pass

    def AddMDProfile (self, xvar, yvar):
        #### add a MultiDraw algorithm class (string reference)
        self.mdprofiles.append ([xvar, yvar])
        pass

    def MDProfile (self, xvar, yvar): self.module.MDHistProfile (xvar, yvar)

    def Run (self):
        if args.routine: self._run (args.routine)
        for routine in ['eventloop', 'multidraw', 'hist']:
            if args.submit: self._submit (routine)
            else: self._run (routine)
            pass
        pass

    def _run (self, routine):
        #### run a single routine

        # update the history log
        history[args.name][routine] += 1
        log = json.dumps(history, sort_keys=True,
                         indent=4, separators=(',', ': '))
        f = open (WorkDir + '/history.json', 'w')
        f.write (log)
        f.close ()

        if routine == 'eventloop':
            # build event loop and add Herakles algorithms
            self._copyinit ()
            el = ELHandler (self.name, self.tree)
            ###################################################################
            ###################################################################
            el.SH (scandir, sname)
            ###################################################################
            ###################################################################
            el.EL ()
            el.NTuple (self.cpybr)
            td = TDHandler (args.gains, args.channels, args.window)
            for alg in self.algs:
                Alg = td.GetAlg (alg)
                el.AddAlg (Alg)
                pass
            el.Submit ()
            pass
        elif routine == 'multidraw':
            # build event loop and add multidraw algorithms
            el = ELHandler (self.name, 'tree')
            ###################################################################
            ###################################################################
            el.SH (scandir, sname)
            ###################################################################
            ###################################################################
            el.EL ()
            self.module = Module (args.name, args.gains, args.channels)
            for gain in args.gains:
                for pmt in args.channels:
                    for alg in self.module.members[gain][pmt].algs:
                        el.AddAlg (alg)
                        pass
                    pass
                pass
            pass
        elif routine == 'hist':
            # collect and summarize hists
            self.module = Module (args.name, args.gains, args.channels)
            self._copyfinal ()
            pass
        pass

    def _submit (self, routine):
        #### submit the job to run on lxbatch
        if routine == 'eventloop':
            queue = '8nh'
            jobname = 'EL_' + args.name
            wait = ''
            pass
        elif routine == 'multidraw':
            queue = '1nh'
            jobname = 'MD_' + args.name
            wait = '-w ended(EL_%s)' % args.name
            pass
        elif routine == 'hist':
            queue = '8nm'
            jobname = 'Hist_' + args.name
            wait = '-w ended(MD_%s)' % args.name
            pass
        logfile = LogDir + '/' + jobname + '.log'
        script = args.script + ' --routine ' + routine

        if os.access(logfile, os.R_OK):
            cmd = 'mv %s %s.old') % (logfile, logfile)
            self._bash (cmd)
            pass
        cmd = 'bsub %s -q %s -J %s -o %s -cwd %s %s/batch.sh %s' % \
              (wait, queue, jobname, logfile, WorkDir, WorkDir, script)
        self._bash (cmd)
        pass

    def _copyinit (self):
        #### start a new session by copying the raw data to a local directory
        if args.data.beginswith ('/eos/'):
            cmd = 'xrdcp root://eosatlas.cern.ch/%s %s/%s/' % \
                  (args.data, DataDir, args.name)
            pass
        else: cmd = 'cp %s %s/%s/' % (args.data, DataDir, args.name)
        self._bash (cmd)
        pass

    def _copyfinal (self):
        #### end the session by copying the histogram summary
        #    to its final directory
        if args.hist.beginswith ('/eos/'):
            cmd = 'xrdcp %s/%s/summary.root root://eosatlas.cern.ch/%s' % \
                  (HistDir, args.name, args.hist)
            pass
        else:
            cmd = 'cp %s/%s/summary.root %s' % \
                  (HistDir, args.name, args.hist)
            pass
        self._bash (cmd)
        pass

    pass
