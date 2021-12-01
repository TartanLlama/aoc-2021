# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("aoc-2021")
find_package(tl-ranges CONFIG REQUIRED)

# Include sub-projects.
add_subdirectory ("aoc-2021")
