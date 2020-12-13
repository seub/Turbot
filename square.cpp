#include "square.h"

Square::Square(uint fileIndex, uint rowIndex)
{
    assert(fileIndex < 8);
    assert(rowIndex < 8);
    index = fileIndex + 8*rowIndex;
}

uint Square::fileIndex() const
{
    return index % 8;
}

uint Square::rowIndex() const
{
    return index / 8;
}

std::string Square::name() const
{
    return filenames[fileIndex()]+rownames[rowIndex()];
}
