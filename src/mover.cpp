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

    bool res = updateKCLegalMoves();

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


bool Mover::updateKCLegalMoves()
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

void Mover::addkcLegalMovePromotions(uint origin, uint target, bool attack)
{
    kCLegalMoves.push_back(Move(origin, target, true, PieceType::Queen));
    kCLegalMoves.push_back(Move(origin, target, true, PieceType::Rook));
    kCLegalMoves.push_back(Move(origin, target, true, PieceType::Bishop));
    kCLegalMoves.push_back(Move(origin, target, true, PieceType::Knight));
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
        else if (rank==6)
        {
            if (!boardHelper.occupiedSquares[i+8]) addkcLegalMovePromotions(i, i+8);
        }
        else
        {
            if (!boardHelper.occupiedSquares[i+8]) addkcLegalMove(i, i+8, false);
        }

        if (file>0)
        {
            target = i+7;
            if ((boardHelper.blackPieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target)))
            {
                if (rank==6) addkcLegalMovePromotions(i,target,true);
                else addkcLegalMove(i, target);
            }
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;

            //NB: We put by convention that if en passant is theoretically, but not actually, possible, the e.p. target square is "under attack", but it does not matter
        }
        if (file<7)
        {
            target = i+9;
            if ((boardHelper.blackPieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target)))
            {
                if (rank==6) addkcLegalMovePromotions(i,target, true);
                else addkcLegalMove(i, target);
            }
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
        else if (rank==1)
        {
            if (!boardHelper.occupiedSquares[i-8]) addkcLegalMovePromotions(i, i-8);
        }
        else
        {
            if (!boardHelper.occupiedSquares[i-8]) addkcLegalMove(i, i-8, false);
        }

        if (file>0)
        {
            target = i-9;
            if ((boardHelper.whitePieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target)))
            {
                if (rank==1) addkcLegalMovePromotions(i,target,true);
                else addkcLegalMove(i, target);
            }
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;
        }
        if (file<7)
        {
            target = i-7;
            if ((boardHelper.whitePieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target)))
            {
                if (rank==1) addkcLegalMovePromotions(i,target,true);
                else addkcLegalMove(i, target);
            }
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;
        }
    }
}

void Mover::updateLegalMoves()
{
    legalMoves.clear();
    for (const auto & move : kCLegalMoves)
    {
        if (isLegalConstruct(move, false)) legalMoves.push_back(move);
    }
}

std::string Mover::printKCLegalMoves() const
{
    std::string res = {};
    for (auto move : kCLegalMoves)
    {
        res += move.longAlgebraicNotation();
        res += ", ";
    }
    if (!kCLegalMoves.empty()) res.erase (res.end()-2, res.end());

    return res;
}

std::string Mover::printLegalMoves() const
{
    std::string res = {};
    for (auto move : legalMoves)
    {
        res += move.longAlgebraicNotation();
        res += ", ";
    }
    if (!legalMoves.empty()) res.erase (res.end()-2, res.end());
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
    return (!(isCapture(m)) && !(isPawnMove(m)));
}

Position Mover::applyKCMove(const Move &m) const
{
    Position res(*position);


    /*if (m.resign || m.claimDraw)
    {
        throw("Move cannot be applied");
    }*/
    // The code above is not quite right: I should only exclude the case where a resignation occurs or a draw is claimed BEFORE playing the move

    bool white = (position->turn==Color::White);
    res.turn = white ? Color::Black : Color::White;
    res.drawOffered = m.offerDraw;

    if (res.turn==Color::White) res.moveNumber++;
    if (isReversible(m)) res.nbReversibleHalfMovesPlayed++;
    else res.nbReversibleHalfMovesPlayed = 0;
    uint origin = m.origin.getIndex();
    uint target = m.target.getIndex();
    Piece p = position->board.pieces[origin];

    res.board.pieces[origin] = Piece();
    if (m.promotion) res.board.pieces[target] = Piece(m.promotedPiece, white ? Color::White : Color::Black);
    else res.board.pieces[target] = p;


    // En passant move
    if (p.isPawn() && position->enPassantPossible && (target==position->enPassantTargetSquare))
    {
        uint opPawn = (position->turn==Color::White) ? target+8 : target-8;
        res.board.pieces[opPawn] = Piece();
    }

    // Pawn move allowing en passant or not
    if (p.isPawn() && white && (target==origin+16))
    {
        res.enPassantPossible = true;
        res.enPassantTargetSquare = origin+8;
    }
    else if (p.isPawn() && (!white) && (origin==target+16))
    {
        res.enPassantPossible = true;
        res.enPassantTargetSquare = target+8;
    }
    else
    {
        res.enPassantPossible = false;
        res.enPassantTargetSquare = 0;
    }

    //Castling and losing castling rights with King move
    if (p.isKing())
    {
        if (target==origin-2)
        {
            res.board.pieces[target+1] = res.board.pieces[target-4];
            res.board.pieces[target-4] = Piece();
        }
        else if (target==origin+2)
        {
            res.board.pieces[target-1] = res.board.pieces[target+3];
            res.board.pieces[target+3] = Piece();
        }

        if (white)
        {
            res.castlingRights[0] = false;
            res.castlingRights[1] = false;
        }
        else
        {
            res.castlingRights[2] = false;
            res.castlingRights[3] = false;
        }
    }

    //Losing castling rights with Rook move
    if (p.isRook())
    {
        if (origin==7) res.castlingRights[0] = false;
        if (origin==0) res.castlingRights[1] = false;
        if (origin==63) res.castlingRights[2] = false;
        if (origin==56) res.castlingRights[3] = false;
    }

    return res;
}

bool Mover::checkIsLegalList(const Move &m) const
{
    return (std::find(legalMoves.begin(), legalMoves.end(), m) != legalMoves.end());
}

bool Mover::isLegalConstruct(const Move &m, bool checkKClegal) //NB: Legal move = KC legal + king cannot be captured next move (+ including while castling)
{
    if (checkKClegal)
    {
        if  (!(std::find(kCLegalMoves.begin(), kCLegalMoves.end(), m) != kCLegalMoves.end())) return false;
    }

    Position newPos = applyKCMove(m);
    Mover newMover(&newPos, false);

    if (newMover.opponentKingUnderAttack) return false;

    uint origin = m.origin.getIndex();
    uint target = m.target.getIndex();
    Piece p = position->board.pieces[origin];
    if (p.isKing())
    {
        if (target==origin-2)
        {
            if (newMover.opponentKsideCastleUnderAttack) return false;
        }
        else if (target==origin+2)
        {
            if (newMover.opponentQsideCastleUnderAttack) return false;
        }
    }

    return true;
}
