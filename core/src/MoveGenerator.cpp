#include "MoveGenerator.hpp"
#include <cassert>

std::vector<Move> MoveGenerator::generateMoves(const Board& board, PieceColor color, bool king_moved, bool rook_kingside, bool rook_queenside)
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
                std::vector<Move> temp = generateKingMoves(board, color, i, j, king_moved, rook_kingside, rook_queenside);
                legal_moves.insert(legal_moves.end(), temp.begin(), temp.end());
                break;
            }
            default:
                assert(false);          //! It shouldn't happen to reach the default case
            }
        }

    for (auto it{legal_moves.begin()}; it != legal_moves.end(); )
    {
        Board copy_board = board;
        copy_board.applyMove(*it);
        if (isInCheck(copy_board, color))   it = legal_moves.erase(it);
        else    ++it;
    }
    return legal_moves;
}

// TODO: special moves and previous moves
std::vector<Move> MoveGenerator::generatePawnMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;
    int dir{(color == PieceColor::White) ? +1 : -1};
    // Normal moves
    if (board.getSquare(row + dir, col) == std::nullopt)
    {
        // Move by 1
        MoveType type{((color == PieceColor::White && row + dir == Constants::BOARD_DIM - 1) ||
            (color == PieceColor::Black && row + dir == 0)) ? MoveType::Promotion : MoveType::Normal};
        moves.push_back(Move{{row, col}, {row + dir, col}, type});
        // Move by 2
        if ((color == PieceColor::White && row == 1 || color == PieceColor::Black && row == 6) &&
            board.getSquare(row + dir * 2, col) == std::nullopt)
            moves.push_back(Move{{row, col}, {row + dir * 2, col}});
    }
    // Normal capture
    std::array<int, 2> capt{-1, 1};
    for (int i : capt)
    {
        if (col + i >= 0 && col + i < Constants::BOARD_DIM &&
            board.getSquare(row + dir, col + i) && board.getSquare(row + dir, col + i)->color != color)
        {
            MoveType type{((color == PieceColor::White && row + dir == Constants::BOARD_DIM - 1) ||
            (color == PieceColor::Black && row + dir == 0)) ? MoveType::Promotion : MoveType::Normal};
            moves.push_back(Move{{row, col}, {row + dir, col + i}, type});
        }
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;
    std::array<std::pair<int, int>, 4> offsets{
        std::pair{1, 0}, {0, 1}, {-1, 0}, {0, -1}
    };
    for (auto [dr, dc] : offsets)
        for (int i{}; ++i < Constants::BOARD_DIM; )
        {
            int new_row{row + dr * i}, new_col{col + dc * i};
            if (new_row < 0 || new_row >= Constants::BOARD_DIM ||
                new_col < 0 || new_col >= Constants::BOARD_DIM)   break;
            if (board.getSquare(new_row, new_col))
            {
                if (board.getSquare(new_row, new_col)->color != color)
                    moves.push_back(Move{{row, col}, {new_row, new_col}});
                break;
            }
            else    moves.push_back(Move{{row, col}, {new_row, new_col}});
        }
    return moves;
}

std::vector<Move> MoveGenerator::generateKnightMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;
    std::array<std::pair<int, int>, 8> offsets{
        std::pair{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}
    };
    for (auto [dr, dc] : offsets)
    {
        int new_row = row + dr;
        int new_col = col + dc;
        if (new_row < 0 || new_row >= Constants::BOARD_DIM ||
            new_col < 0 || new_col >= Constants::BOARD_DIM) continue;
        if (board.getSquare(new_row, new_col) == std::nullopt ||
            board.getSquare(new_row, new_col)->color != color)
            moves.push_back(Move{{row, col}, {new_row, new_col}});
    }
    return moves;
}

std::vector<Move> MoveGenerator::generateBishopMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;
    std::array<std::pair<int, int>, 4> offsets{
        std::pair{1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };
    for (auto [dr, dc] : offsets)
        for (int i{}; ++i < Constants::BOARD_DIM; )
        {
            int new_row{row + dr * i}, new_col{col + dc * i};
            if (new_row < 0 || new_row >= Constants::BOARD_DIM ||
                new_col < 0 || new_col >= Constants::BOARD_DIM)   break;
            if (board.getSquare(new_row, new_col))
            {
                if (board.getSquare(new_row, new_col)->color != color)
                    moves.push_back(Move{{row, col}, {new_row, new_col}});
                break;
            }
            else    moves.push_back(Move{{row, col}, {new_row, new_col}});
        }
    return moves;
}

std::vector<Move> MoveGenerator::generateQueenMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> rook_moves{generateRookMoves(board, color, row, col)};
    std::vector<Move> bishop_moves{generateBishopMoves(board, color, row, col)};
    rook_moves.insert(rook_moves.end(), bishop_moves.begin(), bishop_moves.end());
    return rook_moves;
}

