#!/bin/bash

export PrometeoSource=/afs/cern.ch/user/d/dbullock/public/NewHerakles
cd $PrometeoSource

if [ -z $ATLASLOCAL_ROOT ]; then
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
fi

if [ -z $ROOTCOREBIN ]; then
    if [ -d $WorkDir/RootCoreBin ]; then
	rcSetup
    else
	rcSetup Base,2.3.15
	rc find_packages
	rc compile
    fi
fi

export HERAKLES=$PrometeoSource/Herakles
export PYTHONPATH=$HERAKLES/python:$PYTHONPATH
export PATH=$HERAKLES/share:$PATH

export EOSDir=/eos/atlas/atlascerngroupdisk/det-tile/demonstrator/2017

export WorkDir=/afs/cern.ch/work/d/dbullock/public/NewHerakles
cd $WorkDir

export DataDir=$WorkDir/data
mkdir -p $DataDir
export HistDir=$WorkDir/hists
mkdir -p $HistDir
export LogDir=$LogDir/logs
mkdir -p $LogDir
export TMPDIR=$WorkDir/TMPDir
mkdir -p $TMPDIR

source $Herakles/share/herakles_scripts.sh
