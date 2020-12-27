#include "game.h"

Game::Game()
{
    moves.clear();
    movePGNs.clear();
    turn = Color::WHITE;
    positions = {Position(true)};
    moveNumber = 1;
    result = GameResult::NOT_FINISHED;
    Tools::currentDate(year, month, day);
    whitePlayer = "Turbot";
    blackPlayer = "Turbot";
}

Position Game::getPosition() const
{
    return positions.back();
}

bool Game::ThreeFoldRepetition() const
{
    std::vector<Board> boards;
    boards.reserve(positions.size());
    for (const auto & position : positions) {boards.push_back(position.getBoard());}
    return Tools::containsTriplicates(boards);
}

bool Game::isFinished() const
{
    return (result!=GameResult::NOT_FINISHED);
}


bool Game::playMove(const Move &move, bool checkLegal, bool checkCKLegal)
{
    bool success=false;

    if (isFinished()) {throw("Game is finished!");}

    Position currentPosition = positions.back(), newPosition;
    LegalMover currentMover(&currentPosition, true);


    if (currentMover.applyMove(newPosition, move, checkLegal, checkCKLegal))
    {
        moves.push_back(move);
        movePGNs.push_back(MovePGN(move, &currentMover));
        positions.push_back(newPosition);
        turn = (turn==Color::WHITE) ? Color::BLACK : Color::WHITE;
        if (turn==Color::WHITE) {++moveNumber;}

        LegalMover newMover(&newPosition, true);
        if (newMover.isCheckmate()) {result = (turn==Color::WHITE) ? GameResult::BLACK_WINS : GameResult::WHITE_WINS;}
        else if (newMover.isStalemate()) {result = GameResult::DRAW;}
        else if (currentPosition.getNbReversibleHalfMoves()>74) {result = GameResult::DRAW;}

        success =  true;
    }

    return success;
}

std::string Game::printTagRoster() const
{
    std::string out = {};

    out += "[Event \"NA\"]";
    out += "\n";

    out += "[Site \"NA\"]";
    out += "\n";

    out += "[Date \"";
    out += printDate();
    out += "\"]";
    out += "\n";

    out += "[Round \"NA\"]";
    out += "\n";

    out += "[White \"";
    out += whitePlayer;
    out += "\"]";
    out += "\n";

    out += "[Black \"";
    out += blackPlayer;
    out += "\"]";
    out += "\n";

    out += "[Result \"";
    out += printResult();
    out += "\"]";

    return out;
}

std::string Game::printDate() const
{
    std::string out = {};

    out += Tools::convertToString(year);
    out += ".";
    out += Tools::convertToString(month);
    out += ".";
    out += Tools::convertToString(day);

    return out;
}

std::string Game::printResult() const
{
    std::string out = {};
    switch(result)
    {
    case (GameResult::NOT_FINISHED) : out = "*"; break;
    case (GameResult::WHITE_WINS) : out = "1-0"; break;
    case (GameResult::BLACK_WINS) : out = "0-1"; break;
    case (GameResult::DRAW) : out = "1/2-1/2"; break;
    default: throw("Invalid game result"); break;
    }
    return out;
}

std::string Game::printPGN(bool printTagRoster) const
{
    std::string out = {};

    if (printTagRoster)
    {
        out += this->printTagRoster();
        out += "\n";
        out += "\n";
    }

    uint moveNum = positions.front().getMoveNumber()-1;
    for (const auto &move : movePGNs)
    {
        if (move.getTurn()==Color::WHITE)
        {
            ++moveNum;
            out += Tools::convertToString(moveNum);
            out += ". ";
        }
        out += move.toPGN();
        out += " ";
    }

    out += printResult();
    return out;
}

bool Game::playRandomMove()
{
    bool success = false;

    if (!isFinished())
    {
        Position currentPosition = positions.back();
        Move move;
        if (currentPosition.getRandomLegalMove(move))
        {
            success = playMove(move);
        }
        else
        {
            std::cout << "Error: failed to get random legal move" << std::endl;
        }
    }

    return success;
}

std::ostream & operator <<(std::ostream &out, const Game &game)
{
    out << game.printPGN();
    return out;
}
