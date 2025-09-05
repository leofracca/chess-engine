#include "evaluate.h"

namespace chess_engine
{
int Evaluate::evaluatePosition(const Board& board)
{
    int score = 0;

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
                    break;
                case BlackBishop:
                    score -= s_bishopTable[63 - std::to_underlying(square)];
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

    return board.getSideToMove() == White ? score : -score;
}
} // namespace chess_engine