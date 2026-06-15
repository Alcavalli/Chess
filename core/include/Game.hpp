#pragma once

#include "MoveGenerator.hpp"        //! MoveGenerator already includes anything else
#include <vector>
#include <optional>

class Game
{
private:
    Board board{};
    GameStatus game_state{GameStatus::Menu};
    PieceColor current_turn{PieceColor::White}, ai_color{PieceColor::Black};
    GameMode game_mode{GameMode::PvP};
    Difficulty difficulty{Difficulty::Medium};
    std::vector<Board> board_history{};
    std::vector<Move> move_history{};
    int current_index{};
    std::optional<Move> processInput();
    void update(Move move);

public:
    Game(GameMode mode = GameMode::PvP, Difficulty diff = Difficulty::Medium, PieceColor ai_c = PieceColor::Black);
    void run();
    void goToStart();
    void goBack();
    void goForward();
    void goToEnd();
    const Board& getBoard() const;
};