#include "evaluator.h"
#include "piece.h"




BasicEvaluator::BasicEvaluator()
{
    // NB: We could try AlphaZero's piece values
}

double BasicEvaluator::evaluatePosition(const Position &position) const
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
