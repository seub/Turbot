#include "legalmover.h"
#include "position.h"
#include "zobrist.h"



LegalMover::LegalMover(const Position *position, bool generateLegalMoves) :
    position(position), turn(position->turn), board(&(position->board)), pieces(position->board.pieces)
{
    initialize();
    if (generateLegalMoves) {updateLegalMoves();}
    legalMovesGenerated = generateLegalMoves;
}


bool LegalMover::initialize()
{
    squaresAttacked.fill(false);
    legalMoves.clear();

    bool res = updatePseudoLegalMoves();
    pseudoLegalMovesGenerated = res;

    uint8f opponentKingSquare;
    if (board->getKingSquare(opponentKingSquare, !turn)) {opponentKingUnderAttack = squaresAttacked[opponentKingSquare];}


    if (turn)
    {
        opponentShortCastleUnderAttack = (squaresAttacked[60] || squaresAttacked[61] || squaresAttacked[62]);
        opponentLongCastleUnderAttack = (squaresAttacked[58] || squaresAttacked[59] || squaresAttacked[60]);
    }
    else
    {
        opponentShortCastleUnderAttack = (squaresAttacked[4] || squaresAttacked[5] || squaresAttacked[6]);
        opponentLongCastleUnderAttack = (squaresAttacked[2] || squaresAttacked[3] || squaresAttacked[4]);
    }
    return res;
}

bool LegalMover::updatePseudoLegalMoves()
{
    pseudoLegalMoves.clear();

    for (uint8f i=0; i<64; ++i)
    {
        if (board->hasPiece(i, turn))
        {
            switch (pieces[i] >> 2)
            {
            case 1: {addPseudoLegalMovesKing(i); break;}
            case 2: {addPseudoLegalMovesQueen(i); break;}
            case 3: {addPseudoLegalMovesRook(i); break;}
            case 4: {addPseudoLegalMovesBishop(i); break;}
            case 5: {addPseudoLegalMovesKnight(i); break;}
            case 6: {addPseudoLegalMovesPawn(i); break;}
            default:   {throw("Error in LegalMover::initialize()"); return false;}
            }
        }
    }
    return true;
}

void LegalMover::updateLegalMoves()
{
    legalMoves.clear();
    for (const auto & move : pseudoLegalMoves)
    {
        if (isLegalConstruct(move, false)) {legalMoves.push_back(move);}
    }
}


void LegalMover::addPseudoLegalMove(uint8f origin, uint8f target, bool attack)
{
    pseudoLegalMoves.push_back(Move(origin, target));
    if (attack) {squaresAttacked[target] = true;}
}

void LegalMover::addPseudoLegalMovePromotions(uint8f origin, uint8f target, bool attack)
{
    pseudoLegalMoves.push_back(Move(origin, target, 5));
    pseudoLegalMoves.push_back(Move(origin, target, 7));
    pseudoLegalMoves.push_back(Move(origin, target, 9));
    pseudoLegalMoves.push_back(Move(origin, target, 11));
    if (attack) {squaresAttacked[target] = true;}
}

