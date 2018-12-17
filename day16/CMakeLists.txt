cmake_minimum_required( VERSION 3.10.2 )
project( aoc-day16 )

include_directories( BEFORE ${MY_SOURCE_DIR} )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O3")
add_executable( day16 cpu.cpp solution.cpp )
