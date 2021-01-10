#include "move.h"
#include "boardhelper.h"
#include "position.h"

Move::Move(const Square & origin, const Square & target, bool promotion, PieceType promotedPiece) :
    origin(origin), target(target), promotion(promotion), promotedPiece(promotedPiece)
{

}

Move::Move(uint originIndex, uint targetIndex, bool promotion, PieceType promotedPiece) :
    origin(originIndex), target(targetIndex), promotion(promotion), promotedPiece(promotedPiece)
{

}

bool Move::fromLAN(Move & res, std::string & LANstring)
{
    uint size = LANstring.size();
    if (!(size==4 || size==6)) return false;

    res.origin = LANstring.substr(0, 2);
    res.target = LANstring.substr(2, 4);

    if (size==4)
    {
        res.promotion=false;
        res.promotedPiece=PieceType::EMPTY;
    }
    else
    {
        char c = LANstring[4];
        if (c!='=') return false;
        c = LANstring[5];
        switch (c)
        {
        case 'Q' : res.promotedPiece=PieceType::QUEEN; break;
        case 'R' : res.promotedPiece=PieceType::ROOK; break;
        case 'B' : res.promotedPiece=PieceType::BISHOP; break;
        case 'N': res.promotedPiece=PieceType::KNIGHT; break;
        default : return false; break;
        }
        res.promotion=true;
    }

    return true;
}

bool Move::operator==(const Move &other) const
{
    return (other.origin==origin) && (other.target==target) && (other.promotion==promotion) && ((!promotion) || (other.promotedPiece==promotedPiece));
}

std::string Move::toLAN() const
{
    std::string res = origin.name()+target.name();
    if (promotion)
    {
        res += "=";
        res += Piece(promotedPiece).name();
    }
    return res;
}


std::ostream & operator <<(std::ostream &out, const Move &M)
{
    out << M.toLAN();
    return out;
}














MovePGN::MovePGN(const Move &move, const LegalMover *mover) : Move(move)
{
    if (fromMove(*this, move, mover)) {}
    else {std::cout << "MovePGN constructor failed!" << std::endl;}
}


std::string MovePGN::toPGN() const
{
    std::string out = {};

    if (castleShort) {out += "O-O";}
    else if (castleLong) {out += "O-O-O";}
    else
    {
        if (piecetype==PieceType::PAWN) {if (capture) {out += origin.fileName();}}
        else
        {
            out += Piece::name(piecetype);
            if (pieceOriginAmbiguous)
            {
                if (ambiguityLiftedByFile) out+= origin.fileName();
                else if (ambiguityLiftedByRank) out += origin.rankName();
                else out += origin.name();
            }
        }
        if (capture) out += "x";
        out += target.name();
        if (promotion)
        {
            out += '=';
            out += Piece::name(promotedPiece);
        }
    }
    if (checkmate) out += "#";
    else if (check) out += "+";

    return out;
}

std::string MovePGN::toPGN(uint moveNum) const
{
    std::string out = Tools::convertToString(moveNum);
    out += (turn==Color::WHITE) ? ". " : "... ";
    out += toPGN();
    return out;
}


bool MovePGN::fromPGN(MovePGN &res, const std::string &PGNstring, const LegalMover *mover)
{
    Move move(Square(0),Square(0));
    if (fromPGN(move, PGNstring, mover)) {return fromMove(res, move, mover);}
    else return false;
}

