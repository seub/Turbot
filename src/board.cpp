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

Board::Board(const std::string &FENstr)
{
    if (setFromFENString(FENstr));
    else throw("Error in Board constructor: Bad FEN String");
}

bool Board::operator==(const Board &other) const
{
    return (pieces==other.pieces);
}

void Board::reset()
{
    pieces.fill(Piece());

    std::vector<PieceType> firstRank = {PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen, PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook};

    for (uint i=0; i!=8; ++i)
    {
        pieces[i] = Piece(firstRank[i], Color::White);
        pieces[8+i] = Piece(PieceType::Pawn, Color::White);
        pieces[56+i] = Piece(firstRank[i], Color::Black);
        pieces[48+i] = Piece(PieceType::Pawn, Color::Black);
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
            if (piece.getColor() == Color::White) whitePieces.push_back(piece.name()+Square(i).name());
            if (piece.getColor() == Color::Black) blackPieces.push_back(piece.name()+Square(i).name());
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

std::string Board::printFENString() const
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
            res += piece.FENchar();
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

bool Board::setFromFENString(const std::string &str)
{
    clear();

    int rank=7, file=0;
    for (const auto & letter : str)
    {
        switch (letter)
        {
        case ' ' : break;
        case 'K' : pieces[file + 8*rank] = Piece(PieceType::King, Color::White); ++file; break;
        case 'Q' : pieces[file + 8*rank] = Piece(PieceType::Queen, Color::White);  ++file; break;
        case 'R' : pieces[file + 8*rank] = Piece(PieceType::Rook, Color::White);  ++file; break;
        case 'B' : pieces[file + 8*rank] = Piece(PieceType::Bishop, Color::White);  ++file; break;
        case 'N' : pieces[file + 8*rank] = Piece(PieceType::Knight, Color::White);  ++file ;break;
        case 'P' : pieces[file + 8*rank] = Piece(PieceType::Pawn, Color::White);  ++file; break;
        case 'k' : pieces[file + 8*rank] = Piece(PieceType::King, Color::Black); ++file; break;
        case 'q' : pieces[file + 8*rank] = Piece(PieceType::Queen, Color::Black);  ++file; break;
        case 'r' : pieces[file + 8*rank] = Piece(PieceType::Rook, Color::Black);  ++file; break;
        case 'b' : pieces[file + 8*rank] = Piece(PieceType::Bishop, Color::Black);  ++file; break;
        case 'n' : pieces[file + 8*rank] = Piece(PieceType::Knight, Color::Black);  ++file ;break;
        case 'p' : pieces[file + 8*rank] = Piece(PieceType::Pawn, Color::Black);  ++file; break;
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

std::ostream & operator <<(std::ostream &out, const Board &B)
{
    out << B.printString();
    return out;
}
