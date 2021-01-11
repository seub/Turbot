#ifndef TYPES_H
#define TYPES_H

#include <array>
#include <chrono>

typedef unsigned int uint;
typedef uint_fast8_t uint8f; // Fastest int type that contains at least 8 bits (for integers 0-255)

typedef std::array<bool, 64> bitboard;

typedef std::array<uint8f, 64> boardZ;
// Each of the 64 square contains an int that has 5 possibly nonzero bits:
// bit #0 is whether the square is nonempty (0 = empty)
// bit #1 is whether the square is occupied by a white piece (0 = empty or black, 1 = white)
// bit #2,3,4 is the type of the piece:
// 000 (decimal : 0) is no piece
// 001 (decimal : 1) is King
// 010 (decimal : 2) is Queen
// 011 (decimal : 3) is Rook
// 100 (decimal : 4) is Bishop
// 101 (decimal : 5) is Pawn

enum class GameResult {NOT_FINISHED, WHITE_WINS, BLACK_WINS, DRAW};
enum class PieceType {EMPTY, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};
//enum class Color {WHITE, BLACK};

#endif // TYPES_H