bool MovePGN::fromPGN(Move &res, const std::string &PGNstring, const LegalMover *mover)
{

    if (PGNstring.empty()) {std::cout << "Empty string" << std::endl; return false;}

    std::string::size_type pos=0;
    char c = PGNstring[pos];

    uint fileIndexOrigin, rankIndexOrigin;
    uint fileIndexTarget, rankIndexTarget, squareIndexTarget;
    Square origin, target;
    bool promotion = false;
    PieceType promotedPiece=PieceType::EMPTY;
    PieceType piecetype;

    const BoardHelper *helper = &(mover->boardHelper);
    Color turn = mover->turn();

    if (Board::fileIndex(fileIndexOrigin, c))
    {
        ++pos;
        if (!(pos<PGNstring.size())) {std::cout << "String ended prematurely" << std::endl; return false;}
        c=PGNstring[pos];
        if (Board::rankIndex(rankIndexTarget, c))
        {
            fileIndexTarget = fileIndexOrigin;
            squareIndexTarget = Square(fileIndexTarget, rankIndexTarget).getIndex();
            if (mover->turn()==Color::WHITE)
            {
                if ((rankIndexTarget < 2) || (rankIndexTarget > 7)) {std::cout << "wrong rank for pawn move" << std::endl; return false;}
                else if (helper->occupiedSquares[squareIndexTarget-8]) {rankIndexOrigin = rankIndexTarget-1;}
                else
                {
                    if (rankIndexTarget != 3) {std::cout << "incorrect pawn move" << std::endl; return false;}
                    rankIndexOrigin = rankIndexTarget-2;
                }
                promotion = (rankIndexTarget==7);
            }
            else
            {
                if (rankIndexTarget > 5) {std::cout << "wrong rank for pawn move" << std::endl; return false;}
                else if (helper->occupiedSquares[squareIndexTarget+8]) {rankIndexOrigin = rankIndexTarget+1;}
                else
                {
                    if (rankIndexTarget != 4) {std::cout << "incorrect pawn move" << std::endl; return false;}
                    rankIndexOrigin = rankIndexTarget+2;
                }
                promotion = (rankIndexTarget==0);
            }
        }
        else if (c=='x')
        {
            if ((PGNstring.size()>pos+2) && (Board::fileIndex(fileIndexTarget, PGNstring[pos+1])) && (Board::rankIndex(rankIndexTarget, PGNstring[pos+2])))
            {
                rankIndexOrigin = (turn==Color::WHITE) ? rankIndexTarget-1 : rankIndexTarget+1;
                promotion = (turn==Color::WHITE) ? (rankIndexTarget==7) :  (rankIndexTarget==0);
                pos += 2;

            }
            else {std::cout << "incorrect pawn move" << std::endl; return false;}
        }
        else {std::cout << "incorrect pawn (?) move" << std::endl; return false;}

        origin = Square(fileIndexOrigin, rankIndexOrigin);
        target = Square(fileIndexTarget, rankIndexTarget);


        if (promotion)
        {
            ++pos;
            if (pos<PGNstring.size())
            {
                c = PGNstring[pos];
                if (c=='=')
                {
                    ++pos;
                    if (pos<PGNstring.size())
                    {
                        c = PGNstring[pos];
                        switch (c)
                        {
                        case 'Q' : promotedPiece=PieceType::QUEEN; break;
                        case 'R' : promotedPiece=PieceType::ROOK; break;
                        case 'B' : promotedPiece=PieceType::BISHOP; break;
                        case 'N' : promotedPiece=PieceType::KNIGHT; break;
                        default: {std::cout << "Incorrect promoted piece" << std::endl; return false;}
                        }
                    }
                    else {std::cout << "String ended prematurely" << std::endl; return false;}
                }
                else {std::cout << "Pawn promotion not followed by =" << std::endl; return false;}
            }
            else {std::cout << "Pawn promotion not followed by =" << std::endl; return false;}
        }
    }
    else if (Piece::fromName(piecetype, c))
    {
        if ((c=='E') || (c=='P')) {std::cout << "Incorrect piece name" << std::endl; return false;}
        ++pos;
        if (!(pos<PGNstring.size())) {std::cout << "String ended prematurely" << std::endl; return false;}
        c = PGNstring[pos];
        uint file, rank;
        if (Board::fileIndex(file, c))
        {
            ++pos;
            if (!(pos<PGNstring.size())) {std::cout << "String ended prematurely" << std::endl; return false;}
            c = PGNstring[pos];
            if (Board::rankIndex(rank, c))
            {
                if ((PGNstring.size()==pos+1) || (PGNstring[pos+1]=='+') || (PGNstring[pos+1]=='#'))
                {
                    fileIndexTarget = file;
                    rankIndexTarget = rank;
                    target = Square(fileIndexTarget, rankIndexTarget);
                    if (mover->uniqueOrigin(origin, target, piecetype)) {}
                    else {std::cout << "Ambiguous or incorrect move" << std::endl; return false;}
                }
                else if (((PGNstring.size()>pos+2) && Square::fromName(squareIndexTarget, PGNstring.substr(pos+1, 2))) ||
                         ((PGNstring.size()>pos+3) && (PGNstring[pos+1]=='x') && Square::fromName(squareIndexTarget, PGNstring.substr(pos+2, 2))))
                {
                    fileIndexOrigin = file;
                    rankIndexOrigin = rank;
                    origin = Square(fileIndexOrigin, rankIndexOrigin);
                    target = Square(squareIndexTarget);
                }
                else {std::cout << "Incorrect move name" << std::endl; return false;}
            }
            else if (((PGNstring.size()>pos+1) && Square::fromName(squareIndexTarget, PGNstring.substr(pos, 2))) ||
                     ((PGNstring.size()>pos+2) && (PGNstring[pos]=='x') && Square::fromName(squareIndexTarget, PGNstring.substr(pos+1, 2))))
            {
                target = Square(squareIndexTarget);
                fileIndexOrigin = file;
                if (mover->uniqueOriginOnFile(origin, target, fileIndexOrigin, piecetype)) {}
                else {std::cout << "Ambiguous or incorrect move" << std::endl; return false;}
            }
            else {std::cout << "Incorrect move name" << std::endl; return false;}
        }
        else if (c=='x')
        {
            if ((PGNstring.size()>pos+2) && Square::fromName(squareIndexTarget, PGNstring.substr(pos+1, 2)))
            {
                target = Square(squareIndexTarget);
                if (mover->uniqueOrigin(origin, target, piecetype)) {}
                else
                {
                    std::cout << "Ambiguous or incorrect move" << std::endl;
                    std::cout << "String was " << PGNstring << std::endl;
                    std::cout << "Position was " << mover->position << std::endl;
                    return false;
                }
            }
            else {std::cout << "Incorrect move name" << std::endl; return false;}
        }
        else if (Board::rankIndex(rank, c))
        {
            if (((PGNstring.size()>pos+2) && Square::fromName(squareIndexTarget, PGNstring.substr(pos+1, 2))) ||
                    ((PGNstring.size()>pos+3) && (PGNstring[pos+1]=='x') && Square::fromName(squareIndexTarget, PGNstring.substr(pos+2, 2))))
            {
                target = Square(squareIndexTarget);
                rankIndexOrigin = rank;
                if (mover->uniqueOriginOnRank(origin, target, rankIndexOrigin, piecetype)) {}
                else {std::cout << "Ambiguous or incorrect move (uniqueOriginOnRank failed)" << std::endl; return false;}
            }
            else {std::cout << "Ambiguous or incorrect move (wrong string continuation)" << std::endl; return false;}
        }
        else {std::cout << "Incorrect piece move" << std::endl; return false;}
    }
    else if ((PGNstring.size()>pos+2) && ((PGNstring.substr(pos, 3)=="O-O") || (PGNstring.substr(pos, 3)=="0-0")))
    {
        origin = (turn==Color::WHITE) ? Square(4, 0) : Square(4, 7) ;
        if ((PGNstring.size()>pos+4) && ((PGNstring.substr(pos, 5)=="O-O-O") || (PGNstring.substr(pos, 5)=="0-0-0")))
        {
            target = (turn==Color::WHITE) ? Square(2, 0) : Square(2, 7) ;
        }
        else
        {
            target = (turn==Color::WHITE) ? Square(6, 0) : Square(6, 7) ;
        }
    }

    res = Move(origin, target, promotion, promotedPiece);
    return true;
}