void LegalMover::addPseudoLegalMovesKing(uint8f origin)
{

    uint8f file = origin % 8;
    uint8f rank = origin / 8;


    if (rank==0)
    {
        if (file==0)
        {
            if (!board->hasPiece(1, turn)) {addPseudoLegalMove(origin, 1);}
            if (!board->hasPiece(8, turn)) {addPseudoLegalMove(origin, 8);}
            if (!board->hasPiece(9, turn)) {addPseudoLegalMove(origin, 9);}
        }
        else if (file==7)
        {
            if (!board->hasPiece(6, turn)) {addPseudoLegalMove(origin, 6);}
            if (!board->hasPiece(14, turn)) {addPseudoLegalMove(origin, 14);}
            if (!board->hasPiece(15, turn)) {addPseudoLegalMove(origin, 15);}
        }
        else
        {
            if (!board->hasPiece(origin-1, turn)) {addPseudoLegalMove(origin, origin-1);}
            if (!board->hasPiece(origin+1, turn)) {addPseudoLegalMove(origin, origin+1);}
            if (!board->hasPiece(origin+7, turn)) {addPseudoLegalMove(origin, origin+7);}
            if (!board->hasPiece(origin+8, turn)) {addPseudoLegalMove(origin, origin+8);}
            if (!board->hasPiece(origin+9, turn)) {addPseudoLegalMove(origin, origin+9);}
        }
    }
    else if (rank==7)
    {
        if (file==0)
        {
            if (!board->hasPiece(48, turn)) {addPseudoLegalMove(origin, 48);}
            if (!board->hasPiece(49, turn)) {addPseudoLegalMove(origin, 49);}
            if (!board->hasPiece(57, turn)) {addPseudoLegalMove(origin, 57);}
        }
        else if (file==7)
        {
            if (!board->hasPiece(54, turn)) {addPseudoLegalMove(origin, 54);}
            if (!board->hasPiece(55, turn)) {addPseudoLegalMove(origin, 55);}
            if (!board->hasPiece(62, turn)) {addPseudoLegalMove(origin, 62);}
        }
        else
        {
            if (!board->hasPiece(origin-9, turn)) {addPseudoLegalMove(origin, origin-9);}
            if (!board->hasPiece(origin-8, turn)) {addPseudoLegalMove(origin, origin-8);}
            if (!board->hasPiece(origin-7, turn)) {addPseudoLegalMove(origin, origin-7);}
            if (!board->hasPiece(origin-1, turn)) {addPseudoLegalMove(origin, origin-1);}
            if (!board->hasPiece(origin+1, turn)) {addPseudoLegalMove(origin, origin+1);}
        }
    }
    else
    {
        if (file==0)
        {
            if (!board->hasPiece(origin-8, turn)) {addPseudoLegalMove(origin, origin-8);}
            if (!board->hasPiece(origin-7, turn)) {addPseudoLegalMove(origin, origin-7);}
            if (!board->hasPiece(origin+1, turn)) {addPseudoLegalMove(origin, origin+1);}
            if (!board->hasPiece(origin+8, turn)) {addPseudoLegalMove(origin, origin+8);}
            if (!board->hasPiece(origin+9, turn)) {addPseudoLegalMove(origin, origin+9);}
        }
        else if (file==7)
        {
            if (!board->hasPiece(origin-9, turn)) {addPseudoLegalMove(origin, origin-9);}
            if (!board->hasPiece(origin-8, turn)) {addPseudoLegalMove(origin, origin-8);}
            if (!board->hasPiece(origin-1, turn)) {addPseudoLegalMove(origin, origin-1);}
            if (!board->hasPiece(origin+7, turn)) {addPseudoLegalMove(origin, origin+7);}
            if (!board->hasPiece(origin+8, turn)) {addPseudoLegalMove(origin, origin+8);}
        }
        else
        {
            if (!board->hasPiece(origin-9, turn)) {addPseudoLegalMove(origin, origin-9);}
            if (!board->hasPiece(origin-8, turn)) {addPseudoLegalMove(origin, origin-8);}
            if (!board->hasPiece(origin-7, turn)) {addPseudoLegalMove(origin, origin-7);}
            if (!board->hasPiece(origin-1, turn)) {addPseudoLegalMove(origin, origin-1);}
            if (!board->hasPiece(origin+1, turn)) {addPseudoLegalMove(origin, origin+1);}
            if (!board->hasPiece(origin+7, turn)) {addPseudoLegalMove(origin, origin+7);}
            if (!board->hasPiece(origin+8, turn)) {addPseudoLegalMove(origin, origin+8);}
            if (!board->hasPiece(origin+9, turn)) {addPseudoLegalMove(origin, origin+9);}
        }
    }

    if (turn)
    {
        if (position->castlingRights & 1)
        {
            if ((!pieces[5]) && (!pieces[6])) {addPseudoLegalMove(4, 6, false);}
        }
        if (position->castlingRights & 2)
        {
            if ((!pieces[1]) && (!pieces[2]) && (!pieces[3])) {addPseudoLegalMove(4, 2, false);}
        }
    }
    else
    {
        if (position->castlingRights & 4)
        {
            if ((!pieces[61]) && (!pieces[62])) {addPseudoLegalMove(60, 62, false);}
        }
        if (position->castlingRights & 8)
        {
            if ((!pieces[57]) && (!pieces[58]) && (!pieces[59])) {addPseudoLegalMove(60, 58, false);}
        }
    }
}

