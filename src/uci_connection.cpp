#include <iostream>
#include <print>
#include <string>

#include "evaluate.h"
#include "search.h"
#include "uci_connection.h"

namespace chess_engine
{
void UCIConnection::loop(Board& board)
{
    std::string command;

    while (true)
    {
        std::getline(std::cin, command);

        if (command == "uci")
        {
            std::println("id name ChessEngine");
            std::println("id author Leo");
            std::println("uciok");
        }
        else if (command.contains("position"))
        {
            parsePosition(command, board);
        }
        else if (command == "ucinewgame")
        {
            parsePosition("position startpos", board);
        }
        else if (command == "isready")
        {
            std::println("readyok");
        }
        else if (command.contains("go"))
        {
            parseGo(command, board);
        }
        else if (command.contains("quit"))
        {
            break;
        }
        else
            break;
    }
}

void UCIConnection::parsePosition(const std::string_view command, Board& board)
{
    // Command is of the form:
    // position [fen <fenstring> | startpos ] moves <move1> .... <movei>

    constexpr std::string_view prefix = "position ";
    size_t index                      = prefix.size();

    if (command.contains("startpos"))
    {
        index += 9; // Move index past "startpos "
        board.parseFENString(Board::s_startingFENString);
    }
    else if (command.contains("fen"))
    {
        index += 4; // Move index past "fen "
        board.parseFENString(command.substr(index));
    }

    index = command.find("moves");
    if (index != std::string_view::npos)
    {
        index += 6; // Move index past "moves "
        // Get all moves
        const std::string_view movesString = command.substr(index);
        size_t start = 0, end = 0;

        // Split movesString by space
        while (end != std::string_view::npos)
        {
            std::string_view move;
            end = movesString.find(' ', start);
            if (end == std::string_view::npos)
            {
                move = movesString.substr(start);
            }
            else
            {
                move = movesString.substr(start, end - start);
            }

            parseMove(move, board);
            board.print();
            start = end + 1;
        }
    }
}

bool UCIConnection::parseMove(const std::string_view moveAsString, Board& board)
{
    std::vector<Move> moves = board.generateMoves();

    for (const auto& move: moves)
    {
        if (move.toString() == moveAsString)
        {
            return board.makeMove(move);
        }
    }
    return false;
}

void UCIConnection::parseGo(const std::string_view command, Board& board)
{
    // TODO: Currently it's a basic implementation.
    // See https://official-stockfish.github.io/docs/stockfish-wiki/UCI-&-Commands.html#go for all options

    constexpr std::string_view prefix = "go ";
    constexpr size_t index            = prefix.size();
    int depth                         = 0;

    if (command.find("depth") != std::string_view::npos)
    {
        depth = std::stoi(std::string(command.substr(index + 6)));
    }
    else
    {
        depth = 6; // Default depth
    }

    Search::search(board, depth);
    std::println("bestmove {}", Search::s_bestMove.toString());
}
} // namespace chess_engine
