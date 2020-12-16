#ifndef BOARDHELPER_H
#define BOARDHELPER_H

#include "piece.h"

class Board;

class BoardHelper
{
public:
    explicit BoardHelper(const Board * const board);
    BoardHelper() = delete;
    BoardHelper(const BoardHelper &) = delete;
    BoardHelper & operator=(BoardHelper) = delete;

private:
    void updateBitboards();

    bitboard whiteKings, whiteQueens, whiteRooks, whiteBishops, whiteKnights,  whitePawns;
    bitboard blackKings, blackQueens, blackRooks, blackBishops, blackKnights,  blackPawns;
    bitboard whitePieces, blackPieces, occupiedSquares, emptySquares;
    const Board * const  board;
};

#endif // BOARDHELPER_H
