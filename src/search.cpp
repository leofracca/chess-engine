#include "search.h"

namespace chess_engine
{
Move Search::s_bestMove;

int Search::search(Board& board, const int depth)
{
    resetSearchData();

    int score = 0;
    PVLine line;

    // Iterative deepening
    for (int currentDepth = 1; currentDepth <= depth; ++currentDepth)
    {
        score = negamax(negativeInfinity, positiveInfinity, board, line, currentDepth);

        // Build the principal variation string
        std::string pvString;
        for (int i = 0; i < line.length && i < currentDepth; ++i)
        {
            pvString += line.moves[i].toString() + " ";
        }
        std::println("info depth {} score cp {} nodes {} pv {}",
                   currentDepth,
                   score,
                   s_nodes,
                   pvString);
    }

    s_bestMove = line.moves[0];
    return score;
}

int Search::negamax(int alpha, const int beta, Board& board, PVLine& pvLine, const int depth, const int ply)
{
    PVLine line;

    // Base case: perform quiescence search
    if (depth == 0)
    {
        pvLine.length = 0;
        return quiescence(alpha, beta, board, ply);
    }

    // Maximum ply reached
    if (ply >= maxPly)
    {
        return Evaluate::evaluatePosition(board);
    }

    s_nodes++;

    bool hasLegalMoves  = false;
    const bool isCheck  = board.isCheck();
    const int extension = isCheck ? 1 : 0;
    auto moves          = board.generateMoves();

    sortMoves(moves, ply, pvLine);

    for (Move move: moves)
    {
        Board newBoard = board;

        if (const bool isValidMove = newBoard.makeMove(move); !isValidMove)
        {
            continue;
        }

        hasLegalMoves   = true;
        const int score = -negamax(-beta, -alpha, newBoard, line, depth - 1 + extension, ply + 1);
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

            // Update the principal variation line
            pvLine.moves[0] = move;
            for (int i = 0; i < line.length; ++i)
            {
                pvLine.moves[i + 1] = line.moves[i];
            }
            pvLine.length = line.length + 1;
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

    return alpha;
}

int Search::quiescence(int alpha, const int beta, Board& board, const int ply)
{
    s_nodes++;

    const int evaluation = Evaluate::evaluatePosition(board);

    if (ply >= maxPly)
    {
        return evaluation;
    }

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

void Search::sortMoves(std::vector<Move>& moves, const int ply, const PVLine& pvLine)
{
    // Sort the moves based on their score
    std::sort(moves.begin(), moves.end(), [ply, &pvLine](const Move& a, const Move& b)
    {
        const bool aIsPV = pvLine.length > 0 && ply < pvLine.length && pvLine.moves[ply] == a;
        const bool bIsPV = pvLine.length > 0 && ply < pvLine.length && pvLine.moves[ply] == b;
        return a.calculateScore(ply, aIsPV) > b.calculateScore(ply, bIsPV);
    });
}

void Search::resetSearchData()
{
    s_nodes = 0;

    s_bestMove = Move();

    for (auto& km: s_killerMoves)
        std::ranges::fill(km, Move());
    for (auto& hh: s_historyHeuristic)
        std::ranges::fill(hh, 0);
}
} // namespace chess_engine