#ifndef SQUARE_H
#define SQUARE_H

#include "tools.h"

class Square
{
public:
    explicit Square(uint index) : index(index) {assert(index < 64);}
    Square(uint fileIndex, uint rankIndex);
    Square(const std::string &name);
    bool operator==(const Square &other) const {return (index==other.index);}

    uint getIndex() const;
    uint fileIndex() const;
    uint rankIndex() const;
    std::string name() const;

private:
    uint index;
    static inline const std::vector< std::string > filenames = {"a", "b", "c", "d", "e", "f", "g", "h"};
    static inline const std::vector< std::string > ranknames = {"1", "2", "3", "4", "5", "6", "7", "8"};
};

#endif // SQUARE_H