void LegalMover::addPseudoLegalMovesQueen(uint8f origin)
{
    addPseudoLegalMovesRook(origin);
    addPseudoLegalMovesBishop(origin);
}


void LegalMover::addPseudoLegalMovesRook(uint8f origin)
{
    std::vector<uint8f> row = {};
    uint8f file = origin % 8;
    uint8f rank = origin / 8;

    uint8f j = origin;
    uint8f filej = file;
    uint8f rankj = rank;
    while(filej!=0)
    {
        --j;
        --filej;
        row.push_back(j);
    }
    addPseudoLegalMovesRow(origin, row);

    j = origin;
    filej = file;
    row.clear();
    while(filej!=7)
    {
        ++j;
        ++filej;
        row.push_back(j);
    }
    addPseudoLegalMovesRow(origin, row);

    j = origin;
    row.clear();
    while(rankj!=0)
    {
        j -= 8;
        --rankj;
        row.push_back(j);
    }
    addPseudoLegalMovesRow(origin, row);

    j = origin;
    rankj=rank;
    row.clear();
    while(rankj!=7)
    {
        j += 8;
        ++rankj;
        row.push_back(j);
    }
    addPseudoLegalMovesRow(origin, row);
}

void LegalMover::addPseudoLegalMovesBishop(uint8f origin)
{
    std::vector<uint8f> diag = {};
    uint8f file = origin % 8;
    uint8f rank = origin / 8;

    uint j = origin;
    uint filej = file;
    uint rankj = rank;
    while((filej!=0) && (rankj!=0))
    {
        j -= 9;
        --filej;
        --rankj;
        diag.push_back(j);
    }
    addPseudoLegalMovesRow(origin, diag);

    j = origin;
    filej = file;
    rankj = rank;
    diag.clear();
    while((filej!=7) && (rankj!=0))
    {
        j -= 7;
        ++filej;
        --rankj;
        diag.push_back(j);
    }
    addPseudoLegalMovesRow(origin, diag);

    j = origin;
    filej = file;
    rankj = rank;
    diag.clear();
    while((filej!=0) && (rankj!=7))
    {
        j += 7;
        --filej;
        ++rankj;
        diag.push_back(j);
    }
    addPseudoLegalMovesRow(origin, diag);


    j = origin;
    filej = file;
    rankj = rank;
    diag.clear();
    while((filej!=7) && (rankj!=7))
    {
        j += 9;
        ++filej;
        ++rankj;
        diag.push_back(j);
    }
    addPseudoLegalMovesRow(origin, diag);
}


void LegalMover::addPseudoLegalMovesRow(uint8f origin, const std::vector<uint8f> &row)
{
    uint8f k=0, target;
    while (k<row.size())
    {
        target = row[k];
        if (!board->hasPiece(target, turn)) {addPseudoLegalMove(origin, target);}
        if (pieces[target]) break;
        ++k;
    }
}


void LegalMover::addPseudoLegalMovesKnight(uint8f origin)
{
    uint8f file = origin % 8;
    uint8f rank = origin / 8;

    if ((rank>1) && (file>0)){if (!board->hasPiece(origin-17, turn)) {addPseudoLegalMove(origin, origin-17);}}
    if ((rank>1) && (file<7)){if (!board->hasPiece(origin-15, turn)) {addPseudoLegalMove(origin, origin-15);}}
    if ((rank>0) && (file<6)){if (!board->hasPiece(origin-6, turn)) {addPseudoLegalMove(origin, origin-6);}}
    if ((rank<7) && (file<6)){if (!board->hasPiece(origin+10, turn)) {addPseudoLegalMove(origin, origin+10);}}
    if ((rank<6) && (file<7)){if (!board->hasPiece(origin+17, turn)) {addPseudoLegalMove(origin, origin+17);}}
    if ((rank<6) && (file>0)){if (!board->hasPiece(origin+15, turn)) {addPseudoLegalMove(origin, origin+15);}}
    if ((rank<7) && (file>1)){if (!board->hasPiece(origin+6, turn)) {addPseudoLegalMove(origin, origin+6);}}
    if ((rank>0) && (file>1)){if (!board->hasPiece(origin-10, turn)) {addPseudoLegalMove(origin, origin-10);}}
}


