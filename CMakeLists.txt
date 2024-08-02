cmake_minimum_required(VERSION 3.13)

# Set project name and version
project(Cornelius VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# get the google test framework
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.11.0)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

add_library(GeneralGeometryElement STATIC src/GeneralGeometryElement.cpp)
add_library(Line STATIC src/Line.cpp)
add_library(Polygon STATIC src/Polygon.cpp)

add_executable(Cornelius src/main.cpp)
target_link_libraries(Cornelius GeneralGeometryElement Line Polygon)

add_executable(testGeneralGeometryElement
               src_test/TestGeneralGeometryElement.cpp)
target_link_libraries(testGeneralGeometryElement GeneralGeometryElement
                      gtest_main gmock_main)
target_include_directories(testGeneralGeometryElement
                           PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(testLine src_test/TestLine.cpp)
target_link_libraries(testLine Line GeneralGeometryElement gtest_main
                      gmock_main)
target_include_directories(testLine PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(testPolygon src_test/TestPolygon.cpp)
target_link_libraries(testPolygon Polygon GeneralGeometryElement Line
                      gtest_main gmock_main)
target_include_directories(testPolygon PRIVATE ${CMAKE_SOURCE_DIR}/src)
