#include "moveGenerator.h"

MoveGenerator::MoveGenerator(Basicevaluator * evaluator, Position *position): evaluator(evaluator), position(position)
{
    mover = new LegalMover(position);
}

MoveGenerator::~MoveGenerator()
{
}


bool MoveGenerator::comparator(Move first, Move second, int reverse)
{    
    return reverse*evaluator->evaluatePosition(mover->applyMove(first)) > reverse*evaluator->evaluatePosition(mover->applyMove(second));
}

Move MoveGenerator::pickMove(std::vector<Move> legalMoves)
{
    int reverse = position->getTurn() == Color::WHITE ? 1: -1;
    std::sort(std::begin(legalMoves),std::end(legalMoves), [this,reverse](const Move & first, const Move & second) { return comparator(first, second,reverse); });
    auto it = std::begin(legalMoves);
    for(; it != std::end(legalMoves)-1; ++it)
    {
        if( reverse*evaluator->evaluatePosition(mover->applyMove(*it)) > reverse*evaluator->evaluatePosition(mover->applyMove(*(it+1)))) break;
    }
    return legalMoves[(uint (clock())) % (uint) (it - legalMoves.begin() + 1)];

}
