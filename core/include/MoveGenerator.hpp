#pragma once

#include "Board.hpp"
#include <vector>

class MoveGenerator
{
public:
    static std::vector<Move> generateMoves(const Board& board, PieceColor color);       //! & not needed after std::vector<Move> thanks to RVO, static is used so that we don't need to create an actual object in the source file (it acts as a namespace, MoveGenerator::generateMoves)

private:
    static std::vector<Move> generatePawnMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateRookMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateKnightMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateBishopMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateQueenMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateKingMoves(const Board& board, PieceColor color, int row, int col);
    static bool isInCheck(const Board& board, PieceColor color);
};