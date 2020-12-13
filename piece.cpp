#include "piece.h"


Color Piece::getColor() const
{
    return color;
}

bool Piece::isNull() const
{
    return (type == PieceType::Null);
}

std::string Piece::name() const
{
    return piecenames[uint(type)];
}
