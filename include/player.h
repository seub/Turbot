#ifndef PLAYER_H
#define PLAYER_H

#include "position.h"

class Player
{
public:
    Player(Color color, std::string name = {}) : color(color), name(name) {}
    virtual bool nextMove(Move &move, std::chrono::duration<double> &time, const Position &position) const = 0;
    virtual bool isHuman() const = 0;
    std::string getName() const {return name;}
    Color getColor() const {return color;}

protected:
    std::string name;
    Color color;
};

class HumanPlayer: Player
{
public:
    HumanPlayer(Color color);
    HumanPlayer(Color color, std::string name) : Player(color, name) {};
    bool isHuman() const override {return true;}
    bool nextMove(Move &res,  std::chrono::duration<double> &time, const Position &position) const override;
};

class ComputerPlayer: Player
{
public:
    ComputerPlayer(Color color, MovePicker *picker, std::string name="Turbot");
    bool isHuman() const override {return false;}
    bool nextMove(Move &res,  std::chrono::duration<double> &time, const Position &position) const override;

private:
    MovePicker *picker;
    bool findBestMove(Move &res, const Position &position) const;
};


#endif // PLAYER_H
