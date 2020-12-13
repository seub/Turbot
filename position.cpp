# include "position.h"

Position::Position()
{
    reset();
}

void Position::reset()
{
    resetPieces();

    turn = Color::White;
    whiteCastled = false;
    blackCastled = false;
    enPassantPossible = false;
    //enPassantSquare = 0;
    drawOffered = false;
    nbReversibleMovesPlayed = 0;
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

std::string Position::print() const
{
    std::string out = {};
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

    out += "White pieces: ";
    for (const auto &p : whitePieces)
    {
        out += p;
        out += ", ";
    }
    out.erase (out.end()-2, out.end());
    out += "\n";

    out += "Black pieces: ";
    for (const auto &p : blackPieces)
    {
        out += p;
        out += ", ";
    }
    out.erase (out.end()-2, out.end());
    return out;
}

