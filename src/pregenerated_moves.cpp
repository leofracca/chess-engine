#include "pregenerated_moves.h"

namespace chess_engine
{
/*consteval*/ Bitboard PregeneratedMoves::generatePawnAttacks(const Side side, const Square square)
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

/*consteval*/ Bitboard PregeneratedMoves::generateKnightAttacks(const Square square)
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

/*consteval*/ Bitboard PregeneratedMoves::generateKingAttacks(const Square square)
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
} // namespace chess_engine