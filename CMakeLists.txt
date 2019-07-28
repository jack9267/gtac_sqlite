cmake_minimum_required(VERSION 3.13)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(gtac_sqlite C CXX)

# Embedded pdb in static libraries
set(EMBEDDED_PDB ON)

include(j-generic)

# include the SDK
include_directories("SDK/include")
include_directories("SDK/Helper")

add_subdirectory(Projects)
