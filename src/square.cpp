#include "square.h"
#include "board.h"





Square::Square(uint8f fileIndex, uint8f rankIndex)
{
    assert(fileIndex < 8);
    assert(rankIndex < 8);
    index = fileIndex + 8*rankIndex;
}

Square::Square(const std::string &name)
{
    if (fromName(index, name)) {}
    else {throw ("Bad square name");}
}

bool Square::fromName(uint8f &resIndex, const std::string &name)
{
    if (name.size()==2)
    {
        char fileChar = name[0], rankChar = name[1];
        uint8f fileIndex, rankIndex;
        if (Board::fileIndex(fileIndex, fileChar) && Board::rankIndex(rankIndex, rankChar))
        {
            resIndex = fileIndex + 8*rankIndex;
            return true;
        }
        else {return false;}
    }
    else {return false;}
}

uint8f Square::getIndex() const
{
    return index;
}

uint8f Square::fileIndex() const
{
    return index % 8;
}

uint8f Square::rankIndex() const
{
    return index / 8;
}

char Square::fileName() const
{
    return filenames[fileIndex()];
}

char Square::rankName() const
{
    return ranknames[rankIndex()];
}

std::string Square::name() const
{
    std::string res(1, fileName());
    res += rankName();
    return res;
}

uint8f Square::enPassantSquare(uint8f enPassantFile, bool turn)
{
    uint8f rank = turn ? 5 : 2;
    return 8*rank + enPassantFile;
}

std::ostream & operator <<(std::ostream &out, const Square &square)
{
    out << square.name();
    return out;
}

