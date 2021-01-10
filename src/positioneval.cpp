#include "positioneval.h"

PositionEval::PositionEval()
{
}

bool PositionEval::operator==(const PositionEval &other) const
{
    if (forceDraw!=other.forceDraw) {return false;}

    if (forcedMate)
    {
        return (other.forcedMate) && (other.forcedGettingMated==forcedGettingMated) && (forcedMateDepth==other.forcedMateDepth);
    }
    else if (forcedGettingMated)
    {
        return (other.forcedGettingMated) && (other.forcedMate==forcedMate) && (forcedGettingMatedDepth==other.forcedGettingMatedDepth);
    }
    else
    {
        return (!other.forcedMate) && (!(other.forcedGettingMated)) && other.eval==eval;
    }
}

bool PositionEval::isLessThan(const PositionEval &other, Color side, bool switchSide) const
{
    if (forceDraw)
    {
        if (other.forceDraw) {return false;}
        else if (other.forcedGettingMated) {return switchSide;}
        else if (other.forcedMate) {return !switchSide;}
        else {return ((side==Color::WHITE) ? (other.eval>0.0) : (other.eval<0.0));}
    }
    else if (other.forceDraw)
    {
        if (forceDraw) {return false;}
        else if (forcedGettingMated) {return !switchSide;}
        else if (forcedMate) {return switchSide;}
        else {return ((side==Color::WHITE) ? (eval<0.0) : (eval>0.0));}
    }
    else if (forcedGettingMated)
    {
        if (switchSide) {return (other.forcedGettingMated && other.forcedGettingMatedDepth<forcedGettingMatedDepth);}
        else {return !(other.forcedGettingMated && (other.forcedGettingMatedDepth<=forcedGettingMatedDepth));}
    }
    else if (forcedMate)
    {
        if (switchSide) {return ((!other.forcedMate) || (other.forcedMate && other.forcedMateDepth>forcedMateDepth));}
        else {return other.forcedMate && (other.forcedMateDepth<forcedMateDepth);}
    }
    else
    {
        if (other.forcedGettingMated) {return switchSide;}
        else if (other.forcedMate) {return !switchSide;}
        else {return ((side==Color::WHITE) ?  (eval<other.eval) : (eval>other.eval));}
    }
}


void PositionEval::constructFromDepthZeroEval(const double &eval)
{
    forcedMate = false;
    forcedGettingMated = false;
    forceDraw = false;
    this->eval = eval;
}


void PositionEval::constructWhenOpponentKingCanBeCaptured()
{
    forcedMate = true;
    forcedGettingMated = false;
    forcedMateDepth = 0;
    forceDraw = false;
}

void PositionEval::constructFromMated()
{
    forcedMate = false;
    forcedGettingMated = true;
    forcedGettingMatedDepth = 0;
    forceDraw = false;
}

void PositionEval::constructFromEvalAfterBestMovePlayed(const PositionEval &evalAfterBestMovePlayed)
{
    forceDraw = false;

    if (evalAfterBestMovePlayed.forceDraw)
    {
        forcedGettingMated = false;
        forcedMate = false;
        eval = 0.0;
    }
    else if (evalAfterBestMovePlayed.forcedMate)
    {
        forcedMate = false;
        forcedGettingMated = true;
        forcedGettingMatedDepth = evalAfterBestMovePlayed.forcedMateDepth;
    }
    else if (evalAfterBestMovePlayed.forcedGettingMated)
    {
        forcedMate = true;
        forcedGettingMated = false;
        forcedMateDepth = evalAfterBestMovePlayed.forcedGettingMatedDepth + 1;
    }
    else
    {
        forcedMate = false;
        forcedGettingMated = false;
        eval = evalAfterBestMovePlayed.eval;
    }
}

void PositionEval::constructFromForceDraw()
{
    forcedMate = false;
    forcedGettingMated = false;
    forceDraw = true;
}

std::ostream & operator <<(std::ostream &out, const PositionEval &PE)
{
    if (PE.forceDraw)
    {
        out << "1/2-1/2";
    }
    else if (PE.forcedMate)
    {
        out << "#" << PE.forcedMateDepth;
    }
    else if (PE.forcedGettingMated)
    {
        out << "#" << PE.forcedGettingMatedDepth;
    }
    else
    {
        out << PE.eval;
    }
    return out;
}










