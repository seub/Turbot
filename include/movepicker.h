#ifndef MOVEPICKER_H
#define MOVEPICKER_H

#include "evaluator.h"
#include "legalmover.h"

class MovePicker
{
public:
    MovePicker(BasicEvaluator *evaluator) : evaluator(evaluator) {}
    bool pickMove(Move &res, const Position &position, const std::vector<Move> &moves, bool checkLegal = false, bool checkKCLegal = false) const;
    bool compareMoves(const Position &position, const Move &first, const Move &second, bool checkLegal = false, bool checkKCLegal = false) const;

private:
    BasicEvaluator *evaluator;
};


#endif // MOVEPICKER_H
