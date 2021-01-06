#include "movepicker.h"
#include "zobrist.h"




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

        if (position.getDrawClaimable())
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

    LegalMover mover(&position, false);
    std::vector<Move> moves = mover.getKCLegalMoves();
    if (moves.empty())
    {
        std::cout << "WARNING in ForcefulMovePicker::findBestLine: no KC moves!?" << std::endl;
        if (mover.isCheck()) {resEval.constructFromMated();}
        else {resEval.constructFromForceDraw();}
        return true;
    }
    else if (mover.isOpponentKingCapturable())
    {
        if (!previousPosAvailable)
        {
            std::cout << "WARNING in ForcefulMovePicker::findBestLine: Unable to determine stalemate!" << std::endl;
            resEval.constructWhenOpponentKingCanBeCaptured();
            return true;
        }
        else
        {
            LegalMover stale(&previousPos, true);
            if (stale.isStalemate()) {resEval.constructFromForceDraw();}
            else {resEval.constructWhenOpponentKingCanBeCaptured();}
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
        bool first, opponentClaimDraw;

        if (position.getDrawClaimable())
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
            if (mover.isCapture(move) && ((depth!=1 && fdepth!=0) || (depth==1 && gdepth!=0)))
            {
                if (depth==1) {if (!findBestLine(opponentLine, eval, opponentClaimDraw, nextPos, position, true, depth, fdepth, gdepth-1)) {return false;}}
                else {if (!findBestLine(opponentLine, eval, opponentClaimDraw, nextPos, position, true, depth, fdepth-1, gdepth)) {return false;}}
            }
            else if (depth==1) {eval.constructFromDepthZeroEval(evaluator->evaluatePosition(nextPos));}
            else {if (!findBestLine(opponentLine, eval, opponentClaimDraw, nextPos, position, true, depth-1, fdepth, this->gdepth)) {return false;}}

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













BasicMovePickerHash::BasicMovePickerHash(const Evaluator *evaluator, uint depth) : MovePicker(evaluator), maxDepth(depth)
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
    if (findBestLine(bestLine, eval, claimDraw, position, position, false, maxDepth))
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


bool BasicMovePickerHash::findBestLine(std::vector<Move> &resLine, PositionEval &resEval, bool &claimDraw,
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
        bool first, opponentClaimDraw;

        if (position.getDrawClaimable())
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
            else if (!findBestLine(opponentLine, eval, opponentClaimDraw, nextPos, position, true, depth-1)) {return false;}

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

        alreadyEvaluated[position] = PositionEvalRich(resEval, resLine, depth);
        return true;
    }
}
