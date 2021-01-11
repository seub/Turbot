#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "tools.h"

namespace Zobrist
{
extern std::array< std::array<std::size_t, 28> , 64 > ZOBRIST_PIECES;
extern std::array<std::size_t, 16> ZOBRIST_CASTLING_RIGHTS;
extern std::array<std::size_t, 16> ZOBRIST_EN_PASSANT;
extern std::array<std::size_t, 4> ZOBRIST_EN_PASSANT_KING;
extern std::size_t ZOBRIST_TURN, ZOBRIST_DRAW_CLAIMABLE;

extern bool ZOBRIST_NUMBERS_GENERATED;

void GENERATE_ZOBRIST_NUMBERS();
}


#endif // ZOBRIST_H
