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

// TODO: take in consideration checks and previous moves
std::vector<Move> MoveGenerator::generatePawnMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;

    return moves;
}

std::vector<Move> MoveGenerator::generateRookMoves(const Board& board, PieceColor color, int row, int col)
{
    std::vector<Move> moves;
    for (int i{row}; ++i < Constants::BOARD_DIM; )      //* Already checks the board limits
    {
        if (board.getSquare(i, col))
        {
            if (board.getSquare(i, col)->color == color)    break;
            else
            {
                moves.push_back(Move{{row, col}, {i, col}});
                break;
            }
        }
        else    moves.push_back(Move{{row, col}, {i, col}});
    }
    for (int i{row}; --i >= 0; )
    {
        if (board.getSquare(i, col))
        {
            if (board.getSquare(i, col)->color == color)    break;
            else
            {
                moves.push_back(Move{{row, col}, {i, col}});
                break;
            }
        }
        else    moves.push_back(Move{{row, col}, {i, col}});
    }
    for (int i{col}; ++i < Constants::BOARD_DIM; )
    {
        if (board.getSquare(row, i))
        {
            if (board.getSquare(row, i)->color == color)    break;
            else
            {
                moves.push_back(Move{{row, col}, {row, i}});
                break;
            }
        }
        else    moves.push_back(Move{{row, col}, {row, i}});
    }
    for (int i{col}; --i >= 0; )
    {
        if (board.getSquare(row, i))
        {
            if (board.getSquare(row, i)->color == color)    break;
            else
            {
                moves.push_back(Move{{row, col}, {row, i}});
                break;
            }
        }
        else    moves.push_back(Move{{row, col}, {row, i}});
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
        if ((new_row >= 0 && new_row < Constants::BOARD_DIM) && (new_col >= 0 && new_col < Constants::BOARD_DIM) && (board.getSquare(new_row, new_col) == std::nullopt || board.getSquare(new_row, new_col)->color != color))
            moves.push_back(Move{{row, col}, {new_row, new_col}});
    }
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