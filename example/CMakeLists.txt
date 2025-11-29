cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)      
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT_NAME examplillo)
project(${PROJECT_NAME})

#find_package(tvm REQUIRED)
FILE(GLOB_RECURSE ${PROJECT_NAME}_HEADERS
    ${CMAKE_SOURCE_DIR}/example/include/*.h
)

FILE(GLOB_RECURSE TVM_LIBS
    /usr/local/lib/libtvm*.so
)
FILE(GLOB_RECURSE ${PROJECT_NAME}_SRC
    ${CMAKE_SOURCE_DIR}/example/src/*.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} ${TVM_LIBS})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../modules/include 
  include
  )

