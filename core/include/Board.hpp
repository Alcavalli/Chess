#pragma once

#include "Piece.hpp"        //* Constants.hpp and Enums.hpp already included = redundant
#include <optional>
#include <array>

using Square = std::optional<Piece>;

class Board
{
private:
    std::array<std::array<Square, Constants::BOARD_DIM>, Consstants::BOARD_DIM> board{};

public:
    Board();        //* Place the pieces at the starting position
};