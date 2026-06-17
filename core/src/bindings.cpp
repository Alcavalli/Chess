//? File bridge between C++ and JavaScript

#include "Game.hpp"
#include <emscripten/bind.h>
#include <string>

//* Global instance (work with pointers or lifetime not needed)
static Game game{};     //? static ensures that game functions are not accessible from other external files

char squareToChar(const Square &sq)
{
    if (!sq)
        return '.';
    if (sq->type == PieceType::Pawn)
        return (sq->color == PieceColor::White) ? 'P' : 'p';
    if (sq->type == PieceType::Rook)
        return (sq->color == PieceColor::White) ? 'R' : 'r';
    if (sq->type == PieceType::Knight)
        return (sq->color == PieceColor::White) ? 'N' : 'n';
    if (sq->type == PieceType::Bishop)
        return (sq->color == PieceColor::White) ? 'B' : 'b';
    if (sq->type == PieceType::Queen)
        return (sq->color == PieceColor::White) ? 'Q' : 'q';
    if (sq->type == PieceType::King)
        return (sq->color == PieceColor::White) ? 'K' : 'k';
    return '#';
}

// Functions that JS is gonna call
std::string getBoardString()        //* Returns the board as a 64-char string
{
    std::string string_board{};
    for (int i{}; i < Constants::BOARD_DIM; ++i)
        for (int j{}; j < Constants::BOARD_DIM; ++j)
            string_board.push_back(squareToChar(game.getBoard().getSquare(i, j)));
    return string_board;
}

int getCurrentTurn()        //* Returns 0 for white and 1 for black
{
    return (game.getCurrentTurn() == PieceColor::White) ? 0 : 1;
}

int getGameStatus()         //* Returns an int for each status of the game
{
    switch (game.getGameStatus())
    {
    case GameStatus::Menu:
        return 0;
    case GameStatus::InProgress:
        return 1;
    case GameStatus::WhiteWin:
        return 2;
    case GameStatus::BlackWin:
        return 3;
    case GameStatus::Draw:
        return 4;
    case GameStatus::Stalemate:
        return 5;
    default:
        return -1;
    }
}

EMSCRIPTEN_BINDINGS(chess)      //? To register the C++ functions, making them callable by JS
{
    emscripten::function("getBoard", &getBoardString);
    emscripten::function("getCurrentTurn", &getCurrentTurn);
    emscripten::function("getGameStatus", &getGameStatus);
}