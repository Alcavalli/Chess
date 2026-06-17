cmake_minimum_required(VERSION 3.16)
project(ChessCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# To include the core header files
include_directories(core/include)

# To include the core source files
add_executable(chess_core
    core/src/main.cpp
    core/src/Board.cpp
    core/src/MoveGenerator.cpp
    core/src/Game.cpp
    core/src/AI.cpp
)

add_executable(chess_test
    tests/test_core.cpp
    core/src/Board.cpp
    core/src/MoveGenerator.cpp
    core/src/AI.cpp
)
target_include_directories(chess_test PRIVATE core/include)