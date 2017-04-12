#!/usr/bin/env python

# Herakles
# by Daniel Bullock, 2013-2017
# https://github.com/dbullock1086/Herakles

#### import supports
print 'Importing ROOT ...'
import ROOT
print 'Loading RootCore packages ...'
ROOT.gROOT.Macro ('$ROOTCOREDIR/scripts/load_packages.C')
ROOT.gROOT.SetBatch (True)
