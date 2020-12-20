#include "board.h"

Board::Board(bool gamestart)
{
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
    pieces.fill(Piece());

    std::vector<PieceType> firstRank = {PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN, PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK};

    for (uint i=0; i!=8; ++i)
    {
        pieces[i] = Piece(firstRank[i], Color::WHITE);
        pieces[8+i] = Piece(PieceType::PAWN, Color::WHITE);
        pieces[56+i] = Piece(firstRank[i], Color::BLACK);
        pieces[48+i] = Piece(PieceType::PAWN, Color::BLACK);
    }
}

void Board::clear()
{
    pieces.fill(Piece());
}

std::string Board::printString() const
{
    std::string res = {};
    std::vector< std::string > whitePieces, blackPieces;

    Piece piece;
    for (uint i=0; i!=64; ++i)
    {
        piece = pieces[i];
        if (!piece.isEmpty())
        {
            if (piece.getColor() == Color::WHITE) whitePieces.push_back(piece.name()+Square(i).name());
            if (piece.getColor() == Color::BLACK) blackPieces.push_back(piece.name()+Square(i).name());
        }
    }

    res += "White pieces: ";
    for (const auto &p : whitePieces)
    {
        res += p;
        res += ", ";
    }
    if (!whitePieces.empty()) res.erase (res.end()-2, res.end());
    res += "\n";

    res += "Black pieces: ";
    for (const auto &p : blackPieces)
    {
        res += p;
        res += ", ";
    }
    if (!blackPieces.empty()) res.erase (res.end()-2, res.end());

    return res;
}

std::string Board::toFENstring() const
{
    std::string res = {};

    Piece piece;
    int rank=7, file=0;
    uint emptySquares = 0;
    while (rank!=-1)
    {
        piece = pieces[file + 8*rank];
        if (piece.isEmpty())
        {
            ++emptySquares;
        }
        else
        {
            if (emptySquares>0)
            {
                res += Tools::convertToString(emptySquares);
            }
            res += piece.toFENchar();
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
        case 'K' : res.pieces[file + 8*rank] = Piece(PieceType::KING, Color::WHITE); ++file; break;
        case 'Q' : res.pieces[file + 8*rank] = Piece(PieceType::QUEEN, Color::WHITE);  ++file; break;
        case 'R' : res.pieces[file + 8*rank] = Piece(PieceType::ROOK, Color::WHITE);  ++file; break;
        case 'B' : res.pieces[file + 8*rank] = Piece(PieceType::BISHOP, Color::WHITE);  ++file; break;
        case 'N' : res.pieces[file + 8*rank] = Piece(PieceType::KNIGHT, Color::WHITE);  ++file ;break;
        case 'P' : res.pieces[file + 8*rank] = Piece(PieceType::PAWN, Color::WHITE);  ++file; break;
        case 'k' : res.pieces[file + 8*rank] = Piece(PieceType::KING, Color::BLACK); ++file; break;
        case 'q' : res.pieces[file + 8*rank] = Piece(PieceType::QUEEN, Color::BLACK);  ++file; break;
        case 'r' : res.pieces[file + 8*rank] = Piece(PieceType::ROOK, Color::BLACK);  ++file; break;
        case 'b' : res.pieces[file + 8*rank] = Piece(PieceType::BISHOP, Color::BLACK);  ++file; break;
        case 'n' : res.pieces[file + 8*rank] = Piece(PieceType::KNIGHT, Color::BLACK);  ++file ;break;
        case 'p' : res.pieces[file + 8*rank] = Piece(PieceType::PAWN, Color::BLACK);  ++file; break;
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

bool Board::fileIndex(uint &res, const char &c)
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

bool Board::rankIndex(uint &res, const char &c)
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

bool Board::getPiece(Piece &res, const Square &square) const
{
    res = pieces[square.getIndex()];
    return !(res.isEmpty());
}



std::ostream & operator <<(std::ostream &out, const Board &B)
{
    out << B.printString();
    return out;
}
