#ifndef GAME_H
#define GAME_H

#include "player.h"

class Game
{
    friend std::ostream & operator<<(std::ostream &out, const Game &game);

public:
    Game(const Player * const whitePlayer, const Player * const blackPlayer);

    Position getPosition() const;

    std::string printPGN(bool printTagRoster = true) const;
    bool exportPGN(bool printTagRoster = true) const;
    std::string printTagRoster() const;
    std::string printResult() const;
    bool isFinished() const;
    void averageMoveTime(std::chrono::duration<double> &white, std::chrono::duration<double> &black) const;
    std::string createFilename() const;

    bool playMove(const Move &move, const bool &forceDraw, std::chrono::duration<double> moveTime, bool checkLegal=false, bool checkCKLegal=false);
    void playGame();
    
private:
    void startGame();
    void play();
    void finishGame();


    std::vector<Move> moves;
    std::vector<MovePGN> movePGNs;
    std::vector<Board> pastBoards;
    std::vector< std::chrono::duration<double> > moveTimes;
    Position currentPosition;
    Color turn;
    uint moveNumber;
    bool drawOffered;
    GameResult result;


    const Player * const whitePlayer, * const blackPlayer;
    std::string date, time;
};



class GameZ
{
public:
    GameZ(const PlayerZ * const whitePlayer, const PlayerZ * const blackPlayer);

    std::string printPGN(bool printTagRoster = true) const;
    bool exportPGN(bool printTagRoster = true) const;
    std::string printTagRoster() const;
    std::string printResult() const;
    bool isFinished() const;
    void averageMoveTime(std::chrono::duration<double> &white, std::chrono::duration<double> &black) const;
    std::string createFilename() const;

    void playMove(const MoveZ &move, const bool &claimDraw, std::chrono::duration<double> moveTime);
    void playGame();

private:
    void startGame();
    void play();
    void finishGame();


    std::vector<MoveZ> moves;
    std::vector<MovePGNZ> movePGNs;
    std::vector< std::chrono::duration<double> > moveTimes;

    PositionZ currentPosition;
    bool turn;
    GameResult result;

    const PlayerZ * const whitePlayer, * const blackPlayer;
    std::string date, time;
};

#endif // GAME_H
