#include "movepicker.h"


bool MovePicker::compareMoves(const Position &position, const Move &first, const Move &second, bool checkLegal, bool checkKCLegal) const
{
    Position firstPos, secondPos;
    double firstEval, secondEval;
    if ((position.applyMove(firstPos, first, checkLegal, checkKCLegal)) && (position.applyMove(secondPos, second, checkLegal, checkKCLegal)))
    {
        firstEval = evaluator->evaluatePosition(firstPos);
        secondEval = evaluator->evaluatePosition(secondPos);
    }
    return (position.getTurn()==Color::WHITE) ? (firstEval>secondEval) : (secondEval>firstEval);
}

bool MovePicker::pickMove(Move &res, const Position &position, const std::vector<Move> &moves, bool checkLegal, bool checkKCLegal)
{
    if (moves.empty())
    {
        std::cout << "No move" << std::endl;
        return false;
    }

    std::vector<Move> sortedMoves = moves;
    std::sort(std::begin(sortedMoves),std::end(sortedMoves), [this, position, checkLegal, checkKCLegal](const Move &first, const Move &second)
    { return compareMoves(position, first, second, checkLegal, checkKCLegal); });

    auto it = std::begin(sortedMoves);
    for(; it != std::end(sortedMoves)-1; ++it)
    {
        if(compareMoves(position, *it, *(it+1), checkLegal, checkKCLegal)) {break;}
    }

    std::vector<Move> bestMoves(sortedMoves.begin(), it+1);
    std::vector<Move> out;
    std::sample(bestMoves.begin(), bestMoves.end(), std::back_inserter(out), 1, std::mt19937{std::random_device{}()});
    res = out.back();


    return true;
}


bool MinMaxMovePicker::pickMove(Move &res, const Position &position, const std::vector<Move> &moves, bool checkLegal, bool checkKCLegal)
{
    std::vector<Value> bestValues;
    if(!pickMove(bestValues, position, moves, checkLegal, checkKCLegal, depth)) return false;

    std::vector<Value> out;
    std::sample(bestValues.begin(), bestValues.end(), std::back_inserter(out), 1, std::mt19937{std::random_device{}()});
    res = out.back().nextMove;

    return true;
}


bool MinMaxMovePicker::pickMove(std::vector<Value> &res, const Position &position, const std::vector<Move> &moves, bool checkLegal, bool checkKCLegal, uint depth)
{

    if(moves.size() == 0) return false;
    /*auto value_pointer = evaluated_res.find(position);

    if(value_pointer != evaluated_res.end() &&  (*(value_pointer->second))[0].depth >= depth)
    {
        //std::cout << std::endl << position.getHash() << std::endl;
        res = std::vector<Value>(*value_pointer->second);
        return true;
    }*/

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
            if(!pickMove(nextvalues, nextPos, nextLegalmoves, checkLegal, checkKCLegal, depth-1)) continue;
            eval = -nextvalues[0].evaluation;
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

/*bool MinMaxMovePicker::bestLine(std::vector<Move> &res, double &eval, const Position &position, uint depth) const
{
    res.clear();

    if (depth==0)
    {
        eval = evaluator->evaluatePosition(position);
        return true;
    }
    else
    {
        std::vector<Move> kCLegalMoves;
        position.getKCLegalMoves(kCLegalMoves);
        uint N = kCLegalMoves.size();
        if (N==0) {return bestLine(res, eval, position, 0);}
        else
        {
           Move move = kCLegalMoves[0];
           Position newPos;
           position.applymo
        }
    }
}*/
