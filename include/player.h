#ifndef PLAYER_H
#define PLAYER_H

#include "position.h"





class Player
{
public:
    Player() {}
    virtual bool nextMove(Move &move, bool &claimDraw, std::chrono::duration<double> &time, const Position &position) const = 0;
    virtual bool isHuman() const = 0;
    std::string getName() const {return name;}

protected:
    std::string name;
};

class HumanPlayer: Player
{
public:
    HumanPlayer();
    HumanPlayer(const std::string &name);
    bool isHuman() const override {return true;}
    bool nextMove(Move &res,  bool &claimDraw, std::chrono::duration<double> &time, const Position &position) const override;
};


class MovePicker;

class ComputerPlayer: Player
{
public:
    ComputerPlayer(MovePicker *picker);
    bool isHuman() const override {return false;}
    bool nextMove(Move &res,  bool &claimDraw, std::chrono::duration<double> &time, const Position &position) const override;

private:
    MovePicker *picker;
    bool findBestMove(Move &res, bool &claimDraw, const Position &position) const;
};


#endif // PLAYER_H
