cmake_minimum_required(VERSION 3.5.1)
project(ns)

list(APPEND CMAKE_MODULE_PATH "${ns_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${ns_SOURCE_DIR}/cmake-modules")

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") # This is to make sure Intel compiler uses C++14 standard.

add_subdirectory(src)

enable_testing()
add_subdirectory(test)

