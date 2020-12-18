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
    assert(name.size() == 2);
    char fileChar = name[0], rankChar = name[1];
    uint fileIndex, rankIndex;
    if (Board::fileIndex(fileIndex, fileChar) && Board::rankIndex(rankIndex, rankChar)) index = fileIndex + 8*rankIndex;
    else throw ("Bad square name");
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

std::string Square::name() const
{
    return filenames[fileIndex()]+ranknames[rankIndex()];
}
