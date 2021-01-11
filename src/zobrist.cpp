#include "zobrist.h"




std::array< std::array<std::size_t, 28> , 64 > Zobrist::ZOBRIST_PIECES;
std::array<std::size_t, 16> Zobrist::ZOBRIST_CASTLING_RIGHTS;
std::array<std::size_t, 16> Zobrist::ZOBRIST_EN_PASSANT;
std::array<std::size_t, 4> Zobrist::ZOBRIST_EN_PASSANT_KING;
std::size_t Zobrist::ZOBRIST_TURN, Zobrist::ZOBRIST_DRAW_CLAIMABLE;

bool Zobrist::ZOBRIST_NUMBERS_GENERATED = false;

void Zobrist::GENERATE_ZOBRIST_NUMBERS()
{
    std::random_device rd;     // Get a random seed from the OS entropy device, or whatever
    std::mt19937_64 eng(rd()); // Use the 64-bit Mersenne Twister 19937 generator and seed it with entropy.
    std::uniform_int_distribution<std::size_t> distr; //Define the distribution

    for (uint i=0; i<64; ++i) {for (uint j=0; j<28; ++j) {Zobrist::ZOBRIST_PIECES[i][j] = distr(eng);}}
    for (uint i=0; i<16; ++i) {Zobrist::ZOBRIST_CASTLING_RIGHTS[i] = distr(eng);}
    for (uint i=0; i<16; ++i) {Zobrist::ZOBRIST_EN_PASSANT[i] = distr(eng);}
    for (uint i=0; i<4; ++i) {Zobrist::ZOBRIST_EN_PASSANT_KING[i] = distr(eng);}
    Zobrist::ZOBRIST_TURN = distr(eng);
    Zobrist::ZOBRIST_DRAW_CLAIMABLE = distr(eng);

    Zobrist::ZOBRIST_NUMBERS_GENERATED = true;
};
