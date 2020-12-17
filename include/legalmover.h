#ifndef LEGALMOVER_H
#define LEGALMOVER_H

#include "square.h"
#include "position.h"
#include "move.h"



class LegalMover
{
public:
    explicit LegalMover(const Position * const position, bool generateLegalMoves = true);
    LegalMover() = delete;
    LegalMover(const LegalMover &) = delete;
    LegalMover & operator=(LegalMover) = delete;

private:
    bool initialize();
    bool updatekcLegalMoves();
    void updateLegalMoves();


    void addkcLegalMove(uint origin, uint target, bool attack=true);
    void addkcLegalMovesKing(uint i);
    void addkcLegalMovesQueen(uint i);
    void addkcLegalMovesRook(uint i);
    void addkcLegalMovesBishop(uint i);
    void addkcLegalMovesKnight(uint i);
    void addkcLegalMovesPawn(uint i);

    std::vector<Move> kcLegalMoves; // We call "KC Chess" the variation of chess where king capture is allowed. It is convenient to first consider this case.
    bitboard squaresAttacked;
    std::vector<Move> legalMoves;
    bool opponentKsideCastleUnderAttack, opponentQsideCastleUnderAttack;
    bool opponentKingUnderAttack;

    const Position * const position;
    const BoardHelper boardHelper;
};

#endif // LEGALMOVER_H
