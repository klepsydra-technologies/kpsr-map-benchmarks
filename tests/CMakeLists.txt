cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)      
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT_NAME map_tests)
project(${PROJECT_NAME})

FILE(GLOB_RECURSE ${PROJECT_NAME}_HEADERS
    ${CMAKE_SOURCE_DIR}/modules/include/*.h
)
FILE(GLOB_RECURSE ${PROJECT_NAME}_SRC
    ${CMAKE_SOURCE_DIR}/modules/src/lockfree_*.cpp
)

add_executable(${PROJECT_NAME}_lockfree_hash_table ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/lockfree_hash_table_test.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_lockfree_hash_table
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )