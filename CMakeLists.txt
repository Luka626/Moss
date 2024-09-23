cmake_minimum_required(VERSION 3.5)

project(Moss VERSION 0.13)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror ")
set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address -ggdb3")
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
    src/attack_tables.cpp
    )

    add_executable(moss ${SOURCES})
    set_target_properties(moss PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    set_property(TARGET moss PROPERTY VERSION "0.15_refactoring_only")