std::ostream & operator <<(std::ostream &out, const MovePGN &M)
{
    out << M.toPGN();
    return out;
}

bool MovePGN::fromMove(MovePGN &res, const Move &move, const LegalMover *mover)
{
    res.origin = move.getOrigin();
    res.target = move.getTarget();
    res.promotion = move.getPromotion();
    res.promotedPiece = move.getPromotedPiece();

    if (!mover->isInLegalMovesList(move))
    {
        //std::cout << std::endl << "WARNING: Illegal move in MovePGN::fromMove" << std::endl;
        /*std::cout << "Move is " << move << std::endl;
        std::cout << "Position is " << *(mover->position) << std::endl;*/
        /*std::cout << "Legal move list is " << mover->printLegalMoves() << std::endl;
        std::cout << "KCLegal move list is " << mover->printKCLegalMoves() << std::endl;*/
        return false;
    }

    Piece piece;
    if (mover->position->getPiece(piece, res.origin))
    {
        res.piecetype = piece.getType();
    }
    else
    {
        std::cout << "Error1: Empty origin square" << std::endl;
        std::cout << "Error2: mover->isInLegalMovesList should have prevented Error1" << std::endl;
        return false;
    }

    res.turn = mover->turn();

    Square dummy(res.origin);
    res.pieceOriginAmbiguous = (!mover->uniqueOrigin(dummy, res.target, res.piecetype));
    res.ambiguityLiftedByFile = ((res.pieceOriginAmbiguous) && (mover->uniqueOriginOnFile(dummy, res.target, res.origin.fileIndex(), res.piecetype)));
    res.ambiguityLiftedByRank = ((res.pieceOriginAmbiguous) && (mover->uniqueOriginOnRank(dummy, res.target, res.origin.rankIndex(), res.piecetype)));

    res.capture=mover->isCapture(move);
    res.castleShort=mover->isCastleShort(move);
    res.castleLong=mover->isCastleLong(move);
    res.check = mover->isCheck(move);
    res.checkmate = mover->isCheckmate(move);

    return true;
}

