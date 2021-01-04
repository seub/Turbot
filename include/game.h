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
    std::vector< std::chrono::duration<double> > moveTimes;
    Position currentPosition;
    Color turn;
    uint moveNumber;
    GameResult result;

    const Player * const whitePlayer, * const blackPlayer;
    std::string date, time;
};

#endif // GAME_H
