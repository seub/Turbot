#ifndef POSITION_H
#define POSITION_H

# include "tools.h"

class Position
{
    friend std::ostream & operator<<(std::ostream &out, const Position &P);

public:
    Position() {}
    //Position(const Position &initialPosition, const MoveSeq &seq);

    uint materialCount();
    //bool isLegal(const Move &move);
    double eval();

private:
};



#endif // POSITION_H
