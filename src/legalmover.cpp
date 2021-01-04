#include "legalmover.h"
#include "position.h"

LegalMover::LegalMover(const Position * const position, bool generateLegalMoves) : position(position), boardHelper(&(position->board))
{
    initialize();
    if (generateLegalMoves) {updateLegalMoves();}
    legalMovesGenerated = generateLegalMoves;
}

bool LegalMover::initialize()
{

    squaresAttacked.fill(false);
    legalMoves.clear();

    bool res = updateKCLegalMoves();
    kCLegalMovesGenerated = res;

    const bitboard *opPieces = (position->turn==Color::WHITE) ? &(boardHelper.blackPieces) : &(boardHelper.whitePieces);
    uint i = 0;
    while(i<64)
    {
        if (boardHelper.kings[i] && opPieces->at(i)) break;
        ++i;
    }
    opponentKingUnderAttack = squaresAttacked[i];

    if (position->turn==Color::WHITE)
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


bool LegalMover::updateKCLegalMoves()
{
    kCLegalMoves.clear();

    const bitboard *myPieces = (position->turn==Color::WHITE) ? &(boardHelper.whitePieces) : &(boardHelper.blackPieces);


    for (uint i=0; i!=64; ++i)
    {
        if (myPieces->at(i))
        {
            if (boardHelper.kings[i]) {addKCLegalMovesKing(i);}
            else if (boardHelper.queens[i]) {addKCLegalMovesQueen(i);}
            else if (boardHelper.rooks[i]) {addKCLegalMovesRook(i);}
            else if (boardHelper.bishops[i]) {addKCLegalMovesBishop(i);}
            else if (boardHelper.knights[i]) {addKCLegalMovesKnight(i);}
            else if (boardHelper.pawns[i]) {addKCLegalMovesPawn(i);}
            else
            {
                throw("Error in LegalMover::initialize()");
                return false;
            }
        }
    }
    return true;
}

void LegalMover::addKCLegalMove(uint origin, uint target, bool attack)
{
    kCLegalMoves.push_back(Move(origin, target));
    if (attack) squaresAttacked[target] = true;
}

void LegalMover::addKCLegalMovePromotions(uint origin, uint target, bool attack)
{
    kCLegalMoves.push_back(Move(origin, target, true, PieceType::QUEEN));
    kCLegalMoves.push_back(Move(origin, target, true, PieceType::ROOK));
    kCLegalMoves.push_back(Move(origin, target, true, PieceType::BISHOP));
    kCLegalMoves.push_back(Move(origin, target, true, PieceType::KNIGHT));
    if (attack) squaresAttacked[target] = true;
}

void LegalMover::addKCLegalMovesKing(uint i)
{

    uint file = i % 8;
    uint rank = i / 8;

    const bitboard *myPieces = (position->turn==Color::WHITE) ? &(boardHelper.whitePieces) : &(boardHelper.blackPieces);

    if (rank==0)
    {
        if (file==0)
        {
            if (!myPieces->at(1)) addKCLegalMove(i, 1);
            if (!myPieces->at(8)) addKCLegalMove(i, 8);
            if (!myPieces->at(9)) addKCLegalMove(i, 9);
        }
        else if (file==7)
        {
            if (!myPieces->at(6)) addKCLegalMove(i, 6);
            if (!myPieces->at(14)) addKCLegalMove(i, 14);
            if (!myPieces->at(15)) addKCLegalMove(i, 15);
        }
        else
        {
            if (!myPieces->at(i-1)) addKCLegalMove(i, i-1);
            if (!myPieces->at(i+1)) addKCLegalMove(i, i+1);
            if (!myPieces->at(i+7)) addKCLegalMove(i, i+7);
            if (!myPieces->at(i+8)) addKCLegalMove(i, i+8);
            if (!myPieces->at(i+9)) addKCLegalMove(i, i+9);
        }
    }
    else if (rank==7)
    {
        if (file==0)
        {
            if (!myPieces->at(48)) addKCLegalMove(i, 48);
            if (!myPieces->at(49)) addKCLegalMove(i, 49);
            if (!myPieces->at(57)) addKCLegalMove(i, 57);
        }
        else if (file==7)
        {
            if (!myPieces->at(54)) addKCLegalMove(i, 54);
            if (!myPieces->at(55)) addKCLegalMove(i, 55);
            if (!myPieces->at(62)) addKCLegalMove(i, 62);
        }
        else
        {
            if (!myPieces->at(i-9)) addKCLegalMove(i, i-9);
            if (!myPieces->at(i-8)) addKCLegalMove(i, i-8);
            if (!myPieces->at(i-7)) addKCLegalMove(i, i-7);
            if (!myPieces->at(i-1)) addKCLegalMove(i, i-1);
            if (!myPieces->at(i+1)) addKCLegalMove(i, i+1);
        }
    }
    else
    {
        if (file==0)
        {
            if (!myPieces->at(i-8)) addKCLegalMove(i, i-8);
            if (!myPieces->at(i-7)) addKCLegalMove(i, i-7);
            if (!myPieces->at(i+1)) addKCLegalMove(i, i+1);
            if (!myPieces->at(i+8)) addKCLegalMove(i, i+8);
            if (!myPieces->at(i+9)) addKCLegalMove(i, i+9);
        }
        else if (file==7)
        {
            if (!myPieces->at(i-9)) addKCLegalMove(i, i-9);
            if (!myPieces->at(i-8)) addKCLegalMove(i, i-8);
            if (!myPieces->at(i-1)) addKCLegalMove(i, i-1);
            if (!myPieces->at(i+7)) addKCLegalMove(i, i+7);
            if (!myPieces->at(i+8)) addKCLegalMove(i, i+8);
        }
        else
        {
            if (!myPieces->at(i-9)) addKCLegalMove(i, i-9);
            if (!myPieces->at(i-8)) addKCLegalMove(i, i-8);
            if (!myPieces->at(i-7)) addKCLegalMove(i, i-7);
            if (!myPieces->at(i-1)) addKCLegalMove(i, i-1);
            if (!myPieces->at(i+1)) addKCLegalMove(i, i+1);
            if (!myPieces->at(i+7)) addKCLegalMove(i, i+7);
            if (!myPieces->at(i+8)) addKCLegalMove(i, i+8);
            if (!myPieces->at(i+9)) addKCLegalMove(i, i+9);
        }
    }

    if (position->turn==Color::WHITE)
    {
        if (position->castlingRights[0])
        {
            if ((!boardHelper.occupiedSquares[5]) && (!boardHelper.occupiedSquares[6])) addKCLegalMove(4, 6, false);
        }
        if (position->castlingRights[1])
        {
            if ((!boardHelper.occupiedSquares[1]) && (!boardHelper.occupiedSquares[2]) && (!boardHelper.occupiedSquares[3])) addKCLegalMove(4, 2, false);
        }
    }
    else
    {
        if (position->castlingRights[2])
        {
            if ((!boardHelper.occupiedSquares[61]) && (!boardHelper.occupiedSquares[62])) addKCLegalMove(60, 62, false);
        }
        if (position->castlingRights[3])
        {
            if ((!boardHelper.occupiedSquares[57]) && (!boardHelper.occupiedSquares[58]) && (!boardHelper.occupiedSquares[59])) addKCLegalMove(60, 58, false);
        }
    }
}

void LegalMover::addKCLegalMovesQueen(uint i)
{
    addKCLegalMovesRook(i);
    addKCLegalMovesBishop(i);
}

void LegalMover::addKCLegalMovesRook(uint i)
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
    addKCLegalMovesRow(i, row);

    j = i;
    filej = filei;
    row.clear();
    while(filej!=7)
    {
        ++j;
        ++filej;
        row.push_back(j);
    }
    addKCLegalMovesRow(i, row);

    j = i;
    row.clear();
    while(rankj!=0)
    {
        j -= 8;
        --rankj;
        row.push_back(j);
    }
    addKCLegalMovesRow(i, row);

    j = i;
    rankj=ranki;
    row.clear();
    while(rankj!=7)
    {
        j += 8;
        ++rankj;
        row.push_back(j);
    }
    addKCLegalMovesRow(i, row);
}

