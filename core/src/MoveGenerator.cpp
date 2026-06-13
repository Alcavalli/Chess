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