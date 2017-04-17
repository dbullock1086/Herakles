#!/bin/bash

function herakles ()
{
    case $1 in
	SampleStability)
	    shift
	    $Herakles/share/SampleStability.py $@
	    ;;
	SampleCorr)
	    shift
	    $Herakles/share/SampleCorr.py $@
	    ;;
	*)
	    echo undefined Herakles test $1
    esac
}
