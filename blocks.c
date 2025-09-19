#include "blocks.h"

Block large_LShape = {
    .width = 3,
    .height = 3,
    .centerX = 1,
    .centerY = 1,
    .pattern = {
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1}
    }
};
Block Line = {
    .width = 4,
    .height = 1,
    .centerX = 1,
    .centerY = 0,
    .pattern = {
        {1, 1, 1, 1}
    }
};
Block Square = {
    .width = 2,
    .height = 2,
    .centerX = 0,
    .centerY = 0,
    .pattern = {
        {1, 1},
        {1, 1}
    }
};
Block TShape = {
    .width = 3,
    .height = 2,
    .centerX = 1,
    .centerY = 1,
    .pattern = {
        {1, 1, 1},
        {0, 1, 1}
    }
};
Block normal_Lshape = {
    .width = 2,
    .height = 3,
    .centerX = 1,
    .centerY = 1,
    .pattern = {
        {1,0},
        {1,0},
        {1,1}
    }
};
Block Zshape = {
    .width = 3,
    .height = 2,
    .centerX = 1,
    .centerY = 1,
    .pattern = {
        {1,1,0},
        {0,1,1}
    }
};
