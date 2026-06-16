#pragma once

#include "Board.hpp"
#include <vector>

class MoveGenerator
{
public:
    static std::vector<Move> generateMoves(const Board& board, PieceColor color, bool king_moved, bool rook_kingside, bool rook_queenside, const std::optional<Move>& last_move);       //! & not needed after std::vector<Move> thanks to RVO, static is used so that we don't need to create an actual object in the source file (it acts as a namespace, MoveGenerator::generateMoves)
    static bool isInCheck(const Board& board, PieceColor color);

private:
    static std::vector<Move> generatePawnMoves(const Board& board, PieceColor color, int row, int col, const std::optional<Move>& last_move);
    static std::vector<Move> generateRookMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateKnightMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateBishopMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateQueenMoves(const Board& board, PieceColor color, int row, int col);
    static std::vector<Move> generateKingMoves(const Board& board, PieceColor color, int row, int col, bool king_moved, bool rook_kingside, bool rook_queenside);
};