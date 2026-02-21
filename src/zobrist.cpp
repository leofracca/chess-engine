#include "zobrist.h"

#include <random>

namespace chess_engine::zobrist
{
/**
 * @brief Initialize Zobrist keys using a pseudo-random number generator.
 */
namespace
{
    [[nodiscard]] constexpr std::array<std::array<uint64_t, board_dimensions::N_SQUARES>, 12> generatePieceKeys()
    {
        // Use a fixed seed for reproducibility
        std::mt19937_64 generator(0x1234567890ABCDEFULL);
        std::uniform_int_distribution<uint64_t> distribution;

        std::array<std::array<uint64_t, board_dimensions::N_SQUARES>, 12> keys = {};

        for (int piece = 0; piece < 12; ++piece)
        {
            for (int square = 0; square < board_dimensions::N_SQUARES; ++square)
            {
                keys[piece][square] = distribution(generator);
            }
        }

        return keys;
    }

    [[nodiscard]] constexpr std::array<uint64_t, 2> generateSideKeys()
    {
        std::mt19937_64 generator(0x2345678901BCDEFULL);
        std::uniform_int_distribution<uint64_t> distribution;

        std::array<uint64_t, 2> keys = {};
        for (int i = 0; i < 2; ++i)
        {
            keys[i] = distribution(generator);
        }

        return keys;
    }

    [[nodiscard]] constexpr std::array<uint64_t, 16> generateCastlingKeys()
    {
        std::mt19937_64 generator(0x3456789012CDEFULL);
        std::uniform_int_distribution<uint64_t> distribution;

        std::array<uint64_t, 16> keys = {};
        for (int i = 0; i < 16; ++i)
        {
            keys[i] = distribution(generator);
        }

        return keys;
    }

    [[nodiscard]] constexpr std::array<uint64_t, 8> generateEnPassantKeys()
    {
        std::mt19937_64 generator(0x456789012DEFULL);
        std::uniform_int_distribution<uint64_t> distribution;

        std::array<uint64_t, 8> keys = {};
        for (int i = 0; i < 8; ++i)
        {
            keys[i] = distribution(generator);
        }

        return keys;
    }
} // namespace

// Pre-generated Zobrist keys
const std::array<std::array<uint64_t, board_dimensions::N_SQUARES>, 12> PIECE_KEYS = generatePieceKeys();
const std::array<uint64_t, 2> SIDE_KEYS                                            = generateSideKeys();
const std::array<uint64_t, 16> CASTLING_KEYS                                       = generateCastlingKeys();
const std::array<uint64_t, 8> EN_PASSANT_KEYS                                      = generateEnPassantKeys();

} // namespace chess_engine::zobrist
