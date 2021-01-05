# include "position.h"
# include "boardhelper.h"
# include "movepicker.h"
# include "zobrist.h"

Position::Position(bool gamestart) : board(gamestart)
{
    if (gamestart)
    {
        reset();
    }
    else
    {
        clear();
    }
}


bool Position::operator==(const Position &other) const
{
    return isEqual(other);
}

bool Position::isEqual(const Position &other) const
{
    return (board==other.board) && (turn == other.turn) && (castlingRights == other.castlingRights) && (enPassantPossible == other.enPassantPossible)
            && (enPassantTargetSquare == other. enPassantTargetSquare) && (moveNumber == other.moveNumber) && (nbReversibleHalfMoves == other.nbReversibleHalfMoves);
}

void Position::reset()
{
    board.reset();
    turn = Color::WHITE;
    castlingRights.fill(true);
    commonReset();
}

void Position::clear()
{
    board.clear();
    turn = Color::WHITE;
    castlingRights.fill(false);
    commonReset();
}

void Position::commonReset()
{
    enPassantPossible = false;
    enPassantTargetSquare = 0;
    moveNumber = 1;
    nbReversibleHalfMoves = 0;

    enPassantKingCapturePossibleK = false;
    enPassantKingCapturePossibleQ = false;
    pastBoards = {board};
    drawOffered = false;
}

std::string Position::printString() const
{
    std::string res = board.printString();

    res += "\n";
    res += "Move number: ";
    res += Tools::convertToString(moveNumber);
    res += ".";

    res += "\n";
    res += "Side to play: ";
    res += (turn==Color::WHITE) ? "White" : "Black";

    res += "\n";
    res += "Castling rights (kingside|queenside) : White = ";
    res += castlingRights[0] ? "Yes|" : "No|";
    res += castlingRights[1] ? "Yes" : "No";
    res += ", Black = ";
    res += castlingRights[2] ? "Yes|" : "No|";
    res += castlingRights[3] ? "Yes" : "No";

    res += "\n";
    res += "Number of reversible half-moves played: " + Tools::convertToString(nbReversibleHalfMoves);
    return res;
}

std::string Position::toFENstring() const
{
    std::string res = board.toFENstring();

    res += ' ';
    res += (turn==Color::WHITE) ? 'w' : 'b';

    res += ' ';
    std::string castlingRightsStr = {};
    if (castlingRights[0]) castlingRightsStr += 'K';
    if (castlingRights[1]) castlingRightsStr += 'Q';
    if (castlingRights[2]) castlingRightsStr += 'k';
    if (castlingRights[3]) castlingRightsStr += 'q';
    res += (castlingRightsStr.empty()) ? "-" : castlingRightsStr;

    res += ' ';
    res += (enPassantPossible)? Square(enPassantTargetSquare).name() : "-";

    res += ' ';
    res += Tools::convertToString(nbReversibleHalfMoves);

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
    res.clear();

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
        case 'w' : res.turn = Color::WHITE; break;
        case 'b' : res.turn = Color::BLACK; break;
        default : return false;
        }
    }
    else return false;

    if (ss>>word)
    {
        res.castlingRights.fill(false);
        for (auto letter : word)
        {
            switch (letter)
            {
            case 'K' : res.castlingRights[0] = true; break;
            case 'Q' : res.castlingRights[1] = true; break;
            case 'k' : res.castlingRights[2] = true; break;
            case 'q' : res.castlingRights[3] = true; break;
            case '-' : break;
            default : return false;
            }
        }
    }
    else return false;

    if(ss>>word)
    {
        if (word[0] == '-')
        {
            res.enPassantPossible = false;
            res.enPassantTargetSquare = 0;
        }
        else
        {
            if (word.size()!=2) return false;
            res.enPassantPossible = true;
            res.enPassantTargetSquare = Square(word).getIndex();
        }
    }
    else return false;

    if (ss>>word)
    {
        res.nbReversibleHalfMoves = std::stoi(word);
    }
    else return false;

    if (ss>>word)
    {
        res.moveNumber = std::stoi(word);
    }
    else return false;

    if (ss>>word) return false;
    return true;
}


bool Position::applyMove(Position &res, const Move &m, bool checkLegal, bool checkKCLegal) const
{
    LegalMover mover(this, true);

    return mover.applyMove(res, m, checkLegal, checkKCLegal);
}

bool Position::getLegalMoves(std::vector<Move> &res) const
{
    LegalMover mover(this, true);
    res = mover.getlegalMoves();
    if(res.size()==0) return false;
    return true;
}