void LegalMover::addPseudoLegalMovesPawn(uint8f origin)
{
    uint8f file = origin % 8;
    uint8f rank = origin / 8;
    uint8f target;

    if (turn)
    {
        if ((rank==0) || (rank==7)){throw("Pawn is on first/last row!?");}
        else if (rank==1)
        {
            if (!pieces[origin+8])
            {
                addPseudoLegalMove(origin, origin+8, false);
                if (!pieces[origin+16]) {addPseudoLegalMove(origin, origin+16, false);}
            }
        }
        else if (rank==6){if (!pieces[origin+8]) {addPseudoLegalMovePromotions(origin, origin+8, false);}}
        else if (!pieces[origin+8]) {addPseudoLegalMove(origin, origin+8, false);}

        if (file>0)
        {
            target = origin+7;
            if ((board->hasPiece(target, false)) || ((target/8 == 5) && (position->enPassant == 1 + ((target % 8) << 1))))
            {
                if (rank==6) {addPseudoLegalMovePromotions(origin, target, true);}
                else {addPseudoLegalMove(origin, target);}
            }
            else if (!pieces[target]) {squaresAttacked[target] = true;}
            //NB: We put that if en passant is theoretically, but not actually, possible, the e.p. target square is "under attack", but it does not matter
        }
        if (file<7)
        {
            target = origin+9;
            if ((board->hasPiece(target, false)) || ((target/8 == 5) && (position->enPassant == 1 + ((target % 8) << 1))))
            {
                if (rank==6) addPseudoLegalMovePromotions(origin,target, true);
                else addPseudoLegalMove(origin, target);
            }
            else if (!pieces[target]) {squaresAttacked[target] = true;}
        }
    }
    else
    {
        if ((rank==0) || (rank==7)) {throw("Pawn is on first/last row!?");}
        else if (rank==6)
        {
            if (!pieces[origin-8])
            {
                addPseudoLegalMove(origin, origin-8, false);
                if (!pieces[origin-16]) {addPseudoLegalMove(origin, origin-16, false);}
            }
        }
        else if (rank==1) {if (!pieces[origin-8]) {addPseudoLegalMovePromotions(origin, origin-8, false);}}
        else if (!pieces[origin-8]) {addPseudoLegalMove(origin, origin-8, false);}

        if (file>0)
        {
            target = origin-9;
            if (board->hasPiece(target, true) || ((target/8 == 2) && (position->enPassant == 1 + ((target % 8) << 1))))
            {
                if (rank==1) {addPseudoLegalMovePromotions(origin, target, true);}
                else {addPseudoLegalMove(origin, target);}
            }
            else if (!pieces[target]) {squaresAttacked[target] = true;}
        }
        if (file<7)
        {
            target = origin-7;
            if (board->hasPiece(target, true) || ((target/8 == 2) && (position->enPassant == 1 + ((target % 8) << 1))))
            {
                if (rank==1) {addPseudoLegalMovePromotions(origin, target, true);}
                else {addPseudoLegalMove(origin, target);}
            }
            else if (!pieces[target]) {squaresAttacked[target] = true;}
        }
    }
}


bool LegalMover::isCapture(const Move &move) const
{
    return (pieces[move.target] & 1) || (isEnPassantPawnMove(move));
}

bool LegalMover::isEnPassantPawnMove(const Move &move) const
{
    return isPawnMove(move) && (move.target/8 == (turn ? 5 : 2)) && (position->enPassant == 1 + ((move.target % 8) << 1));
}