PositionEvalZ::PositionEvalZ()
{
}

bool PositionEvalZ::operator==(const PositionEvalZ &other) const
{
    if (forceDraw!=other.forceDraw) {return false;}

    if (forcedMate)
    {
        return (other.forcedMate) && (other.forcedGettingMated==forcedGettingMated) && (forcedMateDepth==other.forcedMateDepth);
    }
    else if (forcedGettingMated)
    {
        return (other.forcedGettingMated) && (other.forcedMate==forcedMate) && (forcedGettingMatedDepth==other.forcedGettingMatedDepth);
    }
    else
    {
        return (!other.forcedMate) && (!(other.forcedGettingMated)) && other.eval==eval;
    }
}

bool PositionEvalZ::isLessThan(const PositionEvalZ &other, bool side, bool switchSide) const
{
    if (forceDraw)
    {
        if (other.forceDraw) {return false;}
        else if (other.forcedGettingMated) {return switchSide;}
        else if (other.forcedMate) {return !switchSide;}
        else {return (side ? (other.eval>0.0) : (other.eval<0.0));}
    }
    else if (other.forceDraw)
    {
        if (forceDraw) {return false;}
        else if (forcedGettingMated) {return !switchSide;}
        else if (forcedMate) {return switchSide;}
        else {return (side ? (eval<0.0) : (eval>0.0));}
    }
    else if (forcedGettingMated)
    {
        if (switchSide) {return (other.forcedGettingMated && other.forcedGettingMatedDepth<forcedGettingMatedDepth);}
        else {return !(other.forcedGettingMated && (other.forcedGettingMatedDepth<=forcedGettingMatedDepth));}
    }
    else if (forcedMate)
    {
        if (switchSide) {return ((!other.forcedMate) || (other.forcedMate && other.forcedMateDepth>forcedMateDepth));}
        else {return other.forcedMate && (other.forcedMateDepth<forcedMateDepth);}
    }
    else
    {
        if (other.forcedGettingMated) {return switchSide;}
        else if (other.forcedMate) {return !switchSide;}
        else {return (side ?  (eval<other.eval) : (eval>other.eval));}
    }
}


void PositionEvalZ::constructFromDepthZeroEval(const double &eval)
{
    forcedMate = false;
    forcedGettingMated = false;
    forceDraw = false;
    this->eval = eval;
}


void PositionEvalZ::constructWhenOpponentKingCanBeCaptured()
{
    forcedMate = true;
    forcedGettingMated = false;
    forcedMateDepth = 0;
    forceDraw = false;
}

void PositionEvalZ::constructFromMated()
{
    forcedMate = false;
    forcedGettingMated = true;
    forcedGettingMatedDepth = 0;
    forceDraw = false;
}

void PositionEvalZ::constructFromEvalAfterBestMovePlayed(const PositionEvalZ &evalAfterBestMovePlayed)
{
    forceDraw = false;

    if (evalAfterBestMovePlayed.forceDraw)
    {
        forcedGettingMated = false;
        forcedMate = false;
        eval = 0.0;
    }
    else if (evalAfterBestMovePlayed.forcedMate)
    {
        forcedMate = false;
        forcedGettingMated = true;
        forcedGettingMatedDepth = evalAfterBestMovePlayed.forcedMateDepth;
    }
    else if (evalAfterBestMovePlayed.forcedGettingMated)
    {
        forcedMate = true;
        forcedGettingMated = false;
        forcedMateDepth = evalAfterBestMovePlayed.forcedGettingMatedDepth + 1;
    }
    else
    {
        forcedMate = false;
        forcedGettingMated = false;
        eval = evalAfterBestMovePlayed.eval;
    }
}

void PositionEvalZ::constructFromForceDraw()
{
    forcedMate = false;
    forcedGettingMated = false;
    forceDraw = true;
}

std::ostream & operator <<(std::ostream &out, const PositionEvalZ &PE)
{
    if (PE.forceDraw)
    {
        out << "1/2-1/2";
    }
    else if (PE.forcedMate)
    {
        out << "#" << PE.forcedMateDepth;
    }
    else if (PE.forcedGettingMated)
    {
        out << "#" << PE.forcedGettingMatedDepth;
    }
    else
    {
        out << PE.eval;
    }
    return out;
}

