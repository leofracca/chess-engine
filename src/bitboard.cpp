#include "bitboard.h"
#include "debug.h"

namespace chess_engine
{
void Bitboard::print() const
{
    for (int rank = 0; rank < board_dimensions::N_RANKS; rank++)
    {
        for (int file = 0; file < board_dimensions::N_FILES; file++)
        {
            // Print the rank number on the left side of the board
            if (file == 0)
            {
                debug::debug_log("{} ", board_dimensions::N_RANKS - rank);
            }

            // Calculate the square index based on rank and file
            const auto square = static_cast<Square>(rank * board_dimensions::N_FILES + file);
            debug::debug_log("{} ", getBit(square));
        }
        debug::debug_log("\n");
    }

    // Print the file letters at the bottom of the board
    debug::debug_log("  a b c d e f g h\n");

    // Print the current bitboard value (useful for debugging)
    debug::debug_log("Current bitboard: {}\n", m_bitboard);
}

uint64_t Bitboard::getBitboard() const
{
    return m_bitboard;
}
} // namespace chess_engine