#pragma once

#include "bitboard.h"

#include <array>

namespace chess_engine
{
class PregeneratedMoves
{
public:
    /**
     * @brief Generates pawn attacks for a given side and square.
     *
     * @param side The side for which to generate pawn attacks.
     * @param square The square from which to generate pawn attacks.
     * @return The bitboard representing the pawn attacks from the given square.
     */
    [[nodiscard]] static /*consteval*/ Bitboard generatePawnAttacks(Side side, Square square);

    /**
     * @brief Generates knight attacks for a given square.
     *
     * @param square The square from which to generate knight attacks.
     * @return The bitboard representing the knight attacks from the given square.
     */
    [[nodiscard]] static /*consteval*/ Bitboard generateKnightAttacks(Square square);

private:
    std::array<Bitboard, 64> m_whitePawnsAttacks; //< Precomputed pawn attacks for white pawns
    std::array<Bitboard, 64> m_blackPawnsAttacks; //< Precomputed pawn attacks for black pawns
    std::array<Bitboard, 64> m_knightAttacks; //< Precomputed knight attacks

    static constexpr Bitboard NOT_A_FILE{18374403900871474942ULL}; //< All squares set to 1 except the 'a' file
    static constexpr Bitboard NOT_H_FILE{9187201950435737471ULL}; //< All squares set to 1 except the 'h' file
    static constexpr Bitboard NOT_AB_FILE{18229723555195321596ULL}; //< All squares set to 1 except the 'a' and 'b' files
    static constexpr Bitboard NOT_GH_FILE{4557430888798830399ULL}; //< All squares set to 1 except the 'h' and 'g' files
};
} // namespace chess_engine