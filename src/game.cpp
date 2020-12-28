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
        if (currentPosition.pickRandomLegalMove(move))
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

bool Game::findBestMove(Move &res, const MovePicker *picker) const
{
    bool success = false;

    if (!isFinished())
    {
        Position currentPosition = positions.back();
        if (currentPosition.pickBestMove(res, picker))
        {
            success = true;
        }
        else
        {
            std::cout << "Error: failed to pick best move" << std::endl;
        }
    }

    return success;
}

bool Game::playBestMove(const MovePicker *picker)
{
    bool success = false;

    if (!isFinished())
    {
        Position currentPosition = positions.back();
        Move move;
        if (currentPosition.pickBestMove(move, picker))
        {
            success = playMove(move);
        }
        else
        {
            std::cout << "Error: failed to pick best move" << std::endl;
        }
    }

    return success;
}

void Game::playWithHuman(const MovePicker *picker)
{
    std::cout << "Hello there, I'm Turbot. What is your name? ";
    std::cin >> whitePlayer;
    std::cout << "Okay " << whitePlayer << ", let's play! You play White." << std::endl << std::endl;

    std::string moveString;
    Position position;
    MovePGN movePGN;
    Move move;

    while (!isFinished())
    {
        position = getPosition();
        LegalMover mover1(&position, true);
        std::cout << "What is your " << ((position.getMoveNumber()==1) ? "first" : "next") << " move? Enter it in PGN notation. " << position.getMoveNumber() << ". ";
        std::cin >> moveString;
        if (moveString=="exit") {break;}
        if (MovePGN::fromPGN(movePGN, moveString, &mover1))
        {
            move = (Move) movePGN;
            if (mover1.isInLegalMovesList(move)) {playMove((Move) move);}
            else
            {
                std::cout << "Sorry, that's an illegal move. Try again: (Enter \"exit\" to exit)" << std::endl;
                continue;
            }
        }
        else
        {
            std::cout << "Sorry, that didn't work. Try again: (Enter \"exit\" to exit)" << std::endl;
            continue;
        }
        std::cout << "Okay, you played " << movePGN << ".";
        if (isFinished()) {break;}

        position = getPosition();
        LegalMover mover2(&position, true);
        findBestMove(move, picker);
        std::cout << " My move is " << position.getMoveNumber() << "... " << MovePGN(move, &mover2) << "." << std::endl << std::endl;
        playMove(move);
    }

    std::cout << std::endl << std::endl;
    switch(result)
    {
    case GameResult::NOT_FINISHED : std::cout << "Game is not finished!" << std::endl << std::endl; break;
    case GameResult::WHITE_WINS : std::cout << "You win. Congratulations!" << std::endl << std::endl; break;
    case GameResult::BLACK_WINS : std::cout << "I win! Hahahaha what a loser!" << std::endl << std::endl; break;
    case GameResult::DRAW : std::cout << "Draw. Good game!" << std::endl << std::endl; break;
    default: throw("Invalid game result"); break;
    }

    std::string print;
    std::cout << "Would you like to see the PGN of the whole game (Y|N)? ";
    std::cin >> print;
    std::cout << std::endl;
    if (print=="Y") {std::cout << printPGN();}
    std::cout << std::endl << std::endl;
}



std::ostream & operator <<(std::ostream &out, const Game &game)
{
    out << game.printPGN();
    return out;
}
