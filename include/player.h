#ifndef PLAYER_H
#define PLAYER_H

#include "position.h"

class Player
{
public:
    Player() {}
    virtual bool nextMove(Move &move, bool &forceDraw, std::chrono::duration<double> &time, const Position &position) const = 0;
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
    bool nextMove(Move &res,  bool &forceDraw, std::chrono::duration<double> &time, const Position &position) const override;
};

class ComputerPlayer: Player
{
public:
    ComputerPlayer(MovePicker *picker);
    bool isHuman() const override {return false;}
    bool nextMove(Move &res,  bool &forceDraw, std::chrono::duration<double> &time, const Position &position) const override;

private:
    MovePicker *picker;
    bool findBestMove(Move &res, bool &claimDraw, const Position &position) const;
};








class PlayerZ
{
public:
    PlayerZ() {}
    virtual bool nextMove(MoveZ &move, bool &claimDraw, std::chrono::duration<double> &time, const PositionZ &position) const = 0;
    virtual bool isHuman() const = 0;
    std::string getName() const {return name;}

protected:
    std::string name;
};

class HumanPlayerZ: PlayerZ
{
public:
    HumanPlayerZ();
    HumanPlayerZ(const std::string &name);
    bool isHuman() const override {return true;}
    bool nextMove(MoveZ &res,  bool &claimDraw, std::chrono::duration<double> &time, const PositionZ &position) const override;
};


class MovePickerZ;

class ComputerPlayerZ: PlayerZ
{
public:
    ComputerPlayerZ(MovePickerZ *picker);
    bool isHuman() const override {return false;}
    bool nextMove(MoveZ &res,  bool &claimDraw, std::chrono::duration<double> &time, const PositionZ &position) const override;

private:
    MovePickerZ *picker;
    bool findBestMove(MoveZ &res, bool &claimDraw, const PositionZ &position) const;
};


#endif // PLAYER_H
