#ifndef POSITIONHELPER_H
#define POSITIONHELPER_H

#include "piece.h"
class Position;

class PositionHelper
{
public:
    explicit PositionHelper(const Position * const position);
    PositionHelper() = delete;
    PositionHelper(const PositionHelper &) = delete;
    PositionHelper & operator=(PositionHelper) = delete;

private:
    void updateBitboards();

    bitboard whiteKings, whiteQueens, whiteRooks, whiteBishops, whiteKnights,  whitePawns;
    bitboard blackKings, blackQueens, blackRooks, blackBishops, blackKnights,  blackPawns;
    bitboard whitePieces, blackPieces, occupiedSquares, emptySquares;
    const std::array<Piece, 64> * const  pieces;
};

#endif // POSITIONHELPER_H
