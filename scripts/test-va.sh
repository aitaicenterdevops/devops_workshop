#!/bin/bash
set -e

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

echo "Running face detection service tests..."

cd $PROJECT_DIR/facedetection_va
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
  if [ "${CMAKE_BUILD_DIR}" != "${PROJECT_DIR}/facedetection_va/cmake-build-debug" ] ; then
    rm -rf CMakeCache.txt Makefile test
  fi
fi

# Run cmake if needed

if [ ! -f Makefile ] ; then
  cmake -DCMAKE_BUILD_TYPE=Debug ..
fi

# Build

make fd_unittest

# Run

./test/fd_unittest
rm -rf code-coverage-info code-coverage-report
lcov --directory ./test --capture --output-file ./code-coverage.info -rc lcov_branch_coverage=1

# Remove dependencies from code coverage

BASE64="${PROJECT_DIR}/facedetection_va/base64*"
JSON="${PROJECT_DIR}/facedetection_va/json*"
TEST="${PROJECT_DIR}/facedetection_va/test*"
WEBCC="${PROJECT_DIR}/facedetection_va/webcc*"

lcov --remove code-coverage.info "/usr*" "*gtest*" "*gmock*" "$BASE64" "$JSON" "$TEST" "$WEBCC" --output-file code-coverage.info > /dev/null

# Generate coverage report

genhtml code-coverage.info --branch-coverage --output-directory ./code-coverage-report
../../scripts/lcov_cobertura.py code-coverage.info

