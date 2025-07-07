#include "pregenerated_moves.h"

namespace chess_engine
{
/*consteval*/ Bitboard PregeneratedMoves::generatePawnAttacks(const Side side, const Square square)
{
    const Bitboard bitboard(square);
    Bitboard attacks;

    if (side == Side::White)
    {
        // Check if the pawn is on the a file to avoid out-of-bounds attacks
        if (((bitboard >> 7) & NOT_A_FILE) != Bitboard())
        {
            attacks |= bitboard >> 7; // Attack left diagonal
        }

        // Check if the pawn is on the h file to avoid out-of-bounds attacks
        if (((bitboard >> 9) & NOT_H_FILE) != Bitboard())
        {
            attacks |= bitboard >> 9; // Attack right diagonal
        }
    }
    else // Black pawns
    {
        // Check if the pawn is on the h file to avoid out-of-bounds attacks
        if (((bitboard << 7) & NOT_H_FILE) != Bitboard())
        {
            attacks |= bitboard << 7; // Attack right diagonal
        }

        // Check if the pawn is on the a file to avoid out-of-bounds attacks
        if (((bitboard << 9) & NOT_A_FILE) != Bitboard())
        {
            attacks |= bitboard << 9; // Attack left diagonal
        }
    }

    return attacks;
}
} // namespace chess_engine