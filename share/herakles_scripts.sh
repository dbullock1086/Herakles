#!/bin/bash

function herakles ()
{
    case $1 in
	SampleStability)
	    shift
	    $Herakles/bin/SampleStability.py $@
	    ;;
	StampleCorr)
	    shift
	    $Herakles/bin/SampleCorr.py $@
	    ;;
	*)
	    echo undefined Herakles test $1
    esac
}
