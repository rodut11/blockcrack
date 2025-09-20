#include <stdio.h>
#include "blocks.h"
#include "utils/util_print_block.h"

int main() {
    //initialize grid
    int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {
        0
    };
    grid_print(grid);
    block_print(Zshape);
}
