cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)      
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT_NAME map_benchmarks)
project(${PROJECT_NAME})

FILE(GLOB_RECURSE ${PROJECT_NAME}_HEADERS
    ${CMAKE_SOURCE_DIR}/modules/include/*.h
)
FILE(GLOB_RECURSE ${PROJECT_NAME}_SRC
    ${CMAKE_SOURCE_DIR}/modules/src/lockfree_*.cpp
)

find_package(benchmark REQUIRED)

add_executable(${PROJECT_NAME}_string_get ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_string_get.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_string_get
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_string_get benchmark::benchmark_main)

add_executable(${PROJECT_NAME}_string_put ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_string_put.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_string_put
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_string_put benchmark::benchmark_main)

add_executable(${PROJECT_NAME}_string_erase ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_string_erase.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_string_erase
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_string_erase benchmark::benchmark_main)

add_executable(${PROJECT_NAME}_uint_get ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_uint_get.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_uint_get
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_uint_get benchmark::benchmark_main)

add_executable(${PROJECT_NAME}_uint_put ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_uint_put.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_uint_put
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_uint_put benchmark::benchmark_main)

add_executable(${PROJECT_NAME}_uint_erase ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_uint_erase.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_uint_erase
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_uint_erase benchmark::benchmark_main)

add_executable(${PROJECT_NAME}_string_vector_put ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_string_vector_put.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_string_vector_put
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_string_vector_put benchmark::benchmark_main)

add_executable(${PROJECT_NAME}_string_vector_get ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_string_vector_get.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_string_vector_get
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_string_vector_get benchmark::benchmark_main)

add_executable(${PROJECT_NAME}_string_vector_erase ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC} src/map_string_vector_erase.cpp)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_string_vector_erase
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  )

target_link_libraries(${PROJECT_NAME}_string_vector_erase benchmark::benchmark_main)