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
    Value(double eval): eval(eval){}
    Value(double eval, const Move &nextMove, uint depth): eval(eval), nextMove(nextMove), depth(depth) {}

private:
    double eval;
    Move nextMove;
    uint depth;
};


class PositionEval
{
    friend std::ostream & operator<<(std::ostream &out, const PositionEval &PE);
    friend class NaiveMovePicker;
    friend class ForcefulMovePicker;

public:
    PositionEval();
    bool operator==(const PositionEval &other) const;

    bool isLessThan(const PositionEval &other, Color side, bool switchSide) const; // "switchSide" is set to true when the two evals were computed from the point of view of the opponent.

private:
    void constructFromDepthZeroEval(const double &eval);
    void constructWhenOpponentKingCanBeCaptured();
    void constructFromEvalAfterBestMovePlayed(const PositionEval &evalAfterBestMovePlayed);
    void constructFromForceDraw();

    bool forcedMate, forcedGettingMated;
    uint forcedMateDepth, forcedGettingMatedDepth;
    double eval;
    bool forceDraw;
};


class MovePicker
{
public:
    MovePicker() : evaluator(nullptr) {}
    MovePicker(const Evaluator *evaluator) : evaluator(evaluator) {}
    virtual bool pickMove(Move &res, bool &claimDraw, const Position &position) = 0;
    virtual std::string createName() const = 0;

protected:
    const Evaluator * const evaluator;
};


class MinMaxMovePicker: public MovePicker
{
public:
    MinMaxMovePicker(const Evaluator *evaluator, uint depth): MovePicker(evaluator), depth(depth) {}
    bool pickMove(Move &res, bool &claimDraw, const Position &position) override;
    bool pickMove(std::vector<Value> &res, const Position &position, const std::vector<Move> &moves, uint depth);
    std::string createName() const override;

private:
    //std::unordered_map< Position, std::vector<Value> * > evaluated_res;
    uint depth;
};


class NaiveMovePicker: public MovePicker
{
public:
    NaiveMovePicker(const Evaluator *evaluator, uint depth): MovePicker(evaluator), depth(depth) {}
    bool pickMove(Move &res, bool &claimDraw, const Position &position) override;
    bool findBestLine(std::vector<Move> &res, PositionEval &eval, bool &claimDraw, const Position &position,
                      const Position &previousPos, bool previousPosAvailable, uint depth) const; //NB: Previous position is only needed to deal with stalemate :(
    std::string createName() const override;

private:
    uint depth;
};


class ForcefulMovePicker: public MovePicker
{
public:
    ForcefulMovePicker(const Evaluator *evaluator, uint depth, uint fdepth, uint gdepth): MovePicker(evaluator), depth(depth), fdepth(fdepth), gdepth(gdepth) {}
    bool pickMove(Move &res, bool &claimDraw, const Position &position) override;
    bool findBestLine(std::vector<Move> &res, PositionEval &eval, bool &claimDraw, const Position &position,
                      const Position &previousPos, bool previousPosAvailable, uint depth, uint fdepth, uint gdepth) const;
    std::string createName() const override;


private:
    uint depth, fdepth, gdepth;
};




#endif // MOVEPICKER_H
