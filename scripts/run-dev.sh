#!/bin/bash

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

echo "Building dev docker image..."
DOCKER_IMG=`docker build -f dev/Dockerfile.dev dev | awk '/Successfully built/{print $NF}'`

echo "Using docker image $DOCKER_IMG"
docker run -it -p 2222:22 -p 4444:444 -v "${PROJECT_DIR}":/home/dev/devops_workshop $DOCKER_IMG bash -c "/etc/init.d/ssh start && /home/dev/devops_workshop/scripts/build-and-run.sh"

# Can do a quick smoke test with
# curl -H "Content-Type: application/json" -X POST -d "{\"faceId\":\"101\"}\n\n" http://localhost:4444/images

