cmake_minimum_required(VERSION 3.16)
project(ChessCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include degli header del core
include_directories(core/include)

# Per ora nessun file sorgente — aggiungeremo man mano
add_executable(chess_core
    core/src/main.cpp
    core/src/Board.cpp
)