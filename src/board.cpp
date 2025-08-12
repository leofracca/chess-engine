#include <print>

#include "board.h"
#include "pregenerated_moves.h"

namespace chess_engine
{
Board::Board()
    : m_sideToMove(Side::White),
      m_castlingRights(CastlingRights::None),
      m_enPassantSquare(Square::INVALID),
      m_halfMoveClock(0),
      m_fullMoveNumber(0)
{
    std::fill(m_bitboardsPieces.begin(), m_bitboardsPieces.end(), 0);
    std::fill(m_occupancies.begin(), m_occupancies.end(), 0);
}

void Board::printBoard() const
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

    std::println("Side to move: {}", m_sideToMove == Side::White ? "White" : "Black");
    std::println("Castling rights: {}", static_cast<int>(m_castlingRights));
    std::println("En passant square: {}", m_enPassantSquare == Square::INVALID ? "None" : std::to_string(std::to_underlying(m_enPassantSquare)));
    std::println("Half-move clock: {}", m_halfMoveClock);
    std::println("Full-move number: {}", m_fullMoveNumber);

    std::println("+++++++++++++++++++++++++++++");
}

void Board::parseFENString(const std::string& fenString)
{
    // See https://www.chess.com/terms/fen-chess

    // Reset the board state
    std::fill(m_bitboardsPieces.begin(), m_bitboardsPieces.end(), 0);
    std::fill(m_occupancies.begin(), m_occupancies.end(), 0);
    m_sideToMove = Side::White;
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
    m_sideToMove = (fenString[index++] == 'w') ? Side::White : Side::Black;

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
    for (PieceWithColor piece = PieceWithColor::WhitePawn; piece <= PieceWithColor::WhiteKing; piece++)
    {
        m_occupancies[std::to_underlying(Side::White)] |= m_bitboardsPieces[std::to_underlying(piece)];
    }
    for (PieceWithColor piece = PieceWithColor::BlackPawn; piece <= PieceWithColor::BlackKing; piece++)
    {
        m_occupancies[std::to_underlying(Side::Black)] |= m_bitboardsPieces[std::to_underlying(piece)];
    }
    m_occupancies[std::to_underlying(Side::WhiteAndBlack)] = m_occupancies[std::to_underlying(Side::White)] | m_occupancies[std::to_underlying(Side::Black)];
}

