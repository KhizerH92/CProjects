# This is the Makefile ... it will need to move down into the src directory to build your indexer (and crawler if you want)
# 
CC = gcc
CFLAGS =-ggdb -Wall -pedantic -std=c11 -lpthread

SRCS2=./src/maze-2.c ./src/amstartupclient2.0.c ./src/avatardaemon2.0.c
SRSH2=./src/maze-2.h ./src/amazing-2.h
OBJS2=./bin/maze-2.o ./bin/amstartupclient2.0.o ./bin/avatardaemon2.0.o

# Project Details
EXEC2 = amstartup
DEBUG_EXEC = debugAmStartup


bfs: $(OBJS2) $(SRSH2)
	$(CC) $(CFLAGS) -o $(EXEC2) $(OBJS2)

$(OBJS2): $(SRCS2)
	$(CC) $(CFLAGS) -c $(SRCS2)
	@mv -f *.o ./bin

debug: $(SRCS2)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS2)
	@mv -f *.o ./bin
	$(CC) $(CFLAGS) -g -ggdb -o $(DEBUG_EXEC) $(OBJS2)

clean:
	@rm -f *~
	@rm -f *#
	@rm -f ./$(EXEC2)	
	@rm -f ./$(DEBUG_EXEC)
	@rm -f *.o
	@cd ./bin; 	rm -f *.o

