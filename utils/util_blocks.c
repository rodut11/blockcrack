
#include "util_print_block.h"
#include <stdio.h>
#include "../blocks.h"

void grid_print(int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH]) {

    for(int i = 0; i < MAX_GRID_HEIGHT; i++) {
        for(int j = 0; j < MAX_GRID_WIDTH; j++) {
            printf("%d ", grid[i][j]); // print the value
        }
        printf("\n"); // new line after each row
    }
}

void block_print(block block) {
    for(int i = 0; i < block.height; i++) {
        for(int j = 0; j < block.width; j++) {
            printf("%d ", block.pattern[i][j]);
        }
        printf("\n");
    }
}