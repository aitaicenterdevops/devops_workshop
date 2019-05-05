#!/bin/bash

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

cd facedetection_va

mkdir -p cmake-build-release
cd cmake-build-release
if [ ! -f Makefile ] ; then
  cmake -DCMAKE_BUILD_TYPE=Release ..
fi
make

