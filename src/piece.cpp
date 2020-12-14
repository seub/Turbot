#include "piece.h"


Color Piece::getColor() const
{
    return color;
}

bool Piece::operator==(const Piece &other) const
{
    bool res=true;
    res &= (type == other.type);
    res &= (color == other.color);
    return res;
}

bool Piece::isNull() const
{
    return (type == PieceType::Null);
}

std::string Piece::name() const
{
    return piecenames[uint(type)];
}


double Piece::value() const
{
    return piecevalues[uint(type)];
}

char Piece::FENchar() const
{
    return FENpiecenames[uint(type) + 7*(color==Color::Black)];
}
