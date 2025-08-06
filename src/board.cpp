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
} // namespace chess_engine