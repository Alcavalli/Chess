#include "MoveGenerator.hpp"
#include <cassert>

std::vector<Move> MoveGenerator::generateMoves(const Board& board, PieceColor color)
{
    std::vector<Move> legal_moves;
    for (int i{}; i < Constants::BOARD_DIM; ++i)
        for (int j{}; j < Constants::BOARD_DIM; ++j)
        {
            if (board.getSquare(i, j) == std::nullopt || board.getSquare(i, j)->color != color)
                continue;
            switch (board.getSquare(i, j)->type)
            {
            case PieceType::Pawn:
            {
                std::vector<Move> temp = generatePawnMoves(board, color, i, j);
                legal_moves.insert(legal_moves.end(), temp.begin(), temp.end());    //? To concatenate the vectors
                break;
            }
            case PieceType::Rook:
            {
                std::vector<Move> temp = generateRookMoves(board, color, i, j);
                legal_moves.insert(legal_moves.end(), temp.begin(), temp.end());
                break;
            }
            case PieceType::Knight:
            {
                std::vector<Move> temp = generateKnightMoves(board, color, i, j);
                legal_moves.insert(legal_moves.end(), temp.begin(), temp.end());
                break;
            }
            case PieceType::Bishop:
            {
                std::vector<Move> temp = generateBishopMoves(board, color, i, j);
                legal_moves.insert(legal_moves.end(), temp.begin(), temp.end());
                break;
            }
            case PieceType::Queen:
            {
                std::vector<Move> temp = generateQueenMoves(board, color, i, j);
                legal_moves.insert(legal_moves.end(), temp.begin(), temp.end());
                break;
            }
            case PieceType::King:
            {
                std::vector<Move> temp = generateKingMoves(board, color, i, j);
                legal_moves.insert(legal_moves.end(), temp.begin(), temp.end());
                break;
            }
            default:
                assert(false);          //! It shouldn't happen to reach the default case
            }
        }
    return legal_moves;
}

// TODO: take in consideration checks
std::vector<Move> MoveGenerator::generatePawnMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;

    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;

    return moves;
}

std::vector<Move> MoveGenerator::generateKnightMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;
    if ((row + 1 >= 0 && row + 1 < Constants::BOARD_DIM) && (col + 2 >= 0 && col + 2 < Constants::BOARD_DIM) && (board.getSquare(row + 1, col + 2) == std::nullopt || board.getSquare(row + 1, col + 2)->color != color))
        moves.push_back(Move({row, col}, {row + 1, col + 2}));
    if ((row - 1 >= 0 && row - 1 < Constants::BOARD_DIM) && (col + 2 >= 0 && col + 2 < Constants::BOARD_DIM) && (board.getSquare(row - 1, col + 2) == std::nullopt || board.getSquare(row - 1, col + 2)->color != color))
        moves.push_back(Move({row, col}, {row - 1, col + 2}));
    if ((row + 1 >= 0 && row + 1 < Constants::BOARD_DIM) && (col - 2 >= 0 && col - 2 < Constants::BOARD_DIM) && (board.getSquare(row + 1, col - 2) == std::nullopt || board.getSquare(row + 1, col - 2)->color != color))
        moves.push_back(Move({row, col}, {row + 1, col - 2}));
    if ((row - 1 >= 0 && row - 1 < Constants::BOARD_DIM) && (col - 2 >= 0 && col - 2 < Constants::BOARD_DIM) && (board.getSquare(row - 1, col - 2) == std::nullopt || board.getSquare(row - 1, col - 2)->color != color))
        moves.push_back(Move({row, col}, {row - 1, col - 2}));
    if ((row + 2 >= 0 && row + 2 < Constants::BOARD_DIM) && (col + 1 >= 0 && col + 1 < Constants::BOARD_DIM) && (board.getSquare(row + 2, col + 1) == std::nullopt || board.getSquare(row + 2, col + 1)->color != color))
        moves.push_back(Move({row, col}, {row + 2, col + 1}));
    if ((row - 2 >= 0 && row - 2 < Constants::BOARD_DIM) && (col + 1 >= 0 && col + 1 < Constants::BOARD_DIM) && (board.getSquare(row - 2, col + 1) == std::nullopt || board.getSquare(row - 2, col + 1)->color != color))
        moves.push_back(Move({row, col}, {row - 2, col + 1}));
    if ((row + 2 >= 0 && row + 2 < Constants::BOARD_DIM) && (col - 1 >= 0 && col - 1 < Constants::BOARD_DIM) && (board.getSquare(row + 2, col - 1) == std::nullopt || board.getSquare(row + 2, col - 1)->color != color))
        moves.push_back(Move({row, col}, {row + 2, col - 1}));
    if ((row - 2 >= 0 && row - 2 < Constants::BOARD_DIM) && (col - 1 >= 0 && col - 1 < Constants::BOARD_DIM) && (board.getSquare(row - 2, col - 1) == std::nullopt || board.getSquare(row - 2, col - 1)->color != color))
        moves.push_back(Move({row, col}, {row - 2, col - 1}));
    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;

    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;

    return moves;
}

std::vector<Move> MoveGenerator::generateKingMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;

    return moves;
}