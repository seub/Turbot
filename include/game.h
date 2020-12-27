#ifndef GAME_H
#define GAME_H


#include "position.h"


enum class GameResult {NOT_FINISHED, WHITE_WINS, BLACK_WINS, DRAW};

class Game
{
    friend std::ostream & operator<<(std::ostream &out, const Game &game);

public:
    Game();

    Position getPosition() const;
    bool ThreeFoldRepetition() const;

    std::string printPGN(bool printTagRoster = true) const;
    std::string printTagRoster() const;
    std::string printDate() const;
    std::string printResult() const;
    bool isFinished() const;

    bool playMove(const Move &move, bool checkLegal=false, bool checkCKLegal=false);
    bool playRandomMove();

private:
    std::vector<Move> moves;
    std::vector<MovePGN> movePGNs;
    std::vector<Position> positions;
    Color turn;
    uint moveNumber;
    GameResult result;

    std::string whitePlayer, blackPlayer;
    uint year, month, day;
};

#endif // GAME_H
