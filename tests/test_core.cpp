#include <iostream>
#include "Game.hpp"

char squareToChar(const Square &sq)
{
    if (!sq)
        return '.';
    if (sq->type == PieceType::Pawn)
        return (sq->color == PieceColor::White) ? 'P' : 'p';
    if (sq->type == PieceType::Rook)
        return (sq->color == PieceColor::White) ? 'R' : 'r';
    if (sq->type == PieceType::Knight)
        return (sq->color == PieceColor::White) ? 'N' : 'n';
    if (sq->type == PieceType::Bishop)
        return (sq->color == PieceColor::White) ? 'B' : 'b';
    if (sq->type == PieceType::Queen)
        return (sq->color == PieceColor::White) ? 'Q' : 'q';
    if (sq->type == PieceType::King)
        return (sq->color == PieceColor::White) ? 'K' : 'k';
    return '#';
}

int main()
{
    Board board{};
    for (int i{Constants::BOARD_DIM}; --i >= 0;)
    {
        for (int j{}; j < Constants::BOARD_DIM; ++j)
            std::cout << squareToChar(board.getSquare(i, j)) << ' ';
        std::cout << '\n';
    }

    std::vector<Move> moves{MoveGenerator::generateMoves(board, PieceColor::White, false, false, false, std::nullopt)};
    for (auto x : moves)
        std::cout << '(' << x.starting_square.first << ", " << x.starting_square.second << ") -> (" << x.arrival_square.first << ", " << x.arrival_square.second << ")\n";

    board.applyMove(Move{{1, 3}, {3, 3}});
    for (int i{Constants::BOARD_DIM}; --i >= 0;)
    {
        for (int j{}; j < Constants::BOARD_DIM; ++j)
            std::cout << squareToChar(board.getSquare(i, j)) << ' ';
        std::cout << '\n';
    }

    board.applyMove(Move{{6, 4}, {4, 4}});
    board.applyMove(Move{{1, 4}, {3, 4}});
    std::cout << '\n' << std::boolalpha << "Check: " << MoveGenerator::isInCheck(board, PieceColor::White) << '\n';
    board.applyMove(Move{{7, 5}, {3, 1}});
    for (int i{Constants::BOARD_DIM}; --i >= 0;)
    {
        for (int j{}; j < Constants::BOARD_DIM; ++j)
            std::cout << squareToChar(board.getSquare(i, j)) << ' ';
        std::cout << '\n';
    }
    std::cout << std::boolalpha << "Check: " << MoveGenerator::isInCheck(board, PieceColor::White) << std::noboolalpha << '\n';

    board.applyMove(Move{{0, 1}, {2, 2}});
    board.applyMove(Move{{3, 3}, {4, 4}});
    board.applyMove(Move{{Constants::BLACK_PAWNS, 3}, {4, 3}});
    for (int i{Constants::BOARD_DIM}; --i >= 0;)
    {
        for (int j{}; j < Constants::BOARD_DIM; ++j)
            std::cout << squareToChar(board.getSquare(i, j)) << ' ';
        std::cout << '\n';
    }
    moves = MoveGenerator::generateMoves(board, PieceColor::White, false, false, false, Move{{Constants::BLACK_PAWNS, 3}, {4, 3}});
    for (auto x : moves)
        std::cout << '(' << x.starting_square.first << ", " << x.starting_square.second << ") -> (" << x.arrival_square.first << ", " << x.arrival_square.second << ") - Type:" << ((x.type_move == MoveType::EnPassant) ? "EnPassant\n" : "Normal\n");
    board.applyMove(moves.back());
    std::cout << '\n';
    for (int i{Constants::BOARD_DIM}; --i >= 0;)
    {
        for (int j{}; j < Constants::BOARD_DIM; ++j)
            std::cout << squareToChar(board.getSquare(i, j)) << ' ';
        std::cout << '\n';
    }
    std::cout << std::noboolalpha << ((moves.back().type_move == MoveType::EnPassant) ? "EnPassant\n" : "/\n");

    system("pause");
    return 0;
}