bool MovePGN::operator==(const MovePGN &other) const
{
    bool res = true;
    res &= other.origin==origin;
    res &= other.target==target;
    res &= other.promotion==promotion && ((!promotion) || (other.promotedPiece==promotedPiece));
    res &= other.piecetype==piecetype;
    res &= other.turn==turn;
    res &= other.pieceOriginAmbiguous==pieceOriginAmbiguous;
    res &= other.ambiguityLiftedByFile==ambiguityLiftedByFile;
    res &= other.ambiguityLiftedByRank==ambiguityLiftedByRank;
    res &= other.capture==capture;
    res &= other.castleShort==castleShort;
    res &= other.castleLong==castleLong;
    res &= other.check==check;
    res &= other.checkmate==checkmate;
    return res;
}

















bool MoveZ::operator==(const MoveZ &other) const
{
    return (other.origin==origin) && (other.target==target) && (other.promotion==promotion);
}


bool MoveZ::fromLAN(MoveZ &res, std::string &LANstring)
{
    uint size = LANstring.size();
    if (!(size==4 || size==6)) return false;

    SquareZ::fromName(res.origin, LANstring.substr(0, 2));
    SquareZ::fromName(res.target, LANstring.substr(2, 2));

    if (size==4)
    {
        res.promotion=0;
    }
    else
    {
        char c = LANstring[4];
        if (c!='=') return false;
        c = LANstring[5];
        switch (c)
        {
        case 'Q' : res.promotion = 1 + (2 << 1); break;
        case 'R' : res.promotion = 1 + (3 << 1); break;
        case 'B' : res.promotion = 1 + (4 << 1); break;
        case 'N' : res.promotion = 1 + (5 << 1); break;
        default : return false; break;
        }
    }

    return true;
}

std::string MoveZ::toLAN() const
{
    std::string res = SquareZ(origin).name()+SquareZ(target).name();
    if (promotion)
    {
        res += "=";
        res += PieceZ::name((PieceType) (promotion >> 1));
    }
    return res;
}

std::ostream & operator <<(std::ostream &out, const MoveZ &move)
{
    out << move.toLAN();
    return out;
}






MovePGNZ::MovePGNZ(const MoveZ &move, const LegalMoverZ *mover) : MoveZ(move)
{
    if (fromMove(*this, move, mover)) {}
    else {std::cout << "MovePGN constructor failed!" << std::endl;}
}


