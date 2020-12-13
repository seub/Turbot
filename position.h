#ifndef POSITION_H
#define POSITION_H

# include "tools.h"
# include "piece.h"

class Position
{
    friend std::ostream & operator<<(std::ostream &out, const Position &P);

public:
    Position();
    //Position(const Position &initialPosition, const MoveSeq &seq);

    void reset();

    std::string printSquare(uint index);

    uint materialCount() const;



    //bool isLegal(const Move &move) const;


private:
    void resetPieces();

    std::vector<Piece> pieces;
    Color turn;
    bool whiteCastled;
    bool blackCastled;
    bool enPassantPossible;
    uint enPassantSquare;
    bool drawOffered;
    uint nbReversibleMovesPlayed; //For 50 move draw rule
    //NB For draw purposes, we need the list of past positions to determine threefold repetition
};



#endif // POSITION_H
