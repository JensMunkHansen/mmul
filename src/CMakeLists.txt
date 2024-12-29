cmake_minimum_required(VERSION 3.14)

project(mmul
  LANGUAGES
  CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../CMake)

find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)

include(spsEmscriptenSettings)
include(spsHardware)

add_subdirectory(baseline)
add_subdirectory(baseline_nonpower)
add_subdirectory(blocked)
add_subdirectory(blocked_column)
