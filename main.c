#include <stdio.h>
#include <blocks.h>

#define MAX_BLOCK_SIZE 10


void PrintGrid(int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH]);

int main() {
	//initialize grid as fully empty
	int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {
		0
	};
}

void PrintGrid(int grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH]) {

	for(int i = 0; i < MAX_GRID_HEIGHT; i++) {
		for(int j = 0; j < MAX_GRID_WIDTH; j++) {
			printf("%d ", grid[i][j]); // print the value
		}
		printf("\n"); // new line after each row
	}

}