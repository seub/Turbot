#include "move.h"

Move::Move(const Square &origin, const Square &target, bool promotion, PieceType promotedPiece,
           bool offerDraw, bool claimDraw, bool resign) :
    origin(origin), target(target), promotion(promotion), promotedPiece(promotedPiece), offerDraw(offerDraw), claimDraw(claimDraw), resign(resign)
{

}

Move::Move(uint originIndex, uint targetIndex) : origin(originIndex), target(targetIndex)
{

}

std::string Move::name() const
{
    std::string res = origin.name()+target.name();
    if (promotion) res += "=" + Piece(promotedPiece).name();
    return res;
}

Square Move::getOrigin() const
{
    return origin;
}

Square Move::getTarget() const
{
    return target;
}


std::ostream & operator <<(std::ostream &out, const Move &M)
{
    out << M.name();
    return out;
}
