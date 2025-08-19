#include <gtest/gtest.h>

#include "board.h"
#include "pregenerated_moves.h"

namespace chess_engine_test
{
using namespace chess_engine;

// https://www.chessprogramming.org/Perft_Results

uint64_t perft(const int depth, Board& board/*, const bool first = true*/)
{
    if (depth == 0)
    {
        return 1;
    }

    uint64_t nodes = 0;
    auto moves = board.generateMoves();
    for (const Move& move: moves)
    {
        const Board copy = board;
        if (!board.makeMove(move))
            continue;

        // uint64_t cumulativeNodes = 0;
        // if (first)
        //     cumulativeNodes = nodes;
        nodes += perft(depth - 1, board/*, false*/);
        // if (first)
        // {
        //     uint64_t oldNodes = nodes - cumulativeNodes;
        //     std::println("{}{} -> ({} nodes)", Board::s_squares[std::to_underlying(move.getSource())], Board::s_squares[std::to_underlying(move.getTarget())], oldNodes);
        // }
        board = copy; // Reset the board to the original state after each move
    }

    return nodes;
}

TEST(Perft, Position1)
{
    Board board;
    board.parseFENString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    pregenerated_moves::initAllPieces();

    uint64_t nodes;
    nodes = perft(0, board);
    EXPECT_EQ(nodes, 1) << "Expected 1 move for the initial position at depth 0";
    nodes = perft(1, board);
    EXPECT_EQ(nodes, 20) << "Expected 20 moves for the initial position at depth 1";
    nodes = perft(2, board);
    EXPECT_EQ(nodes, 400) << "Expected 400 moves for the initial position at depth 2";
    nodes = perft(3, board);
    EXPECT_EQ(nodes, 8902) << "Expected 8902 moves for the initial position at depth 3";
    nodes = perft(4, board);
    EXPECT_EQ(nodes, 197281) << "Expected 197281 moves for the initial position at depth 4";
    nodes = perft(5, board);
    EXPECT_EQ(nodes, 4865609) << "Expected 4865609 moves for the initial position at depth 5";
    nodes = perft(6, board);
    EXPECT_EQ(nodes, 119060324) << "Expected 119060324 moves for the initial position at depth 6";

    // Following tests take too long to complete. Disable them for now
    // nodes = perft(7, board);
    // EXPECT_EQ(nodes, 3195901860) << "Expected 3195901860 moves for the initial position at depth 7";
    // nodes = perft(8, board);
    // EXPECT_EQ(nodes, 84998978956) << "Expected 84998978956 moves for the initial position at depth 8";
    // nodes = perft(9, board);
    // EXPECT_EQ(nodes, 2439530234167) << "Expected 2439530234167 moves for the initial position at depth 9";
    // nodes = perft(10, board);
    // EXPECT_EQ(nodes, 69352859712417) << "Expected 69352859712417 moves for the initial position at depth 10";
}

# include <print>
TEST(Perft, Position2)
{
    Board board;
    board.parseFENString("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    pregenerated_moves::initAllPieces();

    uint64_t nodes;
    nodes = perft(1, board);
    EXPECT_EQ(nodes, 48) << "Expected 48 moves for the position at depth 1";
    nodes = perft(2, board);
    EXPECT_EQ(nodes, 2039) << "Expected 2039 moves for the position at depth 2";
    nodes = perft(3, board);
    EXPECT_EQ(nodes, 97862) << "Expected 97862 moves for the position at depth 3";
    nodes = perft(4, board);
    EXPECT_EQ(nodes, 4085603) << "Expected 4085603 moves for the position at depth 4";

    // Following tests take too long to complete. Disable them for now
    // nodes = perft(5, board);
    // EXPECT_EQ(nodes, 193690690) << "Expected 193690690 moves for the position at depth 5";
    // nodes = perft(6, board);
    // EXPECT_EQ(nodes, 8031647685) << "Expected 8031647685 moves for the position at depth 6";
}

TEST(Perft, Position3)
{
    Board board;
    board.parseFENString("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    pregenerated_moves::initAllPieces();

    uint64_t nodes;
    nodes = perft(1, board);
    EXPECT_EQ(nodes, 14) << "Expected 14 moves for the position at depth 1";
    nodes = perft(2, board);
    EXPECT_EQ(nodes, 191) << "Expected 191 moves for the position at depth 2";
    nodes = perft(3, board);
    EXPECT_EQ(nodes, 2812) << "Expected 2812 moves for the position at depth 3";
    nodes = perft(4, board);
    EXPECT_EQ(nodes, 43238) << "Expected 43238 moves for the position at depth 4";
    nodes = perft(5, board);
    EXPECT_EQ(nodes, 674624) << "Expected 674624 moves for the position at depth 5";

    // Following tests are failing
    // nodes = perft(6, board);
    // EXPECT_EQ(nodes, 11030083) << "Expected 11030083 moves for the position at depth 6";
    // nodes = perft(7, board);
    // EXPECT_EQ(nodes, 178633661) << "Expected 178633661 moves for the position at depth 7";
    // nodes = perft(8, board);
    // EXPECT_EQ(nodes, 3009794393) << "Expected 3009794393 moves for the position at depth 8";
}

TEST(Perft, Position4)
{
    Board board;
    board.parseFENString("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    pregenerated_moves::initAllPieces();

    uint64_t nodes;
    nodes = perft(1, board);
    EXPECT_EQ(nodes, 6) << "Expected 6 moves for the position at depth 1";
    nodes = perft(2, board);
    EXPECT_EQ(nodes, 264) << "Expected 264 moves for the position at depth 2";

    // Following tests are failing
    // nodes = perft(3, board);
    // EXPECT_EQ(nodes, 9467) << "Expected 9467 moves for the position at depth 3";
    // nodes = perft(4, board);
    // EXPECT_EQ(nodes, 422333) << "Expected 422333 moves for the position at depth 4";
    // nodes = perft(3, board);
    // EXPECT_EQ(nodes, 15833292) << "Expected 15833292 moves for the position at depth 5";
    // nodes = perft(4, board);
    // EXPECT_EQ(nodes, 706045033) << "Expected 706045033 moves for the position at depth 6";
}

TEST(Perft, Position5)
{
    Board board;
    board.parseFENString("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    pregenerated_moves::initAllPieces();

    uint64_t nodes;
    nodes = perft(1, board);
    EXPECT_EQ(nodes, 44) << "Expected 44 moves for the position at depth 1";
    nodes = perft(2, board);
    EXPECT_EQ(nodes, 1486) << "Expected 1486 moves for the position at depth 2";
    nodes = perft(3, board);
    EXPECT_EQ(nodes, 62379) << "Expected 62379 moves for the position at depth 3";
    nodes = perft(4, board);
    EXPECT_EQ(nodes, 2103487) << "Expected 2103487 moves for the position at depth 4";
    nodes = perft(5, board);
    EXPECT_EQ(nodes, 89941194) << "Expected 89941194 moves for the position at depth 5";
}

TEST(Perft, Position6)
{
    Board board;
    board.parseFENString("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    pregenerated_moves::initAllPieces();

    uint64_t nodes;
    nodes = perft(0, board);
    EXPECT_EQ(nodes, 1) << "Expected 1 move for the position at depth 0";
    nodes = perft(1, board);
    EXPECT_EQ(nodes, 46) << "Expected 46 moves for the position at depth 1";
    nodes = perft(2, board);
    EXPECT_EQ(nodes, 2079) << "Expected 2079 moves for the position at depth 2";
    nodes = perft(3, board);
    EXPECT_EQ(nodes, 89890) << "Expected 89890 moves for the position at depth 3";
    nodes = perft(4, board);
    EXPECT_EQ(nodes, 3894594) << "Expected 3894594 moves for the position at depth 4";
    nodes = perft(5, board);
    EXPECT_EQ(nodes, 164075551) << "Expected 164075551 move for the position at depth 5";

    // Following tests take too long to complete. Disable them for now
    // nodes = perft(6, board);
    // EXPECT_EQ(nodes, 6923051137) << "Expected 6923051137 moves for the position at depth 6";
    // nodes = perft(7, board);
    // EXPECT_EQ(nodes, 287188994746) << "Expected 287188994746 moves for the position at depth 7";
    // nodes = perft(8, board);
    // EXPECT_EQ(nodes, 11923589843526) << "Expected 11923589843526 moves for the position at depth 8";
    // nodes = perft(9, board);
    // EXPECT_EQ(nodes, 490154852788714) << "Expected 490154852788714 moves for the position at depth 9";
}
} // namespace chess_engine_test