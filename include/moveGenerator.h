#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "evaluator.h"
#include "mover.h"
#include <boost/multiprecision/cpp_int.hpp>

class Value
{
public:
    Value(){}
    Value(float evaluation): evaluation(evaluation){}
    Value(float evaluation, Move nextmove,uint depth): evaluation(evaluation), nextmove(nextmove), depth(depth){}
    ~Value(){}
    uint depth;
    float evaluation;
    Move nextmove;
};

class MoveGenerator
{
protected:
    Basicevaluator *evaluator;
    bool comparator(Move first, Move second, Position * position, int reverse);

public:
    MoveGenerator(Basicevaluator *evaluator);
    ~MoveGenerator();
    Move pickMove( Position * posititon);
};

class MinMaxMoveGenerator: public MoveGenerator
{
protected:
    uint depth;
    std::map<boost::multiprecision::uint512_t,std::vector<Value> * > evaluated_res;
    
public:
    MinMaxMoveGenerator(Basicevaluator *evaluator, uint depth);
    ~MinMaxMoveGenerator();
    Move pickMove(Position * posititon);
    Value pickMove(Position *position, uint depth);
};


#endif // MOVEGENERATOR_H
