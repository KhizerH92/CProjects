Amazing Maze Project


Basic Use:  Solves unknown mazes by directing avatars on a remote server.

Compile the basic version using the make command.

To run:  ./amstartup -n 2 -d 2 -h flume.cs.dartmouth.edu   where -n -d -h are options that take a given arguement type regardless of placement.  -n represents the number of avatars, -d represents the maze difficulty, and -d represents the server to connect with.  Currently only flume.cs.dartmouth.edu will be accepted, but further functionality could be enabled in conjunction with another server.


Additional builds which can be run include a make debug and make bfs.  Make bfs uses our most advanced algorithm, and has no known bugs, but has not yet been moved to the default version.



Structure
.
├── Espadeiro
│   ├── bin
│   │   └── placeholder
│   ├── graphics
│   │   ├── asciigraphics.c
│   │   ├── MazeSolved
│   │   │   ├── avatar_log.txt
│   │   │   ├── cs1lib.py
│   │   │   ├── cs1lib.pyc
│   │   │   ├── load_maze.py
│   │   │   ├── load_maze.pyc
│   │   │   ├── maze_draw.py
│   │   │   ├── maze.py
│   │   │   ├── maze.pyc
│   │   │   ├── mazetest.dat
│   │   │   └── test_maze.txt
│   │   └── placeholder
│   ├── library
│   │   ├── queue.c
│   │   ├── queue.h
│   │   ├── stack.c
│   │   ├── stack.h
│   │   └── testscripts
│   │       ├── queuetest.c
│   │       ├── queuetest.log
│   │       ├── stacktest.c
│   │       └── stacktest.log
│   ├── logfiles
│   ├── Makefile
│   ├── README.md
│   ├── Spec.docx
│   ├── Spec.pdf
│   ├── src
│   │   ├── amazing-2.h
│   │   ├── amazing.h
│   │   ├── amstartupclient2.0.c
│   │   ├── amstartupclient.c
│   │   ├── avatardaemon2.0.c
│   │   ├── avatardaemon.c
│   │   ├── maze-2.c
│   │   ├── maze-2.h
│   │   ├── maze.c
│   │   └── maze.h
│   └── testing
│       ├── EllaREADME
│       ├── KhizarREADME
│       └── mazetest.c

