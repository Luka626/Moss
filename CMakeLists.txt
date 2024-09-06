cmake_minimum_required(VERSION 3.5)

project(lchess VERSION 0.1)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(SOURCES
    src/main.cpp
    src/move_generator.cpp
    src/position.cpp
    src/utils.cpp
    src/uci.cpp
    src/search.cpp
    src/eval.cpp
    )

add_executable(main ${SOURCES})

