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
     * @param depth The remaining depth to search. Can be extended in certain situations (e.g., check).
     * @param ply The current ply (depth from the root).
     * @return The evaluation score of the best move found.
     */
    [[nodiscard]] static int negamax(int alpha, int beta, Board& board, int depth, int ply = 0);

    /**
     * @brief Quiescence search to evaluate "quiet" positions.
     *
     * Extend the search in positions where there are potential captures
     * or checks to avoid the horizon effect. It only considers capture moves.
     *
     * @param alpha The alpha value for alpha-beta pruning.
     * @param beta The beta value for alpha-beta pruning.
     * @param board The current board position.
     * @param ply The current ply (depth from the root).
     * @return The evaluation score of the position.
     */
    [[nodiscard]] static int quiescence(int alpha, int beta, Board& board, int ply);

    /** @brief Sort moves based on their scores to improve search efficiency.
     *
     * @param moves The list of moves to sort.
     * @param ply The current ply for killer move and history heuristic.
     */
    static void sortMoves(std::vector<Move>& moves, int ply);

public:
    static Move s_bestMove;        //< The best move found during the search

    static inline int s_nodes = 0; //< The number of nodes searched

    static inline std::array<std::array<Move, 256>, 2> s_killerMoves     = {}; //< Killer moves table for move ordering (2 moves per ply)
    static inline std::array<std::array<int, 64>, 12> s_historyHeuristic = {}; //< History heuristic table for move ordering

private:
    static constexpr int positiveInfinity = std::numeric_limits<int>::max();
    static constexpr int negativeInfinity = -positiveInfinity;
};
} // namespace chess_engine