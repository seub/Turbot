#ifndef PIECE_H
#define PIECE_H


enum class PieceType {Null, King, Queen, Rook, Bishop, Knight, Pawn};
enum class Color {White, Black};

class Piece
{
public:
    Piece() : type(PieceType::Null) {}
    Piece(PieceType type, Color color) : type(type), color(color) {}

private:
    PieceType type;
    Color color;
};

#endif // PIECE_H
