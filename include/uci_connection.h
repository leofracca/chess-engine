#pragma once

#include "board.h"

namespace chess_engine
{
class UCIConnection
{
public:
    /**
     * @brief Main loop to handle UCI commands.
     *
     * This function continuously listens for UCI commands and processes them
     * accordingly. It handles commands such as "uci", "isready",
     * "position", "go", and "quit".
     *
     * @param board The board to interact with.
     * @see https://official-stockfish.github.io/docs/stockfish-wiki/UCI-&-Commands.html
     */
    static void loop(Board& board);

private:
    /**
     * @brief Parse the "position" command to set up the board state.
     *
     * @param command The full "position" command string.
     * param board The board to set up.
     */
    static void parsePosition(std::string_view command, Board& board);

    static bool parseMove(std::string_view moveAsString, Board& board);

    /**
     * @brief Parse the "go" command to start the engine's move calculation.
     *
     * @param command The full "go" command string.
     * param board The board to analyze.
     */
    static void parseGo(std::string_view command, Board& board);
};
} // namespace chess_engine