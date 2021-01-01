#ifndef EVALUATOR_H
#define EVALUATOR_H


//#include <unordered_map>

#include "piece.h"
#include "position.h"

//#include <boost/multiprecision/cpp_int.hpp>

class Evaluator
{
public:
    Evaluator() {}
    virtual double evaluatePosition(const Position &position) const = 0; // Pure virtual function --> Evaluator is an abstract class
protected:
    //std::unordered_map<Position,double> evaluated_res;
};


class BasicEvaluator: public Evaluator
{
public:
    BasicEvaluator();

double evaluatePosition(const Position &position) const override;

private:
    std::unordered_map<PieceType, double> pieceValues;
};




#endif // EVALUATOR_H
