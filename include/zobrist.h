/**
 * @file zobrist.h
 * @brief Zobrist hashing for position representation.
 */
#pragma once

#include <array>
#include <cstdint>

#include "bitboard.h"
#include "pieces.h"

namespace chess_engine::zobrist
{
/**
 * @brief Piece type on each square (12 pieces × 64 squares).
 */
extern const std::array<std::array<uint64_t, board_dimensions::N_SQUARES>, 12> PIECE_KEYS;

/**
 * @brief Side to move (0 = white to move, 1 = black to move).
 */
extern const std::array<uint64_t, 2> SIDE_KEYS;

/**
 * @brief Castling rights (16 combinations of 4 castling rights bits).
 */
extern const std::array<uint64_t, 16> CASTLING_KEYS;

/**
 * @brief En passant squares.
 */
extern const std::array<uint64_t, 8> EN_PASSANT_KEYS;

/**
 * @brief Get the Zobrist key for a piece on a square.
 *
 * @param piece The piece with color.
 * @param square The square.
 * @return The Zobrist key for the piece on the square.
 */
[[nodiscard]] inline uint64_t getPieceKey(const PieceWithColor piece, const Square square)
{
    return PIECE_KEYS[std::to_underlying(piece)][std::to_underlying(square)];
}

/**
 * @brief Get the Zobrist key for the side to move.
 *
 * @param side The side to move (0 for white, 1 for black).
 * @return The Zobrist key for the side.
 */
[[nodiscard]] inline uint64_t getSideKey(const int side)
{
    return SIDE_KEYS[side];
}

/**
 * @brief Get the Zobrist key for castling rights.
 *
 * @param castlingRights The castling rights (4 bits).
 * @return The Zobrist key for the castling rights.
 */
[[nodiscard]] inline uint64_t getCastlingKey(const int castlingRights)
{
    return CASTLING_KEYS[castlingRights & 0xF]; // Mask to 4 bits
}

/**
 * @brief Get the Zobrist key for an en passant file.
 *
 * @param file The file (0-7, where 0 = 'a' and 7 = 'h'). Only valid if en passant is possible.
 * @return The Zobrist key for the en passant file.
 */
[[nodiscard]] inline uint64_t getEnPassantKey(int file)
{
    return EN_PASSANT_KEYS[file & 0x7]; // Mask to 3 bits
}

} // namespace chess_engine::zobrist

