# CMakeList.txt : CMake project for aoc-2021, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SOURCES "**.cpp")

foreach(FILE IN LISTS SOURCES)
  cmake_path(GET FILE STEM FILE_STEM)
  add_executable(${FILE_STEM} ${FILE})
  target_link_libraries(${FILE_STEM} PRIVATE tl::ranges)
  if(MSVC)
  target_compile_options(${FILE_STEM} PRIVATE "/std:c++latest")
  else()
  target_compile_options(${FILE_STEM} PRIVATE "-std=c++20")
  endif()
endforeach()

# TODO: Add tests and install targets if needed.
