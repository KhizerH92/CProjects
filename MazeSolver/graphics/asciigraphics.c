#include "../network/maze.h"

void asciiDisplay(Maze* maze){

	//Loop through each row of maze 3 times.  Once for ceiling
	// Twice for walls.

	// First loop through for top of maze:
	for(int i = 0; i < maze->x_max; i++){
		printf("+---");
	}
	printf("+\n");

	// Now for each y in maze,:
	for (int y = 0; y < maze->y_max; y++){

		// Print outer wall:
		printf("|");


		// Draw each node middle and bottom.
		for (int x = 0; x < maze->x_max; x++){

			// Display the avatars:
			if (maze->grid[x][y]->visited >= 0){
				printf(" %d ", maze->grid[x][y]->visited);
				maze->grid[x][y]->visited = -1;

			} else{

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
			printf("+\n");
		}
	}
}