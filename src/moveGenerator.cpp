#include "moveGenerator.h"


MoveGenerator::MoveGenerator(Basicevaluator * evaluator): evaluator(evaluator)
{
}

MoveGenerator::~MoveGenerator()
{
}


bool MoveGenerator::comparator(Move first, Move second, Position * position, int reverse)
{    
    Mover mover(position);
    return reverse*evaluator->evaluatePosition(mover.applyKCMove(first)) > reverse*evaluator->evaluatePosition(mover.applyKCMove(second));
}

Move MoveGenerator::pickMove(Position *position)
{
    Mover mover(position);
    auto legalMoves = mover.getlegalMoves();
    int reverse = position->getTurn() == Color::WHITE ? 1: -1;
    std::sort(std::begin(legalMoves),std::end(legalMoves), [this,reverse,position](const Move & first, const Move & second) { return comparator(first, second, position, reverse); });
    auto it = std::begin(legalMoves);
    for(; it != std::end(legalMoves)-1; ++it)
    {
        if( reverse*evaluator->evaluatePosition(mover.applyKCMove(*it)) > reverse*evaluator->evaluatePosition(mover.applyKCMove(*(it+1)))) break;
    }
    return legalMoves[(uint (clock())) % (uint) (it - legalMoves.begin() + 1)];

}
