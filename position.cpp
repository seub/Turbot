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
