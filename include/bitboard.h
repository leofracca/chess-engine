/**
 * @file bitboard.h
 * @brief Header file for the Bitboard class and related constants.
 *
 * This file defines the Bitboard class, which represents a chess board using a 64-bit integer,
 * and enumerates the squares on the chess board.
 */
#pragma once

#include <cstdint>

namespace chess_engine
{
namespace board_dimensions
{
    constexpr int N_RANKS = 8; //< Number of ranks on a chess board.
    constexpr int N_FILES = 8; //< Number of files on a chess board.
    constexpr int N_SQUARES = N_RANKS * N_FILES; //< Total number of squares on a chess board (64).
} // namespace board_dimensions

/**
 * @brief Squares on a chess board.
 */
enum class Square : int
{
    // clang-format off
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
    // clang-format on
};

/**
 * @brief Class representing a chess board using a 64-bit integer (bitboard).
 *
 * The Bitboard class provides methods to manipulate and query the state of a chess board.
 * Each square on the board corresponds to a bit in the 64-bit integer.
 */
class Bitboard
{
public:
    /**
     * @brief Default constructor initializes the bitboard to zero.
     */
    Bitboard();

    /**
     * @brief Prints the current state of the bitboard.
     */
    void print() const;

    /**
     * @brief Gets the bit corresponding to a given square.
     * @param square The square to set (0-63).
     * @return The value of the bit at the specified square.
     */
    [[nodiscard]] int getBit(int square) const;

    /**
     * @brief Sets the bit corresponding to a given square.
     * @param square The square to set (0-63).
     */
    void setBit(int square);

    /**
     * @brief Clears the bit corresponding to a given square.
     * @param square The square to clear (0-63).
     */
    void clearBit(int square);

    /**
     * @brief Gets the entire bitboard as a 64-bit integer.
     * @return The current state of the bitboard.
     */
    [[nodiscard]] uint64_t getBitboard() const;

private:
    uint64_t m_bitboard; //< The 64-bit integer representing the chess board.
};
} // namespace chess_engine
