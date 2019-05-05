#!/bin/bash

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

echo "Building dev docker image..."
DOCKER_IMG=`docker build -f dev/Dockerfile.dev dev | awk '/Successfully built/{print $NF}'`

echo "Using docker image $DOCKER_IMG"
docker run -it -v "${PROJECT_DIR}":/home/dev/devops_workshop $DOCKER_IMG /home/dev/devops_workshop/scripts/build-and-run.sh

