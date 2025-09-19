#include <stdio.h>
#include "blocks.h"
#include "utils/printBlockUtils.h"

int main() {
    //initialize grid
    int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {
        0
    };
    PrintGrid(grid);
    PrintBlock(Zshape);
}
