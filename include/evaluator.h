#ifndef EVALUATOR_H
#define EVALUATOR_H


#include <map>

#include "piece.h"
#include "position.h"

class Evaluator
{
private:
    /* data */
public:
    Evaluator(/* args */);
    ~Evaluator();
    float evaluatePosition(const Position &position) const;
};

class Basicevaluator: public Evaluator
{
private:
    std::map<PieceType,float> pieceValue;
public:
    Basicevaluator(std::map<PieceType,float> pieceValue);
    ~Basicevaluator();
float evaluatePosition(const Position &position) const;

};




#endif // EVALUATOR_H