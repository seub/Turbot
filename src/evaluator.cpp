#include "evaluator.h"
#include "boardhelper.h"
#include "square.h"
#include "piece.h"



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


BasicEvaluator::BasicEvaluator()
{
    // NB: We could try AlphaZero's piece values
    pieceValues =
    {
        {PieceType::EMPTY,0},
        {PieceType::KING, 0},
        {PieceType::QUEEN, 9.1},
        {PieceType::ROOK, 5.1},
        {PieceType::BISHOP, 3.3},
        {PieceType::KNIGHT, 3.2},
        {PieceType::PAWN, 1.0}
    };
}
    
double BasicEvaluator::evaluatePosition(const Position &position) const
{

    double score = 0;
    Piece piece;
    double value;
    for(uint index=0; index!=64; ++index)
    {
        if(position.getPiece(piece,Square(index)))
        {
            value = pieceValues.at(piece.getType());
            score += (piece.getColor()==Color::WHITE) ? value : -value;
        }
    }
    return score;
}
