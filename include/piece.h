#ifndef PIECE_H
#define PIECE_H

# include "tools.h"



class Piece
{
    friend std::ostream & operator<<(std::ostream &out, const Piece &square);

public:
    Piece() : type(PieceType::EMPTY) {}
    Piece(PieceType type) : type(type) {}
    Piece(PieceType type, Color color) : type(type), color(color) {}
    bool operator==(const Piece &other) const;


    char name() const;
    static char name(PieceType type);
    static bool fromName(PieceType &res, char c);

    Color getColor() const;
    bool isEmpty() const;
    bool isKing() const;
    bool isRook() const;
    bool isPawn() const;
    PieceType getType() const;
    char toFENchar() const;

private:
    static inline const std::array<char, 7> pieceNames = {'E', 'K', 'Q', 'R', 'B', 'N', 'P'};
    static inline const std::array<char, 14> FENpieceNames = {(char) 0, 'K', 'Q', 'R', 'B', 'N', 'P', (char) 0, 'k', 'q', 'r', 'b', 'n', 'p'};
    PieceType type;
    Color color;
};



// See board.h for explanation of the implementation of a piece
class PieceZ
{

public:
    PieceZ(uint8f num = 0) : num(num) {}
    PieceZ(PieceType type, Color color);

    char name() const;
    static char name(PieceType type);
    static bool fromName(PieceType &res, char c);

    bool isWhite() const;
    PieceType getType() const;

    bool isEmpty() const;
    bool isKing() const;
    bool isRook() const;
    bool isPawn() const;
    char toFENchar() const;

private:
    static inline const std::array<char, 7> pieceNames = {'E', 'K', 'Q', 'R', 'B', 'N', 'P'};
    static inline const std::array<char, 14> FENpieceNames = {(char) 0, 'K', 'Q', 'R', 'B', 'N', 'P', (char) 0, 'k', 'q', 'r', 'b', 'n', 'p'};
    uint8f num;
};

#endif // PIECE_H
