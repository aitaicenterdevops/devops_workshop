#!/bin/bash

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

cd facedetection_va
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

# Run

echo "Running detection service..."
./facedetector server

