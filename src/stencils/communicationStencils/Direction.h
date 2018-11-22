//
// Created by tommaso on 22/11/18.
//

#ifndef NS_DIRECTION_H
#define NS_DIRECTION_H

const int NUMBER_OF_DIRECTIONS = 6;

typedef enum Direction
{
    // In the code we always assume the values here go from 0 to NUMBER_OF_DIRECTIONS-1,
    // order does not matter but the couples (LR, BT, FB) must stay next to each other!
    LEFT = 0, RIGHT = 1, BOTTOM = 2, TOP = 3, FRONT = 4, BACK = 5
} Direction;

namespace DirectionUtils
{
    unsigned char getOppositeDirection(Direction direction)
    {
        unsigned char u = direction & 1;
        return ((direction>>1)<<1) + !u;
    }
}

#endif //NS_DIRECTION_H
