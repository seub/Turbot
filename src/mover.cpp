#include "mover.h"

Mover::Mover(const Position * const position, bool generateLegalMoves) : position(position), boardHelper(&(position->board))
{
    initialize();
    if (generateLegalMoves) updateLegalMoves();

}

bool Mover::initialize()
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


bool Mover::updatekcLegalMoves()
{
    kCLegalMoves.clear();

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

void Mover::addkcLegalMove(uint origin, uint target, bool attack)
{
    kCLegalMoves.push_back(Move(origin, target));
    if (attack) squaresAttacked[target] = true;
}

void Mover::addkcLegalMovesKing(uint i)
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

void Mover::addkcLegalMovesQueen(uint i)
{
    addkcLegalMovesRook(i);
    addkcLegalMovesBishop(i);
}

void Mover::addkcLegalMovesRook(uint i)
{
    std::vector<uint> row = {};
    uint filei = i % 8;
    uint ranki = i / 8;

    uint j = i;
    uint filej = filei;
    uint rankj = ranki;
    while(filej!=0)
    {
        --j;
        --filej;
        row.push_back(j);
    }
    addkcLegalMovesRow(i, row);

    j = i;
    filej = filei;
    row.clear();
    while(filej!=7)
    {
        ++j;
        ++filej;
        row.push_back(j);
    }
    addkcLegalMovesRow(i, row);

    j = i;
    row.clear();
    while(rankj!=0)
    {
        j -= 8;
        --rankj;
        row.push_back(j);
    }
    addkcLegalMovesRow(i, row);

    j = i;
    rankj=ranki;
    row.clear();
    while(rankj!=7)
    {
        j += 8;
        ++rankj;
        row.push_back(j);
    }
    addkcLegalMovesRow(i, row);
}

void Mover::addkcLegalMovesBishop(uint i)
{
    std::vector<uint> diag = {};
    uint filei = i % 8;
    uint ranki = i / 8;

    uint j = i;
    uint filej = filei;
    uint rankj = ranki;
    while((filej!=0) && (rankj!=0))
    {
        j -= 9;
        --filej;
        --rankj;
        diag.push_back(j);
    }
    addkcLegalMovesRow(i, diag);

    j = i;
    filej = filei;
    rankj = ranki;
    diag.clear();
    while((filej!=7) && (rankj!=0))
    {
        j -= 7;
        ++filej;
        --rankj;
        diag.push_back(j);
    }
    addkcLegalMovesRow(i, diag);

    j = i;
    filej = filei;
    rankj = ranki;
    diag.clear();
    while((filej!=0) && (rankj!=7))
    {
        j += 7;
        --filej;
        ++rankj;
        diag.push_back(j);
    }
    addkcLegalMovesRow(i, diag);


    j = i;
    filej = filei;
    rankj = ranki;
    diag.clear();
    while((filej!=7) && (rankj!=7))
    {
        j += 9;
        ++filej;
        ++rankj;
        diag.push_back(j);
    }
    addkcLegalMovesRow(i, diag);
}

void Mover::addkcLegalMovesRow(uint i, const std::vector<uint> row)
{
    const bitboard *myPieces = (position->turn==Color::White) ? &(boardHelper.whitePieces) : &(boardHelper.blackPieces);

    uint k=0, target;
    while (k<row.size())
    {
        target = row[k];
        if (!(myPieces->at(target))) addkcLegalMove(i, target);
        if (boardHelper.occupiedSquares[target]) break;
        ++k;
    }
}

void Mover::addkcLegalMovesKnight(uint i)
{
    uint file = i % 8;
    uint rank = i / 8;

    const bitboard *myPieces = (position->turn==Color::White) ? &(boardHelper.whitePieces) : &(boardHelper.blackPieces);

    if ((rank>1) && (file>0))
    {
        if (!myPieces->at(i-17)) addkcLegalMove(i, i-17);
    }
    if ((rank>1) && (file<7))
    {
        if (!myPieces->at(i-15)) addkcLegalMove(i, i-15);
    }
    if ((rank>0) && (file<6))
    {
        if (!myPieces->at(i-6)) addkcLegalMove(i, i-6);
    }
    if ((rank<7) && (file<6))
    {
        if (!myPieces->at(i+10)) addkcLegalMove(i, i+10);
    }
    if ((rank<6) && (file<7))
    {
        if (!myPieces->at(i+17)) addkcLegalMove(i, i+17);
    }
    if ((rank<6) && (file>0))
    {
        if (!myPieces->at(i+15)) addkcLegalMove(i, i+15);
    }
    if ((rank<7) && (file>1))
    {
        if (!myPieces->at(i+6)) addkcLegalMove(i, i+6);
    }
    if ((rank>0) && (file>1))
    {
        if (!myPieces->at(i-10)) addkcLegalMove(i, i-10);
    }
}


void Mover::addkcLegalMovesPawn(uint i)
{
    uint file = i % 8;
    uint rank = i / 8;
    uint target;

    if (position->turn==Color::White)
    {
        if ((rank==0) || (rank==7)) throw("Pawn is on first/last row!?");
        else if (rank==1)
        {
            if (!boardHelper.occupiedSquares[i+8])
            {
                addkcLegalMove(i, i+8, false);
                if (!boardHelper.occupiedSquares[i+16]) addkcLegalMove(i, i+16, false);
            }
        }
        else
        {
            if (!boardHelper.occupiedSquares[i+8]) addkcLegalMove(i, i+8, false);
        }

        if (file>0)
        {
            target = i+7;
            if ((boardHelper.blackPieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target))) addkcLegalMove(i, target);
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;

            //NB: We put by convention that if en passant is possible, the e.p. target square is "under attack", but it does not matter
        }
        if (file<7)
        {
            target = i+9;
            if ((boardHelper.blackPieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target))) addkcLegalMove(i, target);
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;
        }
    }
    else
    {
        if ((rank==0) || (rank==7)) throw("Pawn is on first/last row!?");
        else if (rank==6)
        {
            if (!boardHelper.occupiedSquares[i-8])
            {
                addkcLegalMove(i, i-8, false);
                if (!boardHelper.occupiedSquares[i-16]) addkcLegalMove(i, i-16, false);
            }
        }
        else
        {
            if (!boardHelper.occupiedSquares[i-8]) addkcLegalMove(i, i-8, false);
        }

        if (file>0)
        {
            target = i-9;
            if ((boardHelper.blackPieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target))) addkcLegalMove(i, target);
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;
        }
        if (file<7)
        {
            target = i-7;
            if ((boardHelper.blackPieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target))) addkcLegalMove(i, target);
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;
        }
    }
}

void Mover::updateLegalMoves()
{

}

std::string Mover::printKCMoves() const
{
    std::string res = {};
    for (auto move : kCLegalMoves)
    {
        res += move.name();
        res += ", ";
    }
    res.erase (res.end()-2, res.end());

    return res;
}

bool Mover::isCapture(const Move &m) const
{
    return boardHelper.occupiedSquares[m.target.getIndex()];
}

bool Mover::isPawnMove(const Move &m) const
{
    return boardHelper.pawns[m.origin.getIndex()];
}

bool Mover::isReversible(const Move &m) const
{
    return (!(isCapture(m)) && !(isReversible(m)));
}

Position Mover::applyKCMove(const Move &m) const
{
    Position res(*position);

    if (m.resign || m.claimDraw) throw("Move cannot be applied");
    else
    {
        res.turn = (position->turn==Color::White) ? Color::Black : Color::White;
        res.drawOffered = m.offerDraw;
        if (res.turn==Color::White) res.moveNumber++;
    }
}
