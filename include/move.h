#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "piece.h"

class Move
{
    friend class Mover;
    friend std::ostream & operator<<(std::ostream &out, const Move &M);

public:
    Move(const Square &origin, const Square &target, bool promotion = false, PieceType promotedPiece= PieceType::Empty);
    bool operator==(const Move &other) const;

    static bool fromLAN(Move & res, std::string &LANstring); // LAN = Long Algebraic Notation // NB: static const function not possible in C++ :/
    std::string toLAN() const;

    Square getOrigin() const;
    Square getTarget() const;

private:
    Move(uint originIndex, uint targetIndex, bool promotion = false, PieceType promotedPiece= PieceType::Empty);
    Square origin, target;
    bool promotion;
    PieceType promotedPiece;
};

#endif // MOVE_H
