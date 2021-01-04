#include "movepicker.h"



bool MinMaxMovePicker::pickMove(Move &res, bool &claimDraw, const Position &position)
{
    claimDraw = false;

    std::vector<Move> moves;
    if (!position.getLegalMoves(moves)) {return false;}

    std::vector<Value> bestValues;
    if(!pickMove(bestValues, position, moves, depth)) return false;

    std::vector<Value> out;
    std::sample(bestValues.begin(), bestValues.end(), std::back_inserter(out), 1, std::mt19937{std::random_device{}()});
    res = out.back().nextMove;

    return true;
}


bool MinMaxMovePicker::pickMove(std::vector<Value> &res, const Position &position, const std::vector<Move> &moves, uint depth)
{

    if(moves.size() == 0) return false;
    //auto value_pointer = evaluated_res.find(position);

    //if(value_pointer != evaluated_res.end() &&  (*(value_pointer->second))[0].depth >= depth)
    //{
    //    //std::cout << std::endl << position.getHash() << std::endl;
    //    res = std::vector<Value>(*value_pointer->second);
    //    return true;
    //}

    int reverse = (position.getTurn() == Color::WHITE) ? 1: -1;
    double bestevaluation = -1000000;
    std::vector<Value> * bestvalues = new std::vector<Value>();
    Position nextPos;
    std::vector<Value> nextvalues;
    double eval;
    std::vector<Move> nextLegalmoves;
    for(auto it = std::begin(moves); it != std::end(moves); ++it)
    {
        if(!position.applyMove(nextPos, *it)) continue;

        if(!nextPos.getKCLegalMoves(nextLegalmoves)) continue;

        if(depth>1)
        {
            if(!pickMove(nextvalues, nextPos, nextLegalmoves, depth-1)) continue;
            eval = -nextvalues[0].eval;
        }
        else
        {
            eval = reverse*evaluator->evaluatePosition(nextPos);
        }

        if(eval < bestevaluation) continue;

        if(eval > bestevaluation)
        {
            bestevaluation = eval;
            (*bestvalues).clear();
        }

        (*bestvalues).push_back(Value(eval,*it, depth));
    }
    //evaluated_res[position] = bestvalues;
    res = std::vector<Value>(*bestvalues);
    return res.size()>0;
}

std::string MinMaxMovePicker::createName() const
{
    std::string res = "TurbotMinMax(";
    res += Tools::convertToString(depth);
    res += ")";
    return res;
}


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

std::string NaiveMovePicker::createName() const
{
    std::string res = "TurbotNaive(";
    res += Tools::convertToString(depth);
    res += ")";
    return res;
}


bool NaiveMovePicker::pickMove(Move &res, bool &claimDraw, const Position &position)
{
    std::vector<Move> bestLine;
    PositionEval eval;
    if (findBestLine(bestLine, eval, claimDraw, position, position, false, depth))
    {
        if (!claimDraw) {res = bestLine.front();}
        std::string bestLineString;
        position.printPGN(bestLineString, bestLine);
        if (claimDraw) {std::cout << "[Best line: 'Claim Draw'  |  eval = " << eval << "]" << std::endl;}
        else {std::cout << "[Best line: " << bestLineString << "  |  eval = " << eval << "]" << std::endl;}
        return true;
    }
    else
    {
        std::cout << "NaiveMovePicker::pickMove failed" << std::endl;
        return false;
    }
}


bool NaiveMovePicker::findBestLine(std::vector<Move> &resLine, PositionEval &resEval, bool &claimDraw,
                                   const Position &position, const Position &previousPos, bool previousPosAvailable, uint depth) const
{
    resLine.clear();

    if (position.getNbReversibleHalfMoves()>74)
    {
        claimDraw = true;
        resEval.constructFromForceDraw();
        return true;
    }

    if (depth==0)
    {
        std::cout << "WARNING: NaiveMovePicker::findBestLine is not supposed to be called for depth 0!" << std::endl;
        return false;
    }
    else
    {
        LegalMover mover(&position, false);
        std::vector<Move> moves = mover.getKCLegalMoves();
        if (moves.empty())
        {
            std::cout << "WARNING in NaiveMovePicker::findBestLine: no KC moves!?" << std::endl;
            claimDraw = false;
            resEval.constructWhenOpponentKingCanBeCaptured();
            return true;
        }
        else if (mover.isOpponentKingCapturable())
        {
            if (!previousPosAvailable)
            {
                std::cout << "WARNING: unable to determine stalemate" << std::endl;
                claimDraw = false;
                resEval.constructWhenOpponentKingCanBeCaptured();
                return true;
            }
            else
            {
                LegalMover stale(&previousPos, true);
                if (stale.isStalemate())
                {
                    claimDraw = true;
                    resEval.constructFromForceDraw();
                    return true;
                }
                else
                {
                    claimDraw = false;
                    resEval.constructWhenOpponentKingCanBeCaptured();
                    return true;
                }
            }
        }
        else
        {

            Position newPos;
            Move bestMove;
            Color turn = position.getTurn();
            PositionEval eval, bestEval;
            std::vector<Move> line, bestLine;
            bool first;

            if (position.drawCanBeClaimed())
            {
                claimDraw = true;
                bestEval.constructFromForceDraw();
                bestLine = line;
                first = false;
            }
            else
            {
                claimDraw = false;
                first = true;
            }

            for (const auto &move : moves)
            {
                newPos = mover.applyMove(move);
                if (depth==1) {eval.constructFromDepthZeroEval(evaluator->evaluatePosition(newPos));}
                else
                {
                    bool opponentClaimDraw;
                    findBestLine(line, eval, opponentClaimDraw, newPos, position, true, depth-1);
                }

                if (first)
                {
                    claimDraw = false;
                    bestMove = move;
                    bestEval = eval;
                    bestLine = line;
                    first = false;
                }
                else if (bestEval.isLessThan(eval, turn, true))
                {
                    claimDraw = false;
                    bestMove = move;
                    bestEval = eval;
                    bestLine = line;
                }
            }

            if (claimDraw)
            {
                resLine.clear();
                resEval.constructFromForceDraw();
            }
            else
            {
                if (depth==1) {resLine = {bestMove};}
                else
                {
                    resLine = bestLine;
                    resLine.insert(resLine.begin(), bestMove);
                }
                resEval.constructFromEvalAfterBestMovePlayed(bestEval);
            }

            return true;

        }
    }
}

