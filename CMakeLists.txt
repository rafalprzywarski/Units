project(units)
cmake_minimum_required(VERSION 2.8)

message("${CMAKE_SYSTEM_PREFIX_PATH}")

add_definitions("-std=c++14")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(SFML REQUIRED system window graphics)
include_directories(${SFML_INCLUDE_DIR})

set(GoogleMock_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/libraries/GoogleMock/include")

add_subdirectory("source")
add_subdirectory("libraries")
