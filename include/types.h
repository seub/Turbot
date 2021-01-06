#ifndef TYPES_H
#define TYPES_H

#include <array>

typedef unsigned int uint;
typedef std::array<bool, 64> bitboard;

enum class GameResult {NOT_FINISHED, WHITE_WINS, BLACK_WINS, DRAW};
enum class PieceType {EMPTY, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};
enum class Color {WHITE, BLACK};

typedef bool boardZ[6][2][64]; // [piece type][side to move][square]
// Order of Pieces: 0=KING, 1=QUEEN, 2=ROOK, 3=BISHOP, 4=KNIGHT, 5=PAWN
// Order of 'side to move': 0=Black, 1=White
// BEWARE! THIS IS NOT THE SAME AS THE TYPEDEFS ABOVE!


#endif // TYPES_H
