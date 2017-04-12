#!/bin/bash

export HERAKLESSCRIPT=$@

export WorkDir=/afs/cern.ch/work/d/dbullock/public/PrometeoTest
cd $WorkDir
source setup.sh

printenv
eval $HERAKLESSCRIPT
