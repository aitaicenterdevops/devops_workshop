#!/bin/bash
set -e

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

if [ ! -f test/libgmock.a ] || [ ! -f test/libgmock_main.a ] || [ ! -f test/libgtest.a ] || [ ! -f test/libgtest_main.a ] ; then
  echo "Building test libs"
  mkdir -p test
  cd test
  cmake /usr/src/googletest -DBUILD_GTEST=true -DBUILD_GMOCK=true
  make
  cp googlemock/gtest/libgtest.a .
  cp googlemock/gtest/libgtest_main.a .
  cp googlemock/libgmock_main.a .
  cp googlemock/libgmock.a .
  rm -rf CMake* cmake* CTest* googlemock Makefile
  cd $PROJECT_DIR
fi

echo "Running tests..."
cd $PROJECT_DIR/facedetection_va

