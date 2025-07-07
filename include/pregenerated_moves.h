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

private:
    std::array<Bitboard, 64> m_whitePawnsAttacks; //< Precomputed pawn attacks for white pawns
    std::array<Bitboard, 64> m_blackPawnsAttacks; //< Precomputed pawn attacks for black pawns

    static constexpr Bitboard NOT_A_FILE{18374403900871474942ULL}; //< All squares set to 1 except the 'a' file
    static constexpr Bitboard NOT_H_FILE{9187201950435737471ULL}; //< All squares set to 1 except the 'h' file
};
} // namespace chess_engine