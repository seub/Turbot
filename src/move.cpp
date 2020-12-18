#include "move.h"

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
        res.promotedPiece=PieceType::Empty;
    }
    else
    {
        char c = LANstring[4];
        if (c!='=') return false;
        c = LANstring[5];
        switch (c)
        {
        case 'Q' : res.promotedPiece=PieceType::Queen; break;
        case 'R' : res.promotedPiece=PieceType::Rook; break;
        case 'B' : res.promotedPiece=PieceType::Bishop; break;
        case 'N': res.promotedPiece=PieceType::Knight; break;
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
    if (promotion) res += "=" + Piece(promotedPiece).name();
    return res;
}

Square Move::getOrigin() const
{
    return origin;
}

Square Move::getTarget() const
{
    return target;
}


std::ostream & operator <<(std::ostream &out, const Move &M)
{
    out << M.toLAN();
    return out;
}



MovePGN::MovePGN(const Move &M) : Move(M)
{
    drawOfferBeforeIncluded = false;
    drawOfferAfterIncluded = false;
    moveNumberIncluded = false;
    actualMoveIncluded = false;
    gameResultIncluded = false;
}

std::string MovePGN::toPGN(bool includeMoveNumber, bool includeActualMove,
                           bool includedDrawOfferBefore, bool includeDrawOfferAfter, bool includeGameResult) const
{
    std::string out = {};

    if(includeMoveNumber)
    {
        if (!moveNumberIncluded) throw("Move number not included!");
        out += Tools::convertToString(moveNumber);
        out += (side==Color::White) ? ". " : "... ";
    }

    if (includedDrawOfferBefore)
    {
        if (!drawOfferBeforeIncluded) throw("Draw offer before not included!");
        out += "=";
    }

    if (includeActualMove)
    {
        if (!actualMoveIncluded) throw("Actual move not included!");

        if (castleShort) out += "O-O";
        else if (castleLong) out += "O-O-O";
        else
        {
            if (piecetype==PieceType::Pawn)
            {
                if (capture) out += origin.name()[0];
            }
            else
            {
                out += Piece::name(piecetype);
                if (pieceOriginAmbiguous)
                {
                    if (ambiguityLiftedByFile) out+= origin.name()[0];
                    else if (ambiguityLiftedByRank) out += origin.name()[1];
                    else out += origin.name();
                }
            }
            if (capture) out += "x";
            out += target.name();
        }
        if (checkmate) out += "#";
        else if (check) out += "+";
    }

    if (includeDrawOfferAfter)
    {
        if (!drawOfferAfterIncluded) throw("Draw offer after not included!");
        out += "=";
    }

    if (includeGameResult)
    {
        if (!gameResultIncluded) throw("Game result not included!");
        switch(result)
        {
        case GameResult::WhiteWins : out += " 1-0"; break;
        case GameResult::BlackWins : out += " 0-1"; break;
        case GameResult::Draw : out += " 1/2-1/2"; break;
        default : throw("Wrong game result");
        }
    }

    return out;
}

bool MovePGN::fromPGN(MovePGN &res, const std::string &PGNstring, const Board &board, Color side)
{
    uint moveNum;
    Color inferredSide;
    std::string trunc = PGNstring;
    if (extractMoveNumberAndSide(moveNum, inferredSide, trunc, PGNstring))
    {
        if (side!=inferredSide)
        {
            std::cout << "Value of side inconsistent with inferred value" << std::endl;
            return false;
        }
        res.moveNumberIncluded = true;
        res.moveNumber = moveNum;
    }
    else res.moveNumberIncluded = false;

    return fromPGNwithoutMoveNum(res, trunc, board, side);
}

bool MovePGN::fromPGN(MovePGN &res, const std::string &PGNstring, const Board &board)
{
    uint moveNum;
    Color inferredSide;
    std::string trunc = PGNstring;
    if (extractMoveNumberAndSide(moveNum, inferredSide, trunc, PGNstring))
    {
        res.moveNumberIncluded = true;
        res.moveNumber = moveNum;
        return fromPGNwithoutMoveNum(res, trunc, board, inferredSide);
    }
    else
    {
        std::cout << "Could not determine side from PGN, and was not given side as input" << std::endl;
        //NB: We could try to derive the side from the position and the move, but fuck that
        return false;
    }


}

bool MovePGN::extractMoveNumberAndSide(uint &move, Color &side, std::string &outputTruncatedString, const std::string &PGNstring)
{
    //Check if the move starts with a move number
    std::string::size_type pos=0;
    outputTruncatedString = PGNstring;
    char c;
    int moveNum;
    int moveNumSuccess = Tools::stoi(PGNstring, &moveNum, &pos);
    if (moveNumSuccess==0)
    {
        move=moveNum;
        ++pos;
        c = PGNstring[pos];
        if (c!='.')
        {
            std::cout << "Move number not followed by '.'" << std::endl;
            return false;
        }
        else
        {
            ++pos;
            c=PGNstring[pos];
            if (c=='.')
            {
                side = Color::Black;
                ++pos;
                c=PGNstring[pos];
                if (c=='.')
                {
                    ++pos;
                    c=PGNstring[pos];
                }
                if (c==' ')
                {
                    ++pos;
                    c=PGNstring[pos];
                }
            }
            else
            {
                side = Color::White;
                if (c==' ')
                {
                    ++pos;
                    c=PGNstring[pos];
                }
            }
            outputTruncatedString = PGNstring.substr(pos);
            return true;
        }
    }
    else if (moveNumSuccess==-1)
    {
        return false;
    }
    else
    {
        throw("In MovePGN::fromPGN, Tools::stoi was not happy");
        return false;
    }
}

bool MovePGN::fromPGNwithoutMoveNum(MovePGN &res, const std::string &PGNstring, const Board &board, Color side)
{
    res.side=side;

    std::string::size_type pos=0;
    char c = PGNstring[pos];
    uint fileIndex, rankIndex;

    if (c==' ')
    {
        std::cout << "Extra empty space" << std::endl;
        return false;
    }

    if (c=='=')
    {
        res.drawOfferBeforeIncluded = true;
        ++pos;
        c=PGNstring[pos];
    }
    else
    {
        res.drawOfferBeforeIncluded = false;
    }

    if (Board::fileIndex(fileIndex, c))
    {
        res.piecetype = PieceType::Pawn;
        ++pos;
        c=PGNstring[pos];
        if (Board::rankIndex(rankIndex, c))
        {
            res.capture = false;
            res.target = Square(fileIndex, rankIndex);
            if ()
        }

    }
    else if (Piece::fromName(res.piecetype, c))
    {

    }
    else if ((PGNstring.size()>pos+2) && ((PGNstring.substr(pos, pos+3)=="O-O") || (PGNstring.substr(pos, pos+3)=="0-0")))
    {
        res.actualMoveIncluded = true;
        pos += 3;
        c = PGNstring[pos];
        if ((PGNstring.size()>pos+1) && ((PGNstring.substr(pos, pos+3)=="-O") || (PGNstring.substr(pos, pos+3)=="-0")))
        {
            pos +=2;
            res.castleShort = false;
            res.castleLong = true;
        }
        else
        {
            res.castleShort = true;
            res.castleLong = false;
        }
    }


std::ostream & operator <<(std::ostream &out, const MovePGN &M)
{
    out << M.toPGN();
    return out;
}
