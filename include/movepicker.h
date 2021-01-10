#ifndef MOVEPICKER_H
#define MOVEPICKER_H

#include "evaluator.h"
#include "legalmover.h"
#include "positioneval.h"

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


class BasicMovePickerHash: public MovePicker
{
public:
    BasicMovePickerHash(const Evaluator *evaluator, uint depth);
    bool pickMove(Move &res, bool &claimDraw, const Position &position) override;

    std::string createName() const override;

private:
    bool findBestLine(std::vector<Move> &resLine, PositionEval &resEval, bool &claimDraw, const Position &position,
                      const Position &previousPos, bool previousPosAvailable, uint depth);

    const uint maxDepth;
    std::unordered_map< Position, PositionEvalRich > alreadyEvaluated;
    uint nbPositions, nbRepetitions;
};










class MovePickerZ
{
public:
    MovePickerZ() : evaluator(nullptr) {}
    MovePickerZ(const EvaluatorZ *evaluator) : evaluator(evaluator) {}
    virtual bool pickMove(MoveZ &res, bool &claimDraw, const PositionZ &position) = 0;
    virtual std::string createName() const = 0;

protected:
    const EvaluatorZ * const evaluator;
};

class BasicMovePickerZ: public MovePickerZ
{
public:
    BasicMovePickerZ(const EvaluatorZ *evaluator, uint depth): MovePickerZ(evaluator), maxDepth(depth) {}
    bool pickMove(MoveZ &res, bool &claimDraw, const PositionZ &position) override;
    bool findBestLine(std::vector<MoveZ> &res, PositionEvalZ &eval, bool &claimDraw, const PositionZ &position,
                      const PositionZ &previousPos, bool previousPosAvailable, uint depth) const; //NB: Previous position is only needed to deal with stalemate :(
    std::string createName() const override;

private:
    uint maxDepth;
};


class BasicMovePickerHashZ: public MovePickerZ
{
public:
    BasicMovePickerHashZ(const EvaluatorZ *evaluator, uint depth);
    bool pickMove(MoveZ &res, bool &claimDraw, const PositionZ &position) override;

    std::string createName() const override;

private:
    bool findBestLine(std::vector<MoveZ> &resLine, PositionEvalZ &resEval, bool &claimDraw, const PositionZ &position,
                      const PositionZ &previousPos, bool previousPosAvailable, uint depth);

    const uint maxDepth;
    std::unordered_map< PositionZ, PositionEvalRichZ > alreadyEvaluated;
    uint nbPositions, nbRepetitions;
};




#endif // MOVEPICKER_H
