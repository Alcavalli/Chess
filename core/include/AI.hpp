#pragma once

#include "Board.hpp"
#include <climits>

class AI
{
private:
    Difficulty difficulty{Difficulty::Medium};
    PieceColor color{PieceColor::Black};
    int max_depth{};
    bool white_king_moved{}, white_rook_kingside_moved{}, white_rook_queenside_moved{}, black_king_moved{}, black_rook_kingside_moved{}, black_rook_queenside_moved{};
    int minimax(Board board, bool is_maximizing, std::optional<Move> last_m, bool w_king, bool w_rook_king, bool w_rook_queen, bool b_king, bool b_rook_king, bool b_rook_queen, int alpha = -Constants::INF, int beta = +Constants::INF, int depth = 0) const;
    const std::optional<Move> getBestMove(const Board& board, const std::optional<Move> last_m);
    int evaluate(const Board& board) const;

public:
    AI(Difficulty diff, PieceColor c);
    const std::optional<Move> makeMove(const Board& board, bool white_king, bool white_rook_kingside, bool white_rook_queenside, bool black_king, bool black_rook_kingside, bool black_rook_queenside, const std::optional<Move> last_m);
};