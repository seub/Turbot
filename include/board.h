#ifndef BOARD_H
#define BOARD_H

# include "tools.h"
# include "piece.h"
# include "square.h"
# include "boardhelper.h"
# include "move.h"

class Board
{
    friend class Position;
    friend class BoardHelper;
    friend class Mover;
    friend std::ostream & operator<<(std::ostream &out, const Board &B);

public:
    Board(bool gamestart=false);

    bool operator==(Board const& other) const;

    std::string printString() const;
    std::string toFENstring() const;
    static bool fromFENstring(Board &res, const std::string &str);

    static bool fileIndex(uint &res, const char &c);
    static bool rankIndex(uint &res, const char &c);


private:
    void clear();
    void reset();

    std::array<Piece, 64> pieces;
};

#endif // BOARD_H
