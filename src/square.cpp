#include "square.h"
#include "board.h"

Square::Square(uint fileIndex, uint rankIndex)
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

bool Square::fromName(uint &resIndex, const std::string &name)
{
    if (name.size()==2)
    {
        char fileChar = name[0], rankChar = name[1];
        uint fileIndex, rankIndex;
        if (Board::fileIndex(fileIndex, fileChar) && Board::rankIndex(rankIndex, rankChar))
        {
            resIndex = fileIndex + 8*rankIndex;
            return true;
        }
        else {return false;}
    }
    else {return false;}
}

uint Square::getIndex() const
{
    return index;
}

uint Square::fileIndex() const
{
    return index % 8;
}

uint Square::rankIndex() const
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

std::ostream & operator <<(std::ostream &out, const Square &square)
{
    out << square.name();
    return out;
}








SquareZ::SquareZ(uint8f fileIndex, uint8f rankIndex)
{
    assert(fileIndex < 8);
    assert(rankIndex < 8);
    index = fileIndex + 8*rankIndex;
}

SquareZ::SquareZ(const std::string &name)
{
    if (fromName(index, name)) {}
    else {throw ("Bad square name");}
}

bool SquareZ::fromName(uint8f &resIndex, const std::string &name)
{
    if (name.size()==2)
    {
        char fileChar = name[0], rankChar = name[1];
        uint8f fileIndex, rankIndex;
        if (BoardZ::fileIndex(fileIndex, fileChar) && BoardZ::rankIndex(rankIndex, rankChar))
        {
            resIndex = fileIndex + 8*rankIndex;
            return true;
        }
        else {return false;}
    }
    else {return false;}
}

uint8f SquareZ::getIndex() const
{
    return index;
}

uint8f SquareZ::fileIndex() const
{
    return index % 8;
}

uint8f SquareZ::rankIndex() const
{
    return index / 8;
}

char SquareZ::fileName() const
{
    return filenames[fileIndex()];
}

char SquareZ::rankName() const
{
    return ranknames[rankIndex()];
}

std::string SquareZ::name() const
{
    std::string res(1, fileName());
    res += rankName();
    return res;
}

std::ostream & operator <<(std::ostream &out, const SquareZ &square)
{
    out << square.name();
    return out;
}

