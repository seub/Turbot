#ifndef TYPES_H
#define TYPES_H

#include <array>

typedef unsigned int uint;
typedef std::array<bool, 64> bitboard;


enum class GameResult {NOT_FINISHED, WHITE_WINS, BLACK_WINS, DRAW};
enum class PieceType {EMPTY, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};
enum class Color {WHITE, BLACK};


#endif // TYPES_H
