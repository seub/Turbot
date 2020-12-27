#ifndef BOARDHELPER_H
#define BOARDHELPER_H

#include "piece.h"
#include "square.h"
#include <boost/multiprecision/cpp_int.hpp>

class Board;

class BoardHelper
{
    friend class Mover;
    friend class MovePGN;

public:
    explicit BoardHelper(const Board * const board);
    BoardHelper() = delete;
    BoardHelper(const BoardHelper &) = delete;
    BoardHelper & operator=(BoardHelper) = delete;
    boost::multiprecision::uint512_t getFullboard() { return fullboard;}
private:
    void updateBitboards();
    boost::multiprecision::uint512_t fullboard;
    bitboard kings, queens, rooks, bishops, knights, pawns;
    bitboard whitePieces, blackPieces, occupiedSquares;
    const Board * const  board;
};

#endif // BOARDHELPER_H
