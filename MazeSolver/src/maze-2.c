//Update maze.c:

/*
 * Author: Douglas Tallmadge
 *
 * Created: Sunday February 20th 2016
 *
 * Script Name: maze.c
 *
 * Program Description: Simple genaric stack implementation
 *
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>
#include <stdlib.h>

// ---------------- Local includes
#include "maze-2.h"

// ---------------- Constant definitions

// ---------------- Macro definitions
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */
void print_maze_tofile(Maze *maze, char* maze_file_name, int x_size, int y_size){
	FILE* maze_file;
 	maze_file = fopen(maze_file_name, "w");

 	// first line is x and y size
 	fprintf(maze_file, "%d;%d\n", x_size, y_size);

 	for(int y = 0; y < maze->y_max; y++){
 		for (int x = 0; x < maze->x_max; x++){
 			//format of line --- x, y; up, down, left, right
 			fprintf(maze_file, "%d, %d; %d, %d, %d, %d\n", x, y, maze->grid[x][y].up, maze->grid[x][y].down, maze->grid[x][y].left, maze->grid[x][y].right);
 		}
 	}

 	fclose(maze_file);
}
/*
 * A function to initialize the maze struct.
 */
void init_maze(Maze *maze, int x_size, int y_size)
{

	if (x_size > 100 || y_size > 100){
		fprintf(stderr, "Invalid Maze Size\n");
		return;
	}


	maze->grid = calloc(x_size,sizeof(MazeNode *));

	for(int i=0; i < x_size;i++){
		maze->grid[i] = (MazeNode *) calloc(y_size,sizeof(MazeNode));
	}

	int x = 0;
	int y = 0;
	while (x < x_size){ //check this is it <= or just <

		while (y < y_size){ //same check for this 

			maze->grid[x][y].x = x;
			maze->grid[x][y].y = y;

			if(y==0){
				maze->grid[x][y].up = -1;					
			}
			else{
				maze->grid[x][y].up = 0;
			}

			if(y==y_size-1){
				maze->grid[x][y].down = -1;					
			}
			else{
				maze->grid[x][y].down = 0;
			}

			if(x==0){
				maze->grid[x][y].left = -1;					
			}
			else{
				maze->grid[x][y].left = 0;
			}

			if(x==x_size-1){
				maze->grid[x][y].right = -1;					
			}
			else{
				maze->grid[x][y].right = 0;
			}
			
			maze->grid[x][y].visited = -1;

			y++;
		}

		x++;
	}

	maze->x_max = x_size;
	maze->y_max = y_size;

}
void asciiDisplay(Maze* maze){

	//Loop through each row of maze 3 times.  Once for ceiling
	// Twice for walls.

	system("clear");

	printf("\n\n");

	// First loop through for top of maze:
	for(int i = 0; i < maze->x_max; i++){
		printf("+---");
	}
	printf("+\n");

	// Now for each y in maze,:
	for (int y = 0; y < maze->y_max; y++){

		// Print outer wall:
		printf("|");
		fflush(stdout);


		// Draw each node middle and bottom.
		for (int x = 0; x < maze->x_max; x++){

			// Display the avatars:
			if (maze->grid[x][y].visited > 0){

				printf(ANSI_COLOR_GREEN " %d " ANSI_COLOR_RESET, maze->grid[x][y].visited);
				//maze->grid[x][y].visited = -1;

			} else if (maze->grid[x][y].visited == 0){
				printf(ANSI_COLOR_YELLOW " %d " ANSI_COLOR_RESET, maze->grid[x][y].visited);
			} else if(maze->grid[x][y].visited == -5){
				printf(ANSI_COLOR_BLUE " ? " ANSI_COLOR_RESET);
			}else{

				printf("   ");

			}
			if (maze->grid[x][y].right == -1){
				printf("|");
			} else{
				printf(" ");
			}

		}
		// Outer wall.
		printf("|\n");

		// draw bottom
		for (int x = 0; x < maze->x_max; x++){

			if (maze->grid[x][y].down == -1){
				printf("+---");
			} else{
				printf("+   ");
			}
		}
		printf("+\n");
	}
	printf("\n\n");
}

void cleanUpMaze(Maze* maze){
	for(int x = 0; x < maze->x_max; x++){
		free(maze->grid[x]);
	}
	free(maze->grid);
	free(maze);
}

