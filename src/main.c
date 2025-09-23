#include <stdio.h>
#include "blocks.h"
#include "../utils/util_blocks.h"
#include "../utils/debug/debug_blocks.h"



int main() {
    int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {0};

    int points = 0;

    place_block(grid, large_square, 0, 0);
    place_block(grid, large_square, 0, 3);
    place_block(grid, rectangle_horizontal, 0, 6);
    place_block(grid, large_square, 3, 0);
    place_block(grid, rectangle_vertical, 6, 0);

    printf("\n");
    grid_print(grid);
    check_full_row(grid);
    grid_print(grid);
}