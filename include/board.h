/**
 * @file board.h
 * @brief This file defines the Board class and related enums.
 */
#pragma once

#include <array>
#include <string>
#include <utility>
#include <vector>

#include "bitboard.h"
#include "move.h"

namespace chess_engine
{
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
 * @brief Overloaded bitwise AND operator for CastlingRights.
 *
 * @param lhs The left-hand side CastlingRights.
 * @param rhs The right-hand side CastlingRights to combine with lhs.
 * @return The result of the AND operation between lhs and rhs as an unsigned integer.
 */
inline uint operator&(const CastlingRights lhs, const CastlingRights rhs)
{
    return (std::to_underlying(lhs) & std::to_underlying(rhs));
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
    [[nodiscard]] bool isSquareAttacked(Square square, Side side) const;

    /**
     * @brief Print all squares attacked by a given side.
     * Used for debugging purposes.
     *
     * @param side The side for which to print attacked squares.
     */
    void printAttackedSquares(Side side) const;

    /**
     * @brief Generate all possible moves for the current board state.
     * This function will populate the internal move list with all legal moves.
     */
    void generateMoves();

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

    /**
     * @brief Generate moves for pawns of a given side.
     *
     * @param piece The piece for which to generate pawn moves (i.e., WhitePawn or BlackPawn).
     */
    inline void generatePawnMoves(PieceWithColor piece);

    /**
     * @brief Generate castling moves for the king of a given side.
     *
     * @param piece The piece for which to generate castling moves (i.e., WhiteKing or BlackKing).
     */
    inline void generateKingCastlingMoves(PieceWithColor piece);

    /**
     * @brief Generate moves for a specific piece with color.
     *
     * This function will generate all possible moves for the given piece,
     * taking into account the current board state, including occupied squares,
     * attacks, and castling rights.
     *
     * Moves for pawns are handled separately in `generatePawnMoves`.
     * Castling moves are handled in `generateKingCastlingMoves`.
     *
     * @param piece The piece with color for which to generate moves.
     * @see generatePawnMoves
     * @see generateKingCastlingMoves
     */
    inline void generatePieceMoves(PieceWithColor piece);

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

    // TODO: Currently using a vector. Profile to check if a fixed-size and a counting approach is faster.
    std::vector<Move> m_moves; //< List of generated moves
};
} // namespace chess_engine