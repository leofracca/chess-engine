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

/*consteval*/ Bitboard PregeneratedMoves::generateKnightAttacks(const Square square)
{
    const Bitboard bitboard(square);
    Bitboard attacks;

    // Upward attacks
    if (((bitboard >> 17) & NOT_H_FILE) != Bitboard())
    {
        attacks |= bitboard >> 17; // Attack 2 squares up and 1 square left
    }

    if (((bitboard >> 15) & NOT_A_FILE) != Bitboard())
    {
        attacks |= bitboard >> 15; // Attack 2 squares up and 1 square right
    }

    if (((bitboard >> 10) & NOT_GH_FILE) != Bitboard())
    {
        attacks |= bitboard >> 10; // Attack 1 square up and 2 squares left
    }

    if (((bitboard >> 6) & NOT_AB_FILE) != Bitboard())
    {
        attacks |= bitboard >> 6; // Attack 1 square up and 2 squares right
    }

    // Downward attacks
    if (((bitboard << 17) & NOT_A_FILE) != Bitboard())
    {
        attacks |= bitboard << 17; // Attack 2 squares down and 1 square right
    }

    if (((bitboard << 15) & NOT_H_FILE) != Bitboard())
    {
        attacks |= bitboard << 15; // Attack 2 squares down and 1 square left
    }

    if (((bitboard << 10) & NOT_AB_FILE) != Bitboard())
    {
        attacks |= bitboard << 10; // Attack 1 square down and 2 squares right
    }

    if (((bitboard << 6) & NOT_GH_FILE) != Bitboard())
    {
        attacks |= bitboard << 6; // Attack 1 square down and 2 squares left
    }

    return attacks;
}
} // namespace chess_engine