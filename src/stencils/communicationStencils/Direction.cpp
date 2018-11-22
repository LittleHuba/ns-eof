//
// Created by tommaso on 22/11/18.
//

#include "Direction.h"

int DirectionUtils::getOppositeDirection(int direction)
{
    int u = direction & 1;
    return ((direction>>1)<<1) + !u;
}
