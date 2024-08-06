cmake_minimum_required(VERSION 3.13)

# Set project name and version
project(Cornelius VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler options for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

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
add_library(Polyhedron STATIC src/Polyhedron.cpp)
add_library(Square STATIC src/Square.cpp)
add_library(Cube STATIC src/Cube.cpp)
add_library(Hypercube STATIC src/Hypercube.cpp)
add_library(Cornelius STATIC src/Cornelius.cpp)
add_library(CorneliusOld STATIC src_old/cornelius_old.cpp)

add_executable(main src/main.cpp)
target_link_libraries(
  main
  GeneralGeometryElement
  Line
  Polygon
  Polyhedron
  Square
  Cube
  Hypercube
  Cornelius
  CorneliusOld)

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

add_executable(testPolyhedron src_test/TestPolyhedron.cpp)
target_link_libraries(
  testPolyhedron
  Polyhedron
  GeneralGeometryElement
  Polygon
  Line
  gtest_main
  gmock_main)
target_include_directories(testPolyhedron PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(testSquare src_test/TestSquare.cpp)
target_link_libraries(testSquare Square GeneralGeometryElement Line gtest_main
                      gmock_main)
target_include_directories(testSquare PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(testCube src_test/TestCube.cpp)
target_link_libraries(
  testCube
  Cube
  GeneralGeometryElement
  Square
  Line
  Polygon
  gtest_main
  gmock_main)
target_include_directories(testCube PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(testHypercube src_test/TestHypercube.cpp)
target_link_libraries(
  testHypercube
  Hypercube
  GeneralGeometryElement
  Cube
  Square
  Line
  Polygon
  Polyhedron
  gtest_main
  gmock_main)
target_include_directories(testHypercube PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(testCornelius src_test/TestCornelius.cpp)
target_link_libraries(
  testCornelius
  Cornelius
  GeneralGeometryElement
  Hypercube
  Cube
  Square
  Line
  Polygon
  Polyhedron
  CorneliusOld
  gtest_main
  gmock_main)
target_include_directories(testCornelius PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(testCornelius PRIVATE ${CMAKE_SOURCE_DIR}/src_old)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src_test/cornelius_test_data_3D
     DESTINATION ${CMAKE_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src_test/cornelius_test_data_4D
     DESTINATION ${CMAKE_BINARY_DIR})
