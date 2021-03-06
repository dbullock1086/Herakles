#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

# Hercules.py
# This class is used to control the flow of the test. The "job" of a test is
# divided into three sub-routines. Therefore, each routine may require a
# different set of objects. This class should only be used for storing string
# references to those objects, and the sub-routines cause other classes to load
# those objects into memory.

#### configure the test from the command line
from ArgParse import *

#### load supports
import os, subprocess, json
from ELHandler import ELHandler
from Branches import Branches
from Module import Module

#### get environment variables
WorkDir = os.environ['WorkDir']
DataDir = os.environ['DataDir']
HistDir = os.environ['HistDir']
LogDir  = os.environ['LogDir']
EOSDir  = os.environ['EOSDir']
TMPDIR  = os.environ['TMPDIR']

#### load a historical log of jobs that have been attempted
execfile (WorkDir + '/history.json')
if args.name not in history:
    history[args.name] = {'eventloop': 0,
                          'multidraw': 0,
                          'hist': 0}
    pass

class Hercules (object):
    def __init__ (self):
        #### some metadata
        self.elalgs     = set (['EvtRange'])
        self.mdhists    = []
        self.mdhists2d  = []
        self.mdprofiles = []
        self.histfits   = {}
        self.cpybr      = set ()
        self.tree       = 'dataTree'
        self.ownel      = set ()
        self.brRef = Branches ()
        pass

    def SetTree (self, tree):
        #### set the TTree name for the raw data (e.g. dataTree or dcsTree)
        assert type(tree).__name__ == 'str', 'tree must be a string'
        self.tree = tree
        pass

    def AddStatic (self, alg):
        #### add a static algorithm class (string reference)
        assert type(alg).__name__ == 'str', 'alg must be a string'
        self.elalgs.add (alg)
        self.ownel.add (alg)
        pass

    def _checkbranch (self, branch):
        #### copy a branch from raw data to the derived dataset
        #    or add the associated algorithm
        self.ownel.add (self.brRef.VarRange(branch)
        if self.brRef.VarRaw (branch):
            if self.brRef.VarChannel (branch):
                if 0 in args.gains: self.cpybr.add (branch + '_lo')
                if 1 in args.gains: self.cpybr.add (branch + '_hi')
                pass
            else: self.cpybr.add (branch)
            pass
        else: self.elalgs.add (self.brRef.VarAlg(branch))
        pass

    def AddMDHist (self, xvar, fit=''):
        #### add a MultiDraw algorithm class (string reference)
        assert type(xvar).__name__ == 'str', 'xvar must be a string'
        assert xvar != 'evt', 'evt not allowed as a 1D histogram'
        assert type(fit).__name__ == 'str', 'fit must be a string'
        self._checkbranch (xvar)
        self.mdhists.append (xvar)
        if fit: self.histfits[xvar] = fit
        pass

    def AddMDHist2D (self, xvar, yvar):
        #### add a MultiDraw algorithm class (string reference)
        assert type(xvar).__name__ == 'str', 'xvar must be a string'
        assert type(yvar).__name__ == 'str', 'yvar must be a string'
        self._checkbranch (xvar)
        self._checkbranch (yvar)
        self.mdhists2d.append ([xvar, yvar])
        pass

    def AddMDProfile (self, xvar, yvar, fit=None):
        #### add a MultiDraw algorithm class (string reference)
        assert type(xvar).__name__ == 'str', 'xvar must be a string'
        assert type(yvar).__name__ == 'str', 'yvar must be a string'
        self._checkbranch (xvar)
        self._checkbranch (yvar)
        self.mdprofiles.append ([xvar, yvar])
        if fit: self.histfits['prf_%s_%s' % (xvar, yvar)] = fit
        pass

    def Run (self):
        #### execute the test
        # run a single routine (i.e. batch)
        if args.routine: self._run (args.routine)

        # loop through routines
        else:
            for routine in ['eventloop', 'multidraw', 'hist']:
                # submit to batch
                if args.submit: self._submit (routine)
                else: self._run (routine)
                pass
            pass
        pass

    def _run (self, routine):
        #### instructions for each routine

        # limit the number of test attempts
        if history[args.name][routine] >= 10: return
        
        # update the history log
        history[args.name][routine] += 1
        log = json.dumps(history, sort_keys=True,
                         indent=4, separators=(',', ': '))
        f = open (WorkDir + '/history.json', 'w')
        f.write ('history = ' + log)
        f.close ()

        if routine == 'eventloop':
            # build event loop and add Herakles algorithms
            self._copyinit ()
            el = ELHandler (args.name, self.tree)
            el.SH (DataDir + '/' + args.name, args.name + '.root')
            el.EL ()

            # copy branches from raw data to the derived set
            if self.cpybr: el.NTuple (self.cpybr)

            # get the algorithm objects
            td = Algorithms (args.gains, args.channels, args.window)
            for alg in self.algs:
                Alg = td.GetAlg (alg)
                el.AddAlg (Alg)
                pass
            el.Submit (TMPDIR + '/' + args.name)

            tempfile = '%s/%s/hist-%s.root' % (TMPDIR, args.name, args.data)
            cmd = 'cp %s %s/%s/eventloop.root' % (tempfile, HistDir, args.name)
            self._bash (cmd)
            tempfile = '%s/%s/ntuple.root' % (TMPDIR, args.name)
            if os.access(tempfile, os.R_OK):
                cmd = 'mv %s %s/%s/ntuple.root' % \
                      (tempfile, HistDir, args.name)
                self._bash (cmd)
                pass
            cmd = 'rm -rf %s/%s' % (TMPDIR, args.name)
            self._bash (cmd)
            pass
        elif routine == 'multidraw':
            # build event loop
            el = ELHandler (self.name, 'ntuple')
            el.SH ('%s/%s' % (HistDir, args.name), 'ntuple.root')
            el.EL ()

            # stage the MD algorithms to each channel
            self.module = Module (args.gains, args.channels, args.window)
            self.module.OpenFile ('%s/%s/eventloop.root' % (TMPDIR, args.name))
            self.module.InitRange ()
            for xvar in self.mdhists: self.module.AddMDHist (xvar)
            for [xvar, yvar] in self.mdhists2d:
                self.module.AddMDHist2D (xvar, yvar)
                pass
            for [xvar, yvar] in self.mdprofiles:
                self.module.AddMDProfile (xvar, yvar)
                pass
            self.module.CloseFile ()

            # add MD algorithms to EventLoop job
            for gain in args.gains:
                for pmt in args.channels:
                    algs = self.module.GetAlgs (gain, pmt)
                    for alg in algs: el.AddAlg (alg)
                    pass
                pass
            el.Submit (TMPDIR + '/' + args.name)

            tempfile = '%s/%s/hist-ntuple.root' % (TMPDIR, args.name)
            cmd = 'cp %s %s/%s/multidraw.root' % (tempfile, HistDir, args.name)
            self._bash (cmd)
            cmd = 'rm -rf %s/%s' % (TMPDIR, args.name)
            self._bash (cmd)
            pass
        elif routine == 'hist':
            # collect and summarize hists
            if hasattr (self, 'module') self.module.Clear ()
            else: self.module = Module (args.gains, args.channels, args.window)

            self.module.OpenFile ('%s/%s/multidraw.root' % \
                                  (HistDir, args.name))
            for xvar in self.mdhists: self.module.OwnMDHist (xvar)
            for [xvar, yvar] in self.mdhists2d:
                self.module.OwnMDHist2D (xvar, yvar)
                pass
            for [xvar, yvar] in self.mdprofiles:
                self.module.OwnMDProfile (xvar, yvar)
                pass
            self.module.CloseFile ()

            for a in self.histfits: self.module.DoFit (a, self.histfits[a])
            self.module.Summarize ()

            self.module.OpenFile ('%s/%s/eventloop.root' % \
                                  (HistDir, args.name))
            for name in self.ownel: self.module.OwnELHist (name)
            self.module.CloseFile ()

            self.module.OpenFile ('%s/%s/hist.root' % (HistDir, args.name))
            self.module.Write ()
            self.module.CloseFile ()

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
            cmd = 'cp %s/%s/hist.root %s' % \
                  (HistDir, args.name, args.hist)
            pass
        self._bash (cmd)
        cmd = 'rm -rf %s/%s/' % (DataDir, args.name)
        self._bash (cmd)
        cmd = 'rm -rf %s/%s/' % (TMPDIR, args.name)
        self._bash (cmd)
        cmd = 'rm -rf %s/%s/' % (HistDir, args.name)
        self._bash (cmd)
        pass
    pass
