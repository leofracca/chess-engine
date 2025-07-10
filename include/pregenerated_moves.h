#pragma once

#include "bitboard.h"

#include <array>

namespace chess_engine
{
class PregeneratedMoves
{
public:
    consteval PregeneratedMoves()
    {
        for (int i = 0; i < board_dimensions::N_SQUARES; ++i)
        {
            m_whitePawnsAttacks[i] = generatePawnAttacks(Side::White, static_cast<Square>(i));
            m_blackPawnsAttacks[i] = generatePawnAttacks(Side::Black, static_cast<Square>(i));
            m_knightAttacks[i] = generateKnightAttacks(static_cast<Square>(i));
            m_kingAttacks[i] = generateKingAttacks(static_cast<Square>(i));
        }
    }

    /**
     * @brief Generates pawn attacks for a given side and square.
     *
     * @param side The side for which to generate pawn attacks.
     * @param square The square from which to generate pawn attacks.
     * @return The bitboard representing the pawn attacks from the given square.
     */
    [[nodiscard]] static consteval Bitboard generatePawnAttacks(const Side side, const Square square)
    {
        const Bitboard bitboard(square);
        Bitboard attacks;

        if (side == Side::White)
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
    [[nodiscard]] static consteval Bitboard generateKnightAttacks(const Square square)
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
    [[nodiscard]] static consteval Bitboard generateKingAttacks(const Square square)
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

    /**
     * @brief Generates bishop attacks for a given square.
     *
     * @param square The square from which to generate bishop attacks.
     * @return The bitboard representing the bishop attacks from the given square.
     */
    [[nodiscard]] static consteval Bitboard generateBishopAttacks(const Square square)
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
     * @brief Generates rook attacks for a given square.
     *
     * @param square The square from which to generate rook attacks.
     * @return The bitboard representing the rook attacks from the given square.
     */
    [[nodiscard]] static consteval Bitboard generateRookAttacks(const Square square)
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

private:
    std::array<Bitboard, 64> m_whitePawnsAttacks; //< Precomputed pawn attacks for white pawns
    std::array<Bitboard, 64> m_blackPawnsAttacks; //< Precomputed pawn attacks for black pawns
    std::array<Bitboard, 64> m_knightAttacks; //< Precomputed knight attacks
    std::array<Bitboard, 64> m_kingAttacks; //< Precomputed king attacks

    static constexpr Bitboard NOT_A_FILE{18374403900871474942ULL}; //< All squares set to 1 except the 'a' file
    static constexpr Bitboard NOT_H_FILE{9187201950435737471ULL}; //< All squares set to 1 except the 'h' file
    static constexpr Bitboard NOT_AB_FILE{18229723555195321596ULL}; //< All squares set to 1 except the 'a' and 'b' files
    static constexpr Bitboard NOT_GH_FILE{4557430888798830399ULL}; //< All squares set to 1 except the 'h' and 'g' files
};
} // namespace chess_engine