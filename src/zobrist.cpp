#include "zobrist.h"

std::size_t Zobrist::ZOBRIST_BOARD[6][2][64];
std::size_t Zobrist::ZOBRIST_CASTLING_RIGHTS[4];
std::size_t Zobrist::ZOBRIST_EN_PASSANT_FILE[8];
std::size_t Zobrist::ZOBRIST_SIDE;
std::size_t Zobrist::ZOBRIST_DRAW_CLAIMABLE;

bool Zobrist::ZOBRIST_NUMBERS_GENERATED = false;

void Zobrist::GENERATE_ZOBRIST_NUMBERS()
{
    std::random_device rd;     // Get a random seed from the OS entropy device, or whatever
    std::mt19937_64 eng(rd()); // Use the 64-bit Mersenne Twister 19937 generator and seed it with entropy.
    std::uniform_int_distribution<std::size_t> distr; //Define the distribution

    for (uint i=0; i!=6; ++i) {for (uint j=0; j!=2; ++j) {for (uint k=0; k!=64; ++k) {Zobrist::ZOBRIST_BOARD[i][j][k] = distr(eng);}}}
    for (uint i=0; i!=4; ++i) {Zobrist::ZOBRIST_CASTLING_RIGHTS[i] = distr(eng);}
    for (uint i=0; i!=8; ++i) {Zobrist::ZOBRIST_EN_PASSANT_FILE[i] = distr(eng);}
    Zobrist::ZOBRIST_SIDE = distr(eng);
    Zobrist::ZOBRIST_DRAW_CLAIMABLE = distr(eng);

    Zobrist::ZOBRIST_NUMBERS_GENERATED = true;
};
