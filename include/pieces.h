/**
 * @file pieces.h
 * @brief Definitions of chess pieces and their colors.
 *
 * This file contains the definitions of chess pieces, their colors, and
 * provides functionality for iterating over the pieces using range-based for loops.
 */
#pragma once


namespace chess_engine
{
/**
 * @brief White and Black sides.
 */
enum class Side : int
{
    White,
    Black,
    WhiteAndBlack
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

/**
 * @brief Pieces with color definitions.
 */
enum class PieceWithColor : int
{
    WhitePawn,
    WhiteKnight,
    WhiteBishop,
    WhiteRook,
    WhiteQueen,
    WhiteKing,
    BlackPawn,
    BlackKnight,
    BlackBishop,
    BlackRook,
    BlackQueen,
    BlackKing,
    InvalidPiece
};

/**
 * @brief Overloaded begin function for PieceWithColor enumeration.
 *
 * Needed to allow iteration over PieceWithColor using range-based for loops.
 *
 * @return The first and last piece in the enumeration.
 */
inline PieceWithColor begin(PieceWithColor)
{
    return PieceWithColor::WhitePawn;
}

/**
 * @brief Overloaded end function for PieceWithColor enumeration.
 *
 * Needed to allow iteration over PieceWithColor using range-based for loops.
 *
 * @return The last piece in the enumeration.
 */
inline PieceWithColor end(PieceWithColor)
{
    return PieceWithColor::InvalidPiece;
}

/**
 * @brief Overloaded prefix increment operator for PieceWithColor enumeration.
 *
 * @param piece The current piece.
 * @return The next piece.
 */
inline PieceWithColor& operator++(PieceWithColor& piece)
{
    piece = static_cast<PieceWithColor>(static_cast<int>(piece) + 1);
    return piece;
}

/**
 * @brief Overloaded postfix increment operator for PieceWithColor enumeration.
 *
 * @param piece The current piece.
 * @return The piece before incrementing.
 */
inline PieceWithColor operator++(PieceWithColor& piece, int)
{
    const PieceWithColor temp = piece;
    ++piece;
    return temp;
}

/**
 * @brief Overloaded dereference operator for PieceWithColor.
 *
 * Needed to allow iteration over PieceWithColor using range-based for loops.
 *
 * @param piece The piece to return.
 * @return The same piece.
 */
inline PieceWithColor operator*(const PieceWithColor piece)
{
    return piece;
}
} // namespace chess_engine