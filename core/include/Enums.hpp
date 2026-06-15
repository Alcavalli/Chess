#pragma once

enum class GameStatus
{
    Menu,
    InProgress,
    WhiteWin,
    BlackWin,
    Draw,
    Stalemate
};

enum class GameMode
{
    PvP,
    PvE
};

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

enum class PieceType
{
    King,
    Queen,
    Bishop,
    Knight,
    Rook,
    Pawn
};

enum class PieceColor
{
    White,
    Black
};

enum class MoveType
{
    Normal,
    ShortCastle,
    LongCastle,
    Promotion,
    EnPassant
};