void LegalMover::addKCLegalMovesBishop(uint i)
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
    addKCLegalMovesRow(i, diag);

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
    addKCLegalMovesRow(i, diag);

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
    addKCLegalMovesRow(i, diag);


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
    addKCLegalMovesRow(i, diag);
}

void LegalMover::addKCLegalMovesRow(uint i, const std::vector<uint> row)
{
    const bitboard *myPieces = (position->turn==Color::WHITE) ? &(boardHelper.whitePieces) : &(boardHelper.blackPieces);

    uint k=0, target;
    while (k<row.size())
    {
        target = row[k];
        if (!(myPieces->at(target))) addKCLegalMove(i, target);
        if (boardHelper.occupiedSquares[target]) break;
        ++k;
    }
}

void LegalMover::addKCLegalMovesKnight(uint i)
{
    uint file = i % 8;
    uint rank = i / 8;

    const bitboard *myPieces = (position->turn==Color::WHITE) ? &(boardHelper.whitePieces) : &(boardHelper.blackPieces);

    if ((rank>1) && (file>0))
    {
        if (!myPieces->at(i-17)) addKCLegalMove(i, i-17);
    }
    if ((rank>1) && (file<7))
    {
        if (!myPieces->at(i-15)) addKCLegalMove(i, i-15);
    }
    if ((rank>0) && (file<6))
    {
        if (!myPieces->at(i-6)) addKCLegalMove(i, i-6);
    }
    if ((rank<7) && (file<6))
    {
        if (!myPieces->at(i+10)) addKCLegalMove(i, i+10);
    }
    if ((rank<6) && (file<7))
    {
        if (!myPieces->at(i+17)) addKCLegalMove(i, i+17);
    }
    if ((rank<6) && (file>0))
    {
        if (!myPieces->at(i+15)) addKCLegalMove(i, i+15);
    }
    if ((rank<7) && (file>1))
    {
        if (!myPieces->at(i+6)) addKCLegalMove(i, i+6);
    }
    if ((rank>0) && (file>1))
    {
        if (!myPieces->at(i-10)) addKCLegalMove(i, i-10);
    }
}


