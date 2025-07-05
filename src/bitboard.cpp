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
            if (file == 0)
            {
                std::print("{} ", 8 - rank);
            }

            // Calculate the square index based on rank and file
            const int square = rank * 8 + file;
            std::print("{} ", getBit(square));
        }
        std::println();
    }

    std::print("  a b c d e f g h\n");
}

int Bitboard::getBit(const int square) const
{
    return (m_bitboard >> square) & 1;
}

void Bitboard::setBit(const int square)
{
    m_bitboard |= (1ULL << square);
}

void Bitboard::clearBit(const int square)
{
    m_bitboard &= ~(1ULL << square);
}

uint64_t Bitboard::getBitboard() const
{
    return m_bitboard;
}
} // namespace chess_engine