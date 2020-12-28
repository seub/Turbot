#ifndef GAME_H
#define GAME_H


#include "position.h"


enum class GameResult {NOT_FINISHED, WHITE_WINS, BLACK_WINS, DRAW};

class Player
{
public:
    Player(Color color, std::string name = {}) : color(color), name(name) {}
    virtual bool nextMove(Move &move, const Position &position) const = 0;
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
    bool nextMove(Move &res, const Position &position) const override;
};

class ComputerPlayer: Player
{
public:
    ComputerPlayer(Color color, MovePicker *picker, std::string name="Turbot"): Player(color, name), picker(picker) {}
    bool isHuman() const override {return false;}
    bool nextMove(Move &res, const Position &position) const override {return findBestMove(res,position);}

private:
    MovePicker *picker;
    bool findBestMove(Move &res, const Position &position) const;
};


class Game
{
    friend std::ostream & operator<<(std::ostream &out, const Game &game);

public:
    Game(const Player * const whitePlayer, const Player * const blackPlayer);

    Position getPosition() const;
    bool ThreeFoldRepetition() const;

    std::string printPGN(bool printTagRoster = true) const;
    std::string printTagRoster() const;
    std::string printDate() const;
    std::string printResult() const;
    bool isFinished() const;

    bool playMove(const Move &move, bool checkLegal=false, bool checkCKLegal=false);
    bool playRandomMove();
    void playGame();
    

private:
    std::vector<Move> moves;
    std::vector<MovePGN> movePGNs;
    std::vector<Position> positions;
    Color turn;
    uint moveNumber;
    GameResult result;

    const Player * const whitePlayer, * const blackPlayer;
    uint year, month, day;
};

#endif // GAME_H