std::string MovePGNZ::toPGN() const
{
    std::string out = {};

    if (castleShort) {out += "O-O";}
    else if (castleLong) {out += "O-O-O";}
    else
    {
        if (piecetype==PieceType::PAWN) {if (capture) {out += Square(origin).fileName();}}
        else
        {
            out += Piece::name(piecetype);
            if (pieceOriginAmbiguous)
            {
                if (ambiguityLiftedByFile) out+= Square(origin).fileName();
                else if (ambiguityLiftedByRank) out += Square(origin).rankName();
                else out += Square(origin).name();
            }
        }
        if (capture) out += "x";
        out += Square(target).name();
        if (promotion)
        {
            out += '=';
            out += PieceZ::name((PieceType) (promotion >> 1));
        }
    }
    if (checkmate) out += "#";
    else if (check) out += "+";

    return out;
}

std::string MovePGNZ::toPGN(uint moveNum) const
{
    std::string out = Tools::convertToString(moveNum);
    out += turn ? ". " : "... ";
    out += toPGN();
    return out;
}


bool MovePGNZ::fromPGN(MovePGNZ &res, const std::string &PGNstring, const LegalMoverZ *mover)
{
    MoveZ move;
    if (fromPGN(move, PGNstring, mover)) {return fromMove(res, move, mover);}
    else {return false;}
}

