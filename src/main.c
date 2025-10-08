#include <stdio.h>
#include "blocks.h"
#include <string.h>
#include "../utils/util_blocks.h"
#include "../utils/waydroid/C/waydroid_comm.h"

#include <stdlib.h>
#include "../utils/debug/debug.h"


int main() {
    int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {0};

    waydroid_connect("192.168.240.112:5555");
    size_t size;
    unsigned char* image = get_screencap(&size);
    grid_print(grid);
    get_block();
    get_grid(grid);


    int fillcount = 0;

    undefined_block testblock ={
        .row = 1,
        .col = 1,
        .pattern = {
            {1}
        }
    };
    place_game_block(grid, testblock, 0, 0);
    for (int i = 0; i < MAX_GRID_HEIGHT; i++) {
        for (int j = 0; j < MAX_GRID_WIDTH; j++) {
            if (grid[i][j] == 1) {
                fillcount++;
            }
        }
    }

    float area = 64/(float)fillcount;

    grid_print(grid);
    printf("%d blocks\n", fillcount);
    printf("%.2f\n", area);
    get_block_coord();
}