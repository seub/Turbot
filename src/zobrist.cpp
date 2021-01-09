#include "zobrist.h"

std::size_t Zobrist::ZOBRIST_BOARD[64][6][2];
std::size_t Zobrist::ZOBRIST_CASTLING_RIGHTS[4];
std::size_t Zobrist::ZOBRIST_EN_PASSANT_FILE[8];
std::size_t Zobrist::ZOBRIST_SIDE;
std::size_t Zobrist::ZOBRIST_DRAW_CLAIMABLE, Zobrist::ZOBRIST_EN_PASSANT_KING_SHORT, Zobrist::ZOBRIST_EN_PASSANT_KING_LONG;

bool Zobrist::ZOBRIST_NUMBERS_GENERATED = false;

void Zobrist::GENERATE_ZOBRIST_NUMBERS()
{
    std::random_device rd;     // Get a random seed from the OS entropy device, or whatever
    std::mt19937_64 eng(rd()); // Use the 64-bit Mersenne Twister 19937 generator and seed it with entropy.
    std::uniform_int_distribution<std::size_t> distr; //Define the distribution

    for (uint i=0; i!=64; ++i) {for (uint j=0; j!=6; ++j) {for (uint k=0; k!=2; ++k) {Zobrist::ZOBRIST_BOARD[i][j][k] = distr(eng);}}}
    for (uint i=0; i!=4; ++i) {Zobrist::ZOBRIST_CASTLING_RIGHTS[i] = distr(eng);}
    for (uint i=0; i!=8; ++i) {Zobrist::ZOBRIST_EN_PASSANT_FILE[i] = distr(eng);}
    Zobrist::ZOBRIST_SIDE = distr(eng);
    Zobrist::ZOBRIST_DRAW_CLAIMABLE = distr(eng);
    Zobrist::ZOBRIST_EN_PASSANT_KING_SHORT = distr(eng);
    Zobrist::ZOBRIST_EN_PASSANT_KING_LONG = distr(eng);

    Zobrist::ZOBRIST_NUMBERS_GENERATED = true;
};






std::array< std::array<std::size_t, 28> , 64 > Zobrist::ZZOBRIST_PIECES;
std::array<std::size_t, 16> Zobrist::ZZOBRIST_CASTLING_RIGHTS;
std::array<std::size_t, 16> Zobrist::ZZOBRIST_EN_PASSANT;
std::array<std::size_t, 4> Zobrist::ZZOBRIST_EN_PASSANT_KING;
std::size_t Zobrist::ZZOBRIST_TURN, Zobrist::ZZOBRIST_DRAW_CLAIMABLE;

bool Zobrist::ZZOBRIST_NUMBERS_GENERATED = false;

void Zobrist::GENERATE_ZZOBRIST_NUMBERS()
{
    std::random_device rd;     // Get a random seed from the OS entropy device, or whatever
    std::mt19937_64 eng(rd()); // Use the 64-bit Mersenne Twister 19937 generator and seed it with entropy.
    std::uniform_int_distribution<std::size_t> distr; //Define the distribution

    for (uint i=0; i<64; ++i) {for (uint j=0; j<28; ++j) {Zobrist::ZZOBRIST_PIECES[i][j] = distr(eng);}}
    for (uint i=0; i<16; ++i) {Zobrist::ZZOBRIST_CASTLING_RIGHTS[i] = distr(eng);}
    for (uint i=0; i<16; ++i) {Zobrist::ZZOBRIST_EN_PASSANT[i] = distr(eng);}
    for (uint i=0; i<4; ++i) {Zobrist::ZZOBRIST_EN_PASSANT_KING[i] = distr(eng);}
    Zobrist::ZZOBRIST_TURN = distr(eng);
    Zobrist::ZZOBRIST_DRAW_CLAIMABLE = distr(eng);

    Zobrist::ZZOBRIST_NUMBERS_GENERATED = true;
};
