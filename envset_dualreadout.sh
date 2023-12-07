#!/bin/sh

source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh

export LD_LIBRARY_PATH=/u/user/swkim/KEY4HEP/dual-readout/install/lib64:$LD_LIBRARY_PATH
export PYTHONPATH=/u/user/swkim/KEY4HEP/dual-readout/install/python:$PYTHONPATH

export LD_LIBRARY_PATH=/u/user/swkim/KEY4HEP/k4geo/install/lib:$LD_LIBRARY_PATH
export k4geo_DIR=/u/user/swkim/KEY4HEP/k4geo/install/lib/cmake/k4geo
