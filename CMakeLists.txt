cmake_minimum_required(VERSION 3.1)

set(CMAKE_VERBOSE_MAKEFILE ON)

if(aoc2021_VERBOSE)
  message(STATUS "Configuring...")
endif()

project(aoc2021 VERSION 1.0.0)
add_executable(aoc2021 src/main.cpp)
target_sources(aoc2021 PRIVATE
    src/utils.cpp
    src/utils.h
    src/day01.cpp
    src/day02.cpp
    src/day03.cpp
    src/day04.cpp
    src/day05.cpp
    src/day06.cpp
    src/day07.cpp
    src/day08.cpp
    src/day09.cpp
    src/day10.cpp
    src/day11.cpp
    src/day12.cpp
    src/day13.cpp
    src/day14.cpp
    src/day15.cpp
    src/day16.cpp
    src/day17.cpp
    src/day18.cpp
    src/day19.cpp
    src/day20.cpp
    src/day21.cpp
    src/day22.cpp
    src/day23.cpp
    src/day24.cpp
    src/day25.cpp
)

set(CMAKE_C_COMPILER "gcc-12")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-12")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -std=c++20 -fcoroutines -pthread")

set(aoc2021_VERSION_STRING "1.0.0")
set(aoc2021_DESCRIPTION "Advent of Code 2021 using C++")

set(aoc2021_VERSION_REGEX "^([0-9]+)\\.([0-9]+)\\.([0-9]+)([-]rc[-]|\\.)?([0-9]*)$")
string(REGEX REPLACE     "${aoc2021_VERSION_REGEX}" "\\1"
  aoc2021_VERSION_MAJOR "${aoc2021_VERSION_STRING}")
string(REGEX REPLACE     "${aoc2021_VERSION_REGEX}" "\\2"
  aoc2021_VERSION_MINOR "${aoc2021_VERSION_STRING}")
string(REGEX REPLACE     "${aoc2021_VERSION_REGEX}" "\\3"
  aoc2021_VERSION_PATCH "${aoc2021_VERSION_STRING}")
string(REGEX REPLACE     "${aoc2021_VERSION_REGEX}" "\\5"
  aoc2021_VERSION_PRERELEASE "${aoc2021_VERSION_STRING}")

message(STATUS "${aoc2021_VERSION_PRERELEASE}")

# Package version
set(aoc2021_VERSION
  "${aoc2021_VERSION_MAJOR}.${aoc2021_VERSION_MINOR}.${aoc2021_VERSION_PATCH}")

if(aoc2021_VERSION_PRERELEASE)
  set(aoc2021_VERSION "${aoc2021_VERSION}.${aoc2021_VERSION_PRERELEASE}")
else()
  set(aoc2021_VERSION "${aoc2021_VERSION}.0")
endif()
message(STATUS "${aoc2021_VERSION}")

if(aoc2021_VERBOSE)
  message(STATUS "Configuration script parsing status [")
  message(STATUS "  Description : ${aoc2021_DESCRIPTION}")
  message(STATUS "  Version     : ${aoc2021_VERSION} (${aoc2021_VERSION_STRING})")
  message(STATUS "]")
endif()

if(aoc2021_VERBOSE)
  message(STATUS "Configuring done")
endif(aoc2021_VERBOSE)
