#!/bin/bash
set -e

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

RESPONSE=`curl -s -H "Content-Type: application/json" -X POST -d "@${PROJECT_DIR}/scripts/smoketest.json" http://localhost:4444/images`

echo $RESPONSE | grep "faceId.*123" > /dev/null || {
  echo "Error: Expected \"faceId\" : \"123\" but got response $RESPONSE"
  exit 1
}

echo $RESPONSE | grep "\"x\" : 298" > /dev/null || {
  echo "Error: Expected \"x\" : 298 but got response $RESPONSE"
  exit 1
}

echo "Face detector service is running correctly."
exit 0

# Expected:
#{
#        "faceId" : "123",
#        "faces" : 
#        [
#                {
#                        "height" : 154,
#                        "width" : 154,
#                        "x" : 298,
#                        "y" : 120
#                }
#        ]
#}