void LegalMover::addKCLegalMovesPawn(uint i)
{
    uint file = i % 8;
    uint rank = i / 8;
    uint target;

    if (position->turn==Color::WHITE)
    {
        if ((rank==0) || (rank==7))
        {
            std::cout << "turn = WHITE" << std::endl;
            std::cout << "i = " << i << std::endl;
            throw("Pawn is on first/last row!?");
        }
        else if (rank==1)
        {
            if (!boardHelper.occupiedSquares[i+8])
            {
                addKCLegalMove(i, i+8, false);
                if (!boardHelper.occupiedSquares[i+16]) addKCLegalMove(i, i+16, false);
            }
        }
        else if (rank==6)
        {
            if (!boardHelper.occupiedSquares[i+8]) addKCLegalMovePromotions(i, i+8);
        }
        else
        {
            if (!boardHelper.occupiedSquares[i+8]) addKCLegalMove(i, i+8, false);
        }

        if (file>0)
        {
            target = i+7;
            if ((boardHelper.blackPieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target)))
            {
                if (rank==6) addKCLegalMovePromotions(i,target,true);
                else addKCLegalMove(i, target);
            }
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;

            //NB: We put by convention that if en passant is theoretically, but not actually, possible, the e.p. target square is "under attack", but it does not matter
        }
        if (file<7)
        {
            target = i+9;
            if ((boardHelper.blackPieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target)))
            {
                if (rank==6) addKCLegalMovePromotions(i,target, true);
                else addKCLegalMove(i, target);
            }
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;
        }
    }
    else
    {
        if ((rank==0) || (rank==7))
        {
            std::cout << "i = " << i << std::endl;
            std::cout << "turn = BLACK" << std::endl;
            throw("Pawn is on first/last row!?");
        }
        else if (rank==6)
        {
            if (!boardHelper.occupiedSquares[i-8])
            {
                addKCLegalMove(i, i-8, false);
                if (!boardHelper.occupiedSquares[i-16]) addKCLegalMove(i, i-16, false);
            }
        }
        else if (rank==1)
        {
            if (!boardHelper.occupiedSquares[i-8]) addKCLegalMovePromotions(i, i-8);
        }
        else
        {
            if (!boardHelper.occupiedSquares[i-8]) addKCLegalMove(i, i-8, false);
        }

        if (file>0)
        {
            target = i-9;
            if ((boardHelper.whitePieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target)))
            {
                if (rank==1) addKCLegalMovePromotions(i,target,true);
                else addKCLegalMove(i, target);
            }
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;
        }
        if (file<7)
        {
            target = i-7;
            if ((boardHelper.whitePieces[target]) || ((position->enPassantPossible==true) && (position->enPassantTargetSquare==target)))
            {
                if (rank==1) addKCLegalMovePromotions(i,target,true);
                else addKCLegalMove(i, target);
            }
            else if (!boardHelper.occupiedSquares[target]) squaresAttacked[target] = true;
        }
    }
}

void LegalMover::updateLegalMoves()
{
    legalMoves.clear();
    for (const auto & move : kCLegalMoves)
    {
        if (isLegalConstruct(move, false)) {legalMoves.push_back(move);}
    }
}

