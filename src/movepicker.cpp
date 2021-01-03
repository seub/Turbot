#include "movepicker.h"



bool MinMaxMovePicker::pickMove(Move &res, const Position &position)
{
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


PositionEval::PositionEval()
{
}

bool PositionEval::operator==(const PositionEval &other) const
{
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
    if (forcedGettingMated)
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
    this->eval = eval;
}


void PositionEval::constructWhenOpponentKingCanBeCaptured()
{
    forcedMate = true;
    forcedGettingMated = false;
    forcedMateDepth = 0;
}

void PositionEval::constructFromEvalAfterBestMove(const PositionEval &evalAfterBestMovePlayed)
{

    if (evalAfterBestMovePlayed.forcedMate)
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

std::ostream & operator <<(std::ostream &out, const PositionEval &PE)
{
    if (PE.forcedMate)
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


bool NaiveMovePicker::pickMove(Move &res, const Position &position)
{
    std::vector<Move> bestLine;
    PositionEval eval;
    if (findBestLine(bestLine, eval, position, depth))
    {
        res = bestLine.front();
        std::string bestLineString;
        position.printPGN(bestLineString, bestLine);
        std::cout << "Best line found: " << bestLineString << " (eval = " << eval << ")" << std::endl;
        return true;
    }
    else {return false;}
}

bool NaiveMovePicker::findBestLine(std::vector<Move> &resLine, PositionEval &resEval, const Position &position, uint depth) const
{
    resLine.clear();

    if (depth==0)
    {
        std::cout << "WARNING: NaiveMovePicker::findBestLine is not supposed to be called for depth 0!" << std::endl;
        return false;
    }
    else
    {
        LegalMover mover(&position, false);
        std::vector<Move> moves = mover.getKCLegalMoves();
        if (moves.empty()) {return false;}
        else if (mover.isOpponentKingCapturable())
        {
            resEval.constructWhenOpponentKingCanBeCaptured();
            return true;
        }
        else
        {
            Position newPos;
            Move bestMove;
            Color turn = position.getTurn();
            PositionEval eval, bestEval;
            std::vector<Move> line, bestLine;
            bool first = true;
            for (const auto &move : moves)
            {
                newPos = mover.applyMove(move);
                if (depth==1) {eval.constructFromDepthZeroEval(evaluator->evaluatePosition(newPos));}
                else
                {
                    findBestLine(line, eval, newPos, depth-1);
                }


                if (first)
                {
                    bestMove = move;
                    bestEval = eval;
                    bestLine = line;
                    first = false;
                }
                else if (bestEval.isLessThan(eval, turn, true))
                {
                    bestMove = move;
                    bestEval = eval;
                    bestLine = line;
                }
            }

            if (depth==1) {resLine = {bestMove};}
            else
            {
                resLine = bestLine;
                resLine.insert(resLine.begin(), bestMove);
            }
            resEval.constructFromEvalAfterBestMove(bestEval);
            return true;
        }
    }
}


