#include "move.h"
#include "board.h"
#include "search.h"

namespace chess_engine
{
Move::Move()
    : m_source(Square::INVALID),
      m_target(Square::INVALID),
      m_piece(InvalidPiece),
      m_promotedPiece(InvalidPiece),
      m_capturedPiece(Pawn),
      m_isCapture(false),
      m_isPawnDoublePush(false),
      m_isEnPassant(false),
      m_isCastling(false)
{
}

Move::Move(const Square source, const Square target, const PieceWithColor piece, const PieceWithColor promotedPiece, const Piece capturedPiece, const bool isCapture, const bool isPawnDoublePush, const bool isEnPassant, const bool isCastling)
    : m_source(source),
      m_target(target),
      m_piece(piece),
      m_promotedPiece(promotedPiece),
      m_capturedPiece(capturedPiece),
      m_isCapture(isCapture),
      m_isPawnDoublePush(isPawnDoublePush),
      m_isEnPassant(isEnPassant),
      m_isCastling(isCastling)
{
}

Move::Move(const Square source, const Square target, const PieceWithColor piece, const PieceWithColor promotedPiece, const bool isCapture, const bool isPawnDoublePush, const bool isEnPassant, const bool isCastling)
    : Move(source, target, piece, promotedPiece, Pawn, isCapture, isPawnDoublePush, isEnPassant, isCastling)
{
}

Square Move::getSource() const
{
    return m_source;
}

Square Move::getTarget() const
{
    return m_target;
}

PieceWithColor Move::getPiece() const
{
    return m_piece;
}

PieceWithColor Move::getPromotedPiece() const
{
    return m_promotedPiece;
}

Piece Move::getCapturedPiece() const
{
    return m_capturedPiece;
}

bool Move::isCapture() const
{
    return m_isCapture;
}

bool Move::isPawnDoublePush() const
{
    return m_isPawnDoublePush;
}

bool Move::isEnPassant() const
{
    return m_isEnPassant;
}

bool Move::isCastling() const
{
    return m_isCastling;
}

bool Move::isPromotion() const
{
    return m_promotedPiece != InvalidPiece;
}


std::string Move::toString() const
{
    std::string result;
    result += Board::s_squares[std::to_underlying(m_source)];
    result += Board::s_squares[std::to_underlying(m_target)];

    if (m_promotedPiece != InvalidPiece)
    {
        switch (m_promotedPiece)
        {
            case WhiteKnight:
            case BlackKnight:
                result += 'n';
                break;
            case WhiteBishop:
            case BlackBishop:
                result += 'b';
                break;
            case WhiteRook:
            case BlackRook:
                result += 'r';
                break;
            case WhiteQueen:
            case BlackQueen:
                result += 'q';
                break;
            default:
                break;
        }
    }

    return result;
}

int Move::calculateScore(const int ply, const bool isPV) const
{
    int score = 0;

    // Principal Variation moves are prioritized
    if (isPV)
    {
        score += 2000;
    }

    // Captures are prioritized
    // Use MVV-LVA (Most Valuable Victim - Least Valuable Attacker) heuristic
    if (m_isCapture)
    {
        score += 1000 + 10 * std::to_underlying(m_capturedPiece) - (std::to_underlying(m_piece) % 6);
    }
    else
    {
        // Killer and history moves
        if (Search::s_killerMoves[0][ply] == *this)
        {
            score += 500;
        }
        else if (Search::s_killerMoves[1][ply] == *this)
        {
            score += 400;
        }
        else
        {
            const int pieceIndex  = std::to_underlying(m_piece);
            const int targetIndex = std::to_underlying(m_target);
            score += Search::s_historyHeuristic[pieceIndex][targetIndex];
        }
    }

    // Promotions are prioritized
    if (m_promotedPiece != InvalidPiece)
    {
        score += 300 + std::to_underlying(m_promotedPiece);
    }

    // Castling is prioritized
    if (m_isCastling)
    {
        score += 200;
    }

    return score;
}
} // namespace chess_engine