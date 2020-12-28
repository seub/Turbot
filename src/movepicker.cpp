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
