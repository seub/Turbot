#ifndef PIECE_H
#define PIECE_H

# include "tools.h"

enum class PieceType {Null, King, Queen, Rook, Bishop, Knight, Pawn};
enum class Color {White, Black};

class Piece
{
public:
    Piece() : type(PieceType::Null) {}
    Piece(PieceType type, Color color) : type(type), color(color) {}

    bool isNull() const;
    std::string name() const;
    Color getColor() const;
    double value() const;
    char FENchar() const;

private:
    static inline const std::vector< std::string > piecenames = {"", "K", "Q", "R", "B", "N", "P"};
    static inline const std::vector<char> FENpiecenames = {(char) 0, 'K', 'Q', 'R', 'B', 'N', 'P', (char) 0, 'k', 'q', 'r', 'b', 'n', 'p'};
    static inline const std::vector<double> piecevalues = {0, 0, 9.5, 5.63, 3.33, 3.05, 1};
    PieceType type;
    Color color;
};

#endif // PIECE_H
