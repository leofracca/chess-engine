#pragma once

#include <limits>

#include "evaluate.h"

namespace chess_engine
{
class Search
{
public:
    static constexpr int maxPly = 256;
    struct PVLine
    {
        std::array<Move, maxPly> moves = {};
        int length                     = 0;
    };

    static inline PVLine emptyLine = {std::array<Move, maxPly>{}, 0};

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
     * @param pvLine The principal variation line to store the best moves.
     * @param depth The remaining depth to search. Can be extended in certain situations (e.g., check).
     * @param ply The current ply (depth from the root).
     * @return The evaluation score of the best move found.
     */
    [[nodiscard]] static int negamax(int alpha, int beta, Board& board, PVLine& pvLine, int depth, int ply = 0);

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
     * @param pvLine The principal variation line to prioritize moves in the PV.
     */
    static void sortMoves(std::vector<Move>& moves, int ply, const PVLine& pvLine = emptyLine);

    /** @brief Reset search data.
     *
     * This function resets the search data, including the number of nodes searched,
     * killer moves, and history heuristic tables.
     */
    static void resetSearchData();

    /** @brief Determine if a move can be reduced using Late Move Reductions (LMR).
     *
     * This function checks if a move can be reduced based on its index, whether it is a check,
     * the current depth, and any extensions applied. It follows the LMR criteria to decide
     * if the move should be searched with reduced depth.
     *
     * @param moveIndex The index of the move in the move list.
     * @param move The move being considered.
     * @param isCheck Indicates if the current position is a check.
     * @param depth The current search depth.
     * @param extension Any extensions applied to the depth.
     * @return True if the move can be reduced, false otherwise.
     */
    static bool canReduce(int moveIndex, const Move& move, bool isCheck, int depth, int extension);

    /** @brief Determine if pruning can be applied in the current position.
     *
     * @param isCheck Indicates if the current position is a check.
     * @param depth The current search depth.
     * @param ply The current ply (depth from the root).
     * @param pvLineLength The length of the principal variation line.
     * @return True if pruning can be applied, false otherwise.
     */
    static bool canPrune(bool isCheck, int depth, int ply, int pvLineLength);

private:
    static constexpr int positiveInfinity           = std::numeric_limits<int>::max();
    static constexpr int negativeInfinity           = -positiveInfinity;
    static constexpr int aspirationWindowSize       = 50;
    static constexpr int lateMoveReductionThreshold = 3;
    static constexpr int minDepthForLMR             = 2;
    static constexpr int LMRReduction               = 2;
    static constexpr int NullMovePruningReduction   = 2;

public:
    static Move s_bestMove; //< The best move found during the search

    static inline int s_nodes = 0; //< The number of nodes searched

    static inline std::array<std::array<Move, maxPly>, 2> s_killerMoves                           = {}; //< Killer moves table for move ordering (2 moves per ply)
    static inline std::array<std::array<int, board_dimensions::N_SQUARES>, 12> s_historyHeuristic = {}; //< History heuristic table for move ordering
};
} // namespace chess_engine