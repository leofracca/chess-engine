#include "bitboard.h"
#include "pregenerated_moves.h"
#include "uci_connection.h"

int main()
{
    chess_engine::pregenerated_moves::initAllPieces();
    chess_engine::Board board;
    board.print();
    chess_engine::UCIConnection::loop(board);
}