bool MovePGNZ::fromPGN(MoveZ &res, const std::string &PGNstring, const LegalMoverZ *mover)
{

    if (PGNstring.empty()) {std::cout << "Empty string" << std::endl; return false;}

    std::string::size_type pos=0;
    char c = PGNstring[pos];

    uint8f fileIndexOrigin, rankIndexOrigin;
    uint8f fileIndexTarget, rankIndexTarget, squareIndexTarget;
    uint8f origin, target;
    uint8f promotion = 0;
    PieceType piecetype;

    bool turn = mover->turn;

    if (BoardZ::fileIndex(fileIndexOrigin, c))
    {
        ++pos;
        if (!(pos<PGNstring.size())) {std::cout << "String ended prematurely" << std::endl; return false;}
        c=PGNstring[pos];
        if (BoardZ::rankIndex(rankIndexTarget, c))
        {
            fileIndexTarget = fileIndexOrigin;
            squareIndexTarget = SquareZ(fileIndexTarget, rankIndexTarget).getIndex();
            if (turn)
            {
                if ((rankIndexTarget < 2) || (rankIndexTarget > 7)) {std::cout << "wrong rank for pawn move" << std::endl; return false;}
                else if (mover->pieces[squareIndexTarget-8]) {rankIndexOrigin = rankIndexTarget-1;}
                else
                {
                    if (rankIndexTarget != 3) {std::cout << "incorrect pawn move" << std::endl; return false;}
                    rankIndexOrigin = rankIndexTarget-2;
                }
                promotion = (rankIndexTarget==7);
            }
            else
            {
                if (rankIndexTarget > 5) {std::cout << "wrong rank for pawn move" << std::endl; return false;}
                else if (mover->pieces[squareIndexTarget+8]) {rankIndexOrigin = rankIndexTarget+1;}
                else
                {
                    if (rankIndexTarget != 4) {std::cout << "incorrect pawn move" << std::endl; return false;}
                    rankIndexOrigin = rankIndexTarget+2;
                }
                promotion = (rankIndexTarget==0);
            }
        }
        else if (c=='x')
        {
            if ((PGNstring.size()>pos+2) && (BoardZ::fileIndex(fileIndexTarget, PGNstring[pos+1])) && (BoardZ::rankIndex(rankIndexTarget, PGNstring[pos+2])))
            {
                rankIndexOrigin = (turn) ? rankIndexTarget-1 : rankIndexTarget+1;
                promotion = (turn) ? (rankIndexTarget==7) :  (rankIndexTarget==0);
                pos += 2;

            }
            else {std::cout << "incorrect pawn move" << std::endl; return false;}
        }
        else {std::cout << "incorrect pawn (?) move" << std::endl; return false;}

        origin = SquareZ(fileIndexOrigin, rankIndexOrigin).getIndex();
        target = SquareZ(fileIndexTarget, rankIndexTarget).getIndex();


        if (promotion)
        {
            ++pos;
            if (pos<PGNstring.size())
            {
                c = PGNstring[pos];
                if (c=='=')
                {
                    ++pos;
                    if (pos<PGNstring.size())
                    {
                        c = PGNstring[pos];
                        switch (c)
                        {
                        case 'Q' : promotion = 1 + (2 << 1); break;
                        case 'R' : promotion = 1 + (3 << 1); break;
                        case 'B' : promotion = 1 + (4 << 1); break;
                        case 'N' : promotion = 1 + (5 << 1); break;
                        default: {std::cout << "Incorrect promoted piece" << std::endl; return false;}
                        }
                    }
                    else {std::cout << "String ended prematurely" << std::endl; return false;}
                }
                else {std::cout << "Pawn promotion not followed by =" << std::endl; return false;}
            }
            else {std::cout << "Pawn promotion not followed by =" << std::endl; return false;}
        }
    }
    else if (Piece::fromName(piecetype, c))
    {
        if ((c=='E') || (c=='P')) {std::cout << "Incorrect piece name" << std::endl; return false;}
        ++pos;
        if (!(pos<PGNstring.size())) {std::cout << "String ended prematurely" << std::endl; return false;}
        c = PGNstring[pos];
        uint file, rank;
        if (Board::fileIndex(file, c))
        {
            ++pos;
            if (!(pos<PGNstring.size())) {std::cout << "String ended prematurely" << std::endl; return false;}
            c = PGNstring[pos];
            if (Board::rankIndex(rank, c))
            {
                if ((PGNstring.size()==pos+1) || (PGNstring[pos+1]=='+') || (PGNstring[pos+1]=='#'))
                {
                    fileIndexTarget = file;
                    rankIndexTarget = rank;
                    target = SquareZ(fileIndexTarget, rankIndexTarget).getIndex();
                    if (mover->uniqueOrigin(origin, target, piecetype)) {}
                    else {std::cout << "Ambiguous or incorrect move" << std::endl; return false;}
                }
                else if (((PGNstring.size()>pos+2) && SquareZ::fromName(squareIndexTarget, PGNstring.substr(pos+1, 2))) ||
                         ((PGNstring.size()>pos+3) && (PGNstring[pos+1]=='x') && SquareZ::fromName(squareIndexTarget, PGNstring.substr(pos+2, 2))))
                {
                    fileIndexOrigin = file;
                    rankIndexOrigin = rank;
                    origin = SquareZ(fileIndexOrigin, rankIndexOrigin).getIndex();
                    target = squareIndexTarget;
                }
                else {std::cout << "Incorrect move name" << std::endl; return false;}
            }
            else if (((PGNstring.size()>pos+1) && SquareZ::fromName(squareIndexTarget, PGNstring.substr(pos, 2))) ||
                     ((PGNstring.size()>pos+2) && (PGNstring[pos]=='x') && SquareZ::fromName(squareIndexTarget, PGNstring.substr(pos+1, 2))))
            {
                target = squareIndexTarget;
                fileIndexOrigin = file;
                if (mover->uniqueOriginOnFile(origin, target, fileIndexOrigin, piecetype)) {}
                else {std::cout << "Ambiguous or incorrect move" << std::endl; return false;}
            }
            else {std::cout << "Incorrect move name" << std::endl; return false;}
        }
        else if (c=='x')
        {
            if ((PGNstring.size()>pos+2) && SquareZ::fromName(squareIndexTarget, PGNstring.substr(pos+1, 2)))
            {
                target = squareIndexTarget;
                if (mover->uniqueOrigin(origin, target, piecetype)) {}
                else
                {
                    std::cout << "Ambiguous or incorrect move" << std::endl;
                    std::cout << "String was " << PGNstring << std::endl;
                    std::cout << "Position was " << mover->position << std::endl;
                    return false;
                }
            }
            else {std::cout << "Incorrect move name" << std::endl; return false;}
        }
        else if (Board::rankIndex(rank, c))
        {
            if (((PGNstring.size()>pos+2) && SquareZ::fromName(squareIndexTarget, PGNstring.substr(pos+1, 2))) ||
                    ((PGNstring.size()>pos+3) && (PGNstring[pos+1]=='x') && SquareZ::fromName(squareIndexTarget, PGNstring.substr(pos+2, 2))))
            {
                target = squareIndexTarget;
                rankIndexOrigin = rank;
                if (mover->uniqueOriginOnRank(origin, target, rankIndexOrigin, piecetype)) {}
                else {std::cout << "Ambiguous or incorrect move (uniqueOriginOnRank failed)" << std::endl; return false;}
            }
            else {std::cout << "Ambiguous or incorrect move (wrong string continuation)" << std::endl; return false;}
        }
        else {std::cout << "Incorrect piece move" << std::endl; return false;}
    }
    else if ((PGNstring.size()>pos+2) && ((PGNstring.substr(pos, 3)=="O-O") || (PGNstring.substr(pos, 3)=="0-0")))
    {
        origin = (turn) ? SquareZ(4, 0).getIndex() : SquareZ(4, 7).getIndex() ;
        if ((PGNstring.size()>pos+4) && ((PGNstring.substr(pos, 5)=="O-O-O") || (PGNstring.substr(pos, 5)=="0-0-0")))
        {
            target = (turn) ? SquareZ(2, 0).getIndex() : SquareZ(2, 7).getIndex() ;
        }
        else
        {
            target = (turn) ? SquareZ(6, 0).getIndex() : SquareZ(6, 7).getIndex() ;
        }
    }

    res = MoveZ(origin, target, promotion);
    return true;
}

