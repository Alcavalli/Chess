#include "AI.hpp"
#include "MoveGenerator.hpp"

AI::AI(Difficulty diff, PieceColor c) : difficulty(diff), color(c)
{
    switch (difficulty)
    {
    case Difficulty::Easy:
        max_depth = 2;
        break;
    case Difficulty::Medium:
        max_depth = 4;
        break;
    case Difficulty::Hard:
        max_depth = 6;
        break;
    }
}

const std::optional<Move> AI::makeMove(const Board& board, bool white_king, bool white_rook_kingside, bool white_rook_queenside, bool black_king, bool black_rook_kingside, bool black_rook_queenside, const std::optional<Move> last_m)
{
    white_king_moved = white_king, white_rook_kingside_moved = white_rook_kingside, white_rook_queenside_moved = white_rook_queenside, black_king_moved = black_king, black_rook_kingside_moved = black_rook_kingside, black_rook_queenside_moved = black_rook_queenside;

    return getBestMove(board, last_m);
}

const std::optional<Move> AI::getBestMove(const Board& board, const std::optional<Move> last_m)
{
    bool king_moved{(color == PieceColor::White) ? white_king_moved : black_king_moved}, rook_kingside_moved{(color == PieceColor::White) ? white_rook_kingside_moved : black_rook_kingside_moved}, rook_queenside_moved{(color == PieceColor::White) ? white_rook_queenside_moved : black_rook_queenside_moved};

    std::optional<Move> best_move{};
    int best_score{INT_MIN};
    std::vector<Move> moves{MoveGenerator::generateMoves(board, color, king_moved, rook_kingside_moved, rook_queenside_moved, last_m)};
    for (auto& x : moves)
    {
        Board copy{board};
        copy.applyMove(x);
        int score{minimax(copy, false)};
        if (score > best_score) best_move = x;
    }
    return best_move;
}

int AI::minimax(Board board, bool is_maximizing, int alpha, int beta, int depth) const
{
    if (depth == max_depth) return evaluate(board);

    PieceColor col{is_maximizing ? PieceColor::White : PieceColor::Black};
    bool king_moved{(col == PieceColor::White) ? white_king_moved : black_king_moved}, rook_kingside_moved{(col == PieceColor::White) ? white_rook_kingside_moved : black_rook_kingside_moved}, rook_queenside_moved{(col == PieceColor::White) ? white_rook_queenside_moved : black_rook_queenside_moved};
    std::vector<Move> moves{MoveGenerator::generateMoves(board, col, king_moved, rook_kingside_moved, rook_queenside_moved, last_move)};
    if (moves.empty())  return INT_MIN;

    if (is_maximizing)
    {
        int best{-Constants::INF};
        for (auto& x : moves)
        {
            Board copy{board};
            copy.applyMove(x);
            int score{minimax(copy, false, alpha, beta, depth + 1)};
            best = std::max(best, score);
            alpha = std::max(alpha, best);
            if (alpha >= beta)  break;
        }
        return best;
    }
    else
    {
        int best{Constants::INF};
        for (auto& x : moves)
        {
            Board copy{board};
            copy.applyMove(x);
            int score{minimax(copy, true, alpha, beta, depth + 1)};
            best = std::min(best, score);
            beta = std::min(beta, best);
            if (beta <= alpha)  break;
        }
        return best;
    }
}

int AI::evaluate(const Board& board) const
{
    int cnt{};
    for (int i{}; i < Constants::BOARD_DIM; ++i)
        for (int j{}; j < Constants::BOARD_DIM; ++j)
        {
            if (!board.getSquare(i, j))  continue;
            int z{board.getSquare(i, j)->color == PieceColor::White ? +1 : -1};
            switch (board.getSquare(i, j)->type)
            {
            case PieceType::Pawn:
                cnt += z;
                break;
            case PieceType::Knight:
            case PieceType::Bishop:
                cnt += z * 3;
                break;
            case PieceType::Rook:
                cnt += z * 5;
                break;
            case PieceType::Queen:
                cnt += z * 9;
                break;
            default:
                cnt = cnt;
            }
        }
    return cnt;
}