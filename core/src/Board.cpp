#include "Board.hpp"
#include <cassert>

Board::Board()
{
    //* White pieces: row 0, White pawns: row 1, Black pieces: row 7, Black pawns: row 6
    //* Rooks: col 0-7, Knights: col 1-6, Bishops: col 2-5, Queen: col 3, King: col 4

    for (int i{}; i < Constants::BOARD_DIM; ++i)
    {
        board[1][i] = {PieceType::Pawn, PieceColor::White};
        board[6][i] = {PieceType::Pawn, PieceColor::Black};
    }

    board[0][0] = {PieceType::Rook, PieceColor::White};
    board[0][7] = {PieceType::Rook, PieceColor::White};
    board[7][0] = {PieceType::Rook, PieceColor::Black};
    board[7][7] = {PieceType::Rook, PieceColor::Black};
    board[0][1] = {PieceType::Knight, PieceColor::White};
    board[0][6] = {PieceType::Knight, PieceColor::White};
    board[7][1] = {PieceType::Knight, PieceColor::Black};
    board[7][6] = {PieceType::Knight, PieceColor::Black};
    board[0][2] = {PieceType::Bishop, PieceColor::White};
    board[0][5] = {PieceType::Bishop, PieceColor::White};
    board[7][2] = {PieceType::Bishop, PieceColor::Black};
    board[7][5] = {PieceType::Bishop, PieceColor::Black};
    board[0][3] = {PieceType::Queen, PieceColor::White};
    board[7][3] = {PieceType::Queen, PieceColor::Black};
    board[0][4] = {PieceType::King, PieceColor::White};
    board[7][4] = {PieceType::King, PieceColor::Black};
}

const Square& Board::getSquare(int row, int col) const
{
    assert(row >= 0 && row < Constants::BOARD_DIM);     //? Debug session = if condition is false,
    assert(col >= 0 && col < Constants::BOARD_DIM);     //? the program is stopped
    return board[row][col];
}

void Board::setSquare(int row, int col, const Square& sq)
{
    assert(row >= 0 && row < Constants::BOARD_DIM);     //? In release = automatically removed
    assert(col >= 0 && col < Constants::BOARD_DIM);
    board[row][col] = sq;
}