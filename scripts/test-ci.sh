#!/bin/bash
set -e

cd "`dirname $0`/.."
PROJECT_DIR=${PWD}

echo "Running face detection service tests..."

cd $PROJECT_DIR
./dist/fd_unittest
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

