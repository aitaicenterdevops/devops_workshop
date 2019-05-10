#!/bin/bash
set -e

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

cd facedetection_va
mkdir -p cmake-build-release
cd cmake-build-release

# Clear cmake cache if necessary

if [ -f Makefile ] ; then
  CMAKE_SOURCE_DIR=`cat Makefile | awk '/CMAKE_SOURCE_DIR = /{print $NF}'`
  if [ "${CMAKE_SOURCE_DIR}" != "${PROJECT_DIR}/facedetection_va" ] ; then
    rm -rf CMakeCache.txt Makefile test
  fi
fi

if [ -f CMakeCache.txt ] ; then
  CMAKE_BUILD_DIR=`cat CMakeCache.txt | awk '/# For build in directory: /{print $NF}'`
  if [ "${CMAKE_BUILD_DIR}" != "${PROJECT_DIR}/facedetection_va/cmake-build-release" ] ; then
    rm -f CMakeCache.txt Makefile test
  fi
fi

# Run cmake if needed

if [ ! -f Makefile ] ; then
  cmake -DCMAKE_BUILD_TYPE=Release ..
fi

# Build

make

