#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "tools.h"

namespace Zobrist
{
extern std::size_t ZOBRIST_BOARD[6][2][64]; // [piece type][side to move][square]
extern std::size_t ZOBRIST_CASTLING_RIGHTS[4]; // W short, W long, B short, B long
extern std::size_t ZOBRIST_EN_PASSANT_FILE[8]; // En passant file where capture is possible
extern std::size_t ZOBRIST_SIDE; // 1=White, 0=Black
extern std::size_t ZOBRIST_DRAW_CLAIMABLE; //

extern bool ZOBRIST_NUMBERS_GENERATED;

void GENERATE_ZOBRIST_NUMBERS();
}


#endif // ZOBRIST_H
