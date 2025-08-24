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
 * @brief Overloaded compound bitwise AND operator for CastlingRights.
 *
 * @param lhs The left-hand side CastlingRights.
 * @param rhs The right-hand side CastlingRights to combine with lhs.
 * @return The result of the AND operation between lhs and rhs.
 */
inline CastlingRights& operator&=(CastlingRights& lhs, const CastlingRights rhs)
{
    lhs = static_cast<CastlingRights>(std::to_underlying(lhs) & std::to_underlying(rhs));
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
 * @brief Overloaded bitwise NOT operator for CastlingRights.
 *
 * @param castling_rights The CastlingRights to negate.
 * @return The negated CastlingRights as an unsigned integer.
 */
inline CastlingRights operator~(const CastlingRights castling_rights)
{
    return static_cast<CastlingRights>(~std::to_underlying(castling_rights));
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
    void print() const;

    /**
     * @brief Parse a FEN string to set up the board state.
     *
     * @param fenString FEN string representing the board state.
     */
    void parseFENString(std::string_view fenString);

    /**
     * @brief Generate all possible moves for the current board state.
     *
     * @note Currently using a vector. Maybe a fixed-size array would be better, even if it wastes some space.
     *
     * @return The list of generated moves.
     */
    std::vector<Move> generateMoves();

    // FIXME: Does makeMove really need to return bool or can it be void and just assume the move is valid?
    /**
     * @brief Make a move.
     *
     * @param move The move to make.
     * @return True if the move was valid, false otherwise.
     */
    /*[[nodiscard]]*/ bool makeMove(const Move& move);

private:
    /**
     * @brief Convert a PieceWithColor to its corresponding FEN character.
     *
     * @param piece The piece to convert.
     * @return The FEN character representing the piece.
     */
    [[nodiscard]] static char pieceToFENCharacter(PieceWithColor piece);

    /**
     * @brief Convert a FEN character to its corresponding PieceWithColor.
     *
     * @param fenChar The FEN character to convert.
     * @return The PieceWithColor corresponding to the FEN character.
     */
    [[nodiscard]] static PieceWithColor FENCharacterToPieceWithColor(char fenChar);

    /**
     * @brief Check if a square is attacked by a given side.
     *
     * @param square The square to check for attacks.
     * @param side The side of the attacker.
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
     * @brief Generate moves for pawns of a given side.
     *
     * @param piece The piece for which to generate pawn moves (i.e., WhitePawn or BlackPawn).
     * @param moves The vector to store the generated moves.
     */
    inline void generatePawnMoves(PieceWithColor piece, std::vector<Move>& moves);

    /**
     * @brief Generate castling moves for the king of a given side.
     *
     * @param piece The piece for which to generate castling moves (i.e., WhiteKing or BlackKing).
     * @param moves The vector to store the generated moves.
     */
    inline void generateKingCastlingMoves(PieceWithColor piece, std::vector<Move>& moves) const;

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
     * @param moves The vector to store the generated moves.
     * @see generatePawnMoves
     * @see generateKingCastlingMoves
     */
    inline void generatePieceMoves(PieceWithColor piece, std::vector<Move>& moves) const;

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

public:
    /// String representations of squares
    static constexpr std::array<std::string_view, board_dimensions::N_SQUARES> s_squares = {
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
    };

    static constexpr std::string_view s_startingFENString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; //< Standard starting position in FEN notation
};
} // namespace chess_engine