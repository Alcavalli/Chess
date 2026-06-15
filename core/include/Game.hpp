#pragma once

#include "MoveGenerator.hpp"        //! MoveGenerator already includes anything else

class Game
{
private:
    Board board{};
    GameStatus game_state{GameStatus::Menu};
    PieceColor current_turn{PieceColor::White};
    GameMode game_mode{GameMode::PvP};
    Difficulty difficulty{Difficulty::Medium};
    std::vector<Board> board_history{};
    std::vector<Move> move_history{};
    size_t current_index{};
    void processInput();
    void update();

public:
    Game(GameMode mode = GameMode::PvP, Difficulty diff = Difficulty::Medium);
    void run();
    void goToStart();
    void goBack();
    void goForward();
    void goToEnd();
};