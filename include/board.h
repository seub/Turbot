#ifndef BOARD_H
#define BOARD_H

# include "tools.h"
# include "piece.h"
# include "square.h"
# include "move.h"


class Board
{
    friend class Position;
    friend class LegalMover;

public:
    Board();
    Board(bool gamestart);
    bool operator==(Board const& other) const;

    bool getKingSquare(uint8f &res, bool side) const;
    bool hasPiece(uint8f square, bool color) const; // true = White, false = Black
    bool isKing(uint8f square) const;
    bool isRook(uint8f square) const;
    bool isPawn(uint8f square) const;


    std::string toFENstring() const;
    static bool fromFENstring(Board &res, const std::string &str);

    static bool fileIndex(uint8f &res, const char &fileName);
    static bool rankIndex(uint8f &res, const char &rankName);

    std::size_t getHash() const {return hash;}

private:
    void clear();
    void reset();

    void addPiece(uint8f square, uint8f piece);
    void removePiece(uint8f square);

    std::size_t recalculateHash() const;

    std::size_t hash;
    boardZ pieces;
};

// typedef std::array<uint8f, 64> boardZ;
// Each of the 64 square contains an int that has 5 possibly nonzero bits:
// bit #0 is whether the square is nonempty (0 = empty)
// bit #1 is whether the square is occupied by a white piece (0 = empty or black, 1 = white)
// bit #2,3,4 is the type of the piece:
// 000 (decimal 0) is Empty
// 001 (decimal 1) is King
// 010 (decimal 2) is Queen
// 011 (decimal 3) is Rook
// 100 (decimal 4) is Bishop
// 101 (decimal 5) is Knight
// 110 (decimal 6) is Pawn
//
// MEMO:
// White King   = 00111 = 7
// White Queen  = 01011 = 11
// White Rook   = 01111 = 15
// White Bishop = 10011 = 19
// White Knight = 10111 = 23
// White Pawn   = 11011 = 27
// Black King   = 00101 = 5
// Black Queen  = 01001 = 9
// Black Rook   = 01101 = 13
// Black Bishop = 10001 = 17
// Black Knight = 10101 = 21
// Black Pawn   = 11001 = 25

namespace std
{
template<> struct hash<Board>
{
    std::size_t operator()(Board const& board) const noexcept
    {
        return board.getHash();
    }
};
}


#endif // BOARD_H
