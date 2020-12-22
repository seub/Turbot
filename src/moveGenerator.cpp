#include "moveGenerator.h"

MoveGenerator::MoveGenerator(Basicevaluator * evaluator, Position *position): evaluator(evaluator), position(position)
{
    mover = new Mover(position);
}

MoveGenerator::~MoveGenerator()
{
}


bool MoveGenerator::comparator(Move first, Move second)
{    
    return evaluator->evaluatePosition(mover->applyKCMove(first)) > evaluator->evaluatePosition(mover->applyKCMove(second));
}

Move MoveGenerator::pickMove(std::vector<Move> legalMoves)
{
    
    std::sort(std::begin(legalMoves),std::end(legalMoves), [this](const Move & first, const Move & second) { return comparator(first, second); });
    auto it = std::begin(legalMoves);
    for(; it != std::end(legalMoves)-1; ++it)
    {
        if( evaluator->evaluatePosition(mover->applyKCMove(*it)) > evaluator->evaluatePosition(mover->applyKCMove(*(it+1)))) break;
    }
    return legalMoves[(uint (clock())) % (uint) (it - legalMoves.begin() + 1)];

}