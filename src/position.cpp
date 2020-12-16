# include "position.h"

Position::Position(bool initialPos)
{
    if (initialPos)
    {
        reset();
    }
    else
    {
        clear();
    }
}

Position::Position(const std::string &FENstr)
{
    setFromFENString(FENstr);
}

bool Position::operator==(const Position &other) const
{
    bool res = true;

    res &= (pieces==other.pieces);
    res &= (turn == other.turn);
    res &= (castlingRights == other.castlingRights);
    res &= (enPassantPossible == other.enPassantPossible);
    res &= (enPassantTargetSquare == other. enPassantTargetSquare);
    res &= (drawOffered == other.drawOffered);
    res &= (moveNumber == other.moveNumber);
    res &= (nbReversibleHalfMovesPlayed == other.nbReversibleHalfMovesPlayed);

    return res;
}

void Position::reset()
{
    resetPieces();

    turn = Color::White;
    castlingRights.fill(true);
    enPassantPossible = false;
    enPassantTargetSquare = 0;
    drawOffered = false;
    moveNumber = 1;
    nbReversibleHalfMovesPlayed = 0;
}

void Position::resetPieces()
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

void Position::clear()
{
    pieces.fill(Piece());

    turn = Color::White;
    castlingRights.fill(false);
    enPassantPossible = false;
    enPassantTargetSquare = 0;
    drawOffered = false;
    moveNumber = 0;
    nbReversibleHalfMovesPlayed = 0;
}

std::string Position::printString() const
{
    std::string res = {};
    std::vector< std::string > whitePieces, blackPieces;

    Piece piece;
    for (uint i=0; i!=64; ++i)
    {
        piece = pieces[i];
        if (!piece.isNull())
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
    res.erase (res.end()-2, res.end());
    res += "\n";

    res += "Black pieces: ";
    for (const auto &p : blackPieces)
    {
        res += p;
        res += ", ";
    }
    res.erase (res.end()-2, res.end());

    res += "\n";
    res += "Move number: ";
    res += Tools::convertToString(moveNumber);
    res += ".";

    res += "\n";
    res += "Side to play: ";
    res += (turn==Color::White) ? "White" : "Black";

    res += "\n";
    res += "Castling rights (kingside|queenside) : White = ";
    res += castlingRights[0] ? "Yes|" : "No|";
    res += castlingRights[1] ? "Yes" : "No";
    res += ", Black = ";
    res += castlingRights[2] ? "Yes|" : "No|";
    res += castlingRights[3] ? "Yes" : "No";

    res += "\n";
    res += "Draw offered by opponent? ";
    res += drawOffered ? "Yes" : "No";

    res += "\n";
    res += "Number of reversible half-moves played: " + Tools::convertToString(nbReversibleHalfMovesPlayed);
    return res;
}

std::string Position::printFENString() const
{
    std::string res = {};

    Piece piece;
    int rank=7, file=0;
    uint emptySquares = 0;
    while (rank!=-1)
    {
        piece = pieces[file + 8*rank];
        if (piece.isNull())
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
            if (emptySquares==8) res += Tools::convertToString(emptySquares);
            emptySquares=0;
            res += '/';
        }
    }
    res.pop_back();

    res += ' ';
    res += (turn==Color::White) ? 'w' : 'b';

    res += ' ';
    std::string castlingRights = {};
    if (castlingRights[0]) castlingRights += 'K';
    if (castlingRights[1]) castlingRights += 'Q';
    if (castlingRights[2]) castlingRights += 'k';
    if (castlingRights[3]) castlingRights += 'q';
    res += (castlingRights.empty()) ? "-" : castlingRights;

    res += ' ';
    res += (enPassantPossible)? Square(enPassantTargetSquare).name() : "-";

    res += ' ';
    res += Tools::convertToString(nbReversibleHalfMovesPlayed);

    res += ' ';
    res += Tools::convertToString(moveNumber);

    return res;
}

bool Position::setFromFENString(const std::string &str)
{
    clear();

    uint i=0;
    int rank=7, file=0;
    char letter = str[i];
    while (letter != ' ')
    {
        letter = str[i];
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
        ++i;
    }

    letter = str[i];
    switch (letter)
    {
    case 'w' : turn = Color::White; break;
    case 'b' : turn = Color::Black; break;
    default : return false;
    }
    ++i;

    letter = str[i];
    if (letter != ' ') throw("Invalid FEN string");
    ++i;

    letter = str[i];
    castlingRights.fill(false);
    while (letter != ' ')
    {
        letter = str[i];
        switch (letter)
        {
        case ' ' : break;
        case 'K' : castlingRights[0] = true; break;
        case 'Q' : castlingRights[1] = true; break;
        case 'k' : castlingRights[2] = true; break;
        case 'q' : castlingRights[3] = true; break;
        case '-' : break;
        default : return false;
        }
        ++i;
    }

    letter = str[i];
    if (letter == '-')
    {
        enPassantPossible = false;
        enPassantTargetSquare = 0;
        ++i;
    }
    else
    {
        enPassantPossible = true;
        std::string enPassantSquareName = {};
        enPassantSquareName += letter;
        ++i;
        letter = str[i];
        enPassantSquareName += letter;
        enPassantTargetSquare = Square(enPassantSquareName).getIndex();
        ++i;
    }

    letter = str[i];
    if (letter != ' ') return false;
    ++i;

    uint j = str.find(' ', i);
    nbReversibleHalfMovesPlayed = std::stoi(str.substr(i, j));
    i = j+1;

    moveNumber = std::stoi(str.substr(i, std::string::npos));
    return true;
}

std::ostream & operator <<(std::ostream &out, const Position &P)
{
    out << P.printString();
    return out;
}
