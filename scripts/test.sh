#!/bin/bash
set -e

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

# Run VA tests

./scripts/test-va.sh

# Run web application tests

${PROJECT_DIR}/scripts/build.sh
cd ${PROJECT_DIR}/facedetection_va/cmake-build-release/
./facedetector server &

cd $PROJECT_DIR/facedetection_webapp
service mysql start
export FACE_DETECTOR_URL="http://localhost:4444/images"
gem install bundler && gem install rails && bundle install
rails db:create db:migrate RAILS_ENV=development
rake test
kill %1

