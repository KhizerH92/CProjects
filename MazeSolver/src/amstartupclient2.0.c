/*
 * Authors: Khizer, Ella, Doug
 *
 * Created: Sunday February 20th 2016
 *
 * Script Name: amstartupclient.c
 *
 * Program Description: Runs the amazing maze solver.  Creates threads using the avatar daemon. Processes arguments.
 *
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#define _GNU_SOURCE

#define BUFFER 1024
#define ERRORCODE -1
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <time.h>

// ---------------- Local includes

#include "amazing-2.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

int determineMove(Maze* m, int x, int y,int LastMoveDirection);
int moveMessage(int Avatar_ID, int MoveDirection, int AvatarSocket);
int checkAndAssignArgs(int argc, char **argv, char *hostName, char *numOfAvatars, char *difficulty);
void initPathFinder();



// Main function:  Initializes Server Connection, Creates avatar threads

int main(int argc, char **argv){
	convergeBool = 0;
	// Initialize Data Structures
	firstMove = 0;
	targetX = 0;
	targetY = 0;
	AreWeDone =0;

	nAv = calloc(AM_MAX_MESSAGE,sizeof(char));
	diffLevel = calloc(AM_MAX_MESSAGE,sizeof(char));
	hostName = calloc(AM_MAX_MESSAGE,sizeof(char));
	struct sockaddr_in remote_server; //contains ip address and port nummber of remote tcp server
	int socketToUse;
	IP = calloc(AM_MAX_MESSAGE,sizeof(char));
	struct AM_Message *initialMessage, *replyRec;


	//takes the arguments, makes sure they are correct and assigns them to holders
	int correctArgs;
	correctArgs = checkAndAssignArgs(argc,argv,hostName,nAv,diffLevel);
	if(correctArgs == 0){
		free(nAv);
		free(diffLevel);
		free(hostName);
		free(IP);
		exit(6);
	}

	// printf("%s\n", hostName );
	// printf("%s\n", nAv);
	// printf("%s\n", diffLevel);

	int numOfAvatars = atoi(nAv);
	int difficultyLevel = atoi(diffLevel);
	
	//BFS STUFF HERE:
	// char binString[100] = {'1','1','1','1','1','1','1','1','1','1'};


	// binString[numOfAvatars] = '\0';
	// convergencePoint = atoi(binString);

	//-------------Wriite the initial message to be sent to the server ------------------------
	initialMessage = (struct AM_Message *)calloc(1,(sizeof(AM_Message)));

	initialMessage->type = htonl(AM_INIT);
	initialMessage->init.nAvatars = htonl((numOfAvatars));
	initialMessage->init.Difficulty = htonl(difficultyLevel);

	//-------------------- Get the IP address figured Out from hostname--------------------------

	struct hostent *host = gethostbyname(hostName);
	struct in_addr address;

	memcpy(&address,host->h_addr_list[0],sizeof(struct in_addr));
		
	strncpy(IP,inet_ntoa(address),AM_MAX_MESSAGE);
	printf("%s\n", IP );

	//--------------------------------------------------------------------------------------------

	if((socketToUse = socket(AF_INET,SOCK_STREAM,0)) == ERRORCODE){
		perror("socket");
		exit(-1);;
	}

	//----------------------- Connect To the Remote Server ---------------------------------------
	
	memset(&remote_server,0,sizeof(remote_server));
	remote_server.sin_family = AF_INET;
	remote_server.sin_addr.s_addr = inet_addr(IP);
	remote_server.sin_port = htons(atoi(AM_SERVER_PORT));
	
	//printf("%u\n", remote_server.sin_addr.s_addr);


	if(connect(socketToUse,(struct sockaddr *)&remote_server,sizeof(remote_server))<0){
		printf("Could not connect");
		return 1;
	}

	//printf("%u\n", ntohl(initialMessage->type));

	//int sent = send(socketToUse,initialMessage,(sizeof(initialMessage->type)+sizeof(initialMessage->init)),0);
	int sent = send(socketToUse,initialMessage,(sizeof(AM_Message)),0);

	if(sent <= 0){
		printf("Sending the initial Message did not work. Please try and sending it again\n");
		exit(2);
	}
	
	
	replyRec = (struct AM_Message *)calloc(1,sizeof(AM_Message));
	int rec;

	if((rec = recv(socketToUse,replyRec,sizeof(AM_Message),0))<0){
		printf("Server terminated prematurely\n");
		exit(4);
	}
	printf("%d\n",rec );

	//Gotta test the received signal for errors here somehow:

	if(rec<0 || ntohl(replyRec->type) == AM_ERROR_MASK){
		printf("Couldn't communicate with the server. Try again.\n");
		exit(5);
	}


	//--------------------------------------------------------
	//IF everything is fine, extract required information:

	mazePort = ntohl(replyRec->init_ok.MazePort);
	width = ntohl(replyRec->init_ok.MazeWidth);
	height = ntohl(replyRec->init_ok.MazeHeight);

	targetX = width/2;
	targetY = height/2;
	// raise(SIGINT);


	// printf("%u\n", mazePort);
	// printf("%u\n", width);
	// printf("%u\n", height);

	//=============== Initializing the log file for the Avatars to right to ===============================================
	char *Username;// = calloc(AM_MAX_MESSAGE, sizeof(char));
	Username = getenv("USER");
	
	logFileName =calloc(AM_MAX_MESSAGE,sizeof(char));
	sprintf(logFileName, "Amazing_%s_%d_%d.log",Username,numOfAvatars,difficultyLevel);

	//Call to create the file:
	char *makeLog = calloc(AM_MAX_MESSAGE,sizeof(char));
	sprintf(makeLog,"echo -n "" > $%s",logFileName);
	//printf("%s\n",makeLog);

	//============================= Add the first line to the file with required infor =================================
	char *firstLine = calloc(AM_MAX_MESSAGE, sizeof(char));
	sprintf(firstLine,"echo %s %u `date` >> $%s", Username, mazePort, logFileName);
	//printf("%s\n", firstLine);

	//===========Initializing Avatar Clients =======================================================


	//Initialize Maze
	mainMaze = calloc(1,sizeof(Maze));
	init_maze(mainMaze, width, height);


	// Open the outfile:
	char* str = calloc(100,sizeof(char));
	sprintf(str,"logfiles/%s", logFileName);
	outfile = fopen(str, "w");
	// first line should be the number of avatars
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	fprintf(outfile, "%s, %d, %d, %d-%d-%d %d:%d:%d\n",Username, mazePort, numOfAvatars, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	free(str);


	pthread_t tid[numOfAvatars];


	if (pthread_mutex_init(&mutex, NULL) != 0){
		printf("Mutex init failed.\n");
		return -1;
	}

	initPathFinder();

	for(int Avatar_ID = 0; Avatar_ID < numOfAvatars; Avatar_ID++){
		printf("Creating Thread: %d\n", Avatar_ID);
		pthread_create(&tid[Avatar_ID], NULL, &avdaemon, (void *) (intptr_t)Avatar_ID);
	}
	
	//printf("it never gets here\n");
	free(initialMessage);
	free(replyRec);
	//free(Username);
	free(firstLine);
	free(makeLog);


	// May need to add:
	//pthread_join() to prevent main from closing, killing sub threads.
	//printf("%d\n", numOfAvatars );
	for(int Avatar_ID = 0; Avatar_ID < numOfAvatars; Avatar_ID++){
		//printf("Threads Join?!?! %d\n", Avatar_ID);
		pthread_join(tid[Avatar_ID],NULL);
	}

	//printf("What about after?!?!\n");
	free(nAv);
	//free(diffLevel);
	free(IP);
	free(hostName);
	free(logFileName);
	//free(numOfAvatars);
	free(diffLevel);
	//free(host);
	//free(Username);
	fclose(outfile);

	cleanUpMaze(mainMaze);

	pthread_exit(NULL);


}

//============ HELPER FUNCTION TO CHECK ARGUMENTS AND ASSIGN THEM TO THE REQUIRED VARIABLES===============

int checkAndAssignArgs(int argc, char **argv, char *hostName, char *numOfAvatars, char *difficulty){
	int nflags =0;
	int dflags = 0;
	int hflags = 0;

	if(argc == 2 && strcmp("--help",argv[1])==0){
		printf("The options need to be prvided in the following format:\n");
		printf("./amstartup -n [number of Avatarts] -d [difficulty of the maze] -h [hostname]\n");
		printf("Please note that the hostname has to be flume.cs.dartmouth.edu\n");
		return 0;
	}

	if(argc!=7){
		printf("Not enough Arguments have been given\n");
		return 0;
	}

	for(int i=0; i<argc;i++){
		if(strcmp(argv[i],"-n")==0 && argv[i+1]){
			if(atoi(argv[i+1])>10 || atoi(argv[i+1])<1){
				printf("Number of Avatars not allowed\n");
				return 0;
			}
			nflags += 1;
			sprintf(numOfAvatars,"%s",argv[i+1]);
			//printf("%s\n", numOfAvatars );
		}

		if(strcmp(argv[i],"-d")==0 && argv[i+1]){
			if(atoi(argv[i+1])>9 || atoi(argv[i+1]) < 0){
				printf("That level of difficult is not allowed! Jesus!\n");
				return 0;
			}
			dflags += 1;
			//*difficulty = atoi(argv[i+1]);
			sprintf(difficulty,"%s",argv[i+1]);
			//printf("%s\n", difficulty);
		}

		if(strcmp(argv[i],"-h")==0 && argv[i+1]){
			if(strcmp(argv[i+1],"flume.cs.dartmouth.edu")!=0){
				printf("Wrong hostname entered. IT'S FLUME!!\n");
				return 0;
			}
			hflags += 1;
			//*hostName = argv[i+1];
			sprintf(hostName,"%s",argv[i+1]);
			//printf("%s\n", hostName );
		}

	}
	if(nflags!=1 || hflags!=1 || dflags!=1){
		printf("Incorrect Arguments given. Wrong number of options found. Try Again\n");
		return 0;
	}

	return 1;
}


void initPathFinder(){
	for(int x=0; x<11; x++){
		pathFinder[x][0] = 1;
	}
}
