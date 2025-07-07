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
 * @brief Overloaded left shift operator for Square enumeration.
 *
 * @param lhs The left-hand side operand.
 * @param rhs The right-hand side operand.
 * @return The result of the left shift operation.
 */
constexpr uint64_t operator<<(const uint64_t lhs, Square rhs)
{
    return lhs << static_cast<int>(rhs);
}

/**
 * @brief Overloaded right shift operator for Square enumeration.
 *
 * @param lhs The left-hand side operand.
 * @param rhs The right-hand side operand.
 * @return The result of the right shift operation.
 */
constexpr uint64_t operator>>(const uint64_t lhs, Square rhs)
{
    return lhs >> static_cast<int>(rhs);
}

/**
 * @brief Represents the two sides in a chess game.
 */
enum class Side : int
{
    White,
    Black
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
     * @brief Initializes the bitboard with a given integer.
     * @param bitboard The state of the bitboard.
     */
    explicit constexpr Bitboard(uint64_t bitboard)
        : m_bitboard(bitboard)
    {
    }

    /**
     * @brief Initializes the bitboard with a given square.
     * @param square The square to set.
     */
    explicit Bitboard(Square square);

    /**
     * @brief Prints the current state of the bitboard.
     */
    void print() const;

    /**
     * @brief Gets the bit corresponding to a given square.
     * @param square The square to set (0-63).
     * @return The value of the bit at the specified square.
     */
    [[nodiscard]] int getBit(Square square) const;

    /**
     * @brief Sets the bit corresponding to a given square.
     * @param square The square to set (0-63).
     */
    void setBit(Square square);

    /**
     * @brief Clears the bit corresponding to a given square.
     * @param square The square to clear (0-63).
     */
    void clearBit(Square square);

    /**
     * @brief Gets the entire bitboard as a 64-bit integer.
     * @return The current state of the bitboard.
     */
    [[nodiscard]] uint64_t getBitboard() const;

    /**
     * @brief Left shift operator.
     *
     * @param s The number of bits to shift.
     * @return The bitboard after shifting left.
     */
    constexpr Bitboard operator<<(const int s) const
    {
        return Bitboard(m_bitboard << s);
    }

    /**
     * @brief Right shift operator.
     *
     * @param s The number of bits to shift.
     * @return The bitboard after shifting right.
     */
    constexpr Bitboard operator>>(const int s) const
    {
        return Bitboard(m_bitboard >> s);
    }

    /**
     * @brief Bitwise AND operator.
     *
     * @param other The other bitboard to AND with.
     * @return The result of the AND operation as a new Bitboard.
     */
    constexpr Bitboard operator&(const Bitboard& other) const
    {
        return Bitboard(m_bitboard & other.m_bitboard);
    }

    /**
     * @brief Compound bitwise OR operator.
     *
     * @param other The other bitboard to OR with.
     * @return This bitboard after the OR operation.
     */
    constexpr Bitboard& operator|=(const Bitboard& other)
    {
        m_bitboard |= other.m_bitboard;
        return *this;
    }

    bool operator==(const Bitboard& bitboard) const = default;

    bool operator!=(const Bitboard& bitboard) const = default;

private:
    uint64_t m_bitboard; //< The 64-bit integer representing the chess board.
};
} // namespace chess_engine
