cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)      
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJ_MAIN_NAME "map_benchmarks")
project(${PROJ_MAIN_NAME})

add_subdirectory(tests)
add_subdirectory(benchmarks)