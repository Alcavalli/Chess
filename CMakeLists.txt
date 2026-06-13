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
)