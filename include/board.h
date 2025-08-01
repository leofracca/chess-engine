#pragma once

namespace  chess_engine
{
/**
 * @brief White and Black sides.
 */
enum class Side : int
{
    White,
    Black
};

/**
 * @brief Pieces definitions.
 */
enum class Piece : int
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};
} // namespace chess_engine