#ifndef POSITION_H
#define POSITION_H

# include "board.h"
# include "move.h"

class Position
{
    friend std::ostream & operator<<(std::ostream &out, const Position &p);
    friend class Mover;

public:
    Position(bool gamestart=false);
    Position(const std::string &FENstr);

    bool operator==(Position const& other) const;

    std::string printString() const;
    std::string printFENString() const;

private:
    void clear();
    void reset();
    void resetPieces();
    bool setFromFENString(const std::string &FENstr, bool setBoard=true);

    Board board;
    Color turn;
    std::array<bool, 4> castlingRights;// White Kside, White Qside, Black Kside, Black Qside
    bool enPassantPossible;
    uint enPassantTargetSquare;
    bool drawOffered;
    uint moveNumber;
    uint nbReversibleHalfMovesPlayed; //For 50 move draw rule
    //NB For draw purposes, we would need the list of past positions to determine threefold repetition
};



#endif // POSITION_H
