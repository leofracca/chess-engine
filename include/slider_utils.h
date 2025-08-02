#pragma once

#include "bitboard.h"

namespace chess_engine::slider_utils
{
/**
 * @brief Generates bishop attacks for a given square.
 *
 * @param square The square from which to generate bishop attacks.
 * @return The bitboard representing the bishop attacks from the given square.
 */
constexpr Bitboard generateBishopAttacks(const Square square)
{
    Bitboard attacks;

    // Get rank and file of the square
    const Square rank = square / board_dimensions::N_RANKS;
    const Square file = square % board_dimensions::N_FILES;

    // Generate attacks in all four diagonal directions
    for (Square r = rank + 1, f = file + 1;
         r < board_dimensions::N_RANKS - 1 && f < board_dimensions::N_FILES - 1;
         r++, f++)
    {
        attacks.setBit(r * board_dimensions::N_FILES + f); // Down-right diagonal
    }

    for (Square r = rank - 1, f = file + 1;
         r > 0 && f < board_dimensions::N_FILES - 1;
         r--, f++)
    {
        attacks.setBit(r * board_dimensions::N_FILES + f); // Up-right diagonal
    }

    for (Square r = rank + 1, f = file - 1;
         r < board_dimensions::N_RANKS - 1 && f > 0;
         r++, f--)
    {
        attacks.setBit(r * board_dimensions::N_FILES + f); // Down-left diagonal
    }

    for (Square r = rank - 1, f = file - 1;
         r > 0 && f > 0;
         r--, f--)
    {
        attacks.setBit(r * board_dimensions::N_FILES + f); // Up-left diagonal
    }

    return attacks;
}

/**
 * @brief Generates bishop attacks on the fly for a given square considering occupied squares.
 *        If the square is occupied, it stops generating attacks in that direction.
 *
 * @param square The square from which to generate bishop attacks.
 * @param occupied The bitboard representing occupied squares.
 * @return The bitboard representing the bishop attacks from the given square.
 */
constexpr Bitboard generateBishopAttacksOnTheFly(const Square square, const Bitboard occupied)
{
    Bitboard attacks;

    // Get rank and file of the square
    const Square rank = square / board_dimensions::N_RANKS;
    const Square file = square % board_dimensions::N_FILES;

    // Generate attacks in all four diagonal directions
    for (Square r = rank + 1, f = file + 1;
         r < board_dimensions::N_RANKS && f < board_dimensions::N_FILES;
         r++, f++)
    {
        const Square targetSquare = r * board_dimensions::N_FILES + f;
        attacks.setBit(targetSquare);
        if (occupied.getBit(targetSquare))
            break; // Stop if we hit an occupied square
    }

    for (Square r = rank - 1, f = file + 1;
         r >= 0 && f < board_dimensions::N_FILES;
         r--, f++)
    {
        const Square targetSquare = r * board_dimensions::N_FILES + f;
        attacks.setBit(targetSquare);
        if (occupied.getBit(targetSquare))
            break;
    }

    for (Square r = rank + 1, f = file - 1;
         r < board_dimensions::N_RANKS && f >= 0;
         r++, f--)
    {
        const Square targetSquare = r * board_dimensions::N_FILES + f;
        attacks.setBit(targetSquare);
        if (occupied.getBit(targetSquare))
            break;
    }

    for (Square r = rank - 1, f = file - 1;
         r >= 0 && f >= 0;
         r--, f--)
    {
        const Square targetSquare = r * board_dimensions::N_FILES + f;
        attacks.setBit(targetSquare);
        if (occupied.getBit(targetSquare))
            break;
    }

    return attacks;
}

/**
 * @brief Generates rook attacks for a given square.
 *
 * @param square The square from which to generate rook attacks.
 * @return The bitboard representing the rook attacks from the given square.
 */
constexpr Bitboard generateRookAttacks(const Square square)
{
    Bitboard attacks;

    // Get rank and file of the square
    const Square rank = square / board_dimensions::N_RANKS;
    const Square file = square % board_dimensions::N_FILES;

    // Generate attacks in all four straight directions
    for (Square r = rank + 1; r < board_dimensions::N_RANKS - 1; r++)
    {
        attacks.setBit(r * board_dimensions::N_FILES + file); // Down
    }

    for (Square r = rank - 1; r > 0; r--)
    {
        attacks.setBit(r * board_dimensions::N_FILES + file); // Up
    }

    for (Square f = file + 1; f < board_dimensions::N_FILES - 1; f++)
    {
        attacks.setBit(rank * board_dimensions::N_RANKS + f); // Right
    }

    for (Square f = file - 1; f > 0; f--)
    {
        attacks.setBit(rank * board_dimensions::N_RANKS + f); // Left
    }

    return attacks;
}

/**
 * @brief Generates rook attacks on the fly for a given square considering occupied squares.
 *        If the square is occupied, it stops generating attacks in that direction.
 *
 * @param square The square from which to generate rook attacks.
 * @param occupied The bitboard representing occupied squares.
 * @return The bitboard representing the rook attacks from the given square.
 */
constexpr Bitboard generateRookAttacksOnTheFly(const Square square, const Bitboard occupied)
{
    Bitboard attacks;

    // Get rank and file of the square
    const Square rank = square / board_dimensions::N_RANKS;
    const Square file = square % board_dimensions::N_FILES;

    // Generate attacks in all four straight directions
    for (Square r = rank + 1; r < board_dimensions::N_RANKS; r++)
    {
        const Square targetSquare = r * board_dimensions::N_FILES + file;
        attacks.setBit(targetSquare);
        if (occupied.getBit(targetSquare))
            break; // Stop if we hit an occupied square
    }

    for (Square r = rank - 1; r >= 0; r--)
    {
        const Square targetSquare = r * board_dimensions::N_FILES + file;
        attacks.setBit(targetSquare);
        if (occupied.getBit(targetSquare))
            break;
    }

    for (Square f = file + 1; f < board_dimensions::N_FILES; f++)
    {
        const Square targetSquare = rank * board_dimensions::N_RANKS + f;
        attacks.setBit(targetSquare);
        if (occupied.getBit(targetSquare))
            break;
    }

    for (Square f = file - 1; f >= 0; f--)
    {
        const Square targetSquare = rank * board_dimensions::N_RANKS + f;
        attacks.setBit(targetSquare);
        if (occupied.getBit(targetSquare))
            break;
    }

    return attacks;
}

/**
 * @brief Generates occupancy mask for a given index and attack mask.
 *
 * @param index The index representing the occupancy configuration.
 * @param attackMask The bitboard representing the attack mask.
 * @return The bitboard representing the occupancy mask.
 */
constexpr Bitboard generateOccupancyMask(const int index, Bitboard attackMask)
{
    Bitboard occupancyMask;

    const int relevantBits = attackMask.getNumberOfBitsSet();
    for (int i = 0; i < relevantBits; i++)
    {
        const Square square = attackMask.getSquareOfLeastSignificantBitIndex();
        attackMask.clearBit(square);

        if (index & (1 << i))
        {
            occupancyMask.setBit(square);
        }
    }

    return occupancyMask;
}

// clang-format off
/// Number of relevant bits for bishop attacks for each square
constexpr std::array<int, 64> BISHOP_RELEVANT_BITS =
{
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

/// Number of relevant bits for rook attacks for each square
constexpr std::array<int, 64> ROOK_RELEVANT_BITS =
{
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};
// clang-format on
} // namespace chess_engine::slider_utils