bool LegalMover::isOpponentKingCapturable() const
{
    return (opponentKingUnderAttack
            || ((position->enPassantKing & 1) && opponentShortCastleUnderAttack) || ((position->enPassant & 2) && opponentLongCastleUnderAttack));
}

bool LegalMover::isPawnMove(const Move &move) const
{
    return board->isPawn(move.origin);
}

bool LegalMover::isKingMove(const Move &move) const
{
    return board->isKing(move.origin);
}

bool LegalMover::isRookMove(const Move &move) const
{
    return board->isRook(move.origin);
}

bool LegalMover::isReversible(const Move &move) const
{
    return (!(isCapture(move)) && !(isPawnMove(move)));
}

bool LegalMover::isCheck() const
{
    Position switchTurn(*position);
    switchTurn.turn = !turn;
    return LegalMover(&switchTurn, false).opponentKingUnderAttack;
}

bool LegalMover::isCheck(const Move &move) const
{
    Position newPos;
    applyMove(move, newPos, false);
    return LegalMover(&newPos, false).isCheck();
}

bool LegalMover::isCheckmate() const
{
    if (!legalMovesGenerated) {throw("ERROR in LegalMover::isCheckmate: Legal moves not generated!");}
    return (isCheck() && (legalMoves.empty()));
}

bool LegalMover::isStalemate() const
{
    if (!legalMovesGenerated) {throw("ERROR in LegalMover::isStalemate: Legal moves not generated!");}
    return ((!isCheck()) && (legalMoves.empty()));
}

bool LegalMover::isCheckmate(const Move &move) const
{
    Position newPos;
    applyMove(move, newPos, false);
    return LegalMover(&newPos, true).isCheckmate();
}

bool LegalMover::isCastleShort(const Move &move) const
{
    return isKingMove(move) && (((position->turn) && (move==Move(4, 6))) || ((!position->turn) && (move==Move(60, 62))));
}

bool LegalMover::isCastleLong(const Move &move) const
{
    return isKingMove(move) && (((position->turn) && (move==Move(4, 2))) || ((!position->turn) && (move==Move(60, 58))));
}


void LegalMover::applyMove(const Move &move, Position &res, bool updateFull) const
{
    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {throw("ERROR in LegalMoverZ::applyMove: Zobrist numbers have not been generated");}

    res.board = position->board;
    res.turn = !turn;
    res.castlingRights = position->castlingRights;
    res.drawClaimable = false;

    res.board.removePiece(move.origin);

    if (move.promotion) {res.board.addPiece(move.target, Piece(move.promotion >> 1, turn).getNum());}
    else {res.board.addPiece(move.target, pieces[move.origin]);}

    // En passant move
    if (isEnPassantPawnMove(move)){res.board.removePiece(turn ? move.target-8 : move.target+8);}

    // Pawn move allowing en passant or not
    if (isPawnMove(move) && ((move.target==move.origin+16) || (move.origin==move.target+16))){res.enPassant = 1 + ((move.origin % 8) << 1);}
    else {res.enPassant = 0;}

    //Castling and losing castling rights with King move
    res.enPassantKing = 0;
    if (isKingMove(move))
    {
        if (move.target==move.origin-2)
        {
            res.board.addPiece(move.target+1, pieces[move.target-2]);
            res.board.removePiece(move.target-2);
            res.enPassantKing = 2;
        }
        else if (move.target==move.origin+2)
        {
            res.board.addPiece(move.target-1, pieces[move.target+1]);
            res.board.removePiece(move.target+1);
            res.enPassantKing = 1;
        }

        if (turn) {res.castlingRights &= 0b1100;}
        else {res.castlingRights &= 0b0011;}
    }

    //Losing castling rights with Rook move
    if (isRookMove(move))
    {
        if (move.origin==7) {res.castlingRights &= 0b1110;}
        if (move.origin==0) {res.castlingRights &= 0b1101;}
        if (move.origin==63) {res.castlingRights &= 0b1011;}
        if (move.origin==56) {res.castlingRights &= 0b0111;}
    }

    //En Passant King Capture
    if (position->enPassantKing & 1)
    {
        if (turn) {if ((move.target==60) || (move.target==61)) {res.board.removePiece(62);}}
        else if ((move.target==4) || (move.target==5)) {res.board.removePiece(6);}
    }
    else if (position->enPassantKing & 2)
    {
        if (turn) {if ((move.target==59) || (move.target==60)) {res.board.removePiece(58);}}
        else if ((move.target==3) || (move.target==4)) {res.board.removePiece(2);}
    }

    res.hash = res.recalculateHash(false);

    if (updateFull)
    {
        res.moveNumber = turn ? position->moveNumber : position->moveNumber+1;
        res.nbReversiblePlies = isReversible(move) ? position->nbReversiblePlies+1 : 0;

        res.fiftyDraw = position->fiftyDraw;
        if (res.nbReversiblePlies>49) {res.fiftyDraw = true;}

        res.threeFoldDraw = position->threeFoldDraw;
        if (!res.threeFoldDraw)
        {
            Board board = res.board;
            res.repetitions = position->repetitions;
            if(res.repetitions.find(board)!=res.repetitions.end())
            {
                res.repetitions[board]++;
                if(res.repetitions[board]==3) {res.threeFoldDraw=true;}
            }
            else {res.repetitions[board]=1;}
        }

        res.updateDrawClaimable();
    }
}

