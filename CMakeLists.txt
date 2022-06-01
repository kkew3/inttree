cmake_minimum_required(VERSION 3.10)
project(inttree VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(inttree SHARED inttree.hpp)
set_target_properties(inttree PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(inttree PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(inttree PROPERTIES PUBLIC_HEADER inttree.hpp)

add_executable(demo demo.cpp inttree.hpp)

add_subdirectory(lib/Catch2)
add_executable(tests test.cpp inttree.hpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
