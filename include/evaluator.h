#ifndef EVALUATOR_H
#define EVALUATOR_H


#include <map>

#include "piece.h"
#include "position.h"

class Evaluator
{
public:
    Evaluator() {}
    virtual double evaluatePosition(const Position &position) const = 0; // Pure virtual function --> Evaluator is an abstract class

private:
};


class BasicEvaluator: public Evaluator
{
public:
    BasicEvaluator();

double evaluatePosition(const Position &position) const override;

private:
    std::map<PieceType, double> pieceValues;
};




#endif // EVALUATOR_H
