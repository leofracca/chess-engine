#pragma once

#include <array>

#include "bitboard.h"
#include "board.h"
#include "magic.h"
#include "slider_utils.h"

namespace chess_engine
{
/**
 * @brief Provides methods to generate and store precomputed moves for chess pieces.
 *
 * Generates pawn, knight, king, bishop, and rook attacks.
 * It also provides methods to generate occupancy masks based on attack masks.
 */
class PregeneratedMoves
{
public:
    /**
     * @brief Initializes the PregeneratedMoves class.
     */
    constexpr PregeneratedMoves()
    {
        initLeaperAttacks();
        initSliderAttacks();
    }

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
    [[nodiscard]] static constexpr Bitboard generateKnightAttacks(const Square square)
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
    [[nodiscard]] static constexpr Bitboard generateKingAttacks(const Square square)
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

    [[nodiscard]] Bitboard getBishopAttacks(const Square square, const Bitboard occupancy) const
    {
        // Get the number of relevant occupancy bits for the given square
        const int relevantBits = slider_utils::BISHOP_RELEVANT_BITS[std::to_underlying(square)];
        // Calculate the magic index based on the occupancy and the precomputed magic number
        const int magicIndex = (occupancy.getBitboard() & m_bishopAttacksMasks[std::to_underlying(square)].getBitboard()) *
                                       Magic::m_bishopMagicNumbers[std::to_underlying(square)].getBitboard() >>
                               (64 - relevantBits);
        return m_bishopAttacks[std::to_underlying(square)][magicIndex];
    }

    [[nodiscard]] Bitboard getRookAttacks(const Square square, const Bitboard occupancy) const
    {
        // Get the number of relevant occupancy bits for the given square
        const int relevantBits = slider_utils::ROOK_RELEVANT_BITS[std::to_underlying(square)];
        // Calculate the magic index based on the occupancy and the precomputed magic number
        const int magicIndex = (occupancy.getBitboard() & m_rookAttacksMasks[std::to_underlying(square)].getBitboard()) *
                                       Magic::m_rookMagicNumbers[std::to_underlying(square)].getBitboard() >>
                               (64 - relevantBits);
        return m_rookAttacks[std::to_underlying(square)][magicIndex];
    }

    [[nodiscard]] Bitboard getQueenAttacks(const Square square, const Bitboard occupancy) const
    {
        // Queen attacks are a combination of bishop and rook attacks
        return getBishopAttacks(square, occupancy) | getRookAttacks(square, occupancy);
    }

    constexpr void initLeaperAttacks()
    {
        for (int i = 0; i < board_dimensions::N_SQUARES; ++i)
        {
            m_whitePawnsAttacks[i] = generatePawnAttacks(Side::White, static_cast<Square>(i));
            m_blackPawnsAttacks[i] = generatePawnAttacks(Side::Black, static_cast<Square>(i));
            m_knightAttacks[i] = generateKnightAttacks(static_cast<Square>(i));
            m_kingAttacks[i] = generateKingAttacks(static_cast<Square>(i));
        }
    }

    constexpr void initSliderAttacks()
    {
        initSlider(
                slider_utils::generateBishopAttacks,
                slider_utils::generateBishopAttacksOnTheFly,
                m_bishopAttacksMasks,
                m_bishopAttacks,
                Magic::m_bishopMagicNumbers,
                slider_utils::BISHOP_RELEVANT_BITS);

        initSlider(
                slider_utils::generateRookAttacks,
                slider_utils::generateRookAttacksOnTheFly,
                m_rookAttacksMasks,
                m_rookAttacks,
                Magic::m_rookMagicNumbers,
                slider_utils::ROOK_RELEVANT_BITS);
    }

private:
    template<typename GenerateAttacks, typename GenerateAttacksOnTheFly, typename AttackMasks, typename Attacks, typename MagicNumbers, typename RelevantBits>
    static constexpr void initSlider(
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

private:
    std::array<Bitboard, 64> m_whitePawnsAttacks; //< Precomputed pawn attacks for white pawns
    std::array<Bitboard, 64> m_blackPawnsAttacks; //< Precomputed pawn attacks for black pawns
    std::array<Bitboard, 64> m_knightAttacks; //< Precomputed knight attacks
    std::array<Bitboard, 64> m_kingAttacks; //< Precomputed king attacks
    std::array<std::array<Bitboard, 512>, 64> m_bishopAttacks; //< Precomputed bishop attacks ([square][occupancy])
    std::array<std::array<Bitboard, 4096>, 64> m_rookAttacks; //< Precomputed rook attacks ([square][occupancy])

    std::array<Bitboard, 64> m_bishopAttacksMasks;
    std::array<Bitboard, 64> m_rookAttacksMasks;

    static constexpr Bitboard NOT_A_FILE{18374403900871474942ULL}; //< All squares set to 1 except the 'a' file
    static constexpr Bitboard NOT_H_FILE{9187201950435737471ULL}; //< All squares set to 1 except the 'h' file
    static constexpr Bitboard NOT_AB_FILE{18229723555195321596ULL}; //< All squares set to 1 except the 'a' and 'b' files
    static constexpr Bitboard NOT_GH_FILE{4557430888798830399ULL}; //< All squares set to 1 except the 'h' and 'g' files
};
} // namespace chess_engine