#pragma once

#include <utility>          //! Needed library to use pair<>

struct Move
{
    std::pair<int, int> starting_square;
    std::pair<int, int> arrival_square;
};