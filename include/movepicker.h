#ifndef MOVEPICKER_H
#define MOVEPICKER_H

#include "evaluator.h"
#include "legalmover.h"


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
    MinMaxMovePicker(const Evaluator *evaluator, uint depth);
    bool pickMove(Move &res, bool &claimDraw, const Position &position) override;
    bool pickMove(std::vector<Value> &res, const Position &position, const std::vector<Move> &moves, uint depth);
    std::string createName() const override;

private:
    std::unordered_map< Position, std::vector<Value> * > evaluated_res;
    uint depth;
};


class BasicMovePicker: public MovePicker
{
public:
    BasicMovePicker(const Evaluator *evaluator, uint depth): MovePicker(evaluator), depth(depth) {}
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

class BasicMovePickerHash: public MovePicker
{
public:
    BasicMovePickerHash(const Evaluator *evaluator, uint depth);
    bool pickMove(Move &res, bool &claimDraw, const Position &position) override;
    bool findBestLine(std::vector<Move> &res, PositionEval &eval, bool &claimDraw, const Position &position,
                      const Position &previousPos, bool previousPosAvailable, uint depth);
    std::string createName() const override;

private:
    uint depth;
    std::unordered_map< Position, PositionEvalRich > alreadyEvaluated;
    uint nbPositions, nbRepetitions;
};




#endif // MOVEPICKER_H
