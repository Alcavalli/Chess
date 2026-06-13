#pragma once

#include "Constants.hpp"
#include "Enums.hpp"

struct Piece
{
    PieceType type{PieceType::Pawn};        //! Default values = to not
    PieceColor color{PieceColor::White};    //! leave them uninitialized
};