bool Board::isSquareAttacked(const Square square, const Side side) const
{
    PieceWithColor piece = PieceWithColor::InvalidPiece;
    constexpr Bitboard empty;

    if (side == Side::White)
    {
        piece = PieceWithColor::WhitePawn;
        if ((pregenerated_moves::blackPawnsAttacks[std::to_underlying(square)] & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
            return true;
    }
    else
    {
        piece = PieceWithColor::BlackPawn;
        if ((pregenerated_moves::whitePawnsAttacks[std::to_underlying(square)] & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
            return true;
    }

    piece = side == Side::White ? PieceWithColor::WhiteKnight : PieceWithColor::BlackKnight;
    if ((pregenerated_moves::knightAttacks[std::to_underlying(square)] & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    piece = side == Side::White ? PieceWithColor::WhiteBishop : PieceWithColor::BlackBishop;
    if ((pregenerated_moves::getBishopAttacks(square, m_occupancies[std::to_underlying(Side::WhiteAndBlack)]) & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    piece = side == Side::White ? PieceWithColor::WhiteRook : PieceWithColor::BlackRook;
    if ((pregenerated_moves::getRookAttacks(square, m_occupancies[std::to_underlying(Side::WhiteAndBlack)]) & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    piece = side == Side::White ? PieceWithColor::WhiteQueen : PieceWithColor::BlackQueen;
    if ((pregenerated_moves::getQueenAttacks(square, m_occupancies[std::to_underlying(Side::WhiteAndBlack)]) & m_bitboardsPieces[std::to_underlying(piece)]) != empty)
        return true;

    piece = side == Side::White ? PieceWithColor::WhiteKing : PieceWithColor::BlackKing;
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
            const Square square = static_cast<Square>(rank * board_dimensions::N_FILES + file);
            std::print("{}", isSquareAttacked(square, side) ? 1 : 0);
        }
        std::println();
    }
}

void Board::generateMoves()
{
    for (const PieceWithColor piece: PieceWithColor())
    {
        using enum PieceWithColor;

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

void Board::generatePawnMoves(const PieceWithColor piece)
{
    using enum PieceWithColor;
    using enum Side;

    const Side side = piece == WhitePawn ? White : Black;
    constexpr Bitboard emptyBitboard;
    Bitboard bitboardPiece = m_bitboardsPieces[std::to_underlying(piece)];

    const int offset = side == White ? -8 : 8; // White pawns move up, black pawns move down

    while (bitboardPiece != emptyBitboard)
    {
        const Square source = bitboardPiece.getSquareOfLeastSignificantBitIndex();
        Square target = source + offset; // Move one square forward

        const bool isPromotion = (side == White && source >= Square::a7 && source <= Square::h7) ||
                                 (side == Black && source >= Square::a2 && source <= Square::h2);
        const bool isDoublePush = (side == White && source >= Square::a2 && source <= Square::h2) ||
                                  (side == Black && source >= Square::a7 && source <= Square::h7);

        Bitboard attacks = side == White
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
            if (side == m_sideToMove)
            {
                Bitboard enPassantBitboard = side == White
                    ? pregenerated_moves::whitePawnsAttacks[std::to_underlying(source)] & Bitboard(m_enPassantSquare)
                    : pregenerated_moves::blackPawnsAttacks[std::to_underlying(source)] & Bitboard(m_enPassantSquare);

                if (enPassantBitboard != emptyBitboard)
                {
                    // If the target square is the en passant square, capture the pawn
                    m_moves.emplace_back(source, m_enPassantSquare, piece, InvalidPiece, true, false, true, false);
                }
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
        using enum PieceWithColor;
        case WhiteKing:
            if (m_castlingRights & CastlingRights::WhiteShort)
            {
                // Check if the squares between the king and rook are empty and not attacked, and the king is not in check
                if (m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::f1) == 0 &&
                    m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::g1) == 0 &&
                    !isSquareAttacked(Square::e1, Side::Black) &&
                    !isSquareAttacked(Square::f1, Side::Black) &&
                    !isSquareAttacked(Square::g1, Side::Black))
                {
                    m_moves.emplace_back(Square::e1, Square::g1, piece, InvalidPiece, false, false, false, true);
                }
            }
            if (m_castlingRights & CastlingRights::WhiteLong)
            {
                // Check if the squares between the king and rook are empty and not attacked, and the king is not in check
                if (m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::b1) == 0 &&
                    m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::c1) == 0 &&
                    m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::d1) == 0 &&
                    !isSquareAttacked(Square::b1, Side::Black) &&
                    !isSquareAttacked(Square::c1, Side::Black) &&
                    !isSquareAttacked(Square::d1, Side::Black) &&
                    !isSquareAttacked(Square::e1, Side::Black))
                {
                    m_moves.emplace_back(Square::e1, Square::c1, piece, InvalidPiece, false, false, false, true);
                }
            }
            break;
        case BlackKing:
            if (m_castlingRights & CastlingRights::BlackShort)
            {
                // Check if the squares between the king and rook are empty and not attacked, and the king is not in check
                if (m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::f8) == 0 &&
                    m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::g8) == 0 &&
                    !isSquareAttacked(Square::e8, Side::White) &&
                    !isSquareAttacked(Square::f8, Side::White) &&
                    !isSquareAttacked(Square::g8, Side::White))
                {
                    m_moves.emplace_back(Square::e8, Square::g8, piece, InvalidPiece, false, false, false, true);
                }
            }

            if (m_castlingRights & CastlingRights::BlackLong)
            {
                // Check if the squares between the king and rook are empty and not attacked, and the king is not in check
                if (m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::b8) == 0 &&
                    m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::c8) == 0 &&
                    m_occupancies[std::to_underlying(Side::WhiteAndBlack)].getBit(Square::d8) == 0 &&
                    !isSquareAttacked(Square::b8, Side::White) &&
                    !isSquareAttacked(Square::c8, Side::White) &&
                    !isSquareAttacked(Square::d8, Side::White) &&
                    !isSquareAttacked(Square::e8, Side::White))
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
    using enum PieceWithColor;
    using enum Side;

    const Side side = piece >= WhitePawn && piece <= WhiteKing ? White : Black;
    constexpr Bitboard emptyBitboard;
    Bitboard bitboardPiece = m_bitboardsPieces[std::to_underlying(piece)];

    // Get the occupancy of the piece's side
    const Bitboard occupancy = m_occupancies[std::to_underlying(side)];
    // Get the occupancy of the opponent's side
    const Bitboard opponentOccupancy = m_occupancies[std::to_underlying(side)];

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
        Bitboard attacks = getAttacks(source, occupancy) & ~occupancy.getBitboard();

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