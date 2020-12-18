#include "move.h"

Move::Move(const Square & origin, const Square & target, bool promotion, PieceType promotedPiece) :
    origin(origin), target(target), promotion(promotion), promotedPiece(promotedPiece)
{

}

Move::Move(uint originIndex, uint targetIndex, bool promotion, PieceType promotedPiece) :
    origin(originIndex), target(targetIndex), promotion(promotion), promotedPiece(promotedPiece)
{

}

bool Move::fromLAN(Move & res, std::string & LANstring)
{
    uint size = LANstring.size();
    if (!(size==4 || size==6)) return false;

    res.origin = LANstring.substr(0, 2);
    res.target = LANstring.substr(2, 4);

    if (size==4)
    {
        res.promotion=false;
        res.promotedPiece=PieceType::Empty;
    }
    else
    {
        char c = LANstring[4];
        if (c!='=') return false;
        c = LANstring[5];
        switch (c)
        {
        case 'Q' : res.promotedPiece=PieceType::Queen; break;
        case 'R' : res.promotedPiece=PieceType::Rook; break;
        case 'B' : res.promotedPiece=PieceType::Bishop; break;
        case 'N': res.promotedPiece=PieceType::Knight; break;
        default : return false; break;
        }
        res.promotion=true;
    }

    return true;
}

bool Move::operator==(const Move &other) const
{
    return (other.origin==origin) && (other.target==target) && (other.promotion==promotion) && ((!promotion) || (other.promotedPiece==promotedPiece));
}

std::string Move::toLAN() const
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
    out << M.toLAN();
    return out;
}
