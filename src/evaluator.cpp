#include "evaluator.h"
#include "piece.h"


BasicEvaluator::BasicEvaluator()
{
    // NB: We could try AlphaZero's piece values
    pieceValues =
    {
        {PieceType::EMPTY,0},
        {PieceType::KING, 0},
        {PieceType::QUEEN, 9.1},
        {PieceType::ROOK, 5.1},
        {PieceType::BISHOP, 3.3},
        {PieceType::KNIGHT, 3.2},
        {PieceType::PAWN, 1.0}
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







BasicEvaluatorZ::BasicEvaluatorZ()
{
    // NB: We could try AlphaZero's piece values
}

double BasicEvaluatorZ::evaluatePosition(const PositionZ &position) const
{

    double score = 0;
    uint8f piece;
    double value;
    for(uint i=0; i<64; ++i)
    {
        piece = position.getPiece(i);
        value = pieceValues[piece >> 2];
        score += ((piece & 2) >> 1) ? value : -value;
    }
    return score;
}
