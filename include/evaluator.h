#ifndef EVALUATOR_H
#define EVALUATOR_H


#include <map>

#include "piece.h"
#include "position.h"

#include <boost/multiprecision/cpp_int.hpp>

class Evaluator
{
private:
    /* data */
public:
    Evaluator(/* args */);
    std::map<boost::multiprecision::uint512_t,float> evaluated_res;
    ~Evaluator();
    float evaluatePosition(const Position &position);
};

class Basicevaluator: public Evaluator
{
private:
    std::map<PieceType,float> pieceValue;

public:
    Basicevaluator(std::map<PieceType,float> pieceValue);
    ~Basicevaluator();
    float evaluatePosition(const Position &position);

};




#endif // EVALUATOR_H