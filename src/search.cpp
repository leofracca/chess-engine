#include "search.h"

namespace chess_engine
{
Move Search::s_bestMove;

int Search::search(Board& board, const int depth)
{
    resetSearchData();

    // Iterative deepening
    for (int currentDepth = 1; currentDepth <= depth; ++currentDepth)
    {
        const int score = negamax(negativeInfinity, positiveInfinity, board, currentDepth);
        debug::debug_log("info depth {} score cp {} nodes {} pv {}\n",
                         currentDepth,
                         score,
                         s_nodes,
                         s_bestMove.toString());
    }

    const int score = negamax(negativeInfinity, positiveInfinity, board, depth);
    return score;
}

int Search::negamax(int alpha, const int beta, Board& board, const int depth, const int ply)
{
    // Base case: evaluate the position
    if (depth == 0)
    {
        return quiescence(alpha, beta, board, ply);
    }

    if constexpr (debug::is_debug)
    {
        s_nodes++;
    }

    Move localBestMove;
    const int oldAlpha  = alpha;
    bool hasLegalMoves  = false;
    const bool isCheck  = board.isCheck();
    const int extension = isCheck ? 1 : 0;
    auto moves          = board.generateMoves();

    sortMoves(moves, ply);

    for (Move move: moves)
    {
        Board newBoard = board;

        if (const bool isValidMove = newBoard.makeMove(move); !isValidMove)
        {
            continue;
        }

        hasLegalMoves   = true;
        const int score = -negamax(-beta, -alpha, newBoard, depth - 1 + extension, ply + 1);
        newBoard        = board;

        // Beta-cutoff
        // If the opponent has found a move that is too good for us, prune the branch
        // No better move possible
        if (score >= beta)
        {
            if (!move.isCapture())
            {
                s_killerMoves[1][ply] = s_killerMoves[0][ply]; // Shift the previous killer move down
                s_killerMoves[0][ply] = move;                  // Store the move as a killer move
            }
            return beta;
        }

        // Found a better move, update alpha
        if (score > alpha)
        {
            alpha = score;

            if (!move.isCapture())
            {
                const int pieceIndex  = std::to_underlying(move.getPiece());
                const int targetIndex = std::to_underlying(move.getTarget());
                s_historyHeuristic[pieceIndex][targetIndex] += depth * depth;
            }

            // Update the best move found so far at the root level
            if (ply == 0)
            {
                localBestMove = move;
            }
        }
    }

    if (!hasLegalMoves)
    {
        // If the side to move has no legal moves, it's either checkmate or stalemate
        if (isCheck)
        {
            return negativeInfinity + ply; // Checkmate, return a very low score
        }
        else
        {
            return 0; // Stalemate, return a neutral score
        }
    }

    // If alpha was updated, store the best move found
    if (oldAlpha != alpha)
    {
        s_bestMove = localBestMove;
    }

    return alpha;
}

int Search::quiescence(int alpha, const int beta, Board& board, int ply)
{
    if constexpr (debug::is_debug)
    {
        s_nodes++;
    }

    const int evaluation = Evaluate::evaluatePosition(board);

    if (evaluation >= beta)
    {
        return beta;
    }

    if (alpha < evaluation)
    {
        alpha = evaluation;
    }

    auto moves = board.generateMoves();
    sortMoves(moves, ply);

    for (const Move move: moves)
    {
        // Only consider capture moves in quiescence search
        if (!move.isCapture())
        {
            continue;
        }

        Board newBoard = board;

        if (const bool isValidMove = newBoard.makeMove(move); !isValidMove)
        {
            continue;
        }

        const int score = -quiescence(-beta, -alpha, newBoard, ply + 1);

        if (score >= beta)
        {
            return beta;
        }

        if (score > alpha)
        {
            alpha = score;
        }
    }

    return alpha;
}

void Search::sortMoves(std::vector<Move>& moves, const int ply)
{
    // Sort the moves based on their score
    std::sort(moves.begin(), moves.end(), [ply](const Move& a, const Move& b)
              { return a.calculateScore(ply) > b.calculateScore(ply); });
}

void Search::resetSearchData()
{
    if constexpr (debug::is_debug)
    {
        s_nodes = 0;
    }

    s_bestMove = Move();

    for (auto& km: s_killerMoves)
        std::ranges::fill(km, Move());
    for (auto& hh: s_historyHeuristic)
        std::ranges::fill(hh, 0);
}
} // namespace chess_engine