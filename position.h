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
    bool operator==(Position const& other) const;

    void reset();
    void clear();
    void setFromFENString(const std::string &str);

    std::string printString() const;
    std::string exportFENString() const;

    double materialCount() const;

    //bool isLegal(const Move &move) const;


private:
    void resetPieces();

    std::array<Piece, 64> pieces;
    Color turn;
    std::array<bool, 4> castlingRights;// White Kside, White Qside, Black Kside, Black Qside
    bool enPassantPossible;
    uint enPassantTargetSquare;
    bool drawOffered;
    uint moveNumber;
    uint nbReversibleHalfMovesPlayed; //For 50 move draw rule
    //NB For draw purposes, we need the list of past positions to determine threefold repetition
};



#endif // POSITION_H
