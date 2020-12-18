#include "piece.h"




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

Color Piece::getColor() const
{
    return color;
}

PieceType Piece::getType() const
{
    return type;
}

bool Piece::isEmpty() const
{
    return (type == PieceType::Empty);
}

bool Piece::isKing() const
{
    return (type == PieceType::King);
}

bool Piece::isRook() const
{
    return (type == PieceType::Rook);
}

bool Piece::isPawn() const
{
    return (type == PieceType::Pawn);
}

char Piece::name() const
{
    return name(type);
}

char Piece::name(PieceType type)
{
    return piecenames[uint(type)];
}

bool Piece::fromName(PieceType &res, char c)
{
    switch(c)
    {
    case 'E': res=PieceType::Empty; return true;
    case 'K': res=PieceType::King; return true;
    case 'Q': res=PieceType::Queen; return true;
    case 'R': res=PieceType::Rook; return true;
    case 'B': res=PieceType::Bishop; return true;
    case 'N': res=PieceType::Knight; return true;
    case 'P': res=PieceType::Pawn; return true;
    default: return false;
    }
}


char Piece::toFENchar() const
{
    return FENpiecenames[uint(type) + 7*(color==Color::Black)];
}

std::ostream & operator <<(std::ostream &out, const Piece &P)
{
    out << (P.color==Color::White ? "White" : "Black") << " " << P.name();
    return out;
}
