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

private:
    static inline const std::vector< std::string > piecenames = {"", "K", "Q", "R", "B", "N", "p"};
    PieceType type;
    Color color;
};

#endif // PIECE_H
