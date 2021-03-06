
cmake_minimum_required(VERSION 3.0)

project(facedetection_va)

set(CMAKE_CXX_COMPILER g++)

find_package(OpenCV 3.4.3 REQUIRED)
find_package(Boost 1.70 REQUIRED COMPONENTS filesystem thread)

add_executable(facedetector ${CMAKE_CURRENT_SOURCE_DIR}/facedetector.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/FaceDetectionService.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/DB.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/json/jsoncpp.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/base64/base64.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest)

target_include_directories(facedetector PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries (facedetector ${OpenCV_LIBS} mysqlcppconn webcc Boost::filesystem Boost::thread)

add_subdirectory(webcc)

add_subdirectory(test)

add_subdirectory(googletest)
