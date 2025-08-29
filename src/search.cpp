#include "search.h"

namespace chess_engine
{
Move Search::s_bestMove;

int Search::search(Board& board, const int depth)
{
    const int score = negamax(negativeInfinity, positiveInfinity, board, depth);
    return score;
}

int Search::negamax(int alpha, const int beta, Board& board, const int depth, const int ply)
{
    // Base case: evaluate the position
    if (depth == 0)
    {
        return Evaluate::evaluatePosition(board);
    }

    Move localBestMove;
    const int oldAlpha = alpha;
    bool hasLegalMoves = false;
    auto moves = board.generateMoves();

    for (Move move : moves)
    {
        Board newBoard = board;

        if (const bool isValidMove = newBoard.makeMove(move); !isValidMove)
        {
            continue;
        }

        hasLegalMoves = true;
        const int score = -negamax(-beta, -alpha, newBoard, depth - 1, ply + 1);
        newBoard = board;

        // Beta-cutoff
        // If the opponent has found a move that is too good for us, prune the branch
        // No better move possible
        if (score >= beta)
        {
            return beta;
        }

        // Found a better move, update alpha
        if (score > alpha)
        {
            alpha = score;

            // Update the best move found so far at the root level
            if (ply == 0)
            {
                localBestMove = move;
            }
        }
    }

    if (!hasLegalMoves)
    {
        // Checkmate or stalemate
        // If the side to move has no legal moves, it's either checkmate or stalemate
        // It's possible to differentiate by checking if the king is in check
        const PieceWithColor king = board.getSideToMove() == White ? BlackKing : WhiteKing;
        const Square kingSquare = board.getBitboardForPiece(king).getSquareOfLeastSignificantBitIndex();
        if (board.isSquareAttacked(kingSquare, board.getSideToMove()))
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
} // namespace chess_engine