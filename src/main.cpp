#include "bitboard.h"

int main() {
    chess_engine::Bitboard bitboard;
    bitboard.print();
    bitboard.setBit(static_cast<int>(chess_engine::Square::e4));
    bitboard.print();
    bitboard.setBit(static_cast<int>(chess_engine::Square::d5));
    bitboard.print();
    bitboard.clearBit(static_cast<int>(chess_engine::Square::e4));
    bitboard.print();
}
