#include "movepicker.h"

#include <iostream>

bool MinMaxMovePicker::pickMove(Move &res, const Position &position, const std::vector<Move> &moves, bool checkLegal, bool checkKCLegal)
{
    std::vector<Value> bestValues;
    if(!pickMove(bestValues,position, moves, checkLegal, checkKCLegal,depth)) return false;
    
    std::vector<Value> out;
    std::sample(bestValues.begin(), bestValues.end(), std::back_inserter(out), 1, std::mt19937{std::random_device{}()});
    res = out.back().nextmove;

    return true;
}


bool MinMaxMovePicker::pickMove(std::vector<Value> &res, const Position &position, const std::vector<Move> &moves, bool checkLegal, bool checkKCLegal, uint depth)
{

    if(moves.size() == 0) return false;
    auto value_pointer = evaluated_res.find(position);

    if(value_pointer != evaluated_res.end() &&  (*(value_pointer->second))[0].depth >= depth)
    {
        std::cout << std::endl << position.getHash() << std::endl;
        res = std::vector<Value>(*value_pointer->second);
        return true;
    }

    int reverse = position.getTurn() == Color::BLACK ? 1: -1;
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
    evaluated_res[position] = bestvalues;
    res = std::vector<Value>(*bestvalues);
    return res.size()>0;
}


