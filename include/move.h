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
    Move(std::string &longAlgebraicNotation, bool offerDraw = false, bool claimDraw = false, bool resign = false); //NB For the 50 move rule, the draw could be claimed after playing the move
    bool operator==(const Move &other) const;

    std::string longAlgebraicNotation() const;
    Square getOrigin() const;
    Square getTarget() const;

private:
    Move(uint originIndex, uint targetIndex,
         bool promotion = false, PieceType promotedPiece= PieceType::Empty, bool offerDraw = false, bool claimDraw = false, bool resign = false);
    Square origin, target;
    bool promotion;
    PieceType promotedPiece;
    bool offerDraw, claimDraw, resign;
};

#endif // MOVE_H
