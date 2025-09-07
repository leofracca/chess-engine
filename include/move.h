/**
 * @file move.h
 * @brief Declaration of the Move class representing a move.
 *
 * This file contains the declaration of the Move class, which encapsulates the details of a move,
 * including the source and target squares, the piece being moved, any promotion that occurs, and various
 * flags indicating the nature of the move (capture, pawn double push, en passant, castling).
 */
#pragma once

#include <string>

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
     * @param capturedPiece The piece that was captured (default is Pawn).
     * @param isCapture Indicates if the move is a capture (default is false).
     * @param isPawnDoublePush Indicates if the move is a pawn double push (default is false).
     * @param isEnPassant Indicates if the move is an en passant capture (default is false).
     * @param isCastling Indicates if the move is a castling move (default is false).
     */
    Move(Square source, Square target, PieceWithColor piece, PieceWithColor promotedPiece = InvalidPiece, Piece capturedPiece = Pawn, bool isCapture = false, bool isPawnDoublePush = false, bool isEnPassant = false, bool isCastling = false);

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
    Move(Square source, Square target, PieceWithColor piece, PieceWithColor promotedPiece = InvalidPiece, bool isCapture = false, bool isPawnDoublePush = false, bool isEnPassant = false, bool isCastling = false);

    /**
     * @brief Get the source square of the move.
     *
     * @return The source square.
     */
    [[nodiscard]] Square getSource() const;

    /**
     * @brief Get the target square of the move.
     *
     * @return The target square.
     */
    [[nodiscard]] Square getTarget() const;

    /**
     * @brief Get the piece being moved.
     *
     * @return The piece being moved.
     */
    [[nodiscard]] PieceWithColor getPiece() const;

    /**
     * @brief Get the piece promoted to, if applicable.
     *
     * @return The promoted piece, or InvalidPiece if no promotion occurred.
     */
    [[nodiscard]] PieceWithColor getPromotedPiece() const;

    /**
     * @brief Get the piece that was captured.
     *
     * @return The captured piece, or Pawn if no capture occurred.
     */
    [[nodiscard]] Piece getCapturedPiece() const;

    /**
     * @brief Check if the move is a capture.
     *
     * @return True if the move is a capture, false otherwise.
     */
    [[nodiscard]] bool isCapture() const;

    /**
     * @brief Check if the move is a pawn double push.
     *
     * @return True if the move is a pawn double push, false otherwise.
     */
    [[nodiscard]] bool isPawnDoublePush() const;

    /**
     * @brief Check if the move is an en passant capture.
     *
     * @return True if the move is an en passant capture, false otherwise.
     */
    [[nodiscard]] bool isEnPassant() const;

    /**
     * @brief Check if the move is a castling move.
     *
     * @return True if the move is a castling move, false otherwise.
     */
    [[nodiscard]] bool isCastling() const;

    /**
     * @brief Convert the move to a string in UCI format.
     *
     * @return The move as a string in UCI format.
     */
    [[nodiscard]] std::string toString() const;

private:
    Square m_source;                //< Source square of the move
    Square m_target;                //< Target square of the move
    PieceWithColor m_piece;         //< Piece being moved
    PieceWithColor m_promotedPiece; //< Piece promoted to, if applicable
    Piece m_capturedPiece;          //< Piece that was captured
    bool m_isCapture;               //< Indicates if the move is a capture
    bool m_isPawnDoublePush;        //< Indicates if the move is a pawn double push
    bool m_isEnPassant;             //< Indicates if the move is an en passant capture
    bool m_isCastling;              //< Indicates if the move is a castling move
};
} // namespace chess_engine