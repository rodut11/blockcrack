#include "util_blocks.h"
#include <stdio.h>
#include <stdbool.h>
#include "../src/blocks.h"
#include "../include/ANSI-Color-Codes.h"

void grid_print(int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH]) {
    for (int i = 0; i < MAX_GRID_HEIGHT; i++) {
        printf(" %d |", i);
    }
    printf("\n");
    for (int i = 0; i < MAX_GRID_HEIGHT; i++) {
        for (int j = 0; j < MAX_GRID_WIDTH; j++) {
            if (grid[i][j] == 1) {
                printf(BLU " %d " COLOR_RESET "|", grid[i][j]);
            }else
                printf(" . |");
        }
        printf(" %d \n", i); // new line after each row
    }
}

void clear_grid(int grid[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = 0;
        }
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

void place_block(int grid[8][8], block b, int pivotX, int pivotY) {

    if (!check_collision(grid, b, pivotX, pivotY))
        return; // abort if any collision

    for (int i = 0; i < b.height; i++) {
        for (int j = 0; j < b.width; j++) {
            if (b.pattern[i][j] != 0) {
                int gx = pivotX + j;
                int gy = pivotY + i;
                grid[gy][gx] = b.pattern[i][j]; // now safe to place
            }
        }
    }
}

bool check_collision(int grid[8][8], block b, int pivotX, int pivotY) {
    int coordX = pivotX;
    int coordY = pivotY;

    for (int i = 0; i < b.height; i++) {
        for (int j = 0; j < b.width; j++) {
            if (b.pattern[i][j] != 0) {
                int gx = pivotX + j;
                int gy = pivotY + i;

                // collision or out-of-bounds
                if (gx < 0 || gx >= 8 || gy < 0 || gy >= 8 || grid[gy][gx] != 0) {
                    printf(RED "Can't place block at (X=%d;Y=%d)!\n" COLOR_RESET, coordX, coordY);
                    return false;
                }
            }
        }
    }

    return true;
}

void check_full_row(int grid[8][8]) {
    typedef struct {
        int x;
        int y;
    } Coord;

    Coord coord[64];
    int clearCount = 0;

    // check rows
    for (int yAxis = 0; yAxis < MAX_GRID_HEIGHT; yAxis++) {
        int xAxisLine = 0;

        for (int xAxis = 0; xAxis < MAX_GRID_WIDTH; xAxis++) {
            if (grid[yAxis][xAxis] == 1) {
                xAxisLine++;
            }
        }

        if (xAxisLine == MAX_GRID_WIDTH) {
            printf("Row %d is full!\n", yAxis);
            for (int xAxis = 0; xAxis < MAX_GRID_WIDTH; xAxis++) {
                coord[clearCount].x = xAxis;
                coord[clearCount].y = yAxis;
                clearCount++;
            }
        }
    }

    // check columns
    for (int xAxis = 0; xAxis < MAX_GRID_WIDTH; xAxis++) {
        int yAxisLine = 0;

        for (int yAxis = 0; yAxis < MAX_GRID_HEIGHT; yAxis++) {
            if (grid[yAxis][xAxis] == 1) {
                yAxisLine++;
            }
        }

        if (yAxisLine == MAX_GRID_HEIGHT) {
            printf("Column %d is full!\n", xAxis);
            for (int yAxis = 0; yAxis < MAX_GRID_HEIGHT; yAxis++) {
                coord[clearCount].x = xAxis;
                coord[clearCount].y = yAxis;
                clearCount++;
            }
        }
    }

    // now clear everything
    for (int i = 0; i < clearCount; i++) {
        grid[coord[i].y][coord[i].x] = 0;  // y first, then x
    }
}
