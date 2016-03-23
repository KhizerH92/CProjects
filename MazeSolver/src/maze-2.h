/* ========================================================================== */
/* File: maze.h
 * Author: Doug Tallmadge
 * Date: 03/02/16
 *
 * Contains function prototypes for a simple genaric stack implementation
 */
/* ========================================================================== */
// #ifndef MAZE_H
// #define MAZE_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants

// ---------------- Structures/Types

typedef struct MazeNode {

	// The key of the node within the maze grid matrix.
	int x;
	int y;

	// Keeps track of information generated about node:
	// 0 represents no information, -1 represents wall.  1 represents visited path.
	int up;
	int down;
	int left;
	int right;
	int binary;
	int Nmoves[4]; // 10+ActualMove

	int visited; // Flags if visited.  Use avatar ID as int. Initialize to -1.

} MazeNode;

/* A struct to store relevant information about a given maze as it is discovered.
 * Maze information is then available through shared memory to Avatar Clients and Graphics.
 * In order to avoid having to allocate memory for the array based on the size we will need,
 * the two dimensional array used will be declared with a large excess of size.  Then, during 
 * initialization, the max x and max y coordinates will be declared.  Thus any program using
 * the maze will only use the square up to the max x and y.
 */
typedef struct Maze {  

	// First index is x position
	// Second index is y position
	MazeNode **grid;

	int x_max;
	int y_max;


} Maze;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * A function to allocate shared memory for a maze and initialize the members.
 * x and y sizes must be less than 250.
 * Returns the shmat 
 */
//MazeNode **init_maze(int x_size, int y_size);
void init_maze(Maze* m, int x_size, int y_size);
void cleanUpMaze(Maze* m);
void print_maze_tofile(Maze *maze, char* maze_file_name, int x_size, int y_size);
void asciiDisplay(Maze* maze);


// #endif // MAZE_H
 
