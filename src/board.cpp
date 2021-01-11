#include "board.h"
#include "zobrist.h"


Board::Board()
{
    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {Zobrist::GENERATE_ZOBRIST_NUMBERS();}
}


Board::Board(bool gamestart)
{
    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {Zobrist::GENERATE_ZOBRIST_NUMBERS();}

    if (gamestart)
    {
        reset();
    }
    else
    {
        clear();
    }
}

bool Board::operator==(const Board &other) const
{
    return (pieces==other.pieces);
}


void Board::reset()
{
    pieces.fill(0);

    std::array<uint8f, 8> firstRank = {15, 23, 19, 11, 7, 19, 23, 15};
    std::array<uint8f, 8> lastRank = {13, 21, 17, 9, 5, 17, 21, 13};

    for (uint i=0; i!=8; ++i)
    {
        pieces[i] = firstRank[i];
        pieces[8+i] = 27;
        pieces[48+i] = 25;
        pieces[56+i] = lastRank[i];
    }

    hash = recalculateHash();
}

void Board::clear()
{
    pieces.fill(0);
    hash = recalculateHash();
}


std::string Board::toFENstring() const
{
    std::string res = {};

    uint8f piece;
    int rank=7, file=0;
    uint8f emptySquares = 0;
    while (rank!=-1)
    {
        piece = pieces[file + 8*rank];
        if (piece == 0)
        {
            ++emptySquares;
        }
        else
        {
            if (emptySquares>0)
            {
                res += Tools::convertToString(emptySquares);
            }
            res += Piece(piece).toFENchar();
            emptySquares=0;
        }
        ++file;
        if (file==8)
        {
            --rank;
            file=0;
            if (emptySquares>0) res += Tools::convertToString(emptySquares);
            emptySquares=0;
            res += '/';
        }
    }
    res.pop_back();

    return res;
}

bool Board::fromFENstring(Board &res, const std::string &str)
{
    res.clear();

    int rank=7, file=0;
    for (const auto & letter : str)
    {
        switch (letter)
        {
        case ' ' : break;
        case 'K' : res.pieces[file + 8*rank] = 7; ++file; break;
        case 'Q' : res.pieces[file + 8*rank] = 11;  ++file; break;
        case 'R' : res.pieces[file + 8*rank] = 15;  ++file; break;
        case 'B' : res.pieces[file + 8*rank] = 19;  ++file; break;
        case 'N' : res.pieces[file + 8*rank] = 23;  ++file ;break;
        case 'P' : res.pieces[file + 8*rank] = 27;  ++file; break;
        case 'k' : res.pieces[file + 8*rank] = 5; ++file; break;
        case 'q' : res.pieces[file + 8*rank] = 9;  ++file; break;
        case 'r' : res.pieces[file + 8*rank] = 13;  ++file; break;
        case 'b' : res.pieces[file + 8*rank] = 17;  ++file; break;
        case 'n' : res.pieces[file + 8*rank] = 21;  ++file ;break;
        case 'p' : res.pieces[file + 8*rank] = 25;  ++file; break;
        case '1' : file += 1; break;
        case '2' : file += 2; break;
        case '3' : file += 3; break;
        case '4' : file += 4; break;
        case '5' : file += 5; break;
        case '6' : file += 6; break;
        case '7' : file += 7; break;
        case '8' : file += 8; break;
        case '/' : file = 0; --rank; break;
        default : return false;
        }
    }

    return true;
}

bool Board::fileIndex(uint8f &res, const char &c)
{
    switch (c)
    {
    case 'a' : res = 0; return true; break;
    case 'b' : res = 1; return true; break;
    case 'c' : res = 2; return true; break;
    case 'd' : res = 3; return true; break;
    case 'e' : res = 4; return true; break;
    case 'f' : res = 5; return true; break;
    case 'g' : res = 6; return true; break;
    case 'h' : res = 7; return true; break;
    default : return false;
    }
}

bool Board::rankIndex(uint8f &res, const char &c)
{
    switch (c)
    {
    case '1' : res = 0; return true; break;
    case '2' : res = 1; return true; break;
    case '3' : res = 2; return true; break;
    case '4' : res = 3; return true; break;
    case '5' : res = 4; return true; break;
    case '6' : res = 5; return true; break;
    case '7' : res = 6; return true; break;
    case '8' : res = 7; return true; break;
    default : return false;
    }
}

bool Board::getKingSquare(uint8f &res, bool side) const
{
    uint8f kingNum = (side ? 7 : 5);
    res = 0;
    while(res<64)
    {
        if (pieces[res] == kingNum) {break;}
        ++res;
    }
    return (res!=64);
}

bool Board::hasPiece(uint8f square, bool color) const
{
    return (pieces[square]) && (((pieces[square] & 2) >> 1) == color);
}

bool Board::isKing(uint8f square) const
{
    return (pieces[square] >> 2) == 1;
}

bool Board::isRook(uint8f square) const
{
    return (pieces[square] >> 2) == 3;
}

bool Board::isPawn(uint8f square) const
{
    return (pieces[square] >> 2) == 6;
}

void Board::addPiece(uint8f square, uint8f piece)
{
    if (pieces[square]) {hash ^= Zobrist::ZOBRIST_PIECES[square][pieces[square]];}
    hash ^= Zobrist::ZOBRIST_PIECES[square][piece];
    pieces[square] = piece;
}

void Board::removePiece(uint8f square)
{
    if (pieces[square])
    {
        hash ^= Zobrist::ZOBRIST_PIECES[square][pieces[square]];
        pieces[square] = 0;
    }
}


std::size_t Board::recalculateHash() const
{
    std::size_t res = 0;
    for (uint i=0; i<64; ++i) {if (pieces[i]) {res ^= Zobrist::ZOBRIST_PIECES[i][pieces[i]];}}

    return res;
}

