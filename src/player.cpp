#include "player.h"

ComputerPlayer::ComputerPlayer(Color color, MovePicker *picker, std::string name) : Player(color, name), picker(picker)
{
    std::cout << ((color==Color::WHITE) ? "White" : "Black") << " player was set as the computer \"" << name << "\"." << std::endl;
}


bool ComputerPlayer::findBestMove(Move &res, bool &bestMoveIsForceDraw, const Position &position) const
{
    bool success = false;

    if (position.pickBestMove(res, bestMoveIsForceDraw, picker))
    {
        success = true;
    }
    else
    {
        std::cout << "Error: failed to pick best move" << std::endl;
    }


    return success;
}

HumanPlayer::HumanPlayer(Color color) : Player(color)
{
    std::cout << "Hi there! You play " << ((color==Color::WHITE)? "White" : "Black") << ". What is your name? ";
    std::cin >> name;
    std::cout << "Okay " << name << ", let's go!" << std::endl << std::endl;
}

bool HumanPlayer::nextMove(Move &res, bool &forceDraw, std::chrono::duration<double> &time, const Position &position) const
{
    auto start = std::chrono::steady_clock::now();
    LegalMover mover1(&position, true);
    std::string moveString;
    bool first = (position.getMoveNumber()==1);
    while(true)
    {
        std::cout << std::endl;
        std::cout << name << ", what is your " << (first ? "first" : "next") << " move? "
                  << (first ? "Enter it in PGN notation. " : "")
                  << position.getMoveNumber() << ((color==Color::WHITE) ? ". " : "... ");
        std::cin >> moveString;
        if (moveString=="exit")
        {
            return false;
        }
        else if (moveString=="draw")
        {
            if (position.drawCanBeClaimed())
            {
                forceDraw = true;
                auto end = std::chrono::steady_clock::now();
                time = end-start;
                return true;
            }
            else
            {
                std::cout << "Sorry, you cannot claim a draw in this position. Try again: (Enter \"exit\" to exit, \"draw\" to claim draw)" << std::endl;
                continue;
            }
        }
        if (MovePGN::fromPGN(res, moveString, &mover1))
        {
            if (mover1.isInLegalMovesList(res))
            {
                forceDraw = false;
                auto end = std::chrono::steady_clock::now();
                time = end-start;
                return true;
            }
            else
            {
                std::cout << "Sorry, that's an illegal move. Try again: (Enter \"exit\" to exit, \"draw\" to claim draw)" << std::endl;
                continue;
            }
        }
        else
        {
            std::cout << "Sorry, that didn't work. Try again: (Enter \"exit\" to exit, \"draw\" to claim draw)" << std::endl;
            continue;
        }
    }
}

bool ComputerPlayer::nextMove(Move &res, bool &forceDraw, std::chrono::duration<double> &time, const Position &position) const
{
    auto start = std::chrono::steady_clock::now();
    bool success = findBestMove(res, forceDraw, position);
    auto end = std::chrono::steady_clock::now();
    time = end-start;
    return success;
}
