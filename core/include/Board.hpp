#pragma once

#include "Piece.hpp"        //* Constants.hpp and Enums.hpp already included = redundant
#include <optional>
#include <array>

using Square = std::optional<Piece>;

class Board
{
private:
    std::array<std::array<Square, Constants::BOARD_DIM>, Constants::BOARD_DIM> board{std::nullopt};

public:
    Board();        //* To place the pieces at the starting position
    const Square &getSquare(int row, int col) const;        //! Don't forget any of the two const
    void setSquare(int row, int col, const Square &sq);
};