std::string ForcefulMovePicker::createName() const
{
    std::string res = "TurbotForceful(";
    res += Tools::convertToString(depth);
    res += ", ";
    res += Tools::convertToString(fdepth);
    res += ", ";
    res += Tools::convertToString(gdepth);
    res += ")";
    return res;
}


bool ForcefulMovePicker::pickMove(Move &res, bool &claimDraw, const Position &position)
{
    std::vector<Move> bestLine;
    PositionEval eval;
    if (findBestLine(bestLine, eval, claimDraw, position, position, false, depth, fdepth, gdepth))
    {
        if (!claimDraw) {res = bestLine.front();}
        std::string bestLineString;
        position.printPGN(bestLineString, bestLine);
        if (claimDraw) {std::cout << "[Best line: 'Claim Draw'  |  eval = " << eval << "]" << std::endl;}
        else {std::cout << "[Best line: " << bestLineString << "  |  eval = " << eval << "]" << std::endl;}
        return true;
    }

    else
    {
        std::cout << "NaiveMovePicker::pickMove failed" << std::endl;
        return false;
    }
}

bool ForcefulMovePicker::findBestLine(std::vector<Move> &resLine, PositionEval &resEval, bool &claimDraw, const Position &position,
                                      const Position &previousPos, bool previousPosAvailable, uint depth, uint fdepth, uint gdepth) const
{
    resLine.clear();

    if (position.getNbReversibleHalfMoves()>74)
    {
        claimDraw = true;
        resEval.constructFromForceDraw();
        return true;
    }

    if (depth==0)
    {
        std::cout << "WARNING: ForcefulMovePicker::findBestLine is not supposed to be called for depth 0!" << std::endl;
        return false;
    }
    else
    {
        LegalMover mover(&position, false);
        std::vector<Move> moves = mover.getKCLegalMoves();
        if (moves.empty())
        {
            std::cout << "WARNING in ForcefulMovePicker::findBestLine: no moves!" << std::endl;
            return false;
        }
        else if (mover.isOpponentKingCapturable())
        {
            if (!previousPosAvailable)
            {
                std::cout << "WARNING: unable to determine stalemate" << std::endl;
                claimDraw = false;
                resEval.constructWhenOpponentKingCanBeCaptured();
                return true;
            }
            else
            {
                LegalMover stale(&previousPos, true);
                if (stale.isStalemate())
                {
                    claimDraw = true;
                    resEval.constructFromForceDraw();
                    return true;
                }
                else
                {
                    claimDraw = false;
                    resEval.constructWhenOpponentKingCanBeCaptured();
                    return true;
                }
            }
        }
        else
        {

            Position newPos;
            Move bestMove;
            Color turn = position.getTurn();
            PositionEval eval, bestEval;
            std::vector<Move> line, bestLine;
            bool first;

            if (position.drawCanBeClaimed())
            {
                claimDraw = true;
                bestEval.constructFromForceDraw();
                bestLine = line;
                first = false;
            }
            else
            {
                claimDraw = false;
                first = true;
            }

            for (const auto &move : moves)
            {
                newPos = mover.applyMove(move);
                bool opponentClaimDraw;
                if (mover.isCapture(move) && ((depth!=1 && fdepth!=0) || (depth==1 && gdepth!=0)))
                {
                    if (depth==1) {findBestLine(line, eval, opponentClaimDraw, newPos, position, true, depth, fdepth, gdepth-1);}
                    else {findBestLine(line, eval, opponentClaimDraw, newPos, position, true, depth, fdepth-1, gdepth);}
                }
                else if (depth==1) {eval.constructFromDepthZeroEval(evaluator->evaluatePosition(newPos));}
                else {findBestLine(line, eval, opponentClaimDraw, newPos, position, true, depth-1, fdepth, this->gdepth);}

                if (first)
                {
                    claimDraw = false;
                    bestMove = move;
                    bestEval = eval;
                    bestLine = line;
                    first = false;
                }
                else if (bestEval.isLessThan(eval, turn, true))
                {
                    claimDraw = false;
                    bestMove = move;
                    bestEval = eval;
                    bestLine = line;
                }
            }

            if (claimDraw)
            {
                resLine.clear();
                resEval.constructFromForceDraw();
            }
            else
            {
                if (depth==1) {resLine = {bestMove};}
                else
                {
                    resLine = bestLine;
                    resLine.insert(resLine.begin(), bestMove);
                }
                resEval.constructFromEvalAfterBestMovePlayed(bestEval);
            }

            return true;

        }
    }
}


