#ifndef MOVER_H
#define MOVER_H

#include "square.h"
#include "position.h"
#include "move.h"



class Mover
{
public:
    explicit Mover(const Position * const position, bool generateLegalMoves = true);
    Mover() = delete;
    Mover(const Mover &) = delete;
    Mover & operator=(Mover) = delete;

    std::string printKCMoves() const;
    bool isCapture(const Move &m) const;
    bool isPawnMove(const Move &m) const;
    bool isReversible(const Move &m) const;


    Position applyKCMove(const Move &m) const;

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
    void addkcLegalMovesRow(uint i, const std::vector<uint> row);

    std::vector<Move> kCLegalMoves; // We call "KC Chess" the variation of chess where king capture is allowed. It is convenient to first consider this case.
    bitboard squaresAttacked; //NB: We put by convention that if en passant is possible, the e.p. target square is "under attack", but it does not matter (the opponent king cannot be on that square)
    std::vector<Move> legalMoves;
    bool opponentKsideCastleUnderAttack, opponentQsideCastleUnderAttack;
    bool opponentKingUnderAttack;

    const Position * const position;
    const BoardHelper boardHelper;
};

#endif // MOVER_H
