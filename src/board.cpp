#include <print>

#include "board.h"
#include "pregenerated_moves.h"

namespace chess_engine
{
Board::Board()
    : m_sideToMove(White),
      m_castlingRights(CastlingRights::None),
      m_enPassantSquare(Square::INVALID),
      m_halfMoveClock(0),
      m_fullMoveNumber(0)
{
    std::fill(m_bitboardsPieces.begin(), m_bitboardsPieces.end(), 0);
    std::fill(m_occupancies.begin(), m_occupancies.end(), 0);
}

void Board::print() const
{
    for (int rank = 0; rank < board_dimensions::N_RANKS; rank++)
    {
        std::print("{} ", board_dimensions::N_RANKS - rank); // Print rank number on the left side

        for (int file = 0; file < board_dimensions::N_FILES; file++)
        {
            const auto square = static_cast<Square>(rank * board_dimensions::N_FILES + file);

            bool isSquareOccupied = false;
            for (const PieceWithColor piece: PieceWithColor())
            {
                if (m_bitboardsPieces[std::to_underlying(piece)].getBit(square) == 1)
                {
                    std::print(" {}", pieceToFENCharacter(piece));
                    isSquareOccupied = true;
                    break;
                }
            }

            if (!isSquareOccupied)
            {
                std::print(" .");
            }
        }
        std::println();
    }
    std::println("   a b c d e f g h\n"); // Print file letters at the bottom of the board

    std::println("Side to move: {}", m_sideToMove == White ? "White" : "Black");
    std::println("Castling rights: {}{}{}{}",
                  (m_castlingRights & CastlingRights::WhiteShort ? "K" : ""),
                  (m_castlingRights & CastlingRights::WhiteLong ? "Q" : ""),
                  (m_castlingRights & CastlingRights::BlackShort ? "k" : ""),
                  (m_castlingRights & CastlingRights::BlackLong ? "q" : ""));
    std::println("En passant square: {}", m_enPassantSquare == Square::INVALID ? "None" : s_squares[std::to_underlying(m_enPassantSquare)]);
    std::println("Half-move clock: {}", m_halfMoveClock);
    std::println("Full-move number: {}", m_fullMoveNumber);

    std::println("+++++++++++++++++++++++++++++");
}

char Board::pieceToFENCharacter(const PieceWithColor piece)
{
    switch (piece)
    {
        // clang-format off
        case WhitePawn:      return 'P';
        case WhiteKnight:    return 'N';
        case WhiteBishop:    return 'B';
        case WhiteRook:      return 'R';
        case WhiteQueen:     return 'Q';
        case WhiteKing:      return 'K';
        case BlackPawn:      return 'p';
        case BlackKnight:    return 'n';
        case BlackBishop:    return 'b';
        case BlackRook:      return 'r';
        case BlackQueen:     return 'q';
        case BlackKing:      return 'k';
        default:             return '?'; // Invalid piece
        // clang-format on
    }
}

PieceWithColor Board::FENCharacterToPieceWithColor(const char fenChar)
{
    switch (fenChar)
    {
        // clang-format off
        case 'P': return WhitePawn;
        case 'N': return WhiteKnight;
        case 'B': return WhiteBishop;
        case 'R': return WhiteRook;
        case 'Q': return WhiteQueen;
        case 'K': return WhiteKing;
        case 'p': return BlackPawn;
        case 'n': return BlackKnight;
        case 'b': return BlackBishop;
        case 'r': return BlackRook;
        case 'q': return BlackQueen;
        case 'k': return BlackKing;
        default:  return InvalidPiece;
        // clang-format on
    }
}

void Board::parseFENString(const std::string& fenString)
{
    // See https://www.chess.com/terms/fen-chess

    // Reset the board state
    std::fill(m_bitboardsPieces.begin(), m_bitboardsPieces.end(), 0);
    std::fill(m_occupancies.begin(), m_occupancies.end(), 0);
    m_sideToMove = White;
    m_castlingRights = CastlingRights::None;
    m_enPassantSquare = Square::INVALID;
    m_halfMoveClock = 0;
    m_fullMoveNumber = 0;

    // Index to track the current position in the FEN string
    int index = 0;

    // Parse FEN: Piece placement
    for (Square square = Square::a8; square <= Square::h1; square++)
    {
        if (fenString[index] == '/')
        {
            index++;
            square--;
            continue;
        }

        if (isdigit(fenString[index]))
        {
            const int emptySquares = fenString[index++] - '0';
            for (int i = 0; i < emptySquares - 1; i++)
            {
                square++;
            }
        }
        else if (isalpha(fenString[index]))
        {
            const PieceWithColor piece = FENCharacterToPieceWithColor(fenString[index++]);
            m_bitboardsPieces[std::to_underlying(piece)].setBit(square);
        }
    }

    index++; // Skip the space after the piece placement

    // Parse FEN: Side to move
    m_sideToMove = (fenString[index++] == 'w') ? White : Black;

    index++; // Skip the space after the side to move

    // Parse FEN: Castling rights
    while (fenString[index] != ' ')
    {
        switch (fenString[index])
        {
            case 'K': m_castlingRights |= CastlingRights::WhiteShort; break;
            case 'Q': m_castlingRights |= CastlingRights::WhiteLong; break;
            case 'k': m_castlingRights |= CastlingRights::BlackShort; break;
            case 'q': m_castlingRights |= CastlingRights::BlackLong; break;
            default: break;
        }
        index++;
    }

    index++; // Skip the space after castling rights

    // Parse FEN: En passant square
    if (fenString[index] == '-')
    {
        m_enPassantSquare = Square::INVALID; // No en passant square
        index++; // Skip the '-'
    }
    else
    {
        const int file = fenString[index++] - 'a';
        const int rank = board_dimensions::N_RANKS - (fenString[index++] - '0');
        m_enPassantSquare = static_cast<Square>(rank * board_dimensions::N_FILES + file);
    }

    index++; // Skip the space after en passant square

    // Parse FEN: Halfmove clock
    while (isdigit(fenString[index]))
    {
        m_halfMoveClock = m_halfMoveClock * 10 + (fenString[index++] - '0');
    }

    index++; // Skip the space after halfmove clock

    // Parse FEN: Fullmove number
    while (isdigit(fenString[index]))
    {
        m_fullMoveNumber = m_fullMoveNumber * 10 + (fenString[index++] - '0');
    }

    // Update occupancies
    for (PieceWithColor piece = WhitePawn; piece <= WhiteKing; ++piece)
    {
        m_occupancies[std::to_underlying(White)] |= m_bitboardsPieces[std::to_underlying(piece)];
    }
    for (PieceWithColor piece = BlackPawn; piece <= BlackKing; ++piece)
    {
        m_occupancies[std::to_underlying(Black)] |= m_bitboardsPieces[std::to_underlying(piece)];
    }
    m_occupancies[std::to_underlying(WhiteAndBlack)] = m_occupancies[std::to_underlying(White)] | m_occupancies[std::to_underlying(Black)];
}

bool Board::isSquareAttacked(const Square square, const Side side) const
{
    PieceWithColor piece;
    constexpr Bitboard empty;

    if (side == White)
    {
        piece = WhitePawn;
        if ((pregenerated_moves::blackPawnsAttacks[std::to_underlying(square)] & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
            return true;
    }
    else
    {
        piece = BlackPawn;
        if ((pregenerated_moves::whitePawnsAttacks[std::to_underlying(square)] & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
            return true;
    }

    piece = side == White ? WhiteKnight : BlackKnight;
    if ((pregenerated_moves::knightAttacks[std::to_underlying(square)] & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    piece = side == White ? WhiteBishop : BlackBishop;
    if ((pregenerated_moves::getBishopAttacks(square, m_occupancies[std::to_underlying(WhiteAndBlack)]) & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    piece = side == White ? WhiteRook : BlackRook;
    if ((pregenerated_moves::getRookAttacks(square, m_occupancies[std::to_underlying(WhiteAndBlack)]) & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    piece = side == White ? WhiteQueen : BlackQueen;
    if ((pregenerated_moves::getQueenAttacks(square, m_occupancies[std::to_underlying(WhiteAndBlack)]) & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    piece = side == White ? WhiteKing : BlackKing;
    if ((pregenerated_moves::kingAttacks[std::to_underlying(square)] & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    return false;
}

void Board::printAttackedSquares(const Side side) const
{
    for (int rank = 0; rank < board_dimensions::N_RANKS; rank++)
    {
        for (int file = 0; file < board_dimensions::N_FILES; file++)
        {
            const auto square = static_cast<Square>(rank * board_dimensions::N_FILES + file);
            std::print("{}", isSquareAttacked(square, side) ? 1 : 0);
        }
        std::println();
    }
}

void Board::generateMoves()
{
    PieceWithColor pawn, king;

    // Determine the side to move and set the pawn and king pieces accordingly
    // This allows us to generate moves for the correct side's pieces
    if (m_sideToMove == White)
    {
        pawn = WhitePawn;
        king = WhiteKing;
    }
    else
    {
        pawn = BlackPawn;
        king = BlackKing;
    }

    for (PieceWithColor piece = pawn; piece <= king; ++piece)
    {
        // Generate pawn moves separately
        if (piece == WhitePawn || piece == BlackPawn)
        {
            generatePawnMoves(piece);
        }

        // Generate moves for all other pieces
        generatePieceMoves(piece);

        // Generate castling moves for kings
        if (piece == WhiteKing || piece == BlackKing)
        {
            generateKingCastlingMoves(piece);
        }
    }
}

bool Board::makeMove(const Move& move)
{
    // Save the current board state before making the move
    const Board boardBeforeMove = *this;

    const Square source = move.getSource();
    const Square target = move.getTarget();
    const PieceWithColor piece = move.getPiece();
    const PieceWithColor promotedPiece = move.getPromotedPiece();
    const bool isCapture = move.isCapture();
    const bool isPawnDoublePush = move.isPawnDoublePush();
    const bool isEnPassant = move.isEnPassant();
    const bool isCastling = move.isCastling();

    // Move the piece
    m_bitboardsPieces[std::to_underlying(piece)].clearBit(source);
    m_bitboardsPieces[std::to_underlying(piece)].setBit(target);

    if (isCapture)
    {
        // Loop over the pieces of the opponent to find the captured piece
        const PieceWithColor pawn = m_sideToMove == White ? BlackPawn : WhitePawn;
        const PieceWithColor king = m_sideToMove == White ? BlackKing : WhiteKing;

        for (PieceWithColor capturedPiece = pawn; capturedPiece <= king; ++capturedPiece)
        {
            // Remove the captured piece from the bitboard
            if (m_bitboardsPieces[std::to_underlying(capturedPiece)].getBit(target) == 1)
            {
                m_bitboardsPieces[std::to_underlying(capturedPiece)].clearBit(target);
                break;
            }
        }
    }

    if (promotedPiece != InvalidPiece)
    {
        // Set the promoted piece and remove the pawn from the bitboard
        const PieceWithColor pawn = m_sideToMove == White ? WhitePawn : BlackPawn;
        m_bitboardsPieces[std::to_underlying(pawn)].clearBit(target);
        m_bitboardsPieces[std::to_underlying(promotedPiece)].setBit(target);
    }

    if (isEnPassant)
    {
        // Remove the captured pawn from the bitboard
        const PieceWithColor pawn = m_sideToMove == White ? BlackPawn : WhitePawn;
        const Square capturedPawnSquare = m_sideToMove == White ? target + 8 : target - 8;
        m_bitboardsPieces[std::to_underlying(pawn)].clearBit(capturedPawnSquare);
    }
    // Reset en passant square after the move
    m_enPassantSquare = Square::INVALID;

    if (isPawnDoublePush)
    {
        // Set the en passant square to the square behind the pawn
        m_enPassantSquare = m_sideToMove == White ? target + 8 : target - 8;
    }

    if (isCastling)
    {
        const PieceWithColor rook = m_sideToMove == White ? WhiteRook : BlackRook;
        switch (target)
        {
            using enum Square;
            case g1:
                m_bitboardsPieces[std::to_underlying(rook)].setBit(f1);
                m_bitboardsPieces[std::to_underlying(rook)].clearBit(h1);
                break;
            case c1:
                m_bitboardsPieces[std::to_underlying(rook)].setBit(d1);
                m_bitboardsPieces[std::to_underlying(rook)].clearBit(a1);
                break;
            case g8:
                m_bitboardsPieces[std::to_underlying(rook)].setBit(f8);
                m_bitboardsPieces[std::to_underlying(rook)].clearBit(h8);
                break;
            case c8:
                m_bitboardsPieces[std::to_underlying(rook)].setBit(d8);
                m_bitboardsPieces[std::to_underlying(rook)].clearBit(a8);
                break;
            default:
                return false; // Invalid castling move
        }
    }

    // Update castling rights
    if (piece == WhiteKing)
    {
        m_castlingRights &= ~CastlingRights::WhiteShort;
        m_castlingRights &= ~CastlingRights::WhiteLong;
    }
    else if (piece == BlackKing)
    {
        m_castlingRights &= ~CastlingRights::BlackShort;
        m_castlingRights &= ~CastlingRights::BlackLong;
    }
    if (source == Square::a1 || target == Square::a1) m_castlingRights &= ~CastlingRights::WhiteLong;
    else if (source == Square::h1 || target == Square::h1) m_castlingRights &= ~CastlingRights::WhiteShort;
    else if (source == Square::a8 || target == Square::a8) m_castlingRights &= ~CastlingRights::BlackLong;
    else if (source == Square::h8 || target == Square::h8) m_castlingRights &= ~CastlingRights::BlackShort;

    // Update occupancies
    std::fill(m_occupancies.begin(), m_occupancies.end(), 0);
    for (PieceWithColor p = WhitePawn; p <= WhiteKing; ++p)
    {
        m_occupancies[std::to_underlying(White)] |= m_bitboardsPieces[std::to_underlying(p)];
    }
    for (PieceWithColor p = BlackPawn; p <= BlackKing; ++p)
    {
        m_occupancies[std::to_underlying(Black)] |= m_bitboardsPieces[std::to_underlying(p)];
    }
    m_occupancies[std::to_underlying(WhiteAndBlack)] = m_occupancies[std::to_underlying(White)] | m_occupancies[std::to_underlying(Black)];

    // Update the side to move
    m_sideToMove = m_sideToMove == White ? Black : White;

    // Check if king is in check after the move
    const PieceWithColor king = m_sideToMove == White ? BlackKing : WhiteKing;
    const Square kingSquare = m_bitboardsPieces[std::to_underlying(king)].getSquareOfLeastSignificantBitIndex();
    if (isSquareAttacked(kingSquare, m_sideToMove))
    {
        // If the king is in check, revert the move
        *this = boardBeforeMove;
        return false; // Invalid move, king is in check
    }

    return true; // Move was valid
}

const std::vector<Move>& Board::getMoves() const
{
    return m_moves;
}

void Board::generatePawnMoves(const PieceWithColor piece)
{
    constexpr Bitboard emptyBitboard;
    Bitboard bitboardPiece = m_bitboardsPieces[std::to_underlying(piece)];

    const int offset = piece == WhitePawn ? -8 : 8; // White pawns move up, black pawns move down

    while (bitboardPiece != emptyBitboard)
    {
        const Square source = bitboardPiece.getSquareOfLeastSignificantBitIndex();
        Square target = source + offset; // Move one square forward

        const bool isPromotion = (piece == WhitePawn && source >= Square::a7 && source <= Square::h7) ||
                                 (piece == BlackPawn && source >= Square::a2 && source <= Square::h2);
        const bool isDoublePush = (piece == WhitePawn && source >= Square::a2 && source <= Square::h2) ||
                                  (piece == BlackPawn && source >= Square::a7 && source <= Square::h7);

        Bitboard attacks = piece == WhitePawn
                                   ? pregenerated_moves::whitePawnsAttacks[std::to_underlying(source)] & m_occupancies[std::to_underlying(Black)]
                                   : pregenerated_moves::blackPawnsAttacks[std::to_underlying(source)] & m_occupancies[std::to_underlying(White)];

        // If the square ahead is empty
        if (m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(target) == 0)
        {
            // Promotion
            if (isPromotion)
            {
                m_moves.emplace_back(source, target, piece, WhiteKnight, false, false, false, false);
                m_moves.emplace_back(source, target, piece, WhiteBishop, false, false, false, false);
                m_moves.emplace_back(source, target, piece, WhiteRook, false, false, false, false);
                m_moves.emplace_back(source, target, piece, WhiteQueen, false, false, false, false);
            }
            // Normal move
            else
            {
                // Move one square forward
                m_moves.emplace_back(source, target, piece, InvalidPiece, false, false, false, false);

                // Move two squares forward
                if (isDoublePush)
                {
                    target = target + offset; // Move two squares forward
                    if (m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(target) == 0)
                    {
                        m_moves.emplace_back(source, target, piece, InvalidPiece, false, true, false, false);
                    }
                }
            }
        }

        while (attacks != emptyBitboard)
        {
            target = attacks.getSquareOfLeastSignificantBitIndex();

            // Capture and promotion
            if (isPromotion)
            {
                m_moves.emplace_back(source, target, piece, WhiteKnight, true, false, false, false);
                m_moves.emplace_back(source, target, piece, WhiteBishop, true, false, false, false);
                m_moves.emplace_back(source, target, piece, WhiteRook, true, false, false, false);
                m_moves.emplace_back(source, target, piece, WhiteQueen, true, false, false, false);
            }
            // Normal capture
            else
            {
                m_moves.emplace_back(source, target, piece, InvalidPiece, true, false, false, false);
            }

            attacks.clearBit(target);
        }

        // En passant capture
        if (m_enPassantSquare != Square::INVALID)
        {
            Bitboard enPassantBitboard;
            if (piece == WhitePawn && m_sideToMove == White)
            {
                enPassantBitboard = pregenerated_moves::whitePawnsAttacks[std::to_underlying(source)] & Bitboard(m_enPassantSquare);
            }
            else if (piece == BlackPawn && m_sideToMove == Black)
            {
                enPassantBitboard = pregenerated_moves::blackPawnsAttacks[std::to_underlying(source)] & Bitboard(m_enPassantSquare);
            }

            if (enPassantBitboard != emptyBitboard)
            {
                // If the target square is the en passant square, capture the pawn
                m_moves.emplace_back(source, m_enPassantSquare, piece, InvalidPiece, true, false, true, false);
            }
        }

        // Remove the piece from the bitboard after processing
        bitboardPiece.clearBit(source);
    }
}

void Board::generateKingCastlingMoves(PieceWithColor piece)
{
    switch (piece)
    {
        case WhiteKing:
            if (m_castlingRights & CastlingRights::WhiteShort)
            {
                // Check if the squares between the king and rook are empty and not attacked, and the king is not in check
                if (m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::f1) == 0 &&
                    m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::g1) == 0 &&
                    !isSquareAttacked(Square::e1, Black) &&
                    !isSquareAttacked(Square::f1, Black) &&
                    !isSquareAttacked(Square::g1, Black))
                {
                    m_moves.emplace_back(Square::e1, Square::g1, piece, InvalidPiece, false, false, false, true);
                }
            }
            if (m_castlingRights & CastlingRights::WhiteLong)
            {
                // Check if the squares between the king and rook are empty and not attacked, and the king is not in check
                if (m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::b1) == 0 &&
                    m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::c1) == 0 &&
                    m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::d1) == 0 &&
                    !isSquareAttacked(Square::b1, Black) &&
                    !isSquareAttacked(Square::c1, Black) &&
                    !isSquareAttacked(Square::d1, Black) &&
                    !isSquareAttacked(Square::e1, Black))
                {
                    m_moves.emplace_back(Square::e1, Square::c1, piece, InvalidPiece, false, false, false, true);
                }
            }
            break;
        case BlackKing:
            if (m_castlingRights & CastlingRights::BlackShort)
            {
                // Check if the squares between the king and rook are empty and not attacked, and the king is not in check
                if (m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::f8) == 0 &&
                    m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::g8) == 0 &&
                    !isSquareAttacked(Square::e8, White) &&
                    !isSquareAttacked(Square::f8, White) &&
                    !isSquareAttacked(Square::g8, White))
                {
                    m_moves.emplace_back(Square::e8, Square::g8, piece, InvalidPiece, false, false, false, true);
                }
            }

            if (m_castlingRights & CastlingRights::BlackLong)
            {
                // Check if the squares between the king and rook are empty and not attacked, and the king is not in check
                if (m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::b8) == 0 &&
                    m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::c8) == 0 &&
                    m_occupancies[std::to_underlying(WhiteAndBlack)].getBit(Square::d8) == 0 &&
                    !isSquareAttacked(Square::b8, White) &&
                    !isSquareAttacked(Square::c8, White) &&
                    !isSquareAttacked(Square::d8, White) &&
                    !isSquareAttacked(Square::e8, White))
                {
                    m_moves.emplace_back(Square::e8, Square::c8, piece, InvalidPiece, false, false, false, true);
                }
            }
            break;
        default:
            break;
    }
}


void Board::generatePieceMoves(const PieceWithColor piece)
{
    const Side side = piece >= WhitePawn && piece <= WhiteKing ? White : Black;
    const Side opponentSide = side == White ? Black : White;
    constexpr Bitboard emptyBitboard;
    Bitboard bitboardPiece = m_bitboardsPieces[std::to_underlying(piece)];

    // Get the occupancy of the piece's side
    const Bitboard occupancy = m_occupancies[std::to_underlying(side)];
    // Get the occupancy of the opponent's side
    const Bitboard opponentOccupancy = m_occupancies[std::to_underlying(opponentSide)];
    // Combine both occupancies to get the full board occupancy
    const Bitboard allOccupancy = occupancy | opponentOccupancy;

    // Save the piece's attacks function pointer
    Bitboard (*getAttacks)(Square, Bitboard);
    switch (piece)
    {
        case WhiteKnight:
        case BlackKnight:
            getAttacks = pregenerated_moves::getKnightAttacks;
            break;
        case WhiteBishop:
        case BlackBishop:
            getAttacks = pregenerated_moves::getBishopAttacks;
            break;
        case WhiteRook:
        case BlackRook:
            getAttacks = pregenerated_moves::getRookAttacks;
            break;
        case WhiteQueen:
        case BlackQueen:
            getAttacks = pregenerated_moves::getQueenAttacks;
            break;
        case WhiteKing:
        case BlackKing:
            getAttacks = pregenerated_moves::getKingAttacks;
            break;
        default:
            return; // Invalid piece
    }


    while (bitboardPiece != emptyBitboard)
    {
        const Square source = bitboardPiece.getSquareOfLeastSignificantBitIndex();
        // Get all possible attacks from the source square (all possible moves except the squares occupied by other pieces of the same side)
        Bitboard attacks = getAttacks(source, allOccupancy) & ~occupancy.getBitboard();

        while (attacks != emptyBitboard)
        {
            const Square target = attacks.getSquareOfLeastSignificantBitIndex();

            // Quiet move
            if (opponentOccupancy.getBit(target) == 0)
            {
                m_moves.emplace_back(source, target, piece, InvalidPiece, false, false, false, false);
            }
            // Capture move
            else
            {
                m_moves.emplace_back(source, target, piece, InvalidPiece, true, false, false, false);
            }

            attacks.clearBit(target);
        }

        bitboardPiece.clearBit(source);
    }
}

} // namespace chess_engine