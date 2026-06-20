#include "AI.hpp"
#include "MoveGenerator.hpp"

AI::AI(Difficulty diff, PieceColor c) : difficulty(diff), color(c)
{
    switch (difficulty)
    {
    case Difficulty::Easy:
        max_depth = 1;
        break;
    case Difficulty::Medium:
        max_depth = 2;
        break;
    case Difficulty::Hard:
        max_depth = 3;
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
    int best_score{color == PieceColor::White ? INT_MIN : INT_MAX};     //? white = starts at lowest
    std::vector<Move> moves{MoveGenerator::generateMoves(board, color, king_moved, rook_kingside_moved, rook_queenside_moved, last_m)};
    for (auto& x : moves)
    {
        Board copy{board};
        copy.applyMove(x);
        int score{minimax(copy, color == PieceColor::Black, x, white_king_moved, white_rook_kingside_moved, white_rook_queenside_moved, black_king_moved, black_rook_kingside_moved, black_rook_queenside_moved)};       //? false = enemy turn
        if (color == PieceColor::White ? score > best_score : score < best_score)
        {
            best_move = x;
            best_score = score;
        }
    }
    return best_move;
}

int AI::minimax(Board board, bool is_maximizing, std::optional<Move> last_m, bool w_king, bool w_rook_king, bool w_rook_queen, bool b_king, bool b_rook_king, bool b_rook_queen, int alpha, int beta, int depth) const
{
    if (depth == max_depth) return evaluate(board);

    //? is maximizing = white turn
    PieceColor col{is_maximizing ? PieceColor::White : PieceColor::Black};
    bool king_moved{(col == PieceColor::White) ? w_king : b_king}, rook_kingside_moved{(col == PieceColor::White) ? w_rook_king : b_rook_king}, rook_queenside_moved{(col == PieceColor::White) ? w_rook_queen : b_rook_queen};
    std::vector<Move> moves{MoveGenerator::generateMoves(board, col, king_moved, rook_kingside_moved, rook_queenside_moved, last_m)};
    if (moves.empty())
    {
        if (!MoveGenerator::isInCheck(board, col))  return 0;
        return is_maximizing ? INT_MIN : INT_MAX;
    }

    if (is_maximizing)
    {
        int best{-Constants::INF};
        for (auto& x : moves)
        {
            bool wk = w_king, wrk = w_rook_king, wrq = w_rook_queen;
            bool bk = b_king, brk = b_rook_king, brq = b_rook_queen;
            Board copy{board};
            copy.applyMove(x);
            if (x.starting_square.first == Constants::WHITE_PIECES && x.starting_square.second == 4 && copy.getSquare(x.arrival_square.first, x.arrival_square.second)->type == PieceType::King)
                wk = true;
            if (x.starting_square.first == Constants::WHITE_PIECES && x.starting_square.second == 0 && copy.getSquare(x.arrival_square.first, x.arrival_square.second)->type == PieceType::Rook)
                wrq = true;
            if (x.starting_square.first == Constants::WHITE_PIECES && x.starting_square.second == Constants::BOARD_DIM - 1 && copy.getSquare(x.arrival_square.first, x.arrival_square.second)->type == PieceType::Rook)
                wrk = true;
            int score{minimax(copy, !is_maximizing, x, wk, wrk, wrq, bk, brk, brq, alpha, beta, depth + 1)};
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
            bool wk = w_king, wrk = w_rook_king, wrq = w_rook_queen;
            bool bk = b_king, brk = b_rook_king, brq = b_rook_queen;
            Board copy{board};
            copy.applyMove(x);
            if (x.starting_square.first == Constants::BLACK_PIECES && x.starting_square.second == 4 && copy.getSquare(x.arrival_square.first, x.arrival_square.second)->type == PieceType::King)
                bk = true;
            if (x.starting_square.first == Constants::BLACK_PIECES && x.starting_square.second == 0 && copy.getSquare(x.arrival_square.first, x.arrival_square.second)->type == PieceType::Rook)
                brq = true;
            if (x.starting_square.first == Constants::BLACK_PIECES && x.starting_square.second == Constants::BOARD_DIM - 1 && copy.getSquare(x.arrival_square.first, x.arrival_square.second)->type == PieceType::Rook)
                brk = true;
            int score{minimax(copy, !is_maximizing, x, wk, wrk, wrq, bk, brk, brq, alpha, beta, depth + 1)};
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
                cnt += z * 100;
                break;
            case PieceType::Knight:
            case PieceType::Bishop:
                cnt += z * 300;
                break;
            case PieceType::Rook:
                cnt += z * 500;
                break;
            case PieceType::Queen:
                cnt += z * 900;
                break;
            default:
                cnt = cnt;
            }
        }
    return cnt;
}