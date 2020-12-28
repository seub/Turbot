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

private:
    PieceType piecetype;
    Color turn;
    bool pieceOriginAmbiguous, ambiguityLiftedByFile, ambiguityLiftedByRank;
    bool capture;
    bool castleShort, castleLong;
    bool check, checkmate;
};



#endif // MOVE_H
