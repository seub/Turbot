#include "move.h"

Move::Move(const Square &origin, const Square &target) : origin(origin), target(target)
{

}

Move::Move(uint originIndex, uint targetIndex) : origin(originIndex), target(targetIndex)
{

}