bool Position::getKCLegalMoves(std::vector<Move> &res) const
{
    LegalMover mover(this, false);
    res = mover.getKCLegalMoves();
    if(res.size() ==0) return false;
    return true;
}

bool Position::pickBestMove(Move &res, bool &bestMoveIsForceDraw, MovePicker *picker) const
{
    return picker->pickMove(res, bestMoveIsForceDraw, *this);
}


std::ostream & operator <<(std::ostream &out, const Position &P)
{
    out << P.printString();
    return out;
}

bool Position::printPGN(std::string &res, const Move &move, bool printMoveNumber) const
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

bool Position::printPGN(std::string &res, const std::vector<Move> &line) const
{
    res.clear();
    Position pos(*this), nextPos;
    uint N = line.size();
    if (N==0) {return true;}

    Move move;
    std::string moveStr;

    move = line.front();
    if (!pos.printPGN(moveStr, move, true)) {return false;}
    else {res += moveStr;}
    if (N==1) {return true;}

    if (pos.applyMove(nextPos, move)) {pos = nextPos;}
    else {return false;}

    for (uint i=1; i<N-1; ++i)
    {
        move = line[i];
        if (!pos.printPGN(moveStr, move, pos.getTurn()==Color::WHITE)) {return false;}
        else
        {
            res += " ";
            res += moveStr;
        }
        if (pos.applyMove(nextPos, move)) {pos = nextPos;}
        else {return false;}
    }

    move = line.back();
    if (!pos.printPGN(moveStr, move, pos.getTurn()==Color::WHITE)) {return false;}
    else
    {
        res += " ";
        res += moveStr;
    }

    return true;
}

bool Position::threeFoldRepetition() const
{
    return Tools::containsTriplicates(pastBoards);
}

bool Position::drawCanBeClaimed() const
{
    return (drawOffered || threeFoldRepetition() || (nbReversibleHalfMoves > 49));
}

std::size_t Position::getHash() const
{
    return ZobristPosition(this, BoardHelper(&board)).getHash();
}


ZobristPosition::ZobristPosition(const Position *position, const BoardHelper &helper)
{
    for (uint k=0; k!=64; ++k)
    {
        pieces[0][0][k] = helper.kings[k] && helper.whitePieces[k];
        pieces[0][1][k] = helper.kings[k] && helper.blackPieces[k];
        pieces[1][0][k] = helper.queens[k] && helper.whitePieces[k];
        pieces[1][1][k] = helper.queens[k] && helper.blackPieces[k];
        pieces[2][0][k] = helper.rooks[k] && helper.whitePieces[k];
        pieces[2][1][k] = helper.rooks[k] && helper.blackPieces[k];
        pieces[3][0][k] = helper.bishops[k] && helper.whitePieces[k];
        pieces[3][1][k] = helper.bishops[k] && helper.blackPieces[k];
        pieces[4][0][k] = helper.knights[k] && helper.whitePieces[k];
        pieces[4][1][k] = helper.knights[k] && helper.blackPieces[k];
        pieces[5][0][k] = helper.pawns[k] && helper.whitePieces[k];
        pieces[5][1][k] = helper.pawns[k] && helper.blackPieces[k];
    }

    castlingRights[0] = position->castlingRights[0];
    castlingRights[1] = position->castlingRights[1];
    castlingRights[2] = position->castlingRights[2];
    castlingRights[3] = position->castlingRights[3];

    for (uint i=0; i!=8; ++i) {enPassantFile[i] = false;}
    if (position->enPassantPossible)
    {
        uint enPassantFileIndex = Square(position->enPassantTargetSquare).fileIndex();
        enPassantFile[enPassantFileIndex] = true;
    }

    side = (position->turn==Color::WHITE);
}

std::size_t ZobristPosition::getHash() const
{
    std::size_t res = 0;

    if (!Zobrist::ZOBRIST_NUMBERS_GENERATED) {throw("Zobrist numbers have not been generated");}

    for (uint i=0; i!=6; ++i) {for (uint j=0; j!=2; ++j) {for (uint k=0; k!=64; ++k) {if (pieces[i][j][k]) {res ^= Zobrist::ZOBRIST_PIECES[i][j][k];}}}}
    for (uint i=0; i!=4; ++i) {if (castlingRights[i]) {res ^= Zobrist::ZOBRIST_CASTLING_RIGHTS[i];}}
    for (uint i=0; i!=8; ++i) {if (enPassantFile[i]) {res ^= Zobrist::ZOBRIST_EN_PASSANT_FILE[i];}}
    if (side) {res ^= Zobrist::ZOBRIST_SIDE;}

    return res;
}




