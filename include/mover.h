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

    std::string printLegalMoves() const;
    std::vector<Move> getlegalMoves() const {return legalMoves;}
    bool isCapture(const Move &m) const;
    bool isPawnMove(const Move &m) const;
    bool isReversible(const Move &m) const;
    bool isInLegalMovesList(const Move &m) const;

    Position applyKCMove(const Move &m) const;

private:
    bool initialize();
    bool updateKCLegalMoves();
    void updateLegalMoves();
    bool isLegalConstruct(const Move &m, bool checkKClegal = true);

    void addkcLegalMove(uint origin, uint target, bool attack=true);
    void addkcLegalMovePromotions(uint origin, uint target, bool attack=false);
    void addkcLegalMovesKing(uint i);
    void addkcLegalMovesQueen(uint i);
    void addkcLegalMovesRook(uint i);
    void addkcLegalMovesBishop(uint i);
    void addkcLegalMovesKnight(uint i);
    void addkcLegalMovesPawn(uint i);
    void addkcLegalMovesRow(uint i, const std::vector<uint> row);

    std::vector<Move> kCLegalMoves, legalMoves; // We call "KC Chess" the variation of chess where king capture is allowed.
    bool kCLegalMovesGenerated, legalMovesGenerated;
    bitboard squaresAttacked;
    bool opponentKsideCastleUnderAttack, opponentQsideCastleUnderAttack;
    bool opponentKingUnderAttack;

    const Position * const position;
    const BoardHelper boardHelper;
};

#endif // MOVER_H