bool LegalMover::isInLegalMovesList(const Move &move) const
{
    return (std::find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end());
}

bool LegalMover::isInPseudoLegalMovesList(const Move &move) const
{
    return (std::find(pseudoLegalMoves.begin(), pseudoLegalMoves.end(), move) != pseudoLegalMoves.end());
}

bool LegalMover::isLegalConstruct(const Move &move, bool checkPseudoLegal)
{
    if (checkPseudoLegal)
    {
        if  (!isInPseudoLegalMovesList(move)) return false;
    }

    Position newPos;
    applyMove(move, newPos, false);
    return !LegalMover(&newPos, false).isOpponentKingCapturable();
}


bool LegalMover::uniqueOrigin(uint8f &resOrigin, const uint8f &target, PieceType type) const
{
    if (legalMovesGenerated)
    {
        uint nbOccurrences = 0;
        resOrigin = 64;
        for (const auto &move : legalMoves)
        {
            if ((move.target==target) && ((pieces[move.origin] >> 2) == (uint8f) type))
            {
                ++nbOccurrences;
                if (nbOccurrences==1) {resOrigin=move.origin;}
            }
        }
        return (nbOccurrences==1);
    }
    else {throw("Legal moves have not been generated!");}
}


bool LegalMover::uniqueOriginOnFile(uint8f &resOrigin, const uint8f &target, uint8f fileIndex, PieceType type) const
{
    if (legalMovesGenerated)
    {
        uint nbOccurrences = 0;
        resOrigin = 64;
        for (const auto &move : legalMoves)
        {
            if ((move.target==target) && ((pieces[move.origin] >> 2) == (uint8f) type) && ((move.origin % 8)==fileIndex))
            {
                ++nbOccurrences;
                if (nbOccurrences==1) {resOrigin=move.origin;}
            }
        }
        return (nbOccurrences==1);
    }
    else {throw("ERROR in LegalMover::uniqueOriginOnFile: Legal moves have not been generated!");}
}



bool LegalMover::uniqueOriginOnRank(uint8f &resOrigin, const uint8f &target, uint8f rankIndex, PieceType type) const
{
    if (legalMovesGenerated)
    {
        uint nbOccurrences = 0;
        resOrigin = 64;
        for (const auto &move : legalMoves)
        {
            if ((move.target==target) && ((pieces[move.origin] >> 2) == (uint8f) type) && ((move.origin / 8)==rankIndex))
            {
                ++nbOccurrences;
                if (nbOccurrences==1) {resOrigin=move.origin;}
            }

        }
        return (nbOccurrences==1);
    }
    else {throw("ERROR in LegalMover::uniqueOriginOnRank: Legal moves have not been generated!");}
}
