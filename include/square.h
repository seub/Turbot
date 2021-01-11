#ifndef SQUARE_H
#define SQUARE_H

#include "tools.h"



class Square
{
    friend std::ostream & operator<<(std::ostream &out, const Square &square);

public:
    Square () {}
    explicit Square(uint8f index) : index(index) {assert(index < 64);}
    Square(uint8f fileIndex, uint8f rankIndex);
    Square(const std::string &name);
    bool operator==(const Square &other) const {return (index==other.index);}

    uint8f getIndex() const;
    uint8f fileIndex() const;
    uint8f rankIndex() const;
    std::string name() const;
    char fileName() const;
    char rankName() const;
    static bool fromName(uint8f &resIndex, const std::string &name);
    static uint8f enPassantSquare(uint8f enPassantFile, bool turn);

private:
    uint8f index;
    static inline const std::vector<char> filenames = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    static inline const std::vector<char> ranknames = {'1', '2', '3', '4', '5', '6', '7', '8'};
};

#endif // SQUARE_H
