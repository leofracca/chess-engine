/**
 * @file board.h
 * @brief This file defines the Board class and related enums.
 */
#pragma once

#include <string>
#include <array>
#include <utility>

#include "bitboard.h"

namespace  chess_engine
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
    piece = static_cast<PieceWithColor>(std::to_underlying(piece) + 1);
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

/**
 * @brief Castling rights for both sides.
 */
enum class CastlingRights : int
{
    WhiteShort = 1 << 0, // White can castle kingside
    WhiteLong = 1 << 1,  // White can castle queenside
    BlackShort = 1 << 2, // Black can castle kingside
    BlackLong = 1 << 3,  // Black can castle queenside
    None = 0             // No castling rights
};

/**
 * @brief Overloaded compound bitwise OR operator for CastlingRights.
 *
 * @param lhs The left-hand side CastlingRights.
 * @param rhs The right-hand side CastlingRights to combine with lhs.
 * @return The updated CastlingRights after combining lhs and rhs.
 */
inline CastlingRights& operator|=(CastlingRights& lhs, const CastlingRights rhs)
{
    lhs = static_cast<CastlingRights>(std::to_underlying(lhs) | std::to_underlying(rhs));
    return lhs;
}

/**
 * @brief Class representing a chess board.
 */
class Board
{
public:
    /**
     * @brief Default constructor initializes the board with an empty state.
     */
    Board();

    /**
     * @brief Print the current state of the board.
     */
    void printBoard() const;

    /**
     * @brief Parse a FEN string to set up the board state.
     *
     * @param fenString FEN string representing the board state.
     */
    void parseFENString(const std::string& fenString);

    /**
     * @brief Check if a square is attacked by a given side.
     *
     * @param square The square to check for attacks.
     * @param side The side to check for attacks.
     * @return True if the square is attacked by the given side, false otherwise.
     */
    bool isSquareAttacked(Square square, Side side) const;

    /**
     * @brief Print all squares attacked by a given side.
     * Used for debugging purposes.
     *
     * @param side The side for which to print attacked squares.
     */
    void printAttackedSquares(Side side) const;

private:
    /**
     * @brief Convert a PieceWithColor to its corresponding FEN character.
     *
     * @param piece The piece to convert.
     * @return The FEN character representing the piece.
     */
    [[nodiscard]] static char pieceToFENCharacter(const PieceWithColor piece)
    {
        using enum PieceWithColor;
        switch (piece)
        {
            // clang-format off
            case WhitePawn:      return 'P';
            case WhiteKnight:    return 'N';
            case WhiteBishop:    return 'B';
            case WhiteRook:      return 'R';
            case WhiteQueen:     return 'Q';
            case WhiteKing:      return 'K';
            case BlackPawn:      return 'p';
            case BlackKnight:    return 'n';
            case BlackBishop:    return 'b';
            case BlackRook:      return 'r';
            case BlackQueen:     return 'q';
            case BlackKing:      return 'k';
            default:             return '?'; // Invalid piece
            // clang-format on
        }
    }

    [[nodiscard]] static PieceWithColor FENCharacterToPieceWithColor(const char fenChar)
    {
        switch (fenChar)
        {
            // clang-format off
            case 'P': return PieceWithColor::WhitePawn;
            case 'N': return PieceWithColor::WhiteKnight;
            case 'B': return PieceWithColor::WhiteBishop;
            case 'R': return PieceWithColor::WhiteRook;
            case 'Q': return PieceWithColor::WhiteQueen;
            case 'K': return PieceWithColor::WhiteKing;
            case 'p': return PieceWithColor::BlackPawn;
            case 'n': return PieceWithColor::BlackKnight;
            case 'b': return PieceWithColor::BlackBishop;
            case 'r': return PieceWithColor::BlackRook;
            case 'q': return PieceWithColor::BlackQueen;
            case 'k': return PieceWithColor::BlackKing;
            default: return  PieceWithColor::InvalidPiece;
            // clang-format on
        }
    }

private:
    static constexpr int N_ALL_PIECES = 12; //< 6 pieces for each side
    static constexpr int N_SIDES = 3; //< White, Black, and both sides combined

    std::array<Bitboard, N_ALL_PIECES> m_bitboardsPieces; //< Bitboards for each piece type
    std::array<Bitboard, N_SIDES> m_occupancies; //< Occupancies for each side

    Side m_sideToMove; //< Side to move
    CastlingRights m_castlingRights; //< Castling rights for both sides
    Square m_enPassantSquare; //< En passant square, if any
    int m_halfMoveClock; //< Half-move clock for the fifty-move rule
    int m_fullMoveNumber; //< Full move number
};
} // namespace chess_engine