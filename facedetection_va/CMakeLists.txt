
cmake_minimum_required(VERSION 3.0)

project(facedetection_va)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_LINKER g++)
set(CMAKE_BUILD_TYPE "Release")

find_package(OpenCV 3.4.1 REQUIRED)

add_executable(facedetector ${CMAKE_CURRENT_SOURCE_DIR}/facedetector.cpp)

target_include_directories (facedetector PUBLIC ${OpenCV_INCLUDE_DIR})

find_library(MYQSL_LIB mysqlclient)

target_link_libraries (facedetector ${OpenCV_LIBS} ${MYSQL_LIB})

