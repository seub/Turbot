#ifndef POSITIONEVAL_H
#define POSITIONEVAL_H

#include "position.h"




class PositionEval
{
    friend std::ostream & operator<<(std::ostream &out, const PositionEval &PE);
    friend class BasicMovePicker;
    friend class BasicMovePickerHash;


public:
    PositionEval();
    bool operator==(const PositionEval &other) const;

    bool isLessThan(const PositionEval &other, bool side, bool switchSide) const; // "switchSide" is set to true when the two evals were computed from the point of view of the opponent.

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

#endif // POSITIONEVAL_H
