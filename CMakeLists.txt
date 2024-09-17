cmake_minimum_required(VERSION 3.5)

project(lchess VERSION 0.1)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(SOURCES
    src/main.cpp
    src/move_generator.cpp
    src/position.cpp
    src/utils.cpp
    src/uci.cpp
    src/search.cpp
    src/eval.cpp
    src/zobrist.cpp
    src/move_list.cpp
    )

add_executable(moss-v0.11-three_fold_repitition_and_TT_fixed ${SOURCES})

