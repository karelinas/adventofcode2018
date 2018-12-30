cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

project(AocCpu)
add_subdirectory(src)
add_subdirectory(test)
enable_testing()
add_test(NAME AocCpuTest COMMAND AocCpuTest)
