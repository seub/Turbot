#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "piece.h"
#include "position.h"



class Evaluator
{
public:
    Evaluator() {}
    virtual double evaluatePosition(const Position &position) const = 0;

protected:
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
