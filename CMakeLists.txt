cmake_minimum_required(VERSION 3.5)

project(Turbot LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)

set(SOURCES
    src/board.cpp
    src/evaluator.cpp
    src/game.cpp
    src/legalmover.cpp
    src/main.cpp
    src/move.cpp
    src/movepicker.cpp
    src/piece.cpp
    src/player.cpp
    src/position.cpp
    src/positioneval.cpp
    src/square.cpp
    src/tools.cpp
    src/zobrist.cpp
    )

add_executable(Turbot ${SOURCES})
