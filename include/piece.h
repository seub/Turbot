#ifndef PIECE_H
#define PIECE_H

# include "tools.h"

enum class PieceType {Empty, King, Queen, Rook, Bishop, Knight, Pawn};
enum class Color {White, Black};

class Piece
{
    friend std::ostream & operator<<(std::ostream &out, const Piece &P);

public:
    Piece() : type(PieceType::Empty) {}
    Piece(PieceType type, Color color) : type(type), color(color) {}
    bool operator==(const Piece &other) const;

    bool isNull() const;
    std::string name() const;
    Color getColor() const;
    char FENchar() const;

private:
    static inline const std::vector< std::string > piecenames = {"E", "K", "Q", "R", "B", "N", "P"};
    static inline const std::vector<char> FENpiecenames = {(char) 0, 'K', 'Q', 'R', 'B', 'N', 'P', (char) 0, 'k', 'q', 'r', 'b', 'n', 'p'};
    PieceType type;
    Color color;
};

#endif // PIECE_H
