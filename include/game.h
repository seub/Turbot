#ifndef GAME_H
#define GAME_H


#include "position.h"


enum class GameResult {NOT_FINISHED, WHITE_WINS, BLACK_WINS, DRAW};

class Player
{
public:
    Player(){name="Turbot";}
    virtual bool play(Move &move, Position &position, MovePGN &movePGN) const = 0;
    std::string getName(){return name;}
protected:
    std::string name;
};

class HumanPlayer: Player
{
public:
    HumanPlayer();
    bool play(Move &move, Position &position, MovePGN &movePGN) const override;
};

class ComputerPlayer: Player
{
public:
    ComputerPlayer() {}
    ComputerPlayer(MovePicker *picker, std::string name="Turbot"): Player(), picker(picker) {this->name = name;}
    bool play(Move &res, Position &position, MovePGN &movePGN) const override { return findBestMove(res,position); }
private:
    MovePicker *picker;
    bool findBestMove(Move &res, Position &position) const;
};


class Game
{
    friend std::ostream & operator<<(std::ostream &out, const Game &game);

public:
    Game();
    Game(Player *whitePlayer, Player *BlackPlayer);

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

    Player *whitePlayer, *blackPlayer;
    uint year, month, day;
};

#endif // GAME_H
