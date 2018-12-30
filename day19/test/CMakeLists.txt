find_package(GTest REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/src ${GTEST_INCLUDE_DIRS})

add_executable(AocCpuTest main.cpp opcodes_test.cpp program_test.cpp cpu_test.cpp)

target_link_libraries(AocCpuTest AocCpu ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