std::string LegalMover::printLegalMoves() const
{
    std::string res = {};
    for (auto move : legalMoves)
    {
        res += move.toLAN();
        res += ", ";
    }
    if (!legalMoves.empty()) res.erase (res.end()-2, res.end());
    return res;
}

std::string LegalMover::printKCLegalMoves() const
{
    std::string res = {};
    for (auto move : kCLegalMoves)
    {
        res += move.toLAN();
        res += ", ";
    }
    if (!kCLegalMoves.empty()) res.erase (res.end()-2, res.end());
    return res;
}

bool LegalMover::isCapture(const Move &move) const
{
    uint target = move.target.getIndex();
    bool enPassant = (isPawnMove(move) && (position->enPassantPossible==true) && (position->enPassantTargetSquare==target));

    return (enPassant || boardHelper.occupiedSquares[target]);
}

bool LegalMover::isPawnMove(const Move &move) const
{
    return boardHelper.pawns[move.origin.getIndex()];
}

bool LegalMover::isKingMove(const Move &move) const
{
    return boardHelper.kings[move.origin.getIndex()];
}

bool LegalMover::isReversible(const Move &move) const
{
    return (!(isCapture(move)) && !(isPawnMove(move)));
}

bool LegalMover::isCheck() const
{
    Position switchTurn(*position);
    switchTurn.turn = (position->turn==Color::WHITE) ? Color::BLACK : Color::WHITE;
    LegalMover newMover(&switchTurn, false);
    return newMover.opponentKingUnderAttack;
}

bool LegalMover::isCheck(const Move &move) const
{
    Position newPos = applyMove(move);
    LegalMover newMover(&newPos, false);
    return newMover.isCheck();
}

bool LegalMover::isCheckmate() const
{
    if (!legalMovesGenerated) throw("ERROR in LegalMover::isCheckmate: Legal moves not generated!");
    return (isCheck() && (legalMoves.empty()));
}

bool LegalMover::isStalemate() const
{
    if (!legalMovesGenerated) throw("ERROR in LegalMover::isStalemate: Legal moves not generated!");
    return ((!isCheck()) && (legalMoves.empty()));
}

bool LegalMover::isCheckmate(const Move &move) const
{
    Position newPos = applyMove(move);
    LegalMover newMover(&newPos, true);
    return newMover.isCheckmate();
}

bool LegalMover::isForceful(const Move &move) const
{
    return (isCapture(move) || isCheck(move));
}

