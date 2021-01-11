#ifndef PIECE_H
#define PIECE_H

# include "tools.h"



// See board.h for explanation of the implementation of a piece
class Piece
{

public:
    Piece(uint8f num = 0) : num(num) {}
    Piece(uint8f type, bool color);
    Piece(PieceType type, bool color);

    char name() const;
    static char name(PieceType type);
    static bool fromName(PieceType &res, char c);

    PieceType getType() const;
    uint8f getNum() const;

    bool isEmpty() const;
    bool isKing() const;
    bool isRook() const;
    bool isPawn() const;
    char toFENchar() const;

private:
    static inline const std::array<char, 7> pieceNames = {'E', 'K', 'Q', 'R', 'B', 'N', 'P'};
    static inline const std::array<char, 14> FENpieceNames = {(char) 0, 'K', 'Q', 'R', 'B', 'N', 'P', (char) 0, 'k', 'q', 'r', 'b', 'n', 'p'};
    uint8f num; // See board.h for explanation of the implementation of a piece
};

#endif // PIECE_H
