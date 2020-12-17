#ifndef MOVE_H
#define MOVE_H

#include "square.h"

class Move
{
    friend class LegalMover;

public:
    Move(const Square &origin, const Square &target);

private:
    Move(uint originIndex, uint targetIndex);
    Square origin, target;
};

#endif // MOVE_H
