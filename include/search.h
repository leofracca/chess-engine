#pragma once

#include <limits>

#include "evaluate.h"

namespace chess_engine
{
class Search
{
public:
    /**
     * @brief Perform a search to find the best move.
     *
     * Initiate a search to find the best move for the current position on the board
     * up to a specified depth. Use the negamax algorithm with alpha-beta pruning.
     *
     * @param board The current board position.
     * @param depth The maximum depth to search.
     * @return The evaluation score of the best move found.
     */
    static int search(Board& board, int depth);

private:
    /**
     * @brief Negamax search with alpha-beta pruning.
     *
     * @param alpha The alpha value for alpha-beta pruning.
     * @param beta The beta value for alpha-beta pruning.
     * @param board The current board position.
     * @param depth The remaining depth to search.
     * @param ply The current ply (depth from the root).
     * @return The evaluation score of the best move found.
     */
    [[nodiscard]] static int negamax(int alpha, int beta, Board& board, int depth, int ply = 0);

public:
    static Move s_bestMove; //< The best move found during the search.

private:
    static constexpr int positiveInfinity = std::numeric_limits<int>::max();
    static constexpr int negativeInfinity = -positiveInfinity;
};
} // namespace chess_engine