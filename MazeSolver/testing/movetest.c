/*
 * Author: Ella Dzenitis
 *
 * Created: Tuesday 8 March 2016
 *
 * Script Name: movetest.c
 *
 * Program Description: Tests the move decision-making from avatardaemon2.0.c.
 *
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>
#include <stdio.h>

// ---------------- Local includes
// #include "../src/maze.	"
#include "../src/avatardaemon2.0.c"
//#include "../src/amazing-2.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */


// initialize a maze (known to user)
// calls determine move function
// check that the function picks the expected move
int main(int argc, char* argv[])
{

	// initialize maze
	Maze* maze;
	maze = calloc(1,sizeof(Maze));
	init_maze(maze, 2, 2);

	maze->grid[0][0].left = -1;
	maze->grid[0][0].up = -1;
	maze->grid[0][0].right = -1;
	maze->grid[0][0].down = 0;

	maze->grid[0][1].left = -1;
	maze->grid[0][1].up = 0;
	maze->grid[0][1].right = 0;
	maze->grid[0][1].down = -1;

	maze->grid[1][0].left = -1;
	maze->grid[1][0].up = -1;
	maze->grid[1][0].down = -1;
	maze->grid[1][0].right = -1;

	maze->grid[1][1].left = 0;
	maze->grid[1][1].up = -1;
	maze->grid[1][1].right = -1;
	maze->grid[1][1].down = -1;

	printf("Next move direction should be M_WEST (0). Function returns: %d\n", determineMove(maze, 1, 1, M_NORTH));

	cleanUpMaze(maze);
	
	printf("maze has been cleaned properly\n");


	return 0;
}