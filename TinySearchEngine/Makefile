# This is the Makefile ... it will need to move down into the src directory to build your indexer (and crawler if you want)
# To make the Crawler component call 'make Crawler', Indexer component call 'make Indexer' and for Query call 'make Query'
CC=gcc
CFLAGS=-std=c11 -Wall -pedantic -ggdb -lm -lcurl
DEPS = ./util/funcFile.h ./util/hashtable.h ./util/ANDORList.h ./util/common.h
OBJ = ./util/funcFile.o ./util/hashtable.o ./util/ANDORList.o 
LIBRARY=./util/libtseutil.a
MAINFILE=./QueryEngine/src/QueryEngine.c
QUERY=./QueryEngine/Query
CRAWLER=./CrawlerFiles/crawler
INDEXER=./IndexerFiles/indexer
EXECUTABLES=$(QUERY) $(CRAWLER) $(INDEXER)
SEARCHPATH=-I./util

Query: $(QUERY)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIBRARY):
	ar -cvq $(LIBRARY) ./util/*.o

$(QUERY): $(OBJ) $(LIBRARY)
	gcc -o $@ $^ $(CFLAGS) $(MAINFILE) $(LIBRARY) $(SEARCHPATH)
	
clean:
	rm -rf ./util/*.o ./CrawlerFiles/*.o ./IndexerFiles/*.o $(EXECUTABLES) $(LIBRARY)

test:
	./QueryEngine/QEBATS.sh

$(CRAWLER):
	cd ./CrawlerFiles && $(MAKE)

$(INDEXER):
	cd ./IndexerFiles && $(MAKE)

Crawler: $(CRAWLER) 

Indexer: $(INDEXER)
	
