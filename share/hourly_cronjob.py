#!/usr/bin/env python

import subprocess
from datetime import date

def Bash (cmd):
    p = subprocess.Popen(cmd, shell=True,
                         stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    p.wait()
    return p.stdout.readlines()

eosdir = '/eos/atlas/atlascerngroupdisk/det-tile/demonstrator/%i' % \
         date.today().year
eosbin = '/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select'
batchdir = '/afs/cern.ch/work/d/dbullock/public/TestHerakles'

data = []
hist = []
jobs = []

lines = Bash ('%s ls %s/rawData' % (eosbin,eosdir))
for filename in lines:
    filename = filename.strip()
    if not '.root' in filename: continue
    if not filename.startswith('Data_'): continue
    for term in nomenclature:
        if term in filename:
            data.append(filename[5:])
            break
        pass
    pass

lines = Bash ('%s ls %s/histograms' % (eosbin,eosdir))
for filename in lines:
    filename = filename.strip()
    if not '.root' in filename: continue
    if not filename.startswith('Hist_'): continue
    for term in nomenclature:
        if term in filename:
            hist.append(filename[5:])
            break
        pass
    pass

lines = Bash ('/usr/bin/bjobs -w')
for filename in lines[1:]:
    filename = filename.strip()
    if len(filename): continue
    job = filename[filename.find('_')+1:]
    if not job in jobs: jobs.append(job)
    pass

lines = Bash ('cd %s; source setup.sh' % batchdir)

for run in data:
    if run in jobs: continue
    if run in hist: continue
    runName = 'Data_' + run
    print
    print 'Launch processing for run', runName
    cmd = 'python python/steering.py ' + runName
    print cmd
    lines = Bash (cmd)
    pass