Position LegalMover::applyMove(const Move &m) const
{
    Position res(*position);

    bool white = (position->turn==Color::WHITE);
    res.turn = white ? Color::BLACK : Color::WHITE;

    if (res.turn==Color::WHITE) res.moveNumber++;
    if (isReversible(m)) res.nbReversibleHalfMoves++;
    else res.nbReversibleHalfMoves = 0;
    uint origin = m.origin.getIndex();
    uint target = m.target.getIndex();
    Piece p = position->board.pieces[origin];

    res.board.pieces[origin] = Piece();
    if (m.promotion) res.board.pieces[target] = Piece(m.promotedPiece, white ? Color::WHITE : Color::BLACK);
    else res.board.pieces[target] = p;


    // En passant move
    if (p.isPawn() && position->enPassantPossible && (target==position->enPassantTargetSquare))
    {
        uint opponentPawn = (position->turn==Color::WHITE) ? target-8 : target+8;
        res.board.pieces[opponentPawn] = Piece();
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
    res.enPassantKingCapturePossibleK = false;
    res.enPassantKingCapturePossibleQ = false;
    if (p.isKing())
    {
        if (target==origin-2)
        {
            res.board.pieces[target+1] = res.board.pieces[target-2];
            res.board.pieces[target-2] = Piece();
            res.enPassantKingCapturePossibleQ = true;
        }
        else if (target==origin+2)
        {
            res.board.pieces[target-1] = res.board.pieces[target+1];
            res.board.pieces[target+1] = Piece();
            res.enPassantKingCapturePossibleK = true;
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

    //En Passant King Capture
    if (position->enPassantKingCapturePossibleK)
    {
        if (white)
        {
            if ((target==60) || (target==61)) {res.board.pieces[62] = Piece();}
        }
        else
        {
            if ((target==4) || (target==5)) {res.board.pieces[6] = Piece();}
        }
    }
    else if (position->enPassantKingCapturePossibleQ)
    {
        if (white)
        {
            if ((target==59) || (target==60)) {res.board.pieces[58] = Piece();}
        }
        else
        {
            if ((target==3) || (target==4)) {res.board.pieces[52] = Piece();}
        }
    }

    res.pastBoards = position->pastBoards;
    res.pastBoards.push_back(res.board);

    return res;
}

bool LegalMover::isInLegalMovesList(const Move &m) const
{
    return (std::find(legalMoves.begin(), legalMoves.end(), m) != legalMoves.end());
}

bool LegalMover::isInKCLegalMovesList(const Move &m) const
{
    return (std::find(kCLegalMoves.begin(), kCLegalMoves.end(), m) != kCLegalMoves.end());
}

bool LegalMover::isLegalConstruct(const Move &m, bool checkKClegal) //NB: Legal move = KC legal + king cannot be captured next move (+ including while castling)
{
    if (checkKClegal)
    {
        if  (!(std::find(kCLegalMoves.begin(), kCLegalMoves.end(), m) != kCLegalMoves.end())) return false;
    }

    Position newPos = applyMove(m);
    LegalMover newMover(&newPos, false);

    return !newMover.isOpponentKingCapturable();
}

bool LegalMover::isOpponentKingCapturable() const
{
    return (opponentKingUnderAttack
            || (position->enPassantKingCapturePossibleK && opponentKsideCastleUnderAttack) || (position->enPassantKingCapturePossibleQ && opponentQsideCastleUnderAttack));
}


Color LegalMover::turn() const
{
    return position->turn;
}

bool LegalMover::uniqueOrigin(Square &resOrigin, const Square &target, PieceType type) const
{
    if (legalMovesGenerated)
    {
        uint nbOccurrences = 0;
        Square res(0);
        for (const auto &move : legalMoves)
        {
            if ((move.target==target) && (position->board.pieces[move.origin.getIndex()].getType()==type))
            {
                ++nbOccurrences;
                if (nbOccurrences==1) {res=move.origin;}
            }
        }
        if (nbOccurrences==0)
        {
            std::cout << "Piece not found!" << std::endl;
            return true;
        }
        else if (nbOccurrences==1)
        {
            resOrigin = res;
            return true;
        }
        else {return false;}
    }
    else {throw("Legal moves have not been generated!");}
}


bool LegalMover::uniqueOriginOnFile(Square &resOrigin, const Square &target, uint fileIndex, PieceType type) const
{
    if (legalMovesGenerated)
    {
        uint nbOccurrences = 0;
        Square res(0);
        for (const auto &move : legalMoves)
        {
            if ((move.target==target) && (position->board.pieces[move.origin.getIndex()].getType()==type) && (move.origin.fileIndex()==fileIndex))
            {
                ++nbOccurrences;
                if (nbOccurrences==1) {res=move.origin;}
            }
        }
        if (nbOccurrences==1)
        {
            resOrigin = res;
            return true;
        }
        else {return false;}
    }
    else {throw("ERROR in LegalMover::uniqueOriginOnFile: Legal moves have not been generated!");}
}



bool LegalMover::uniqueOriginOnRank(Square &resOrigin, const Square &target, uint rankIndex, PieceType type) const
{
    if (legalMovesGenerated)
    {
        uint nbOccurrences = 0;
        Square res(0);
        for (const auto &move : legalMoves)
        {
            if ((move.target==target) && (position->board.pieces[move.origin.getIndex()].getType()==type) && (move.origin.rankIndex()==rankIndex))
            {
                ++nbOccurrences;
                if (nbOccurrences==1) {res=move.origin;}
            }

        }
        if (nbOccurrences==1)
        {
            resOrigin = res;
            return true;
        }
        else {return false;}
    }
    else {throw("ERROR in LegalMover::uniqueOriginOnRank: Legal moves have not been generated!");}
}

bool LegalMover::isCastleShort(const Move &move) const
{
    return isKingMove(move) && (((position->turn==Color::WHITE) && (move==Move(4, 6))) || ((position->turn==Color::BLACK) && (move==Move(60, 62))));
}

bool LegalMover::isCastleLong(const Move &move) const
{
    return isKingMove(move) && (((position->turn==Color::WHITE) && (move==Move(4, 2))) || ((position->turn==Color::BLACK) && (move==Move(60, 58))));
}

bool LegalMover::applyMove(Position &res, const Move &m, bool checkLegal, bool checkKCLegal) const
{
    if (legalMovesGenerated)
    {
        if (checkLegal) {if (!isInLegalMovesList(m)) {return false;}}
        if (checkKCLegal) {if (!isInKCLegalMovesList(m)) {return false;}}

        res = applyMove(m);

        return true;
    }
    else {throw("ERROR in LegalMover::applyMove: Legal moves have not been generated!");}
}
