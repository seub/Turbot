#include "move.h"

Move::Move(const Square & origin, const Square & target, bool promotion, PieceType promotedPiece,
           bool offerDraw, bool claimDraw, bool resign) :
    origin(origin), target(target), promotion(promotion), promotedPiece(promotedPiece), offerDraw(offerDraw), claimDraw(claimDraw), resign(resign)
{

}

Move::Move(uint originIndex, uint targetIndex, bool promotion, PieceType promotedPiece, bool offerDraw, bool claimDraw, bool resign) :
    origin(originIndex), target(targetIndex), promotion(promotion), promotedPiece(promotedPiece), offerDraw(offerDraw), claimDraw(claimDraw), resign(resign)
{

}

Move::Move(std::string & longAlgebraicNotation, bool offerDraw, bool claimDraw, bool resign) :
    origin(longAlgebraicNotation.substr(0, 2)),
    target(longAlgebraicNotation.substr(2, 4)),
    offerDraw(offerDraw), claimDraw(claimDraw), resign(resign)
{
    if (longAlgebraicNotation.size()==4)
    {
        promotion=false;
        promotedPiece=PieceType::Empty;
    }
    else
    {
        assert(longAlgebraicNotation.size()==6);
        char c = longAlgebraicNotation[4];
        assert(c=='=');
        c = longAlgebraicNotation[5];
        switch (c)
        {
        case 'Q' : promotedPiece=PieceType::Queen; break;
        case 'R' : promotedPiece=PieceType::Rook; break;
        case 'B' : promotedPiece=PieceType::Bishop; break;
        case 'N': promotedPiece=PieceType::Knight; break;
        default : throw("Bad promoted piece name"); break;
        }
        promotion=true;
    }
}

bool Move::operator==(const Move &other) const
{
    bool res = true;

    if (claimDraw) res = other.claimDraw;
    else if (resign) res = other.resign;
    else
    {
        res &= (other.origin==origin);
        res &= (other.target==target);
        res &= (other.offerDraw==offerDraw);
        res &= (other.promotion==promotion);
        if (promotion) res &= (other.promotedPiece==promotedPiece);
    }

    return res;
}

std::string Move::longAlgebraicNotation() const
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
    out << M.longAlgebraicNotation();
    return out;
}
