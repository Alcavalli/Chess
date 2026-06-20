//? File bridge between C++ and JavaScript

#include "Game.hpp"
#include <emscripten/bind.h>
#include <string>

//* Global instance (work with pointers or lifetime not needed)
static Game game{};     //? static ensures that game functions are not accessible from other external files

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

// Functions that JS is gonna call
std::string getBoardString()        //* Returns the board as a 64-char string
{
    std::string string_board{};
    for (int i{}; i < Constants::BOARD_DIM; ++i)
        for (int j{}; j < Constants::BOARD_DIM; ++j)
            string_board.push_back(squareToChar(game.getBoard().getSquare(i, j)));
    return string_board;
}

int getCurrentTurn()        //* Returns 0 for white and 1 for black
{
    return (game.getCurrentTurn() == PieceColor::White) ? 0 : 1;
}

int getGameStatus()         //* Returns an int for each status of the game
{
    switch (game.getGameStatus())
    {
    case GameStatus::Menu:
        return 0;
    case GameStatus::InProgress:
        return 1;
    case GameStatus::WhiteWin:
        return 2;
    case GameStatus::BlackWin:
        return 3;
    case GameStatus::Draw:
        return 4;
    case GameStatus::Stalemate:
        return 5;
    default:
        return -1;
    }
}

std::string getLegalMoves(int row, int col)     //* Returns all legal moves of a piece
{
    std::string moves{};
    std::vector<Move> all_moves{game.getLegalMoves(game.getCurrentTurn())};
    for (auto& x : all_moves)
        if (x.starting_square.first == row && x.starting_square.second == col)
        {
            moves += std::to_string(x.arrival_square.first) + ',' + std::to_string(x.arrival_square.second) + ',';
            switch (x.type_move)
            {
            case MoveType::Normal:
                moves.push_back('0');
                break;
            case MoveType::ShortCastle:
                moves.push_back('1');
                break;
            case MoveType::LongCastle:
                moves.push_back('2');
                break;
            case MoveType::EnPassant:
                moves.push_back('3');
                break;
            case MoveType::Promotion:
                moves.push_back('4');
                break;
            default:
                moves.push_back('5');
            }
            moves.push_back('|');
        }
    return moves;
}

void applyMove(int from_row, int from_col, int to_row, int to_col, std::string temp_c)
{
    char c = temp_c.front();
    std::vector<Move> moves{game.getLegalMoves(game.getCurrentTurn())};
    for (auto& x : moves)
        if (x.starting_square.first == from_row && x.starting_square.second == from_col &&
            x.arrival_square.first == to_row && x.arrival_square.second == to_col)
        {
            switch (c)
            {
            case 'N':
                x.promotion = PieceType::Knight;
                break;
            case 'B':
                x.promotion = PieceType::Bishop;
                break;
            case 'R':
                x.promotion = PieceType::Rook;
                break;
            case 'Q':
                x.promotion = PieceType::Queen;
                break;
            default:
                x.promotion = x.promotion;
            }
            game.update(x);
        }
}

void startGame(int temp_mode, int temp_difficulty, int temp_color)
{
    GameMode mode{(temp_mode == 0) ? GameMode::PvP : GameMode::PvE};
    Difficulty difficulty{(temp_difficulty == 0) ? Difficulty::Easy : ((temp_difficulty == 1) ? Difficulty::Medium : Difficulty::Hard)};
    PieceColor color{(temp_color == 0) ? PieceColor::White : PieceColor::Black};
    game = Game{mode, difficulty, color};
}

std::string getAiMove()
{
    std::optional<Move> temp_move{game.getAiMove()};
    if (!temp_move) return "";

    std::string move{std::to_string(temp_move->starting_square.first) + ',' + std::to_string(temp_move->starting_square.second) + ',' + std::to_string(temp_move->arrival_square.first) + ',' + std::to_string(temp_move->arrival_square.second) + ','};
    switch (temp_move->type_move)
    {
    case MoveType::Normal:
        move.push_back('0');
        break;
    case MoveType::ShortCastle:
        move.push_back('1');
        break;
    case MoveType::LongCastle:
        move.push_back('2');
        break;
    case MoveType::EnPassant:
        move.push_back('3');
        break;
    case MoveType::Promotion:
        move.push_back('4');
        break;
    default:
        move.push_back('5');
    }
    return move;
}

void goToStart() { game.goToStart(); }

void goBack() { game.goBack(); }

void goForward() { game.goForward(); }

void goToEnd() { game.goToEnd(); }

int historyIndex() { return game.getIndex(); }

int historySize() { return game.getHistorySize(); }

EMSCRIPTEN_BINDINGS(chess)      //? To register the C++ functions, making them callable by JS
{
    emscripten::function("getBoard", &getBoardString);
    emscripten::function("getCurrentTurn", &getCurrentTurn);
    emscripten::function("getGameStatus", &getGameStatus);
    emscripten::function("getLegalMoves", &getLegalMoves);
    emscripten::function("applyMove", &applyMove);
    emscripten::function("startGame", &startGame);
    emscripten::function("getAiMove", &getAiMove);
    emscripten::function("goToStart", &goToStart);
    emscripten::function("goBack", &goBack);
    emscripten::function("goForward", &goForward);
    emscripten::function("goToEnd", &goToEnd);
    emscripten::function("historyIndex", &historyIndex);
    emscripten::function("historySize", &historySize);
}