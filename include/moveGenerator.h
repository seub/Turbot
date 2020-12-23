#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "evaluator.h"
#include "mover.h"

class MoveGenerator
{
private:
    Basicevaluator *evaluator;
    Position *position;
    Mover *mover;
    bool comparator(Move first, Move second, int reverse);
public:
    MoveGenerator(Basicevaluator *evaluator, Position * posititon);
    ~MoveGenerator();
    Move pickMove( std::vector<Move> legalMoves);
};


#endif // MOVEGENERATOR_H
