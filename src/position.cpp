# include "position.h"

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

Position::Position(const std::string &FENstr) : board(FENstr.substr(0, FENstr.find(' ')))
{
    setFromFENString(FENstr, false);
}

bool Position::operator==(const Position &other) const
{
    bool res = true;

    res &= (board==other.board);
    res &= (turn == other.turn);
    res &= (castlingRights == other.castlingRights);
    res &= (enPassantPossible == other.enPassantPossible);
    res &= (enPassantTargetSquare == other. enPassantTargetSquare);
    res &= (drawOffered == other.drawOffered);
    res &= (moveNumber == other.moveNumber);
    res &= (nbReversibleHalfMovesPlayed == other.nbReversibleHalfMovesPlayed);

    return res;
}

void Position::reset()
{
    board.reset();
    turn = Color::White;
    castlingRights.fill(true);
    enPassantPossible = false;
    enPassantTargetSquare = 0;
    drawOffered = false;
    moveNumber = 1;
    nbReversibleHalfMovesPlayed = 0;
}

void Position::clear()
{
    board.clear();
    turn = Color::White;
    castlingRights.fill(false);
    enPassantPossible = false;
    enPassantTargetSquare = 0;
    drawOffered = false;
    moveNumber = 0;
    nbReversibleHalfMovesPlayed = 0;
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
    res += (turn==Color::White) ? "White" : "Black";

    res += "\n";
    res += "Castling rights (kingside|queenside) : White = ";
    res += castlingRights[0] ? "Yes|" : "No|";
    res += castlingRights[1] ? "Yes" : "No";
    res += ", Black = ";
    res += castlingRights[2] ? "Yes|" : "No|";
    res += castlingRights[3] ? "Yes" : "No";

    res += "\n";
    res += "Draw offered by opponent? ";
    res += drawOffered ? "Yes" : "No";

    res += "\n";
    res += "Number of reversible half-moves played: " + Tools::convertToString(nbReversibleHalfMovesPlayed);
    return res;
}

std::string Position::printFENString() const
{
    std::string res = board.printFENString();

    res += ' ';
    res += (turn==Color::White) ? 'w' : 'b';

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
    res += Tools::convertToString(nbReversibleHalfMovesPlayed);

    res += ' ';
    res += Tools::convertToString(moveNumber);

    return res;
}

bool Position::setFromFENString(const std::string &FENstr, bool setBoard)
{
    bool res = true;
    clear();

    std::stringstream ss(FENstr);
    std::string word;

    if (ss>>word)
    {
        if (setBoard)
        {
            res &= board.setFromFENString(word);
        }
    }
    else return false;


    if (ss>>word)
    {
    res &= (word.size()==1);
    switch (word[0])
    {
    case 'w' : turn = Color::White; break;
    case 'b' : turn = Color::Black; break;
    default : res = false;
    }
    }
    else return false;

    if (ss>>word)
    {
    castlingRights.fill(false);
    for (auto letter : word)
    {
        switch (letter)
        {
        case 'K' : castlingRights[0] = true; break;
        case 'Q' : castlingRights[1] = true; break;
        case 'k' : castlingRights[2] = true; break;
        case 'q' : castlingRights[3] = true; break;
        case '-' : break;
        default : res = false;
        }
    }
    }
    else return false;

    if(ss>>word)
    {
    if (word[0] == '-')
    {
        enPassantPossible = false;
        enPassantTargetSquare = 0;
    }
    else
    {
        res &= (word.size()==2);
        enPassantPossible = true;
        enPassantTargetSquare = Square(word).getIndex();
    }
    }
    else return false;

    if (ss>>word)
    {
    nbReversibleHalfMovesPlayed = std::stoi(word);
    }
    else return false;

    if (ss>>word)
    {
            moveNumber = std::stoi(word);
    }
     else return false;

    if (ss>>word)
    {
        return false;
    }
    else return res;

}

std::ostream & operator <<(std::ostream &out, const Position &P)
{
    out << P.printString();
    return out;
}

