#include "Game.hpp"
#include <algorithm>

Game::Game(GameMode mode, Difficulty diff, PieceColor ai_c) : game_mode{mode}, difficulty{diff}, ai_color{ai_c}
{
    board_history.push_back(board);
}

void Game::run()
{
    while (game_state == GameStatus::InProgress)
    {
        if (game_mode == GameMode::PvP)
        {
            auto move{processInput()};
            if (move)   update(*move);
        }
        else if (ai_color == PieceColor::White)
        {
            //TODO: AI move
            auto move{processInput()};
            if (move)   update(*move);
        }
        else
        {
            auto move{processInput()};
            if (move)   update(*move);
            //TODO: AI move
        }
    }
}

std::optional<Move> Game::processInput()        //! TEMPORARY: managed later in JavaScript
{
    return std::nullopt;
}

void Game::update(Move move)
{
    if (current_index != board_history.size() - 1) return;

    bool king_moved{(current_turn == PieceColor::White) ? white_king_moved : black_king_moved}, rook_kingside_moved{(current_turn == PieceColor::White) ? white_rook_kingside_moved : black_rook_kingside_moved}, rook_queenside_moved{(current_turn == PieceColor::White) ? white_rook_queenside_moved : black_rook_queenside_moved};
    std::vector<Move> legal_moves{MoveGenerator::generateMoves(board, current_turn, king_moved, rook_kingside_moved, rook_queenside_moved)};
    if (std::find_if(legal_moves.begin(), legal_moves.end(), [move](Move m){ return m.starting_square == move.starting_square && m.arrival_square == move.arrival_square && m.type_move == move.type_move && m.promotion == move.promotion; }) == legal_moves.end())   return;

    if (board.getSquare(move.starting_square.first, move.starting_square.second)->type == PieceType::King)
    {
        if (current_turn == PieceColor::White)  white_king_moved = true;
        else    black_king_moved = true;
    }
    if (board.getSquare(move.starting_square.first, move.starting_square.second)->type == PieceType::Rook)
    {
        if (current_turn == PieceColor::White && move.starting_square.second == Constants::BOARD_DIM - 1)
            white_rook_kingside_moved = true;
        else if (current_turn == PieceColor::White)
            white_rook_queenside_moved = true;
        else if (move.starting_square.second == Constants::BOARD_DIM - 1)
            black_rook_kingside_moved = true;
        else    black_rook_queenside_moved = true;
    }
    board.applyMove(move);
    board_history.push_back(board);
    ++current_index;
    move_history.push_back(move);
    current_turn = (current_turn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    bool king_moved_2{(current_turn == PieceColor::White) ? white_king_moved : black_king_moved}, rook_kingside_moved_2{(current_turn == PieceColor::White) ? white_rook_kingside_moved : black_rook_kingside_moved}, rook_queenside_moved_2{(current_turn == PieceColor::White) ? white_rook_queenside_moved : black_rook_queenside_moved};
    if (MoveGenerator::generateMoves(board, current_turn, king_moved_2, rook_kingside_moved_2, rook_queenside_moved_2).empty())
    {
        if (!MoveGenerator::isInCheck(board, current_turn)) game_state = GameStatus::Draw;
        else    game_state = (current_turn == PieceColor::White) ? GameStatus::WhiteWin : GameStatus::BlackWin;
    }
}

void Game::goToStart ()
{
    if (current_index == 0) return;
    board = board_history[current_index = 0];
}

void Game::goBack()
{
    if (current_index == 0) return;
    board = board_history[--current_index];
}

void Game::goForward()
{
    if (current_index == board_history.size() - 1)   return;
    board = board_history[++current_index];
}

void Game::goToEnd()
{
    if (current_index == board_history.size() - 1)   return;
    board = board_history[current_index = board_history.size() - 1];
}

const Board& Game::getBoard() const { return board; }