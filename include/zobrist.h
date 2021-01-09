#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "tools.h"

namespace Zobrist
{
extern std::size_t ZOBRIST_BOARD[64][6][2]; // [square][piece type][side to move]
extern std::size_t ZOBRIST_CASTLING_RIGHTS[4]; // W short, W long, B short, B long
extern std::size_t ZOBRIST_EN_PASSANT_FILE[8]; // En passant file where capture is possible
extern std::size_t ZOBRIST_SIDE; // 1=White, 0=Black
extern std::size_t ZOBRIST_DRAW_CLAIMABLE, ZOBRIST_EN_PASSANT_KING_SHORT, ZOBRIST_EN_PASSANT_KING_LONG; //

extern bool ZOBRIST_NUMBERS_GENERATED;

void GENERATE_ZOBRIST_NUMBERS();


extern std::array< std::array<std::size_t, 28> , 64 > ZZOBRIST_PIECES;
extern std::array<std::size_t, 16> ZZOBRIST_CASTLING_RIGHTS;
extern std::array<std::size_t, 16> ZZOBRIST_EN_PASSANT;
extern std::array<std::size_t, 4> ZZOBRIST_EN_PASSANT_KING;
extern std::size_t ZZOBRIST_TURN, ZZOBRIST_DRAW_CLAIMABLE;

extern bool ZZOBRIST_NUMBERS_GENERATED;

void GENERATE_ZZOBRIST_NUMBERS();
}


#endif // ZOBRIST_H
