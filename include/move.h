#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "piece.h"

class Move
{
    friend class Mover;
    friend std::ostream & operator<<(std::ostream &out, const Move &M);

public:
    Move(const Square &origin, const Square &target,
         bool promotion = false, PieceType promotedPiece= PieceType::Empty, bool offerDraw = false, bool claimDraw = false, bool resign = false);

    std::string name() const;

private:
    Move(uint originIndex, uint targetIndex);
    Square origin, target;
    bool promotion;
    PieceType promotedPiece;
    bool offerDraw, claimDraw, resign;
};

#endif // MOVE_H
