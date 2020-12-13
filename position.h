#ifndef POSITION_H
#define POSITION_H

# include "tools.h"
# include "piece.h"
# include "square.h"

class Position
{
    friend std::ostream & operator<<(std::ostream &out, const Position &P);

public:
    Position();
    //Position(const Position &initialPosition, const MoveSeq &seq);

    void reset();
    void inputFENString(const std::string &in);

    std::string printString() const;
    std::string printFENString() const;

    double materialCount() const;

    //bool isLegal(const Move &move) const;


private:
    void resetPieces();

    std::vector<Piece> pieces;
    Color turn;
    bool whiteCastlingRightK, whiteCastlingRightQ, blackCastlingRightK, blackCastlingRightQ;
    bool enPassantPossible;
    uint enPassantTargetSquare;
    bool drawOffered;
    uint moveNumber;
    uint nbReversibleHalfMovesPlayed; //For 50 move draw rule
    //NB For draw purposes, we need the list of past positions to determine threefold repetition
};



#endif // POSITION_H
