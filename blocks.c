#include "blocks.h"

block large_LShape = {
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
block Line = {
    .width = 4,
    .height = 1,
    .centerX = 1,
    .centerY = 0,
    .pattern = {
        {1, 1, 1, 1}
    }
};
block Square = {
    .width = 2,
    .height = 2,
    .centerX = 0,
    .centerY = 0,
    .pattern = {
        {1, 1},
        {1, 1}
    }
};
block TShape = {
    .width = 3,
    .height = 2,
    .centerX = 1,
    .centerY = 1,
    .pattern = {
        {1, 1, 1},
        {0, 1, 1}
    }
};
block normal_Lshape = {
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
block Zshape = {
    .width = 3,
    .height = 2,
    .centerX = 1,
    .centerY = 1,
    .pattern = {
        {1,1,0},
        {0,1,1}
    }
};
