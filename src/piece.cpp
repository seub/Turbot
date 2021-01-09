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








PieceZ::PieceZ(uint8f type, bool color)
{
    num = type;
    num <<= 1;
    if (color) {++num;}
    num <<= 1;
    if (type) {++num;}
}



PieceZ::PieceZ(PieceType type, Color color) : PieceZ((uint8f) type, color==Color::WHITE)
{
}

PieceType PieceZ::getType() const
{
    return (PieceType) (num >> 2);
}

uint8f PieceZ::getNum() const
{
    return num;
}

char PieceZ::name(PieceType type)
{
    return pieceNames[uint(type)];
}

char PieceZ::name() const
{
    return name(getType());
}

bool PieceZ::fromName(PieceType &res, char c)
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

bool PieceZ::isEmpty() const
{
    return num == 0;
}

bool PieceZ::isKing() const
{
    return (num >> 2) == 1;
}

bool PieceZ::isPawn() const
{
    return (num >> 2) == 6;
}

bool PieceZ::isRook() const
{
    return (num >> 2) == 3;
}

char PieceZ::toFENchar() const
{
    return FENpieceNames[(num >> 2) + 7*( (num & 2)==0 )];
}
