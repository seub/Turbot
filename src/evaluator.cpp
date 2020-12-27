#include "evaluator.h"
#include "boardhelper.h"
#include "square.h"
#include "piece.h"


BasicEvaluator::BasicEvaluator()
{
    // NB: We could try ALphaZero's piece values
    pieceValues =
    {
        {PieceType::EMPTY,0},
        {PieceType::KING, 0},
        {PieceType::QUEEN, 9.5},
        {PieceType::ROOK, 5},
        {PieceType::BISHOP, 3},
        {PieceType::KNIGHT, 3},
        {PieceType::PAWN, 1}
    };
}
    
double BasicEvaluator::evaluatePosition(const Position &position) const
{
    double score = 0;
    Piece piece;
    double value;
    for(uint index=0; index!=64; ++index)
    {
        if(position.getPiece(piece,Square(index)))
        {
            value = pieceValues.at(piece.getType());
            score += (piece.getColor()==Color::WHITE) ? value : -value;
        }
    }
    return score;
}
