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

        Position nextPos(false);
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
            nextPos = mover.applyMove(move);
            if (depth==1) {eval.constructFromDepthZeroEval(evaluator->evaluatePosition(nextPos));}
            else
            {
                bool opponentClaimDraw;
                findBestLine(line, eval, opponentClaimDraw, nextPos, position, true, depth-1);
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
        Position nextPos(false);
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




































std::string BasicMovePickerZ::createName() const
{
    std::string res = "TurbotBasicZ(";
    res += Tools::convertToString(maxDepth);
    res += ")";
    return res;
}


bool BasicMovePickerZ::pickMove(MoveZ &res, bool &claimDraw, const PositionZ &position)
{
    std::vector<MoveZ> bestLine;
    PositionEvalZ eval;
    if (findBestLine(bestLine, eval, claimDraw, position, position, false, maxDepth))
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
        std::cout << "BasicMovePickerZ::pickMove failed" << std::endl;
        return false;
    }
}


bool BasicMovePickerZ::findBestLine(std::vector<MoveZ> &resLine, PositionEvalZ &resEval, bool &claimDraw,
                                   const PositionZ &position, const PositionZ &previousPos, bool previousPosAvailable, uint depth) const
{
    resLine.clear();

    if (position.getNbReversiblePlies()>74)
    {
        claimDraw = true;
        resEval.constructFromForceDraw();
        return true;
    }

    if (depth==0)
    {
        std::cout << "WARNING: BasicMovePickerZ::findBestLine is not supposed to be called for depth 0!" << std::endl;
        return false;
    }

    LegalMoverZ mover(&position, false);
    std::vector<MoveZ> moves = mover.getPseudoLegalMoves();
    if (moves.empty())
    {
        std::cout << "WARNING in BasicMovePickerZ::findBestLine: no pseudo-legal moves!?" << std::endl;
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
            LegalMoverZ stale(&previousPos, true);
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

        PositionZ nextPos(false);
        MoveZ bestMove;
        bool turn = position.getTurn();
        PositionEvalZ eval, bestEval;
        std::vector<MoveZ> line, bestLine;
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
            mover.applyMove(move, nextPos, true);
            if (depth==1) {eval.constructFromDepthZeroEval(evaluator->evaluatePosition(nextPos));}
            else
            {
                bool opponentClaimDraw;
                findBestLine(line, eval, opponentClaimDraw, nextPos, position, true, depth-1);
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



BasicMovePickerHashZ::BasicMovePickerHashZ(const EvaluatorZ *evaluator, uint depth) : MovePickerZ(evaluator), maxDepth(depth)
{
    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {Zobrist::GENERATE_ZOBRIST_NUMBERS();}
}


std::string BasicMovePickerHashZ::createName() const
{
    std::string res = "TurbotBasicHashZ(";
    res += Tools::convertToString(maxDepth);
    res += ")";
    return res;
}


bool BasicMovePickerHashZ::pickMove(MoveZ &res, bool &claimDraw, const PositionZ &position)
{
    nbPositions = 0;
    nbRepetitions = 0;

    std::vector<MoveZ> bestLine;
    PositionEvalZ eval;
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
        std::cout << "BasicMovePickerHashZ::pickMove failed" << std::endl;
        return false;
    }
}


bool BasicMovePickerHashZ::findBestLine(std::vector<MoveZ> &resLine, PositionEvalZ &resEval, bool &claimDraw,
                                       const PositionZ &position, const PositionZ &previousPos, bool previousPosAvailable, uint depth)
{
    ++nbPositions;

    resLine.clear();

    auto it = alreadyEvaluated.find(position);
    if( (it!=alreadyEvaluated.end()) &&  ((it->second).depth >= depth) )
    {
        ++nbRepetitions;
        resLine = it->second.bestLine;
        resEval = it->second;
        return true;
    }

    LegalMoverZ mover(&position, false);
    std::vector<MoveZ> moves = mover.getPseudoLegalMoves();
    if (moves.empty())
    {
        std::cout << "WARNING in BasicMovePickerHash::findBestLine: no KC moves!?" << std::endl;
        if (mover.isCheck()) {resEval.constructFromMated();}
        else {resEval.constructFromForceDraw();}
        alreadyEvaluated[position] = PositionEvalRichZ(resEval, resLine, depth);
        return true;
    }
    else if (mover.isOpponentKingCapturable())
    {
        if (!previousPosAvailable)
        {
            std::cout << "WARNING in BasicMovePickerHash::findBestLine: Unable to determine stalemate!" << std::endl;
            resEval.constructWhenOpponentKingCanBeCaptured();
            alreadyEvaluated[position] = PositionEvalRichZ(resEval, resLine, depth);
            return true;
        }
        else
        {
            LegalMoverZ stale(&previousPos, true);
            if (stale.isStalemate()) {resEval.constructFromForceDraw();}
            else {resEval.constructWhenOpponentKingCanBeCaptured();}
            alreadyEvaluated[position] = PositionEvalRichZ(resEval, resLine, depth);
            return true;
        }
    }
    else
    {
        PositionZ nextPos(false);
        MoveZ bestMove;
        bool turn = position.getTurn();
        PositionEvalZ eval, bestEval;
        std::vector<MoveZ> opponentLine, opponentBestLine;
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
            mover.applyMove(move, nextPos, true);
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

        alreadyEvaluated[position] = PositionEvalRichZ(resEval, resLine, depth);
        return true;
    }
}
