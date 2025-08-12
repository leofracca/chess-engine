#pragma once

#include "bitboard.h"
#include "pieces.h"

namespace chess_engine
{
/**
 * @brief Represents a move.
 *
 * This class encapsulates the details of a move, including the source and target squares,
 * the piece being moved, any promotion that occurs, and whether the move is a capture, pawn double push,
 * en passant, or castling.
 */
class Move
{
public:
    /**
     * @brief Default constructor initializes an invalid move.
     */
    Move();

    /**
     * @brief Constructs a move with the specified parameters.
     *
     * @param source The source square of the move.
     * @param target The target square of the move.
     * @param piece The piece being moved.
     * @param promotedPiece The piece promoted to, if applicable (default is InvalidPiece).
     * @param isCapture Indicates if the move is a capture (default is false).
     * @param isPawnDoublePush Indicates if the move is a pawn double push (default is false).
     * @param isEnPassant Indicates if the move is an en passant capture (default is false).
     * @param isCastling Indicates if the move is a castling move (default is false).
     */
    Move(Square source, Square target, PieceWithColor piece, PieceWithColor promotedPiece = PieceWithColor::InvalidPiece,
         bool isCapture = false, bool isPawnDoublePush = false, bool isEnPassant = false, bool isCastling = false);

private:
    Square m_source;  //< Source square of the move
    Square m_target;  //< Target square of the move
    PieceWithColor m_piece; //< Piece being moved
    PieceWithColor m_promotedPiece; //< Piece promoted to, if applicable
    bool m_isCapture; //< Indicates if the move is a capture
    bool m_isPawnDoublePush; //< Indicates if the move is a pawn double push
    bool m_isEnPassant; //< Indicates if the move is an en passant capture
    bool m_isCastling; //< Indicates if the move is a castling move
};
} // namespace chess_engine