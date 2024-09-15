cmake_minimum_required(VERSION 3.5)

project(lchess VERSION 0.1)

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

add_executable(moss-v.0.10 ${SOURCES})

