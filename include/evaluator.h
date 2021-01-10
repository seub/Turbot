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






class EvaluatorZ
{
public:
    EvaluatorZ() {}
    virtual double evaluatePosition(const PositionZ &position) const = 0;

protected:
};


class BasicEvaluatorZ: public EvaluatorZ
{
public:
    BasicEvaluatorZ();

    double evaluatePosition(const PositionZ &position) const override;

private:
    static inline const std::array<double, 7> pieceValues = {0, 0, 9.1, 5.1, 3.3, 3.2, 1.0};
};

#endif // EVALUATOR_H
