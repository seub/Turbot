cmake_minimum_required(VERSION 3.5)

project(Turbot LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/board.cpp
    src/boardhelper.cpp
    src/game.cpp
    src/main.cpp
    src/move.cpp
    src/mover.cpp
    src/piece.cpp
    src/position.cpp
    src/square.cpp
    src/tools.cpp
    src/evaluator.cpp
    src/moveGenerator.cpp
    )

add_executable(Turbot ${SOURCES})
