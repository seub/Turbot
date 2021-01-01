#ifndef MOVEPICKER_H
#define MOVEPICKER_H

#include "evaluator.h"
#include "legalmover.h"
//#include <boost/multiprecision/cpp_int.hpp>
//#include <unordered_map>


class Value
{
    friend class MinMaxMovePicker;

public:
    Value() {}
    Value(double evaluation): evaluation(evaluation){}
    Value(double evaluation, const Move &nextMove, uint depth): evaluation(evaluation), nextMove(nextMove), depth(depth) {}

private:
    double evaluation;
    Move nextMove;
    uint depth;
};


class MovePicker
{
public:
    MovePicker() {}
    MovePicker(Evaluator *evaluator) : evaluator(evaluator) {}
    virtual bool pickMove(Move &res, const Position &position, const std::vector<Move> &moves, bool checkLegal = false, bool checkKCLegal = false);
    bool compareMoves(const Position &position, const Move &first, const Move &second, bool checkLegal = false, bool checkKCLegal = false) const;
protected:
    Evaluator *evaluator;
};


class MinMaxMovePicker: public MovePicker
{
public:
    MinMaxMovePicker(Evaluator *evaluator, uint depth): MovePicker(evaluator), depth(depth) {}
    bool pickMove(Move &res, const Position &position, const std::vector<Move> &moves, bool checkLegal = false, bool checkKCLegal = false) override;
    bool pickMove(std::vector<Value> &res, const Position &position, const std::vector<Move> &moves, bool checkLegal, bool checkKCLegal, uint depth);

private:
    //bool bestLine(std::vector<Move> &res, double &eval, const Position &position, uint depth) const;

    //std::unordered_map< Position, std::vector<Value> * > evaluated_res;
    uint depth;
};

#endif // MOVEPICKER_H
