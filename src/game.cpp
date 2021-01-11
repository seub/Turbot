#include "game.h"
#include <fstream>







Game::Game(const Player * const whitePlayer, const Player * const blackPlayer) : currentPosition(true), whitePlayer(whitePlayer), blackPlayer(blackPlayer)
{
    moves.clear();
    movePGNs.clear();
    moveTimes.clear();
    turn = true;
    result = GameResult::NOT_FINISHED;

    date = Tools::currentDate();
    time = Tools::currentTime();
}

bool Game::isFinished() const
{
    return (result!=GameResult::NOT_FINISHED);
}

void Game::playMove(const Move &move, const bool &claimDraw, std::chrono::duration<double> moveTime)
{
    if (isFinished()) {throw("Game is finished!");}

    if (claimDraw) {result = GameResult::DRAW;}
    else
    {
        Position newPosition(false);
        LegalMover currentMover(&currentPosition, true);

        currentMover.applyMove(move, newPosition, true);

        moves.push_back(move);
        moveTimes.push_back(moveTime);
        movePGNs.push_back(MovePGN(move, &currentMover));
        turn = !turn;

        LegalMover newMover(&newPosition, true);
        if (newMover.isCheckmate()) {result = turn ? GameResult::BLACK_WINS : GameResult::WHITE_WINS;}
        else if (newMover.isStalemate()) {result = GameResult::DRAW;}
        else if (newPosition.getNbReversiblePlies()>74) {result = GameResult::DRAW;}

        currentPosition = newPosition;
    }
}

std::string Game::printTagRoster() const
{
    std::string out = {};

    out += "[Event \"NA\"]";
    out += "\n";

    out += "[Site \"NA\"]";
    out += "\n";

    out += "[Date \"";
    out += date;
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

    uint moveNum = 0;
    for (const auto &move : movePGNs)
    {
        if (move.getTurn())
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

bool Game::exportPGN(bool printTagRoster) const
{
    std::ofstream file(createFilename());
    if (file.is_open())
    {
        file << printPGN(printTagRoster);
        return true;
    }
    else {return false;}
}

void Game::playGame()
{
    startGame();
    play();
    finishGame();
}

void Game::startGame()
{
    date = Tools::currentDate();
    time = Tools::currentTime();

    std::cout << std::endl;
    std::cout << "Let's start the game!" << std::endl;
    std::cout << whitePlayer->getName() << " plays White." << std::endl;
    std::cout << blackPlayer->getName() << " plays Black." << std::endl;
    std::cout << std::endl;
}

void Game::play()
{
    Position position;
    MovePGN movePGN;
    Move move;
    std::chrono::duration<double> moveTime;

    while (!isFinished())
    {
        position = currentPosition;
        const Player *nextPlayer = turn ? whitePlayer : blackPlayer;
        bool claimDraw;
        if(!nextPlayer->nextMove(move, claimDraw, moveTime, position))
        {
            std::cout << "Failed to play next move!" << std::endl;
            break;
        }
        playMove(move, claimDraw, moveTime);
        std::string message = {};
        message += nextPlayer->getName();
        if (claimDraw) {message += " claimed a draw!";}
        else
        {
            LegalMover mover(&position, true);
            message += " played ";
            message += MovePGN(move, &mover).toPGN(position.getMoveNumber());
            message += ".";
        }
        message += " [Move time: ";
        message += Tools::convertDoubleToString(moveTime.count());
        message += "s]";
        std::cout << message << std::endl;
    }
}

void Game::finishGame()
{
    std::chrono::duration<double> averageWhite, averageBlack;

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
    std::cout << "Average time per move spent by " << whitePlayer->getName() << ": " << averageWhite.count() << "s" << std::endl;
    std::cout << "Average time per move spent by " << blackPlayer->getName() << ": " << averageBlack.count() << "s" << std::endl;
    std::cout << std::endl;

    std::string print;
    std::cout << "Would you like to see the PGN of the whole game here (Y|N)? ";
    std::cin >> print;
    std::cout << std::endl;
    if ((print=="Y") || (print=="y"))
    {
        std::cout << printPGN();
        std::cout << std::endl << std::endl;
        exportPGN();
        std::cout << "The game was also exported as the PGN file \"" << createFilename() << "\"" << std::endl;
    }
    else
    {
        exportPGN();
        std::cout << "Ok. The game was exported as the PGN file \"" << createFilename() << "\"" << std::endl;
    }
    std::cout << std::endl << std::endl;
}

std::string Game::createFilename() const
{
    std::string res = {};
    res += "Turbot_";
    res += date;
    res += "_";
    res += time;
    res += ".pgn";

    return res;
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
