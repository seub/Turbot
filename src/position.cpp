# include "position.h"
# include "movepicker.h"
# include "zobrist.h"






Position::Position()
{
    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {Zobrist::GENERATE_ZOBRIST_NUMBERS();}
}

Position::Position(bool gamestart) : board(Board(gamestart))
{
    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {Zobrist::GENERATE_ZOBRIST_NUMBERS();}

    turn = true;
    castlingRights = gamestart ? 0b1111 : 0;
    enPassant = 0;

    enPassantKing = 0;
    drawClaimable = false;
    hash = recalculateHash(false);

    moveNumber = 1;
    nbReversiblePlies = 0;
    fiftyDraw = false;
    threeFoldDraw = false;
    repetitions.clear();
    repetitions[board] = 1;
}

bool Position::operator==(const Position &other) const
{
    return (other.turn==turn) && (other.castlingRights==castlingRights) && (other.enPassant==enPassant)
            && (enPassantKing==other.enPassantKing) && (other.drawClaimable==drawClaimable) && (other.board==board);
}


std::size_t Position::getHash() const
{
    return hash;
}

void Position::updateDrawClaimable()
{
    bool b = fiftyDraw || threeFoldDraw;
    if (b != drawClaimable)
    {
        drawClaimable = b;
        hash ^= Zobrist::ZOBRIST_DRAW_CLAIMABLE;
    }
}


std::size_t Position::recalculateHash(bool recalculateBoardHash) const
{
    std::size_t res = recalculateBoardHash ? board.recalculateHash() : board.hash;

    if (turn) {res ^= Zobrist::ZOBRIST_TURN;}
    res ^= Zobrist::ZOBRIST_CASTLING_RIGHTS[castlingRights];
    res ^= Zobrist::ZOBRIST_EN_PASSANT[enPassant];

    res ^= Zobrist::ZOBRIST_EN_PASSANT_KING[enPassantKing];
    if (drawClaimable) {res ^= Zobrist::ZOBRIST_DRAW_CLAIMABLE;}

    return res;
}

bool Position::printLinePGN(std::string &res, const std::vector<Move> &line) const
{
    res.clear();
    Position pos(*this), nextPos(false);
    uint N = line.size();
    if (N==0) {return true;}

    Move move;
    std::string moveStr;

    move = line.front();
    if (!pos.printMovePGN(moveStr, move, true)) {return false;}
    else {res += moveStr;}
    if (N==1) {return true;}

    pos.applyMove(nextPos, move);
    pos = nextPos;

    for (uint i=1; i<N-1; ++i)
    {
        move = line[i];
        if (!pos.printMovePGN(moveStr, move, pos.turn)) {return false;}
        else
        {
            res += " ";
            res += moveStr;
        }
        pos.applyMove(nextPos, move);
        pos = nextPos;
    }

    move = line.back();
    if (!pos.printMovePGN(moveStr, move, pos.turn)) {return false;}
    else
    {
        res += " ";
        res += moveStr;
    }

    return true;
}

bool Position::printMovePGN(std::string &res, const Move &move, bool printMoveNumber) const
{
    LegalMover mover(this, true);
    MovePGN movePGN;
    bool success = MovePGN::fromMove(movePGN, move, &mover);
    if (success)
    {
        if (printMoveNumber) {res = movePGN.toPGN(moveNumber);}
        else {res = movePGN.toPGN();}
    }
    return success;
}

void Position::applyMove(Position &res, const Move &move) const
{
    LegalMover(this, false).applyMove(move, res, true);
}


std::string Position::toFENstring() const
{
    std::string res = board.toFENstring();

    res += ' ';
    res += (turn) ? 'w' : 'b';

    res += ' ';
    std::string castlingRightsStr = {};
    if (castlingRights & 1) castlingRightsStr += 'K';
    if (castlingRights & 2) castlingRightsStr += 'Q';
    if (castlingRights & 4) castlingRightsStr += 'k';
    if (castlingRights & 8) castlingRightsStr += 'q';
    res += (castlingRightsStr.empty()) ? "-" : castlingRightsStr;

    res += ' ';
    res += (enPassant & 1)? Square(Square::enPassantSquare(enPassant >> 1, turn)).name() : "-";

    res += ' ';
    res += Tools::convertToString(nbReversiblePlies);

    res += ' ';
    res += Tools::convertToString(moveNumber);

    return res;
}

std::string Position::toLichessURL() const
{
    std::string res = "https://lichess.org/analysis/standard/";
    std::string fen = toFENstring();
    for (auto c : fen)
    {
        res += (c==' ') ? '_' : c;
    }
    return res;
}

bool Position::fromFENstring(Position &res, const std::string &FENstr)
{
    res = Position(false);

    std::stringstream ss(FENstr);
    std::string word;

    if (ss>>word)
    {
        if(!(Board::fromFENstring(res.board, word))) return false;
    }
    else return false;


    if (ss>>word)
    {
        if (word.size()!=1) return false;
        switch (word[0])
        {
        case 'w' : res.turn = true; break;
        case 'b' : res.turn = false; break;
        default : return false;
        }
    }
    else return false;

    if (ss>>word)
    {
        res.castlingRights = 0;
        for (auto letter : word)
        {
            switch (letter)
            {
            case 'K' : res.castlingRights |= 1; break;
            case 'Q' : res.castlingRights |= 2; break;
            case 'k' : res.castlingRights |= 4; break;
            case 'q' : res.castlingRights |= 8; break;
            case '-' : break;
            default : return false;
            }
        }
    }
    else return false;

    if(ss>>word)
    {
        if (word[0] == '-'){res.enPassant = 0;}
        else
        {
            if (word.size()!=2) {return false;}
            res.enPassant = 1 + (Square(word).fileIndex() << 1);
        }
    }
    else {return false;}

    if (ss>>word){res.nbReversiblePlies = std::stoi(word);}
    else {return false;}

    if (ss>>word){res.moveNumber = std::stoi(word);}
    else {return false;}

    if (ss>>word) {return false;}

    return true;
}

