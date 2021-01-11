#include "piece.h"


Piece::Piece(uint8f type, bool color)
{
    num = type;
    num <<= 1;
    if (color) {++num;}
    num <<= 1;
    if (type) {++num;}
}



Piece::Piece(PieceType type, bool color) : Piece((uint8f) type, color)
{
}

PieceType Piece::getType() const
{
    return (PieceType) (num >> 2);
}

uint8f Piece::getNum() const
{
    return num;
}

char Piece::name(PieceType type)
{
    return pieceNames[uint(type)];
}

char Piece::name() const
{
    return name(getType());
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

bool Piece::isEmpty() const
{
    return num == 0;
}

bool Piece::isKing() const
{
    return (num >> 2) == 1;
}

bool Piece::isPawn() const
{
    return (num >> 2) == 6;
}

bool Piece::isRook() const
{
    return (num >> 2) == 3;
}

char Piece::toFENchar() const
{
    return FENpieceNames[(num >> 2) + 7*( (num & 2)==0 )];
}
