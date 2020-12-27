#include "moveGenerator.h"

#include <iostream>

MinMaxMoveGenerator::MinMaxMoveGenerator(Basicevaluator * evaluator, uint depth): MoveGenerator(evaluator), depth(depth)
{
}

MinMaxMoveGenerator::~MinMaxMoveGenerator()
{
}

Move MinMaxMoveGenerator::pickMove(Position *position)
{
    Mover mover(position);
    auto legalMoves = mover.getlegalMoves();
    int reverse = position->getTurn() == Color::WHITE ? 1: -1;
    std::sort(std::begin(legalMoves),std::end(legalMoves), [this,reverse,position](const Move & first, const Move & second) { return comparator(first, second,position, reverse); });
    auto it = std::begin(legalMoves);
    for(; it != std::end(legalMoves)-1; ++it)
    {
        if( reverse*evaluator->evaluatePosition(mover.applyKCMove(*it)) > reverse*evaluator->evaluatePosition(mover.applyKCMove(*(it+1)))) break;
    }
    return legalMoves[(uint (clock())) % (uint) (it - legalMoves.begin() + 1)];

}


Value MinMaxMoveGenerator::pickMove(Position *position, uint depth)
{
    auto position_hash = position->getHash();
    auto value_pointer = evaluated_res.find(position_hash);
    if(value_pointer != evaluated_res.end() &&  (*(value_pointer->second))[0].depth >= depth)
    {
        //std::cout << std::endl << position_hash << std::endl;
        std::vector<Value> bestvalues = *value_pointer->second;
        return bestvalues[(uint (clock())) % (uint) bestvalues.size()];
    }

    Mover mover(position);
    int reverse = position->getTurn() == Color::BLACK ? 1: -1;
    auto legalMoves = mover.getlegalMoves();
    if(legalMoves.size() == 0) return Value(-1000000);
    float bestevaluation = -1000000;
    std::vector<Value> * bestvalues = new std::vector<Value>();
    for(auto it = std::begin(legalMoves); it != std::end(legalMoves); ++it)
    {
        Position nextPos = mover.applyKCMove(*it);
        Value nextvalue;
        if(depth>1) 
        {
            nextvalue = pickMove(&nextPos, depth-1);
        }
        else
        {
            nextvalue = Value(-reverse*evaluator->evaluatePosition(mover.applyKCMove(*it)));
        }
        
        if(-nextvalue.evaluation < bestevaluation)
        {
            continue;
        }
        if(-nextvalue.evaluation > bestevaluation)
        {
            bestevaluation = -nextvalue.evaluation;
            (*bestvalues).clear();
        }

        (*bestvalues).push_back(Value(-nextvalue.evaluation,*it, depth));
    }
    evaluated_res[position_hash] = bestvalues;
    return (*bestvalues)[(uint (clock())) % (uint) (*bestvalues).size()];
}


