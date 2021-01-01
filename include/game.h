#ifndef GAME_H
#define GAME_H

#include "player.h"


enum class GameResult {NOT_FINISHED, WHITE_WINS, BLACK_WINS, DRAW};


class Game
{
    friend std::ostream & operator<<(std::ostream &out, const Game &game);

public:
    Game(const Player * const whitePlayer, const Player * const blackPlayer);

    Position getPosition() const;
    bool ThreeFoldRepetition() const;

    std::string printPGN(bool printTagRoster = true) const;
    bool exportPGN(std::string fileName = "turbot.pgn", bool printTagRoster = true) const;
    std::string printTagRoster() const;
    std::string printDate() const;
    std::string printResult() const;
    bool isFinished() const;
    void averageMoveTime(std::chrono::duration<double> &white, std::chrono::duration<double> &black) const;

    bool playMove(const Move &move, std::chrono::duration<double> moveTime, bool checkLegal=false, bool checkCKLegal=false);
    void playGame();
    

private:
    std::vector<Move> moves;
    std::vector<MovePGN> movePGNs;
    std::vector< std::chrono::duration<double> > moveTimes;
    std::vector<Position> positions;
    Color turn;
    uint moveNumber;
    GameResult result;

    const Player * const whitePlayer, * const blackPlayer;
    uint year, month, day;
};

#endif // GAME_H
