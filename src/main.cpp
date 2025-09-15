#include "bitboard.h"
#include "evaluate.h"
#include "pregenerated_moves.h"
#include "search.h"
#include "uci_connection.h"

int main()
{
    chess_engine::pregenerated_moves::initAllPieces();
    chess_engine::Board board;
    // Set up board with starting position
    // board.parseFENString("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq - 0 1");
    // board.parseFENString(chess_engine::Board::s_startingFENString);
    // board.print();

    // board.parseFENString("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    // board.print();
    // auto moves = board.generateMoves();
    // for (const auto& move : moves) {
    //     std::print("{} ", move.toString());
    // }
    // std::print("\n");
    // chess_engine::Search::sortMoves(moves);
    // for (const auto& move : moves) {
    //     std::print("{} ", move.toString());
    // }
    // std::print("\n");

    // int score = chess_engine::Evaluate::evaluatePosition(board);
    // std::print("Score: {}\n", score);
    // int eval = chess_engine::Evaluate::evaluateBestMove(board, 2);
    // std::print("Best move: {}, eval: {}\n", chess_engine::Evaluate::s_bestMove.toString(), eval);

    chess_engine::UCIConnection::loop(board);
}
