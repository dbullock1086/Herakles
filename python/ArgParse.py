#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

# ArgParse.py
# Command-line control arguments
# e.g.: python SampleNoise.py /path/to/data.root /path/to/hist.root --gains 0 1 --channels 0 1 2 3 --window 0 32
# try python SampleNoise.py --help for more options

import sys
from argparse import ArgumentParser as AP

p = AP (description='Herakles executable for test routines')
p.add_argument ('data', help='input ROOT file containing raw data')
p.add_argument ('hist', help='output ROOT file to hold final histograms')
p.add_argument ('--gains', type=int, nargs='+',
                choices=xrange(2), default=xrange(2),
                help='gains (Low=0, High=1) to run')
p.add_argument ('--channels', type=int, nargs='+',
                choices=xrange(48), default=xrange(48),
                help='PMT channels (0,1,2,...,46,47) to run')
p.add_argument ('--window', type=int, nargs=2,
                choices=xrange(128), default=[0,127],
                help='window of sample indices to run')
p.add_argument ('--submit', action='store_true', default=False,
                help='submit jobs to lxbatch')
p.add_argument ('--name', default='',
                help='specify a custom name (optional)')
p.add_argument ('--routine',
                choices=['eventloop', 'multidraw', 'hist'],
                default='',
                help='Do NOT use this one. This is only for batch control')
args = p.parse_args ()

# I/O must be ROOT files
assert args.data.endswith ('.root'), 'data file must end in ".root"'
assert args.hist.endswith ('.root'), 'hist file must end in ".root"'

# "name" of the job is the data input without its prefix
if not args.name: args.name = args.data
if '/' in args.name: args.name = args.name.split('/')[-1]
args.name = args.name.replace('Data_', '').replace('.root','')

# remove the submit option from batch jobs
args.script = ' '.join(sys.argv).replace(' --submit', '')
