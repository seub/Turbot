#ifndef BOARD_H
#define BOARD_H

# include "tools.h"
# include "piece.h"
# include "square.h"
# include "boardhelper.h"

class Board
{
    friend class Position;
    friend class BoardHelper;
    friend std::ostream & operator<<(std::ostream &out, const Board &B);

public:
    Board(bool gamestart=false);
    Board(const std::string &FENstr);

    bool operator==(Board const& other) const;

    std::string printString() const;
    std::string printFENString() const;


private:
    void clear();
    void reset();
    bool setFromFENString(const std::string &str);

    std::array<Piece, 64> pieces;
};

#endif // BOARD_H
