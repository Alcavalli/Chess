#pragma once

#include <utility>          //! Needed library to use pair<>
#include "Enums.hpp"
#include <optional>

struct Move
{
    std::pair<int, int> starting_square{};
    std::pair<int, int> arrival_square{};
    MoveType type_move{MoveType::Normal};
    std::optional<PieceType> promotion{std::nullopt};       //* Default value (if promotion = change)
};