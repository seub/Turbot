#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "piece.h"
#include "board.h"



class Move
{
    friend class LegalMover;
    friend std::ostream & operator<<(std::ostream &out, const Move &M);

public:
    Move() {}
    Move(const Square &origin, const Square &target, bool promotion = false, PieceType promotedPiece= PieceType::EMPTY);
    bool operator==(const Move &other) const;

    static bool fromLAN(Move & res, std::string &LANstring); // LAN = Long Algebraic Notation // NB: static const function not possible in C++ :/
    std::string toLAN() const;

    Square getOrigin() const {return origin;}
    Square getTarget() const {return target;}
    bool getPromotion() const {return promotion;}
    PieceType getPromotedPiece() const {return promotedPiece;}

protected:
    Move(uint originIndex, uint targetIndex, bool promotion = false, PieceType promotedPiece= PieceType::EMPTY);
    Square origin, target;
    bool promotion;
    PieceType promotedPiece;
};




class LegalMover;

class MovePGN : public Move
{
    // Remark: The MovePGN class is not responsible for writing the move number, any draw offers (=) before or after playing a move,
    // nor the move annotation symbols (https://en.wikipedia.org/wiki/Chess_annotation_symbols), nor the game result (1-0, 0-1, or 1/2-1/2).
    friend std::ostream & operator<<(std::ostream &out, const MovePGN &M);

public:
    MovePGN() {}
    MovePGN(const Move &move, const LegalMover *mover);
    bool operator==(const MovePGN &other) const;

    static bool fromPGN(MovePGN &res, const std::string &PGNstring, const LegalMover *mover);
    static bool fromPGN(Move &res, const std::string &PGNstring, const LegalMover *mover);
    static bool fromMove(MovePGN &res, const Move &m, const LegalMover *mover);

    Color getTurn() const {return turn;}

    std::string toPGN() const;
    std::string toPGN(uint moveNum) const;

private:
    PieceType piecetype;
    Color turn;
    bool pieceOriginAmbiguous, ambiguityLiftedByFile, ambiguityLiftedByRank;
    bool capture;
    bool castleShort, castleLong;
    bool check, checkmate;
};







class MoveZ
{
    friend class MovePGNZ;
    friend class LegalMoverZ;

public:
    MoveZ() {}
    MoveZ(uint8f origin, uint8f target, uint8f promotion = 0) : origin(origin), target(target), promotion(promotion) {}
    bool operator==(const MoveZ &other) const;

protected:
    uint8f origin, target;
    uint8f promotion;
    // 4-bit integer: the 0-bit says whether the move is a promotion, and the 1,2,3-bits say he type of the promoted piece
    // promotion = 5 = 0101 means that the move is a promotion, and the promoted piece is 010 = 2 = Queen
    // promotion = 7 = 0111 means that the move is a promotion, and the promoted piece is 011 = 3 = Rook
    // promotion = 9 = 1001 means that the move is a promotion, and the promoted piece is 100 = 4 = Bishop
    // promotion = 11 = 1011 means that the move is a promotion, and the promoted piece is 101 = 5 = Knight
};



class LegalMoverZ;

class MovePGNZ : public MoveZ
{
    // Remark: The MovePGNZ class is not responsible for writing the move number, any draw offers (=) before or after playing a move,
    // nor the move annotation symbols (https://en.wikipedia.org/wiki/Chess_annotation_symbols), nor the game result (1-0, 0-1, or 1/2-1/2).
    friend std::ostream & operator<<(std::ostream &out, const MovePGNZ &M);

public:
    MovePGNZ() {}
    MovePGNZ(const MoveZ &move, const LegalMoverZ *mover);
    bool operator==(const MovePGNZ &other) const;

    static bool fromPGN(MovePGNZ &res, const std::string &PGNstring, const LegalMoverZ *mover);
    static bool fromPGN(MoveZ &res, const std::string &PGNstring, const LegalMoverZ *mover);
    static bool fromMove(MovePGNZ &res, const MoveZ &move, const LegalMoverZ *mover);

    bool getTurn() const {return turn;}

    std::string toPGN() const;
    std::string toPGN(uint moveNum) const;

private:
    PieceType piecetype;
    bool turn;
    bool pieceOriginAmbiguous, ambiguityLiftedByFile, ambiguityLiftedByRank;
    bool capture;
    bool castleShort, castleLong;
    bool check, checkmate;
};




#endif // MOVE_H
