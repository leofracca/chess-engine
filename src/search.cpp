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
        return quiescence(alpha, beta, board);
    }

    Move localBestMove;
    const int oldAlpha = alpha;
    bool hasLegalMoves = false;
    auto moves         = board.generateMoves();

    sortMoves(moves);

    for (Move move: moves)
    {
        Board newBoard = board;

        if (const bool isValidMove = newBoard.makeMove(move); !isValidMove)
        {
            continue;
        }

        hasLegalMoves   = true;
        const int score = -negamax(-beta, -alpha, newBoard, depth - 1, ply + 1);
        newBoard        = board;

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
        const PieceWithColor king = board.getSideToMove() == White ? WhiteKing : BlackKing;
        const Square kingSquare   = board.getBitboardForPiece(king).getSquareOfLeastSignificantBitIndex();
        if (board.isSquareAttacked(kingSquare, board.getSideToMove() == White ? Black : White))
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

int Search::quiescence(int alpha, int beta, Board& board)
{
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
    sortMoves(moves);

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

        const int score = -quiescence(-beta, -alpha, newBoard);

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

void Search::sortMoves(std::vector<Move>& moves)
{
    // Sort the moves based on their score
    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        return compareMoves(a, b);
    });
}

bool Search::compareMoves(const Move& a, const Move& b)
{
    int scoreA = 0, scoreB = 0;

    // Captures are prioritized
    // Use MVV-LVA (Most Valuable Victim - Least Valuable Attacker) heuristic
    if (a.isCapture())
    {
        scoreA += 1000 + 10 * std::to_underlying(a.getCapturedPiece()) - (std::to_underlying(a.getPiece()) % 6);
    }
    if (b.isCapture())
    {
        scoreB += 1000 + 10 * std::to_underlying(b.getCapturedPiece()) - (std::to_underlying(b.getPiece()) % 6);
    }

    // Promotions are prioritized
    if (a.getPromotedPiece() != InvalidPiece)
    {
        scoreA += 800 + std::to_underlying(a.getPromotedPiece());
    }
    if (b.getPromotedPiece() != InvalidPiece)
    {
        scoreB += 800 + std::to_underlying(b.getPromotedPiece());
    }

    // Castling is prioritized
    if (a.isCastling())
    {
        scoreA += 500;
    }
    if (b.isCastling())
    {
        scoreB += 500;
    }

    return scoreA > scoreB;
}

} // namespace chess_engine