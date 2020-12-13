#ifndef SQUARE_H
#define SQUARE_H

#include "tools.h"

class Square
{
public:
    explicit Square(uint index) : index(index) {assert(index < 64);}
    Square(uint fileIndex, uint rowIndex);

    uint fileIndex() const;
    uint rowIndex() const;
    std::string name() const;

private:
    uint index;
    static inline const std::vector< std::string > filenames = {"a", "b", "c", "d", "e", "f", "g", "h"};
    static inline const std::vector< std::string > rownames = {"1", "2", "3", "4", "5", "6", "7", "8"};
};

#endif // SQUARE_H
