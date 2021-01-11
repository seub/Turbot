#ifndef LEGALMOVER_H
#define LEGALMOVER_H


# include "move.h"





class Position;

class LegalMover
{
    friend class MovePGN;
    friend class Position;

public:
    explicit LegalMover(const Position *position, bool generateLegalMoves = true);
    LegalMover() = delete;
    LegalMover(const LegalMover &) = delete;
    LegalMover & operator=(LegalMover) = delete;

    bool isCapture(const Move &move) const;
    bool isEnPassantPawnMove(const Move &move) const;
    bool isOpponentKingCapturable() const;
    bool isPawnMove(const Move &move) const;
    bool isKingMove(const Move &move) const;
    bool isRookMove(const Move &move) const;
    bool isReversible(const Move &move) const;
    bool isInLegalMovesList(const Move &move) const;
    bool isInPseudoLegalMovesList(const Move &move) const;
    bool isCheck() const;
    bool isCheck(const Move &move) const;
    bool isCheckmate() const;
    bool isCheckmate(const Move &move) const;
    bool isStalemate() const;
    bool isCastleShort(const Move &move) const;
    bool isCastleLong(const Move &move) const;
    std::vector<Move> getPseudoLegalMoves() const {return pseudoLegalMoves;}

    bool uniqueOrigin(uint8f &resOrigin, const uint8f &target, PieceType type) const;
    bool uniqueOriginOnFile(uint8f &resOrigin, const uint8f &target, uint8f fileIndex, PieceType type) const;
    bool uniqueOriginOnRank(uint8f &resOrigin, const uint8f &target, uint8f rankIndex, PieceType type) const;

    void applyMove(const Move &move, Position &res, bool updateFull) const;

private:
    bool initialize();
    bool updatePseudoLegalMoves();
    void updateLegalMoves();
    bool isLegalConstruct(const Move &move, bool checkPseudoLegal = true);

    void addPseudoLegalMove(uint8f origin, uint8f target, bool attack=true);
    void addPseudoLegalMovePromotions(uint8f origin, uint8f target, bool attack);
    void addPseudoLegalMovesKing(uint8f origin);
    void addPseudoLegalMovesQueen(uint8f origin);
    void addPseudoLegalMovesRook(uint8f origin);
    void addPseudoLegalMovesBishop(uint8f origin);
    void addPseudoLegalMovesKnight(uint8f origin);
    void addPseudoLegalMovesPawn(uint8f origin);
    void addPseudoLegalMovesRow(uint8f origin, const std::vector<uint8f> &row);

    std::vector<Move> pseudoLegalMoves, legalMoves; // "pseudo-legal move" = leaves the king on capture
    bool pseudoLegalMovesGenerated, legalMovesGenerated;
    bitboard squaresAttacked;
    bool opponentKingUnderAttack, opponentShortCastleUnderAttack, opponentLongCastleUnderAttack;

    const Position * const position;
    bool turn;
    const Board * const board;
    const boardZ &pieces;
};













#endif // LEGALMOVER_H
