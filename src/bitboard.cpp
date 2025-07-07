#include "bitboard.h"

#include <print>

namespace chess_engine
{
Bitboard::Bitboard()
    : m_bitboard(0)
{
}

void Bitboard::print() const
{
    for (int rank = 0; rank < board_dimensions::N_RANKS; rank++)
    {
        for (int file = 0; file < board_dimensions::N_FILES; file++)
        {
            // Print the rank number on the left side of the board
            if (file == 0)
            {
                std::print("{} ", board_dimensions::N_RANKS - rank);
            }

            // Calculate the square index based on rank and file
            const auto square = static_cast<Square>(rank * board_dimensions::N_FILES + file);
            std::print("{} ", getBit(square));
        }
        std::println();
    }

    // Print the file letters at the bottom of the board
    std::println("  a b c d e f g h");
}

int Bitboard::getBit(const Square square) const
{
    return (m_bitboard >> square) & 1;
}

void Bitboard::setBit(const Square square)
{
    m_bitboard |= (1ULL << square);
}

void Bitboard::clearBit(const Square square)
{
    m_bitboard &= ~(1ULL << square);
}

uint64_t Bitboard::getBitboard() const
{
    return m_bitboard;
}
} // namespace chess_engine