#!/bin/bash
set -e

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

# Run webapp

service mysql start

echo 'export FACE_DETECTOR_URL="http://localhost:444/images"' >> ~/.bashrc
source ~/.bashrc

cd facedetection_webapp/

gem install bundler && gem install rails && bundle install
rails assets:precompile RAILS_ENV=production
rails db:create db:migrate RAILS_ENV=production
rails s -e production &

# Build FD service

cd ${PROJECT_DIR}

${PROJECT_DIR}/scripts/build.sh

# Run FD service

cd ${PROJECT_DIR}/facedetection_va/cmake-build-release/
echo "Running detection service..."
./facedetector server

