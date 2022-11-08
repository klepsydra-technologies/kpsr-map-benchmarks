cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)      
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT_NAME map_benchmarks)
project(${PROJECT_NAME})

find_package(benchmark REQUIRED)
FILE(GLOB_RECURSE ${PROJECT_NAME}_HEADERS
    ${CMAKE_SOURCE_DIR}/modules/include/*.h
)
FILE(GLOB_RECURSE ${PROJECT_NAME}_SINGLE_THREAD_SRC
    ${CMAKE_SOURCE_DIR}/modules/src/lockfree_*.cpp
    src/map_parameters/*.cpp
    src/single_thread/*.cpp
)


FILE(GLOB_RECURSE ${PROJECT_NAME}_MULTI_THREAD_SRC
    ${CMAKE_SOURCE_DIR}/modules/src/lockfree_*.cpp
    src/map_parameters/*.cpp
    src/multi_thread/*.cpp
)
#MAP_STRING_INT

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SINGLE_THREAD_SRC})
add_executable(${PROJECT_NAME}_multi_thread ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_MULTI_THREAD_SRC})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  include
  )
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_multi_thread
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  include
  )
target_link_libraries(${PROJECT_NAME} benchmark::benchmark_main)
target_link_libraries(${PROJECT_NAME}_multi_thread benchmark::benchmark_main)