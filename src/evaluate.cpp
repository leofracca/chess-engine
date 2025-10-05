#include "evaluate.h"

namespace chess_engine
{
int Evaluate::evaluatePosition(const Board& board)
{
    int score        = 0;
    int whiteBishops = 0, blackBishops = 0;

    for (const PieceWithColor piece: PieceWithColor())
    {
        Bitboard bitboardPiece = board.getBitboardForPiece(piece);

        while (bitboardPiece != Bitboard())
        {
            score += s_piecesValues[std::to_underlying(piece)];
            const Square square = bitboardPiece.getSquareOfLeastSignificantBitIndex();

            switch (piece)
            {
                case WhitePawn:
                    score += s_pawnTable[std::to_underlying(square)];
                    break;
                case BlackPawn:
                    score -= s_pawnTable[63 - std::to_underlying(square)];
                    break;
                case WhiteKnight:
                    score += s_knightTable[std::to_underlying(square)];
                    break;
                case BlackKnight:
                    score -= s_knightTable[63 - std::to_underlying(square)];
                    break;
                case WhiteBishop:
                    score += s_bishopTable[std::to_underlying(square)];
                    whiteBishops++;
                    break;
                case BlackBishop:
                    score -= s_bishopTable[63 - std::to_underlying(square)];
                    blackBishops++;
                    break;
                case WhiteRook:
                    score += s_rookTable[std::to_underlying(square)];
                    break;
                case BlackRook:
                    score -= s_rookTable[63 - std::to_underlying(square)];
                    break;
                case WhiteQueen:
                    score += s_queenTable[std::to_underlying(square)];
                    break;
                case BlackQueen:
                    score -= s_queenTable[63 - std::to_underlying(square)];
                    break;
                case WhiteKing:
                    // FIXME: Differentiate between opening/middle game and endgame
                    score += s_kingMiddleGameTable[std::to_underlying(square)];
                    break;
                case BlackKing:
                    // FIXME: Differentiate between opening/middle game and endgame
                    score -= s_kingMiddleGameTable[63 - std::to_underlying(square)];
                    break;
                default:
                    break;
            }

            bitboardPiece.clearBit(square);
        }
    }

    if (whiteBishops >= 2)
    {
        score += s_bishopPairBonus;
    }
    if (blackBishops >= 2)
    {
        score -= s_bishopPairBonus;
    }

    score += evaluatePawnStructure(board, White);
    score -= evaluatePawnStructure(board, Black);

    return board.getSideToMove() == White ? score : -score;
}

int Evaluate::evaluatePawnStructure(const Board& board, Side side)
{
    int score                                                = 0;
    std::array<int, board_dimensions::N_FILES> pawnCountFile = {};
    Bitboard pawnsBitboard                                   = board.getBitboardForPiece(side == White ? WhitePawn : BlackPawn);

    // Count pawns per file
    while (pawnsBitboard != Bitboard())
    {
        const Square square = pawnsBitboard.getSquareOfLeastSignificantBitIndex();
        const int file      = std::to_underlying(square) % board_dimensions::N_FILES;
        pawnCountFile[file]++;
        pawnsBitboard.clearBit(square);
    }

    // Double pawns penalty
    for (int file = 0; file < board_dimensions::N_FILES; file++)
    {
        if (pawnCountFile[file] > 1)
        {
            score += (pawnCountFile[file] - 1) * s_doublePawnPenalty;
        }
    }

    // Isolated pawns penalty
    for (int file = 0; file < board_dimensions::N_FILES; file++)
    {
        if (pawnCountFile[file] > 0)
        {
            const bool hasLeftNeighbor  = file > 0 && (pawnCountFile[file - 1] > 0);
            const bool hasRightNeighbor = file < board_dimensions::N_FILES - 1 && (pawnCountFile[file + 1] > 0);
            if (!hasLeftNeighbor && !hasRightNeighbor)
            {
                score += pawnCountFile[file] * s_isolatedPawnPenalty;
            }
        }
    }

    // Passed pawns bonus
    pawnsBitboard = board.getBitboardForPiece(side == White ? WhitePawn : BlackPawn);
    while (pawnsBitboard != Bitboard())
    {
        const Square square = pawnsBitboard.getSquareOfLeastSignificantBitIndex();
        const int file      = std::to_underlying(square) % board_dimensions::N_FILES;
        const int rank      = std::to_underlying(square) / board_dimensions::N_RANKS;
        bool isPassed       = true;

        debug::debug_log("checking if {} is a passed pawn", Board::s_squares[std::to_underlying(square)]);
        for (int r = rank + (side == White ? -1 : 1); r >= 0 && r < board_dimensions::N_RANKS; r += (side == White ? -1 : 1))
        {
            for (int f = std::max(0, file - 1); f <= std::min(board_dimensions::N_FILES - 1, file + 1); f++)
            {
                const auto checkSquare = static_cast<Square>(r * board_dimensions::N_FILES + f);
                debug::debug_log("checking square {}", Board::s_squares[std::to_underlying(checkSquare)]);
                if (board.getBitboardForPiece(side == White ? BlackPawn : WhitePawn).getBit(checkSquare) == 1)
                {
                    isPassed = false;
                    break;
                }
            }
            if (!isPassed)
                break;
        }
        if (isPassed)
        {
            debug::debug_log("{} is a passed pawn", Board::s_squares[std::to_underlying(square)]);
            score += s_passedPawnBonus;
        }
        pawnsBitboard.clearBit(square);
    }

    return score;
}
} // namespace chess_engine