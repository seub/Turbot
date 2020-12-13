# include "position.h"

Position::Position()
{
    reset();
}

void Position::reset()
{
    resetPieces();

    turn = Color::White;
    whiteCastlingRightK = true;
    whiteCastlingRightQ = true;
    blackCastlingRightK = true;
    blackCastlingRightQ = true;
    enPassantPossible = false;
    enPassantTargetSquare = 0;
    drawOffered = false;
    moveNumber = 1;
    nbReversibleHalfMovesPlayed = 0;
}

void Position::resetPieces()
{
    pieces = std::vector<Piece>(64, Piece());

    std::vector<PieceType> firstRow = {PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen, PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook};

    for (uint i=0; i!=8; ++i)
    {
        pieces[i] = Piece(firstRow[i], Color::White);
        pieces[8+i] = Piece(PieceType::Pawn, Color::White);
        pieces[56+i] = Piece(firstRow[i], Color::Black);
        pieces[48+i] = Piece(PieceType::Pawn, Color::Black);
    }
}

double Position::materialCount() const
{
    double res=0;
    for (const auto & piece : pieces)
    {
        if (!piece.isNull())
        {
            if (piece.getColor() == Color::White) res += piece.value();
            if (piece.getColor() == Color::Black) res -= piece.value();
        }
    }
    return res;
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
            if (piece.getColor() == Color::White) whitePieces.push_back(" "+piece.name()+Square(i).name());
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
    res += whiteCastlingRightK ? "Yes|" : "No|";
    res += whiteCastlingRightQ ? "Yes" : "No";
    res += ", Black = ";
    res += blackCastlingRightK ? "Yes|" : "No|";
    res += blackCastlingRightQ ? "Yes" : "No";

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
    int row=7, file=0;
    uint emptySquares = 0;
    while (row!=-1)
    {
        piece = pieces[file + 8*row];
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
            --row;
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
    if (whiteCastlingRightK) castlingRights += 'K';
    if (whiteCastlingRightQ) castlingRights += 'Q';
    if (blackCastlingRightK) castlingRights += 'k';
    if (blackCastlingRightK) castlingRights += 'q';
    res += (castlingRights.empty()) ? "-" : castlingRights;

    res += ' ';
    res += (enPassantPossible)? Square(enPassantTargetSquare).name() : "-";

    res += ' ';
    res += Tools::convertToString(nbReversibleHalfMovesPlayed);

    res += ' ';
    res += Tools::convertToString(moveNumber);

    return res;
}

