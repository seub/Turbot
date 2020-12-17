#include "legalmover.h"

LegalMover::LegalMover(const Position * const position, bool generateLegalMoves) : position(position), boardHelper(&(position->board))
{
    initialize();
    if (generateLegalMoves) updateLegalMoves();
}

bool LegalMover::initialize()
{
    squaresAttacked.fill(false);
    legalMoves.clear();

    bool res = updatekcLegalMoves();

    const bitboard *opPieces = (position->turn==Color::White) ? &(boardHelper.blackPieces) : &(boardHelper.whitePieces);
    uint i = 0;
    while(i<64)
    {
        if (boardHelper.kings[i] && opPieces->at(i)) break;
        ++i;
    }
    opponentKingUnderAttack = squaresAttacked[i];

    if (position->turn==Color::White)
    {
        opponentKsideCastleUnderAttack = (squaresAttacked[60] || squaresAttacked[61] || squaresAttacked[62]);
        opponentQsideCastleUnderAttack = (squaresAttacked[58] || squaresAttacked[59] || squaresAttacked[60]);
    }
    else
    {
        opponentKsideCastleUnderAttack = (squaresAttacked[4] || squaresAttacked[5] || squaresAttacked[6]);
        opponentQsideCastleUnderAttack = (squaresAttacked[2] || squaresAttacked[3] || squaresAttacked[4]);
    }
    return res;
}


bool LegalMover::updatekcLegalMoves()
{
    kcLegalMoves.clear();

    const bitboard *myPieces = (position->turn==Color::White) ? &(boardHelper.whitePieces) : &(boardHelper.blackPieces);

    for (uint i=0; i!=64; ++i)
    {
        if (myPieces->at(i))
        {
            if (boardHelper.kings[i]) addkcLegalMovesKing(i);
            else if (boardHelper.queens[i]) addkcLegalMovesQueen(i);
            else if (boardHelper.rooks[i]) addkcLegalMovesRook(i);
            else if (boardHelper.bishops[i]) addkcLegalMovesBishop(i);
            else if (boardHelper.knights[i]) addkcLegalMovesKnight(i);
            else if (boardHelper.pawns[i]) addkcLegalMovesPawn(i);
            else
            {
                throw("Error in LegalMover::initialize()");
                return false;
            }
        }
    }
    return true;
}

void LegalMover::addkcLegalMove(uint origin, uint target, bool attack)
{
    kcLegalMoves.push_back(Move(origin, target));
    if (attack) squaresAttacked[target] = true;
}

void LegalMover::addkcLegalMovesKing(uint i)
{
    uint file = i % 8;
    uint rank = i / 8;

    const bitboard *myPieces = (position->turn==Color::White) ? &(boardHelper.whitePieces) : &(boardHelper.blackPieces);

    if (rank==0)
    {
        if (file==0)
        {
            if (!myPieces->at(1)) addkcLegalMove(i, 1);
            if (!myPieces->at(8)) addkcLegalMove(i, 8);
            if (!myPieces->at(9)) addkcLegalMove(i, 9);
        }
        else if (file==7)
        {
            if (!myPieces->at(6)) addkcLegalMove(i, 6);
            if (!myPieces->at(14)) addkcLegalMove(i, 14);
            if (!myPieces->at(15)) addkcLegalMove(i, 15);
        }
        else
        {
            if (!myPieces->at(i-1)) addkcLegalMove(i, i-1);
            if (!myPieces->at(i+1)) addkcLegalMove(i, i+1);
            if (!myPieces->at(i+7)) addkcLegalMove(i, i+7);
            if (!myPieces->at(i+8)) addkcLegalMove(i, i+8);
            if (!myPieces->at(i+9)) addkcLegalMove(i, i+9);
        }
    }
    else if (rank==7)
    {
        if (file==0)
        {
            if (!myPieces->at(48)) addkcLegalMove(i, 48);
            if (!myPieces->at(49)) addkcLegalMove(i, 49);
            if (!myPieces->at(56)) addkcLegalMove(i, 56);
        }
        else if (file==7)
        {
            if (!myPieces->at(54)) addkcLegalMove(i, 54);
            if (!myPieces->at(55)) addkcLegalMove(i, 55);
            if (!myPieces->at(62)) addkcLegalMove(i, 62);
        }
        else
        {
            if (!myPieces->at(i-9)) addkcLegalMove(i, i-9);
            if (!myPieces->at(i-8)) addkcLegalMove(i, i-8);
            if (!myPieces->at(i-7)) addkcLegalMove(i, i-7);
            if (!myPieces->at(i-1)) addkcLegalMove(i, i-1);
            if (!myPieces->at(i+1)) addkcLegalMove(i, i+1);
        }
    }
    else
    {
        if (file==0)
        {
            if (!myPieces->at(i-8)) addkcLegalMove(i, i-8);
            if (!myPieces->at(i-7)) addkcLegalMove(i, i-7);
            if (!myPieces->at(i+1)) addkcLegalMove(i, i+1);
            if (!myPieces->at(i+8)) addkcLegalMove(i, i+8);
            if (!myPieces->at(i+9)) addkcLegalMove(i, i+9);
        }
        else if (file==7)
        {
            if (!myPieces->at(i-9)) addkcLegalMove(i, i-9);
            if (!myPieces->at(i-8)) addkcLegalMove(i, i-8);
            if (!myPieces->at(i-1)) addkcLegalMove(i, i-1);
            if (!myPieces->at(i+7)) addkcLegalMove(i, i+7);
            if (!myPieces->at(i+8)) addkcLegalMove(i, i+8);
        }
        else
        {
            if (!myPieces->at(i-9)) addkcLegalMove(i, i-9);
            if (!myPieces->at(i-8)) addkcLegalMove(i, i-8);
            if (!myPieces->at(i-7)) addkcLegalMove(i, i-7);
            if (!myPieces->at(i-1)) addkcLegalMove(i, i-1);
            if (!myPieces->at(i+1)) addkcLegalMove(i, i+1);
            if (!myPieces->at(i+7)) addkcLegalMove(i, i+7);
            if (!myPieces->at(i+8)) addkcLegalMove(i, i+8);
            if (!myPieces->at(i+9)) addkcLegalMove(i, i+9);
        }
    }

    if (position->turn==Color::White)
    {
        if (position->castlingRights[0])
        {
            if ((!boardHelper.occupiedSquares[5]) && (!boardHelper.occupiedSquares[6])) addkcLegalMove(4, 6, false);
        }
        else if (position->castlingRights[1])
        {
            if ((!boardHelper.occupiedSquares[2]) && (!boardHelper.occupiedSquares[3])) addkcLegalMove(4, 2, false);
        }
    }
    else
    {
        if (position->castlingRights[2])
        {
            if ((!boardHelper.occupiedSquares[61]) && (!boardHelper.occupiedSquares[62])) addkcLegalMove(60, 62, false);
        }
        else if (position->castlingRights[3])
        {
            if ((!boardHelper.occupiedSquares[58]) && (!boardHelper.occupiedSquares[59])) addkcLegalMove(60, 58, false);
        }
    }
}

void LegalMover::addkcLegalMovesQueen(uint i)
{
    addkcLegalMovesRook(i);
    addkcLegalMovesBishop(i);
}

void LegalMover::addkcLegalMovesRook(uint i)
{

}

void LegalMover::addkcLegalMovesBishop(uint i)
{

}


void LegalMover::addkcLegalMovesKnight(uint i)
{

}


void LegalMover::addkcLegalMovesPawn(uint i)
{

}

void LegalMover::updateLegalMoves()
{

}