std::ostream & operator <<(std::ostream &out, const MovePGNZ &M)
{
    out << M.toPGN();
    return out;
}

bool MovePGNZ::fromMove(MovePGNZ &res, const MoveZ &move, const LegalMoverZ *mover)
{
    res.origin = move.origin;
    res.target = move.target;
    res.promotion = move.promotion;

    if (!mover->isInLegalMovesList(move)) {return false;}

    if (mover->pieces[res.origin]) {res.piecetype = (PieceType) (mover->pieces[res.origin] >> 2);}
    else
    {
        std::cout << "Error1: Empty origin square" << std::endl;
        std::cout << "Error2: mover->isInLegalMovesList should have prevented Error1" << std::endl;
        return false;
    }

    res.turn = mover->turn;

    uint8f dummy(res.origin);
    res.pieceOriginAmbiguous = (!mover->uniqueOrigin(dummy, res.target, res.piecetype));
    res.ambiguityLiftedByFile = ((res.pieceOriginAmbiguous) && (mover->uniqueOriginOnFile(dummy, res.target, res.origin % 8, res.piecetype)));
    res.ambiguityLiftedByRank = ((res.pieceOriginAmbiguous) && (mover->uniqueOriginOnRank(dummy, res.target, res.origin / 8, res.piecetype)));

    res.capture=mover->isCapture(move);
    res.castleShort=mover->isCastleShort(move);
    res.castleLong=mover->isCastleLong(move);
    res.check = mover->isCheck(move);
    res.checkmate = mover->isCheckmate(move);

    return true;
}

bool MovePGNZ::operator==(const MovePGNZ &other) const
{
    bool res = true;
    res &= other.origin==origin;
    res &= other.target==target;
    res &= other.promotion==promotion;
    res &= other.piecetype==piecetype;
    res &= other.turn==turn;
    res &= other.pieceOriginAmbiguous==pieceOriginAmbiguous;
    res &= other.ambiguityLiftedByFile==ambiguityLiftedByFile;
    res &= other.ambiguityLiftedByRank==ambiguityLiftedByRank;
    res &= other.capture==capture;
    res &= other.castleShort==castleShort;
    res &= other.castleLong==castleLong;
    res &= other.check==check;
    res &= other.checkmate==checkmate;
    return res;
}

