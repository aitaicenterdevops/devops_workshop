#!/bin/bash
set -e

PROJECT_DIR=${PWD}

mkdir -p cmake-build-release
cd cmake-build-release

# Clear cmake cache if necessary

if [ -f Makefile ] ; then
  CMAKE_SOURCE_DIR=`cat Makefile | awk '/CMAKE_SOURCE_DIR = /{print $NF}'`
  if [ "${CMAKE_SOURCE_DIR}" != "${PROJECT_DIR}/facedetection_va" ] ; then
    rm -f CMakeCache.txt Makefile
  fi
fi

# Run cmake if needed

if [ ! -f Makefile ] ; then
  cmake -DCMAKE_BUILD_TYPE=Release ..
fi

# Build

make