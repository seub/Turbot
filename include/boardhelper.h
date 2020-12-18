#ifndef BOARDHELPER_H
#define BOARDHELPER_H

#include "piece.h"
#include "square.h"

class Board;

class BoardHelper
{
    friend class Mover;

public:
    explicit BoardHelper(const Board * const board);
    BoardHelper() = delete;
    BoardHelper(const BoardHelper &) = delete;
    BoardHelper & operator=(BoardHelper) = delete;

    bool firstOccurrence(Square &res, const Piece &piece) const;
    bool firstOccurrenceOnFile(Square &res, uint file, const Piece &piece) const;
    bool firstOccurrenceOnRank(Square &res, uint rank, const Piece &piece) const;

private:
    void updateBitboards();

    bitboard kings, queens, rooks, bishops, knights, pawns;
    bitboard whitePieces, blackPieces, occupiedSquares;
    const Board * const  board;
};

#endif // BOARDHELPER_H
