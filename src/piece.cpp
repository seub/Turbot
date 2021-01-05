#include "piece.h"




bool Piece::operator==(const Piece &other) const
{
    bool res=true;
    res &= (type == other.type);
    if (type!=PieceType::EMPTY)
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
    return (type == PieceType::EMPTY);
}

bool Piece::isKing() const
{
    return (type == PieceType::KING);
}

bool Piece::isRook() const
{
    return (type == PieceType::ROOK);
}

bool Piece::isPawn() const
{
    return (type == PieceType::PAWN);
}

char Piece::name() const
{
    return name(type);
}

char Piece::name(PieceType type)
{
    return pieceNames[uint(type)];
}

bool Piece::fromName(PieceType &res, char c)
{
    switch(c)
    {
    case 'E': res=PieceType::EMPTY; return true;
    case 'K': res=PieceType::KING; return true;
    case 'Q': res=PieceType::QUEEN; return true;
    case 'R': res=PieceType::ROOK; return true;
    case 'B': res=PieceType::BISHOP; return true;
    case 'N': res=PieceType::KNIGHT; return true;
    case 'P': res=PieceType::PAWN; return true;
    default: return false;
    }
}


char Piece::toFENchar() const
{
    return FENpieceNames[uint(type) + 7*(color==Color::BLACK)];
}

std::ostream & operator <<(std::ostream &out, const Piece &piece)
{
    out << (piece.color==Color::WHITE ? "White" : "Black") << " " << piece.name();
    return out;
}
