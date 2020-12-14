#include "square.h"

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
    switch (fileChar)
    {
    case 'a' : fileIndex = 0; break;
    case 'b' : fileIndex = 1; break;
    case 'c' : fileIndex = 2; break;
    case 'd' : fileIndex = 3; break;
    case 'e' : fileIndex = 4; break;
    case 'f' : fileIndex = 5; break;
    case 'g' : fileIndex = 6; break;
    case 'h' : fileIndex = 7; break;
    default : throw("bad square name"); break;
    }
    switch (rankChar)
    {
    case '1' : rankIndex = 0; break;
    case '2' : rankIndex = 1; break;
    case '3' : rankIndex = 2; break;
    case '4' : rankIndex = 3; break;
    case '5' : rankIndex = 4; break;
    case '6' : rankIndex = 5; break;
    case '7' : rankIndex = 6; break;
    case '8' : rankIndex = 7; break;
    default : throw("bad square name"); break;
    }

    index = fileIndex + 8*rankIndex;
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
