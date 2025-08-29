/**
 * @file evaluate.h
 * @brief Provide functions to evaluate a position on the board.
 *
 * @note Currently using the simplified evaluation function. In the future might use the PeSTO's evaluation function.
 *
 * @see https://www.chessprogramming.org/Point_Value
 * @see https://www.chessprogramming.org/Simplified_Evaluation_Function
 * @see https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function
 */
#pragma once

#include <array>

#include "board.h"

namespace chess_engine
{
class Evaluate
{
public:
    /**
     * @brief Evaluate the position on the board.
     *
     * This function evaluates the current position on the board and returns a score in centipawns.
     *
     * @param board The board to evaluate.
     * @return The evaluation score in centipawns.
     */
    [[nodiscard]] static int evaluatePosition(const Board& board);

private:
    // clang-format off
    //< Piece values in centipawns
    static constexpr std::array<int , 12> s_piecesValues =
    {
        100,        // White Pawn
        300,        // White Knight
        300,        // White Bishop
        500,        // White Rook
        900,        // White Queen
        20000,      // White King
        -100,       // Black Pawn
        -300,       // Black Knight
        -300,       // Black Bishop
        -500,       // Black Rook
        -900,       // Black Queen
        -20000      // Black King
    };

    //< Pawn square table
    static constexpr std::array<int, 64> s_pawnTable =
    {
        0,  0,  0,  0,  0,  0,  0,  0,
       50, 50, 50, 50, 50, 50, 50, 50,
       10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    };

    //< Knight square table
    static constexpr std::array<int, 64> s_knightTable =
    {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
    };

    //< Bishop square table
    static constexpr std::array<int, 64> s_bishopTable =
    {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    };

    //< Rook square table
    static constexpr std::array<int, 64> s_rookTable =
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
       -5,  0,  0,  0,  0,  0,  0, -5,
       -5,  0,  0,  0,  0,  0,  0, -5,
       -5,  0,  0,  0,  0,  0,  0, -5,
       -5,  0,  0,  0,  0,  0,  0, -5,
       -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    };

    //< Queen square table
    static constexpr std::array<int, 64> s_queenTable =
    {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
         -5,  0,  5,  5,  5,  5,  0, -5,
          0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };

    //< King square table for middle game and opening
    static constexpr std::array<int, 64> s_kingMiddleGameTable =
    {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
         20, 20,  0,  0,  0,  0, 20, 20,
         20, 30, 10,  0,  0, 10, 30, 20
    };

    // TODO: Implement a way to differentiate between opening/middle game and endgame
    //< King square table for endgame
    static constexpr std::array<int, 64> s_kingEndGameTable =
    {
        -50,-40,-30,-20,-20,-30,-40,-50,
        -30,-20,-10,  0,  0,-10,-20,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-30,  0,  0,  0,  0,-30,-30,
        -50,-30,-30,-30,-30,-30,-30,-50
    };
    // clang-format on
};
} // namespace chess_engine