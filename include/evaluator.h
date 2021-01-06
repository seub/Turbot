#ifndef EVALUATOR_H
#define EVALUATOR_H


#include "piece.h"
#include "position.h"


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
    friend class BasicMovePicker;
    friend class ForcefulMovePicker;
    friend class BasicMovePickerHash;

public:
    PositionEval();
    bool operator==(const PositionEval &other) const;

    bool isLessThan(const PositionEval &other, Color side, bool switchSide) const; // "switchSide" is set to true when the two evals were computed from the point of view of the opponent.

private:
    void constructFromDepthZeroEval(const double &eval);
    void constructWhenOpponentKingCanBeCaptured();
    void constructFromEvalAfterBestMovePlayed(const PositionEval &evalAfterBestMovePlayed);
    void constructFromForceDraw();
    void constructFromMated();

    bool forcedMate, forcedGettingMated;
    uint forcedMateDepth, forcedGettingMatedDepth;
    double eval;
    bool forceDraw;
};



class PositionEvalRich : PositionEval
{
    friend class BasicMovePickerHash;

public:
    PositionEvalRich() {}
    PositionEvalRich(const PositionEval &posEval, const std::vector<Move> &bestLine, uint depth) : PositionEval(posEval), bestLine(bestLine), depth(depth) {}

private:
    std::vector<Move> bestLine;
    uint depth;
};




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
