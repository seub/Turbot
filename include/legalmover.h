#ifndef LEGALMOVER_H
#define LEGALMOVER_H


# include "move.h"

class Position;

class LegalMover
{
    friend class MovePGN;
    friend class Position;
    friend class BasicMovePicker;
    friend class ForcefulMovePicker;
    friend class BasicMovePickerHash;

public:
    explicit LegalMover(const Position *position, bool generateLegalMoves = true);
    LegalMover() = delete;
    LegalMover(const LegalMover &) = delete;
    LegalMover & operator=(LegalMover) = delete;

    std::string printLegalMoves() const;
    std::string printKCLegalMoves() const;
    std::vector<Move> getlegalMoves() const {return legalMoves;}
    std::vector<Move> getKCLegalMoves() const {return kCLegalMoves;}
    bool isCapture(const Move &move) const;
    bool isOpponentKingCapturable() const;
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
    bool isForceful(const Move &move) const;
    bool isCastleShort(const Move &move) const;
    bool isCastleLong(const Move &move) const;
    Color turn() const;

    bool uniqueOrigin(Square &resOrigin, const Square &target, PieceType type) const;
    bool uniqueOriginOnFile(Square &resOrigin, const Square &target, uint fileIndex, PieceType type) const;
    bool uniqueOriginOnRank(Square &resOrigin, const Square &target, uint rankIndex, PieceType type) const;

    bool applyMove(Position &res, const Move &move, bool checkLegal = false, bool checkKCLegal = false) const;

private:
    bool initialize();
    bool updateKCLegalMoves();
    void updateLegalMoves();
    bool isLegalConstruct(const Move &m, bool checkKClegal = true);

    void addKCLegalMove(uint origin, uint target, bool attack=true);
    void addKCLegalMovePromotions(uint origin, uint target, bool attack=false);
    void addKCLegalMovesKing(uint i);
    void addKCLegalMovesQueen(uint i);
    void addKCLegalMovesRook(uint i);
    void addKCLegalMovesBishop(uint i);
    void addKCLegalMovesKnight(uint i);
    void addKCLegalMovesPawn(uint i);
    void addKCLegalMovesRow(uint i, const std::vector<uint> row);

    Position applyMove(const Move &move) const;

    std::vector<Move> kCLegalMoves, legalMoves; // We call "KC Chess" the variation of chess where king capture is allowed.
    bool kCLegalMovesGenerated, legalMovesGenerated;
    bitboard squaresAttacked;
    bool opponentKingUnderAttack, opponentShortCastleUnderAttack, opponentLongCastleUnderAttack;

    const Position * const position;
    const BoardHelper boardHelper;
};









class PositionZ;

class LegalMoverZ
{
    friend class MovePGNZ;
    friend class PositionZ;

public:
    explicit LegalMoverZ(const PositionZ *position, bool generateLegalMoves = true);
    LegalMoverZ() = delete;
    LegalMoverZ(const LegalMover &) = delete;
    LegalMoverZ & operator=(LegalMoverZ) = delete;

    bool isCapture(const MoveZ &move) const;
    bool isOpponentKingCapturable() const;
    bool isPawnMove(const MoveZ &move) const;
    bool isKingMove(const MoveZ &move) const;
    bool isRookMove(const MoveZ &move) const;
    bool isReversible(const MoveZ &move) const;
    bool isInLegalMovesList(const MoveZ &move) const;
    bool isInPseudoLegalMovesList(const MoveZ &move) const;
    bool isCheck() const;
    bool isCheck(const MoveZ &move) const;
    bool isCheckmate() const;
    bool isCheckmate(const MoveZ &move) const;
    bool isStalemate() const;
    bool isCastleShort(const MoveZ &move) const;
    bool isCastleLong(const MoveZ &move) const;
    std::vector<MoveZ> getPseudoLegalMoves() const {return pseudoLegalMoves;}

    bool uniqueOrigin(uint8f &resOrigin, const uint8f &target, PieceType type) const;
    bool uniqueOriginOnFile(uint8f &resOrigin, const uint8f &target, uint8f fileIndex, PieceType type) const;
    bool uniqueOriginOnRank(uint8f &resOrigin, const uint8f &target, uint8f rankIndex, PieceType type) const;

    void applyMove(const MoveZ &move, PositionZ &res, bool updateFull) const;

private:
    bool initialize();
    bool updatePseudoLegalMoves();
    void updateLegalMoves();
    bool isLegalConstruct(const MoveZ &move, bool checkPseudoLegal = true);

    void addPseudoLegalMove(uint8f origin, uint8f target, bool attack=true);
    void addPseudoLegalMovePromotions(uint8f origin, uint8f target, bool attack);
    void addPseudoLegalMovesKing(uint8f origin);
    void addPseudoLegalMovesQueen(uint8f origin);
    void addPseudoLegalMovesRook(uint8f origin);
    void addPseudoLegalMovesBishop(uint8f origin);
    void addPseudoLegalMovesKnight(uint8f origin);
    void addPseudoLegalMovesPawn(uint8f origin);
    void addPseudoLegalMovesRow(uint8f origin, const std::vector<uint8f> &row);

    std::vector<MoveZ> pseudoLegalMoves, legalMoves; // "pseudo-legal move" = leaves the king on capture
    bool pseudoLegalMovesGenerated, legalMovesGenerated;
    bitboard squaresAttacked;
    bool opponentKingUnderAttack, opponentShortCastleUnderAttack, opponentLongCastleUnderAttack;

    const PositionZ * const position;
    bool turn;
    const BoardZ * const board;
    const boardZ &pieces;
};













#endif // LEGALMOVER_H
