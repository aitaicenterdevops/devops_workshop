#!/bin/bash
set -e

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

# Build

${PROJECT_DIR}/scripts/build.sh

# Run

echo "Running detection service..."
./facedetector server

