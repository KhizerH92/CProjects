/*
 * Author: Douglas Tallmadge
 *
 * Created: Sunday February 20th 2016
 *
 * Script Name: mazetest.c
 *
 * Program Description: Tests the maze data structure without shared memory.
 *
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>
#include <stdio.h>

// ---------------- Local includes
#include "../src/maze-2.h"
#include <time.h> 

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */


int test_maze(Maze* maze, char* test_file, int x_size, int y_size){
	FILE* f;
	f = fopen(test_file, "w");

	fprintf(f,"TESTING SIZE: \n");
	int i = 1;
	for(int y = 0; y < maze->y_max; y++){
		for(int x = 0; x < maze->x_max; x++){
			fprintf(f, "%d", i);
			i++;
		}
		fprintf(f, "\n");
	}

	fprintf(f,"\nTESTING INDEXING INTO 2D ARRAY: \n");
	fprintf(f,"prints values of up, down, right, and left for each node in array\n");
	for(int y = 0; y < maze->y_max; y++){
		for(int x = 0; x < maze->x_max; x++){
			fprintf(f,"%d,%d: location (%d,%d), up: %d, down: %d, right: %d, left: %d\n", x, y, maze->grid[x][y].x, maze->grid[x][y].y, maze->grid[x][y].up, maze->grid[x][y].down, maze->grid[x][y].right, maze->grid[x][y].left);
		}
	}

	fprintf(f,"\n\nChanged values for up, down, right, and left of four random positions\n");
	srand(time(NULL));

	maze->grid[rand() % x_size][rand() % y_size].up = -1;
	maze->grid[rand() % x_size][rand() % y_size].down = -1;
	maze->grid[rand() % x_size][rand() % y_size].left = -1;
	maze->grid[rand() % x_size][rand() % y_size].right = -1;

	for(int y = 0; y < maze->y_max; y++){
		for(int x = 0; x < maze->x_max; x++){
			fprintf(f,"%d,%d: location (%d,%d), up: %d, down: %d, right: %d, left: %d\n", x, y, maze->grid[x][y].x, maze->grid[x][y].y, maze->grid[x][y].up, maze->grid[x][y].down, maze->grid[x][y].right, maze->grid[x][y].left);
		}
	}

	fclose(f);
	return 0;
}

// tests maze
// takes arguments for x and y size
int main(int argc, char* argv[])
{
	// get arguments
	int x_size = atoi(argv[1]);
	int y_size = atoi(argv[2]);

	// initialize maze
	Maze* maze;
	maze = calloc(1,sizeof(Maze));
	init_maze(maze, x_size, y_size);

	test_maze(maze, "maze_testing.txt", x_size, y_size);
	cleanUpMaze(maze);

	return 0;
}