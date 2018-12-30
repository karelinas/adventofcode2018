# boost program options
find_package(Boost 1.65 COMPONENTS program_options REQUIRED)
include_directories( ${Boost_INCLUDE_DIR})

add_library( AocCpu cpu.hpp cpu.cpp opcodes.cpp opcodes.hpp program.cpp program.hpp )

add_executable( AocRun main.cpp )
target_link_libraries( AocRun AocCpu ${Boost_LIBRARIES})
