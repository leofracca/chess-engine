#include "bitboard.h"
#include "pregenerated_moves.h"

int main()
{
    chess_engine::Bitboard bitboard;
    // bitboard.print();
    // bitboard.setBit(chess_engine::Square::e4);
    // chess_engine::PregeneratedMoves::generateKnightAttacks(chess_engine::Square::b7).print();
    // bitboard.print()
    // bitboard.print();
    // bitboard.setBit(chess_engine::Square::d5);
    // bitboard.print();
    // bitboard.clearBit(chess_engine::Square::e4);
    // bitboard.print();

    // for (int i = 0; i < 64; ++i)
    // {
    //     chess_engine::Bitboard attacks = chess_engine::PregeneratedMoves::generateKnightAttacks(static_cast<chess_engine::Square>(i));
    //     attacks.print();
    // }

    bitboard.print();
}
