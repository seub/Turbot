#ifndef LEGALMOVER_H
#define LEGALMOVER_H


# include "move.h"

class Position;

class LegalMover
{
    friend class MovePGN;
    friend class Position;

public:
    explicit LegalMover(const Position * const position, bool generateLegalMoves = true);
    LegalMover() = delete;
    LegalMover(const LegalMover &) = delete;
    LegalMover & operator=(LegalMover) = delete;

    std::string printLegalMoves() const;
    std::string printKCLegalMoves() const;
    std::vector<Move> getlegalMoves() const {return legalMoves;}
    bool getRandomLegalMove(Move &res) const;
    bool isCapture(const Move &move) const;
    bool isPawnMove(const Move &move) const;
    bool isKingMove(const Move &move) const;
    bool isReversible(const Move &move) const;
    bool isInLegalMovesList(const Move &move) const;
    bool isInKCLegalMovesList(const Move &move) const;
    bool isCheck() const;
    bool isCheck(const Move &move) const;
    bool isCheckmate() const;
    bool isCheckmate(const Move &move) const;
    bool isStalemate() const;
    bool isCastleShort(const Move &move) const;
    bool isCastleLong(const Move &move) const;
    Color turn() const;

    bool uniqueOrigin(Square &resOrigin, const Square &target, PieceType type) const;
    bool uniqueOriginOnFile(Square &resOrigin, const Square &target, uint fileIndex, PieceType type) const;
    bool uniqueOriginOnRank(Square &resOrigin, const Square &target, uint rankIndex, PieceType type) const;

    bool applyMove(Position &res, const Move &m, bool checkLegal = false, bool checkKCLegal = false) const;

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

    Position applyMove(const Move &m) const;

    std::vector<Move> kCLegalMoves, legalMoves; // We call "KC Chess" the variation of chess where king capture is allowed.
    bool kCLegalMovesGenerated, legalMovesGenerated;
    bitboard squaresAttacked;
    bool opponentKsideCastleUnderAttack, opponentQsideCastleUnderAttack;
    bool opponentKingUnderAttack;

    const Position * const position;
    const BoardHelper boardHelper;
};

#endif // LEGALMOVER_H
