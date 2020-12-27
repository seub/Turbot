# include "position.h"
# include "movepicker.h"

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
    bool res = true;

    res &= (board==other.board);
    res &= (turn == other.turn);
    res &= (castlingRights == other.castlingRights);
    res &= (enPassantPossible == other.enPassantPossible);
    res &= (enPassantTargetSquare == other. enPassantTargetSquare);
    res &= (moveNumber == other.moveNumber);
    res &= (nbReversibleHalfMoves == other.nbReversibleHalfMoves);

    return res;
}

void Position::reset()
{
    board.reset();
    turn = Color::WHITE;
    castlingRights.fill(true);
    enPassantPossible = false;
    enPassantTargetSquare = 0;
    moveNumber = 1;
    nbReversibleHalfMoves = 0;
}

void Position::clear()
{
    board.clear();
    turn = Color::WHITE;
    castlingRights.fill(false);
    enPassantPossible = false;
    enPassantTargetSquare = 0;
    moveNumber = 0;
    nbReversibleHalfMoves = 0;
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

bool Position::pickRandomLegalMove(Move &res) const
{
    LegalMover mover(this, true);
    return mover.getRandomLegalMove(res);

}

bool Position::pickBestMove(Move &res, const MovePicker *picker) const
{
    LegalMover mover(this, true);
    return picker->pickMove(res, *this, mover.getlegalMoves(), false, false);
}


std::ostream & operator <<(std::ostream &out, const Position &P)
{
    out << P.printString();
    return out;
}


