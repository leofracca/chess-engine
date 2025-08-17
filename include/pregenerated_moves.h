#pragma once

#include <array>

#include "bitboard.h"
#include "board.h"
#include "magic.h"
#include "slider_utils.h"

// Note: Not sure if a static class is preferred here, instead of a namespace
namespace chess_engine::pregenerated_moves
{
inline std::array<Bitboard, 64> whitePawnsAttacks; //< Precomputed pawn attacks for white pawns
inline std::array<Bitboard, 64> blackPawnsAttacks; //< Precomputed pawn attacks for black pawns
inline std::array<Bitboard, 64> knightAttacks; //< Precomputed knight attacks
inline std::array<Bitboard, 64> kingAttacks; //< Precomputed king attacks
inline std::array<std::array<Bitboard, 512>, 64> bishopAttacks; //< Precomputed bishop attacks ([square][occupancy])
inline std::array<std::array<Bitboard, 4096>, 64> rookAttacks; //< Precomputed rook attacks ([square][occupancy])

inline std::array<Bitboard, 64> bishopAttacksMasks;
inline std::array<Bitboard, 64> rookAttacksMasks;

static constexpr Bitboard NOT_A_FILE{18374403900871474942ULL}; //< All squares set to 1 except the 'a' file
static constexpr Bitboard NOT_H_FILE{9187201950435737471ULL}; //< All squares set to 1 except the 'h' file
static constexpr Bitboard NOT_AB_FILE{18229723555195321596ULL}; //< All squares set to 1 except the 'a' and 'b' files
static constexpr Bitboard NOT_GH_FILE{4557430888798830399ULL}; //< All squares set to 1 except the 'h' and 'g' files

/**
 * @brief Generates pawn attacks for a given side and square.
 *
 * @param side The side for which to generate pawn attacks.
 * @param square The square from which to generate pawn attacks.
 * @return The bitboard representing the pawn attacks from the given square.
 */
[[nodiscard]] static constexpr Bitboard generatePawnAttacks(const Side side, const Square square)
{
    const Bitboard bitboard(square);
    Bitboard attacks;

    if (side == White)
    {
        attacks |= (bitboard >> 7) & NOT_A_FILE; // Attack left diagonal
        attacks |= (bitboard >> 9) & NOT_H_FILE; // Attack right diagonal
    }
    else // Black pawns
    {
        attacks |= (bitboard << 7) & NOT_H_FILE; // Attack right diagonal
        attacks |= (bitboard << 9) & NOT_A_FILE; // Attack left diagonal
    }

    return attacks;
}

/**
 * @brief Generates knight attacks for a given square.
 *
 * @param square The square from which to generate knight attacks.
 * @return The bitboard representing the knight attacks from the given square.
 */
[[nodiscard]] constexpr Bitboard generateKnightAttacks(const Square square)
{
    const Bitboard bitboard(square);
    Bitboard attacks;

    // Upward attacks
    attacks |= (bitboard >> 17) & NOT_H_FILE; // Attack 2 squares up and 1 square left
    attacks |= (bitboard >> 15) & NOT_A_FILE; // Attack 2 squares up and 1 square right
    attacks |= (bitboard >> 10) & NOT_GH_FILE; // Attack 1 square up and 2 squares left
    attacks |= (bitboard >> 6) & NOT_AB_FILE; // Attack 1 square up and 2 squares right

    // Downward attacks
    attacks |= (bitboard << 17) & NOT_A_FILE; // Attack 2 squares down and 1 square right
    attacks |= (bitboard << 15) & NOT_H_FILE; // Attack 2 squares down and 1 square left
    attacks |= (bitboard << 10) & NOT_AB_FILE; // Attack 1 square down and 2 squares right
    attacks |= (bitboard << 6) & NOT_GH_FILE; // Attack 1 square down and 2 squares left

    return attacks;
}

/**
 * @brief Generates king attacks for a given square.
 *
 * @param square The square from which to generate king attacks.
 * @return The bitboard representing the king attacks from the given square.
 */
[[nodiscard]] constexpr Bitboard generateKingAttacks(const Square square)
{
    const Bitboard bitboard(square);
    Bitboard attacks;

    // Upward attacks
    attacks |= (bitboard >> 8); // Attack 1 square up
    attacks |= (bitboard >> 7) & NOT_A_FILE; // Attack 1 square up and left
    attacks |= (bitboard >> 9) & NOT_H_FILE; // Attack 1 square up and right

    // Downward attacks
    attacks |= (bitboard << 8); // Attack 1 square down
    attacks |= (bitboard << 7) & NOT_H_FILE; // Attack 1 square down and right
    attacks |= (bitboard << 9) & NOT_A_FILE; // Attack 1 square down and left

    // Horizontal attacks
    attacks |= (bitboard >> 1) & NOT_H_FILE; // Attack 1 square left
    attacks |= (bitboard << 1) & NOT_A_FILE; // Attack 1 square right

    return attacks;
}

[[nodiscard]] constexpr Bitboard getKnightAttacks(const Square square, const Bitboard occupancy)
{
    // Knight attacks do not depend on occupancy, so we can return the precomputed attacks directly
    // The occupancy parameter is included for consistency with other piece attack functions
    (void)occupancy; // Avoid unused parameter warning

    // Knight attacks are precomputed for each square
    return knightAttacks[std::to_underlying(square)];
}

[[nodiscard]] constexpr Bitboard getBishopAttacks(const Square square, const Bitboard occupancy)
{
    // Get the number of relevant occupancy bits for the given square
    const int relevantBits = slider_utils::BISHOP_RELEVANT_BITS[std::to_underlying(square)];
    // Calculate the magic index based on the occupancy and the precomputed magic number
    const int magicIndex = (occupancy.getBitboard() & bishopAttacksMasks[std::to_underlying(square)].getBitboard()) *
                                   Magic::m_bishopMagicNumbers[std::to_underlying(square)].getBitboard() >>
                           (64 - relevantBits);
    return bishopAttacks[std::to_underlying(square)][magicIndex];
}

[[nodiscard]] constexpr Bitboard getRookAttacks(const Square square, const Bitboard occupancy)
{
    // Get the number of relevant occupancy bits for the given square
    const int relevantBits = slider_utils::ROOK_RELEVANT_BITS[std::to_underlying(square)];
    // Calculate the magic index based on the occupancy and the precomputed magic number
    const int magicIndex = (occupancy.getBitboard() & rookAttacksMasks[std::to_underlying(square)].getBitboard()) *
                                   Magic::m_rookMagicNumbers[std::to_underlying(square)].getBitboard() >>
                           (64 - relevantBits);
    return rookAttacks[std::to_underlying(square)][magicIndex];
}

[[nodiscard]] constexpr Bitboard getQueenAttacks(const Square square, const Bitboard occupancy)
{
    // Queen attacks are a combination of bishop and rook attacks
    return getBishopAttacks(square, occupancy) | getRookAttacks(square, occupancy);
}

[[nodiscard]] constexpr Bitboard getKingAttacks(const Square square, const Bitboard occupancy)
{
    // King attacks do not depend on occupancy, so we can return the precomputed attacks directly
    // The occupancy parameter is included for consistency with other piece attack functions
    (void)occupancy; // Avoid unused parameter warning

    // King attacks are precomputed for each square
    return kingAttacks[std::to_underlying(square)];
}

constexpr void initLeaperAttacks()
{
    for (int i = 0; i < board_dimensions::N_SQUARES; ++i)
    {
        whitePawnsAttacks[i] = generatePawnAttacks(White, static_cast<Square>(i));
        blackPawnsAttacks[i] = generatePawnAttacks(Black, static_cast<Square>(i));
        knightAttacks[i] = generateKnightAttacks(static_cast<Square>(i));
        kingAttacks[i] = generateKingAttacks(static_cast<Square>(i));
    }
}

template<typename GenerateAttacks, typename GenerateAttacksOnTheFly, typename AttackMasks, typename Attacks, typename MagicNumbers, typename RelevantBits>
constexpr void initSlider(
        GenerateAttacks generateAttacks,
        GenerateAttacksOnTheFly generateAttacksOnTheFly,
        AttackMasks& attackMasks,
        Attacks& attacks,
        const MagicNumbers& magicNumbers,
        const RelevantBits& relevantBitsArray)
{
    for (Square square = Square::a8; square <= Square::h1; square++)
    {
        // Get all possible attacks for the square
        attackMasks[static_cast<int>(square)] = generateAttacks(square);

        // Determine the number of relevant occupancy bits for the given square and piece
        // (how many different blocker configurations can exist for this square)
        const int relevantBits = relevantBitsArray[static_cast<int>(square)];
        const int totalOccupancies = 1 << relevantBits;

        for (int index = 0; index < totalOccupancies; index++)
        {
            // Determine the number of relevant occupancy bits for the given square and piece
            // (how many different blocker configurations can exist for this square)
            Bitboard occupancy = slider_utils::generateOccupancyMask(index, attackMasks[static_cast<int>(square)]);
            int magicIndex = (occupancy.getBitboard() * magicNumbers[static_cast<int>(square)].getBitboard()) >> (64 - relevantBits);
            // Save the set of squares that can be attacked given a specific blocker configuration for the square
            attacks[static_cast<int>(square)][magicIndex] = generateAttacksOnTheFly(square, occupancy);
        }
    }
}

constexpr void initSliderAttacks()
{
    initSlider(
            slider_utils::generateBishopAttacks,
            slider_utils::generateBishopAttacksOnTheFly,
            bishopAttacksMasks,
            bishopAttacks,
            Magic::m_bishopMagicNumbers,
            slider_utils::BISHOP_RELEVANT_BITS);

    initSlider(
            slider_utils::generateRookAttacks,
            slider_utils::generateRookAttacksOnTheFly,
            rookAttacksMasks,
            rookAttacks,
            Magic::m_rookMagicNumbers,
            slider_utils::ROOK_RELEVANT_BITS);
}

inline void initAllPieces()
{
    initLeaperAttacks();
    initSliderAttacks();
}
} // namespace chess_engine::pregenerated_moves