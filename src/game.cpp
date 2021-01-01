#include "game.h"
#include <fstream>

Game::Game(const Player * const whitePlayer, const Player * const blackPlayer) : whitePlayer(whitePlayer), blackPlayer(blackPlayer)
{
    assert(whitePlayer->getColor()==Color::WHITE);
    assert(blackPlayer->getColor()==Color::BLACK);

    moves.clear();
    movePGNs.clear();
    moveTimes.clear();
    turn = Color::WHITE;
    positions = {Position(true)};
    moveNumber = 1;
    result = GameResult::NOT_FINISHED;
    Tools::currentDate(year, month, day);
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

bool Game::playMove(const Move &move, std::chrono::duration<double> moveTime, bool checkLegal, bool checkCKLegal)
{
    bool success=false;

    if (isFinished()) {throw("Game is finished!");}

    Position currentPosition = positions.back(), newPosition;
    LegalMover currentMover(&currentPosition, true);


    if (currentMover.applyMove(newPosition, move, checkLegal, checkCKLegal))
    {
        moves.push_back(move);
        moveTimes.push_back(moveTime);
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
    out += whitePlayer->getName();
    out += "\"]";
    out += "\n";

    out += "[Black \"";
    out += blackPlayer->getName();
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

bool Game::exportPGN(std::string fileName, bool printTagRoster) const
{
    std::ofstream file(fileName);
    if (file.is_open())
    {
        file << printPGN(printTagRoster);
        return true;
    }
    else {return false;}
}

void Game::playGame()
{
    Position position;
    MovePGN movePGN;
    Move move;
    std::chrono::duration<double> moveTime, averageWhite, averageBlack;

    std::cout << std::endl;

    while (!isFinished())
    {
        position = getPosition();
        const Player *nextPlayer = (turn==Color::WHITE) ? whitePlayer : blackPlayer;
        if(!nextPlayer->nextMove(move, moveTime, position))
        {
            std::cout << "Failed to play next move!" << std::endl;
            break;
        }
        playMove(move, moveTime);
        LegalMover mover(&position, true);
        if (nextPlayer->isHuman())
        {
            std::cout << "Okay, " << nextPlayer->getName();
        }
        else
        {
            std::cout << "The computer \"" << nextPlayer->getName() << "\"";
        }
        std::cout << " played " << MovePGN(move, &mover).toPGN(position.getMoveNumber()) << ". (move time: " << moveTime.count() << "s)" << std::endl;
    }

    std::cout << std::endl << std::endl;
    switch(result)
    {
    case GameResult::NOT_FINISHED : std::cout << "Game was terminated prematurely." << std::endl << std::endl; break;
    case GameResult::WHITE_WINS : std::cout << whitePlayer->getName() << " wins!" << std::endl << std::endl; break;
    case GameResult::BLACK_WINS : std::cout << blackPlayer->getName() << " wins!" << std::endl << std::endl; break;
    case GameResult::DRAW : std::cout << "Draw!" << std::endl << std::endl; break;
    default: throw("Invalid game result"); break;
    }

    averageMoveTime(averageWhite, averageBlack);
    std::cout << "Average time per move spent by White: " <<  averageWhite.count() << "s" << std::endl;
    std::cout << "Average time per move spent by Black: " <<  averageBlack.count() << "s" << std::endl;
    std::cout << std::endl;

    std::string print;
    std::cout << "Would you like to see the PGN of the whole game here (Y|N)? ";
    std::cin >> print;
    std::cout << std::endl;
    if (print=="Y")
    {
        std::cout << printPGN();
        std::cout << std::endl << std::endl;
        exportPGN();
        std::cout << "The game was also exported as a PGN file." << std::endl;
    }
    else
    {
        exportPGN();
        std::cout << "Ok. The game was exported as a PGN file." << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void Game::averageMoveTime(std::chrono::duration<double> &white, std::chrono::duration<double> &black) const
{
    uint nbWhite =0 , nbBlack = 0;
    white = std::chrono::duration<double>::zero();
    black = std::chrono::duration<double>::zero();
    for (uint i=0; i!=moves.size(); ++i)
    {
        if ((i%2)==0)
        {
            white += moveTimes[i];
            ++nbWhite;
        }
        else
        {
            black += moveTimes[i];
            ++nbBlack;
        }
    }
    white = white/nbWhite;
    black = black/nbWhite;
}



std::ostream & operator <<(std::ostream &out, const Game &game)
{
    out << game.printPGN();
    return out;
}
