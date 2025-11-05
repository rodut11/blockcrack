#include <stdio.h>
#include "../include/blocks.h"
#include <string.h>
#include "../include/util_blocks.h"
#include "../include/waydroid/waydroid_comm.h"

#include <stdlib.h>
#include "../utils/debug/debug.h"


void get_grid_origin(int* x, int* y);

int main() {
    int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {0};

    waydroid_connect("192.168.240.112:5555");
    // size_t size;
    // unsigned char* image = get_screencap(&size);

    get_grid(grid);
    grid_print(grid);

    int x, y;
    get_grid_origin(&x, &y);
    printf("Grid origin: %d,%d\n", x, y);

    // free(image);
    return 0;
}