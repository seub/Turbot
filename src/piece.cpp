#include "piece.h"


Color Piece::getColor() const
{
    return color;
}

bool Piece::operator==(const Piece &other) const
{
    bool res=true;
    res &= (type == other.type);
    if (type!=PieceType::Empty)
    {
        res &= (color == other.color);
    }
    return res;
}

bool Piece::isNull() const
{
    return (type == PieceType::Empty);
}

std::string Piece::name() const
{
    return piecenames[uint(type)];
}

char Piece::FENchar() const
{
    return FENpiecenames[uint(type) + 7*(color==Color::Black)];
}

std::ostream & operator <<(std::ostream &out, const Piece &P)
{
    out << (P.color==Color::White ? "White" : "Black") << " " << P.name();
    return out;
}
