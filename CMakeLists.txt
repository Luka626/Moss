cmake_minimum_required(VERSION 3.5)

project(lchess)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

add_library(lib src/position.cpp src/move_generator.cpp src/utils.cpp)

add_executable(main src/main.cpp)

target_link_libraries(main lib)
