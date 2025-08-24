#include "move.h"

#include "board.h"

namespace chess_engine
{
Move::Move()
    : m_source(Square::INVALID), m_target(Square::INVALID), m_piece(InvalidPiece),
      m_promotedPiece(InvalidPiece), m_isCapture(false), m_isPawnDoublePush(false),
      m_isEnPassant(false), m_isCastling(false)
{}

Move::Move(const Square source, const Square target, const PieceWithColor piece, const PieceWithColor promotedPiece,
           const bool isCapture, const bool isPawnDoublePush, const bool isEnPassant, const bool isCastling)
    : m_source(source), m_target(target), m_piece(piece), m_promotedPiece(promotedPiece),
      m_isCapture(isCapture), m_isPawnDoublePush(isPawnDoublePush), m_isEnPassant(isEnPassant),
      m_isCastling(isCastling)
{}

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

} // namespace chess_engine