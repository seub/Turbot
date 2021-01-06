#include "movepicker.h"
#include "zobrist.h"

MinMaxMovePicker::MinMaxMovePicker(const Evaluator *evaluator, uint depth) : MovePicker(evaluator), depth(depth)
{
    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {Zobrist::GENERATE_ZOBRIST_NUMBERS();}
}


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

    auto value_pointer = evaluated_res.find(position);
    if(value_pointer != evaluated_res.end() &&  (*(value_pointer->second))[0].depth >= depth)
    {
        //std::cout << std::endl << position.getHash() << std::endl;
        res = std::vector<Value>(*value_pointer->second);
        return true;
    }

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

    evaluated_res[position] = bestvalues;
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

std::string BasicMovePicker::createName() const
{
    std::string res = "TurbotBasic(";
    res += Tools::convertToString(depth);
    res += ")";
    return res;
}


bool BasicMovePicker::pickMove(Move &res, bool &claimDraw, const Position &position)
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
        std::cout << "BasicMovePicker::pickMove failed" << std::endl;
        return false;
    }
}


bool BasicMovePicker::findBestLine(std::vector<Move> &resLine, PositionEval &resEval, bool &claimDraw,
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
        std::cout << "WARNING: BasicMovePicker::findBestLine is not supposed to be called for depth 0!" << std::endl;
        return false;
    }

    LegalMover mover(&position, false);
    std::vector<Move> moves = mover.getKCLegalMoves();
    if (moves.empty())
    {
        std::cout << "WARNING in BasicMovePicker::findBestLine: no KC moves!?" << std::endl;
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
        std::cout << "ForcefulMovePicker::pickMove failed" << std::endl;
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


    LegalMover mover(&position, false);
    std::vector<Move> moves = mover.getKCLegalMoves();
    if (moves.empty())
    {
        std::cout << "WARNING in BaiscMovePicker::findBestLine: no KC moves!?" << std::endl;
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

BasicMovePickerHash::BasicMovePickerHash(const Evaluator *evaluator, uint depth) : MovePicker(evaluator), maxDepth(depth), maxDrawClaimDepth(2)
{
    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {Zobrist::GENERATE_ZOBRIST_NUMBERS();}
}


std::string BasicMovePickerHash::createName() const
{
    std::string res = "TurbotBasicHash(";
    res += Tools::convertToString(maxDepth);
    res += ")";
    return res;
}


bool BasicMovePickerHash::pickMove(Move &res, bool &claimDraw, const Position &position)
{
    nbPositions = 0;
    nbRepetitions = 0;

    std::vector<Move> bestLine;
    PositionEval eval;
    if (findBestLine(bestLine, eval, claimDraw, position, position, false, maxDepth, maxDrawClaimDepth))
    {
        if (!claimDraw) {res = bestLine.front();}
        std::string bestLineString;
        position.printPGN(bestLineString, bestLine);
        if (claimDraw) {std::cout << "[Best line: 'Claim Draw'  |  eval = " << eval << "]" << std::endl;}
        else {std::cout << "[Best line: " << bestLineString << "  |  eval = " << eval << "]" << std::endl;}
        int ratio = Tools::intRound((double (100*nbRepetitions))/nbPositions);
        std::cout << "[nbPositions = " << nbPositions << ", nbRepetitions = " << nbRepetitions << " (" << ratio << "%)]" << std::endl;
        return true;
    }
    else
    {
        std::cout << "BasicMovePicker::pickMove failed" << std::endl;
        return false;
    }
}


bool BasicMovePickerHash::findBestLineNoDrawClaim(std::vector<Move> &resLine, PositionEval &resEval,
                                                  const Position &position, const Position &previousPos, bool previousPosAvailable, uint depth)
{
    ++nbPositions;

    resLine.clear();

    auto it = alreadyEvaluated.find(position);
    if( (it!=alreadyEvaluated.end()) &&  ((it->second).depth >= depth) )
    {
        ++nbRepetitions;
        PositionEval eval(it->second);
        resLine = it->second.bestLine;
        resEval = eval;
        return true;
    }

    LegalMover mover(&position, false);
    std::vector<Move> moves = mover.getKCLegalMoves();
    if (moves.empty())
    {
        std::cout << "WARNING in BasicMovePickerHash::findBestLineNoDrawClaim: no KC moves!?" << std::endl;
        if (mover.isCheck()) {resEval.constructFromMated();}
        else {resEval.constructFromForceDraw();}
        alreadyEvaluated[position] = PositionEvalRich(resEval, resLine, depth);
        return true;
    }
    else if (mover.isOpponentKingCapturable())
    {
        if (!previousPosAvailable)
        {
            std::cout << "WARNING in BasicMovePickerHash::findBestLineNoDrawClaim: Unable to determine stalemate!" << std::endl;
            resEval.constructWhenOpponentKingCanBeCaptured();
            alreadyEvaluated[position] = PositionEvalRich(resEval, resLine, depth);
            return true;
        }
        else
        {
            LegalMover stale(&previousPos, true);
            if (stale.isStalemate()) {resEval.constructFromForceDraw();}
            else {resEval.constructWhenOpponentKingCanBeCaptured();}
            alreadyEvaluated[position] = PositionEvalRich(resEval, resLine, depth);
            return true;
        }
    }
    else
    {
        Position nextPos;
        Move bestMove;
        Color turn = position.getTurn();
        PositionEval eval, bestEval;
        std::vector<Move> opponentLine, opponentBestLine;
        bool first = true;

        for (const auto &move : moves)
        {
            nextPos = mover.applyMove(move);
            if (depth==1)
            {
                opponentLine.clear();
                eval.constructFromDepthZeroEval(evaluator->evaluatePosition(nextPos));
            }
            else if (!findBestLineNoDrawClaim(opponentLine, eval, nextPos, position, true, depth-1)) {return false;}

            if (first)
            {
                bestMove = move;
                bestEval = eval;
                opponentBestLine = opponentLine;
                first = false;
            }
            else if (bestEval.isLessThan(eval, turn, true))
            {
                bestMove = move;
                bestEval = eval;
                opponentBestLine = opponentLine;
            }
        }

        resLine = opponentBestLine;
        resLine.insert(resLine.begin(), bestMove);
        resEval.constructFromEvalAfterBestMovePlayed(bestEval);

        alreadyEvaluated[position] = PositionEvalRich(resEval, resLine, depth);
        return true;
    }
}


bool BasicMovePickerHash::findBestLine(std::vector<Move> &resLine, PositionEval &resEval, bool &claimDraw, const Position &position,
                                       const Position &previousPos, bool previousPosAvailable, uint depth, uint drawClaimDepth)
{
    ++nbPositions;

    resLine.clear();

    if (position.getNbReversibleHalfMoves()>74)
    {
        claimDraw = true;
        resEval.constructFromForceDraw();
        return true;
    }

    if (drawClaimDepth==0) {return findBestLineNoDrawClaim(resLine, resEval, position, previousPos, previousPosAvailable, depth);}
    else
    {

        LegalMover mover(&position, false);
        std::vector<Move> moves = mover.getKCLegalMoves();
        if (moves.empty())
        {
            std::cout << "WARNING in BasicMovePickerHash::findBestLine: no KC moves!?" << std::endl;
            if (mover.isCheck()) {resEval.constructFromMated();}
            else {resEval.constructFromForceDraw();}
            alreadyEvaluated[position] = PositionEvalRich(resEval, resLine, depth);
            return true;
        }
        else if (mover.isOpponentKingCapturable())
        {
            if (!previousPosAvailable)
            {
                std::cout << "WARNING in BasicMovePickerHash::findBestLine: Unable to determine stalemate!" << std::endl;
                resEval.constructWhenOpponentKingCanBeCaptured();
                alreadyEvaluated[position] = PositionEvalRich(resEval, resLine, depth);
                return true;
            }
            else
            {
                LegalMover stale(&previousPos, true);
                if (stale.isStalemate()) {resEval.constructFromForceDraw();}
                else {resEval.constructWhenOpponentKingCanBeCaptured();}
                alreadyEvaluated[position] = PositionEvalRich(resEval, resLine, depth);
                return true;
            }
        }
        else
        {
            Position nextPos;
            Move bestMove;
            Color turn = position.getTurn();
            PositionEval eval, bestEval;
            std::vector<Move> opponentLine, opponentBestLine;
            bool first;

            if (position.drawCanBeClaimed())
            {
                claimDraw = true;
                opponentBestLine.clear();
                bestEval.constructFromForceDraw();
                first = false;
            }
            else
            {
                claimDraw = false;
                first = true;
            }

            for (const auto &move : moves)
            {
                nextPos = mover.applyMove(move);
                if (depth==1)
                {
                    opponentLine.clear();
                    eval.constructFromDepthZeroEval(evaluator->evaluatePosition(nextPos));
                }
                else
                {
                    bool opponentClaimDraw;
                    if (!findBestLine(opponentLine, eval, opponentClaimDraw, nextPos, position, true, depth-1, drawClaimDepth-1)) {return false;}
                }

                if (first)
                {
                    claimDraw = false;
                    bestMove = move;
                    bestEval = eval;
                    opponentBestLine = opponentLine;
                    first = false;
                }
                else if (bestEval.isLessThan(eval, turn, true))
                {
                    claimDraw = false;
                    bestMove = move;
                    bestEval = eval;
                    opponentBestLine = opponentLine;
                }
            }


            if (claimDraw)
            {
                resLine.clear();
                resEval.constructFromForceDraw();
            }
            else
            {
                resLine = opponentBestLine;
                resLine.insert(resLine.begin(), bestMove);
                resEval.constructFromEvalAfterBestMovePlayed(bestEval);
            }

            return true;
        }
    }
}
