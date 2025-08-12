#include "move.h"

namespace chess_engine
{
Move::Move()
    : m_source(Square::INVALID), m_target(Square::INVALID), m_piece(PieceWithColor::InvalidPiece),
      m_promotedPiece(PieceWithColor::InvalidPiece), m_isCapture(false), m_isPawnDoublePush(false),
      m_isEnPassant(false), m_isCastling(false)
{}

Move::Move(const Square source, const Square target, const PieceWithColor piece, const PieceWithColor promotedPiece,
           const bool isCapture, const bool isPawnDoublePush, const bool isEnPassant, const bool isCastling)
    : m_source(source), m_target(target), m_piece(piece), m_promotedPiece(promotedPiece),
      m_isCapture(isCapture), m_isPawnDoublePush(isPawnDoublePush), m_isEnPassant(isEnPassant),
      m_isCastling(isCastling)
{}

} // namespace chess_engine