std::vector<Move> MoveGenerator::generateKingMoves(const Board& board, PieceColor color, int row, int col, bool king_moved, bool rook_kingside, bool rook_queenside)
{
    std::vector<Move> moves;
    std::array<std::pair<int, int>, 8> offsets{
        std::pair{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };
    for (auto [dr, dc] : offsets)
    {
        int new_row{row + dr}, new_col{col + dc};
        if (new_row < 0 || new_row >= Constants::BOARD_DIM ||
            new_col < 0 || new_col >= Constants::BOARD_DIM)   continue;
        if (board.getSquare(new_row, new_col))
        {
            if (board.getSquare(new_row, new_col)->color != color)
                moves.push_back(Move{{row, col}, {new_row, new_col}});
            continue;
        }
        else    moves.push_back(Move{{row, col}, {new_row, new_col}});
    }

    if (!king_moved)
    {
        if (!rook_kingside && board.getSquare(row, col + 1) == std::nullopt && board.getSquare(row, col + 2) == std::nullopt)
        {
            Board copy{board};
            copy.applyMove(Move{{row, col}, {row, col + 1}});
            if (!isInCheck(copy, color))
            {
                copy.applyMove(Move{{row, col + 1}, {row, col + 2}});
                if (!isInCheck(copy, color))
                    moves.push_back(Move{{row, col}, {row, col + 2}, MoveType::ShortCastle});
            }
        }
        if (!rook_queenside && board.getSquare(row, col - 1) == std::nullopt && board.getSquare(row, col - 2) == std::nullopt && board.getSquare(row, col - 3) == std::nullopt)
        {
            Board copy{board};
            copy.applyMove(Move{{row, col}, {row, col - 1}});
            if (!isInCheck(copy, color))
            {
                copy.applyMove(Move{{row, col - 1}, {row, col - 2}});
                if (!isInCheck(copy, color))
                {
                    copy.applyMove(Move{{row, col - 2}, {row, col - 3}});
                    if (!isInCheck(copy, color))
                        moves.push_back(Move{{row, col}, {row, col - 3}, MoveType::LongCastle});
                }
            }
        }
    }
    return moves;
}

bool MoveGenerator::isInCheck(const Board& board, PieceColor color)
{
    int row{}, col{};
    for (int i{}; i < Constants::BOARD_DIM; ++i)
        for (int j{}; j < Constants::BOARD_DIM; ++j)
            if (board.getSquare(i, j) && board.getSquare(i, j)->type == PieceType::King && board.getSquare(i, j)->color == color)
                row = i, col = j;

    // Check by rook or queen
    std::array<std::pair<int, int>, 4> rook_offsets{
        std::pair{1, 0}, {0, 1}, {-1, 0}, {0, -1}
    };
    for (auto [dr, dc] : rook_offsets)
        for (int i{}; ++i < Constants::BOARD_DIM; )
        {
            int new_row{row + dr * i}, new_col{col + dc * i};
            if (new_row < 0 || new_row >= Constants::BOARD_DIM ||
                new_col < 0 || new_col >= Constants::BOARD_DIM)   break;
            if (board.getSquare(new_row, new_col))
            {
                if (board.getSquare(new_row, new_col)->color != color &&
                   (board.getSquare(new_row, new_col)->type == PieceType::Rook ||
                    board.getSquare(new_row, new_col)->type == PieceType::Queen))
                    return true;
                break;      //? If we find a friendly piece
            }
        }

    // Check by bishop or queen
    std::array<std::pair<int, int>, 4> bishop_offsets{
        std::pair{1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };
    for (auto [dr, dc] : bishop_offsets)
        for (int i{}; ++i < Constants::BOARD_DIM; )
        {
            int new_row{row + dr * i}, new_col{col + dc * i};
            if (new_row < 0 || new_row >= Constants::BOARD_DIM ||
                new_col < 0 || new_col >= Constants::BOARD_DIM)   break;
            if (board.getSquare(new_row, new_col))
            {
                if (board.getSquare(new_row, new_col)->color != color &&
                   (board.getSquare(new_row, new_col)->type == PieceType::Bishop ||
                    board.getSquare(new_row, new_col)->type == PieceType::Queen))
                    return true;
                break;
            }
        }

    // Check by knight
    std::array<std::pair<int, int>, 8> knight_offsets{
        std::pair{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}
    };
    for (auto [dr, dc] : knight_offsets)
    {
        int new_row = row + dr;
        int new_col = col + dc;
        if (new_row >= 0 && new_row < Constants::BOARD_DIM &&
            new_col >= 0 && new_col < Constants::BOARD_DIM &&
            board.getSquare(new_row, new_col) &&
            board.getSquare(new_row, new_col)->color != color &&
            board.getSquare(new_row, new_col)->type == PieceType::Knight)
            return true;
    }

    // Check by pawn
    int dir{(color == PieceColor::White) ? +1 : -1};
    std::array<int, 2> capt{-1, 1};
    for (int i : capt)
        if (col + i >= 0 && col + i < Constants::BOARD_DIM &&
            board.getSquare(row + dir, col + i) &&
            board.getSquare(row + dir, col + i)->color != color &&
            board.getSquare(row + dir, col + i)->type == PieceType::Pawn)
            return true;

    // "Check" by other king
    std::array<std::pair<int, int>, 8> king_offsets{
        std::pair{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };
    for (auto [dr, dc] : king_offsets)
    {
        int new_row{row + dr}, new_col{col + dc};
        if (new_row >= 0 && new_row < Constants::BOARD_DIM &&
            new_col >= 0 && new_col < Constants::BOARD_DIM &&
            board.getSquare(new_row, new_col) &&
            board.getSquare(new_row, new_col)->color != color &&
            board.getSquare(new_row, new_col)->type == PieceType::King)
            return true;
    }

    return false;
}