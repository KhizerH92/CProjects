
// A series of C functions used to maintain an avatar client.

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

#include "amazing.h"

// Private Prototypes:
int determineMove(Maze* m, int x, int y,int LastMoveDirection);
int moveMessage(int Avatar_ID, int MoveDirection, int AvatarSocket);
int p(int base, int exp);
int determPresence(int Avatar_ID, int binary);
int BFS(int x, int y, int GoalX, int GoalY, int Avatar_ID, int level, int BFSvisited[][100]);



/*
 * A function to run as the controller for a single avatar.  initiated as a thread and runs until maze is solved.
 */
void *avdaemon(void* threadIdOrAvatarId){


	//pthread_mutex_lock(&mutex);

	Tuple center;
	center.x = mainMaze->x_max / 2;
	center.y = mainMaze->y_max / 2;
	printf("Maze Center %d, %d   grid[2][1]: %d\n", center.x, center.y, mainMaze->grid[2][1].up);

	//================================ TO keep track of the previous moves ========================

	Tuple AvatarBoy;
	AvatarBoy.x = -1;
	AvatarBoy.y = -1;
	AvatarBoy.LastMoveDirection = -1;

	Tuple prev; // used for moving the node in the ascii graphics.
	prev.x = 0;
	prev.y = 0;

	printf("How many times? %d\n", AvatarBoy.LastMoveDirection);


	int Avatar_ID = (intptr_t)threadIdOrAvatarId;


	//============= Initialize Avatar Connection ===============================
	struct sockaddr_in remote_server;

	int turnNumber;
	int totalAvatars = atoi(nAv);
	int AvatarSocket;
	int AvRec; //To gauge received message size


	AM_Message *AvatarInitMsg, *AvatarRetMsg, *AvatarMoveRetMsg;
	AvatarInitMsg = (struct AM_Message *)calloc(1,(sizeof(AM_Message)));
	AvatarRetMsg = (struct AM_Message *)calloc(1,(sizeof(AM_Message)));
	AvatarMoveRetMsg = (struct AM_Message *)calloc(1,(sizeof(AM_Message)));
	//-------------Wriite the initial message to be sent to the server ------------------------
	

	AvatarInitMsg->type = htonl(AM_AVATAR_READY);
	AvatarInitMsg->avatar_ready.AvatarId = htonl(Avatar_ID); //gets the first argument
	

	if((AvatarSocket=socket(AF_INET,SOCK_STREAM,0))== ERRORCODE){
		perror("socket");
		//pthread_mutex_unlock(&mutex);
		exit(-1);;
	}

	//----------------------- Connect To the Remote Server ---------------------------------------
	
	memset(&remote_server,0,sizeof(remote_server));
	remote_server.sin_family = AF_INET;
	remote_server.sin_addr.s_addr = inet_addr(IP);
	remote_server.sin_port = htons(mazePort);
	

	if(connect(AvatarSocket,(struct sockaddr *)&remote_server,sizeof(remote_server))<0){
		printf("Could not connect");
		//pthread_mutex_unlock(&mutex);
		exit(20);
	}

	int XUpdated;
	int YUpdated;


	int sent = send(AvatarSocket,AvatarInitMsg,(sizeof(AM_Message)),0);

	if(sent<=0){
		printf("Sending the initial Message did not work. Please try and sending it again\n");
		//pthread_mutex_unlock(&mutex);
		exit(2);
	}

	int returnMoveVal;
	int BFSdone = 0;

	printf("mazePort %d: \n", mazePort);

	while ((AvRec=recv(AvatarSocket,AvatarRetMsg,sizeof(AM_Message),0))>0){


		if(AvRec<=0 || ntohl(AvatarRetMsg->type)==AM_ERROR_MASK){
			printf("Avatar communication is down\n");
			//pthread_mutex_unlock(&mutex);
			exit(4);
		}

		if(AvRec<=0 || ntohl(AvatarRetMsg->type)==AM_MAZE_SOLVED){
			printf("MAZE HAS BEEN SOLVED. PROCEED TO CLEANUP\n");

			pthread_mutex_lock(&mutex);
			if (mazeprintflag == 0){  // Only print it once....

				fprintf(outfile, "Maze Has Been Solved\n");
				fclose(outfile);

				printf("Printing Maze to file....\n");
				print_maze_tofile(mainMaze,"logfiles/maze.dat", mainMaze->x_max, mainMaze->y_max);
				printf("done\n");
				mazeprintflag = 1;
			}
			pthread_mutex_unlock(&mutex);
			exit(4);
		}

		//Get all the info from the return message and save it:

		turnNumber = ntohl(AvatarRetMsg->avatar_turn.TurnId);
		//printf("Turn Number: %d\n", turnNumber);
		//printf("Avatar_ID: %d\n", Avatar_ID);
		//printf("%d\n", mazePort);



		// Make a move if it is the correct turn
		if (Avatar_ID == turnNumber){
			//pthread_mutex_lock(&mutex);

			//=========== Get the updated position =====================
			XUpdated = ntohl(AvatarRetMsg->avatar_turn.Pos[Avatar_ID].x);
			YUpdated =ntohl(AvatarRetMsg->avatar_turn.Pos[Avatar_ID].y);

			printf("Avatar_ID: %d Current X %d Y %d \n", Avatar_ID, XUpdated, YUpdated);
			printf("Targt Location X: %d Y: %d \n", targetX, targetY);


			// Update the logfile...
			//fprintf(outfile, "%d: %d,%d\n", Avatar_ID, XUpdated, YUpdated);
			

			//Update the the draw position of the avatar accordingly:

			mainMaze->grid[prev.x][prev.y].visited = -1;
			mainMaze->grid[XUpdated][YUpdated].visited = Avatar_ID;
			prev.x = XUpdated;
			prev.y = YUpdated;

			if(determPresence(Avatar_ID, mainMaze->grid[XUpdated][YUpdated].binary)==0){
				mainMaze->grid[XUpdated][YUpdated].binary += p(10,Avatar_ID);
			}

			//printf("X:%d Y:%d\n", XUpdated, YUpdated);

			// if(turnNumber==0 && firstMove==0){
			// 	targetX = XUpdated;
			// 	targetY = YUpdated;
			// 	mainMaze->grid[XUpdated][YUpdated].visited = Avatar_ID;
			// 	firstMove = 1;
			// }
			//=============================================================

			// If prev move failed, update maze with edge information.
			if(1){

				if(XUpdated != targetX || YUpdated != targetY){


					if (AvatarBoy.x == XUpdated && AvatarBoy.y == YUpdated){

						if(AvatarBoy.LastMoveDirection == M_NORTH) {
							mainMaze->grid[XUpdated][YUpdated].up=-1;
							AvatarBoy.LastMoveDirection = M_EAST;
						}
						else if(AvatarBoy.LastMoveDirection == M_SOUTH){
							mainMaze->grid[XUpdated][YUpdated].down=-1;
								AvatarBoy.LastMoveDirection=M_WEST;
						} 

						else if(AvatarBoy.LastMoveDirection == M_EAST){
							mainMaze->grid[XUpdated][YUpdated].right=-1;
							AvatarBoy.LastMoveDirection = M_SOUTH;
						}
						
						else if(AvatarBoy.LastMoveDirection == M_WEST){
							mainMaze->grid[XUpdated][YUpdated].left=-1;
							AvatarBoy.LastMoveDirection = M_NORTH;
						}

					}

					else if(AvatarBoy.LastMoveDirection==-1){
						AvatarBoy.x = XUpdated;
						AvatarBoy.y = YUpdated;
						AvatarBoy.LastMoveDirection=M_NORTH;

					}
					else{
						MazeNode n;

						if(AvatarBoy.LastMoveDirection == M_WEST){
							n = mainMaze->grid[XUpdated+1][YUpdated];
							if(n.up==-1 && n.down==-1 && n.right==-1){
								mainMaze->grid[XUpdated][YUpdated].right=-1;

							}
							else{
								mainMaze->grid[XUpdated][YUpdated].Nmoves[M_EAST]=M_EAST+10;

							}

							mainMaze->grid[XUpdated+1][YUpdated].Nmoves[M_WEST]=M_WEST+10;

						}
						else if(AvatarBoy.LastMoveDirection == M_EAST){
							n = mainMaze->grid[XUpdated-1][YUpdated];
							if(n.up==-1 && n.down== -1 && n.left==-1){
								mainMaze->grid[XUpdated][YUpdated].left=-1;
							}
							else{
								mainMaze->grid[XUpdated][YUpdated].Nmoves[M_WEST]=M_WEST+10;
							}
							
							mainMaze->grid[XUpdated-1][YUpdated].Nmoves[M_EAST]=M_EAST+10;
						}
						else if(AvatarBoy.LastMoveDirection == M_NORTH){
							n = mainMaze->grid[XUpdated][YUpdated+1];
							if(n.right==-1 && n.down==-1 && n.left==-1){
								mainMaze->grid[XUpdated][YUpdated].down=-1;
							}
							else{
								mainMaze->grid[XUpdated][YUpdated].Nmoves[M_SOUTH]=M_SOUTH+10;
							}
							
							mainMaze->grid[XUpdated][YUpdated+1].Nmoves[M_NORTH]=M_NORTH+10;
						}
						else if(AvatarBoy.LastMoveDirection == M_SOUTH){
							n = mainMaze->grid[XUpdated][YUpdated-1];
							if(n.up==-1 && n.left==-1 && n.right==-1){
								mainMaze->grid[XUpdated][YUpdated].up=-1;
							}
							else{
								mainMaze->grid[XUpdated][YUpdated].Nmoves[M_NORTH]=M_NORTH+10;
							}
							
							mainMaze->grid[XUpdated][YUpdated-1].Nmoves[M_SOUTH]=M_SOUTH+10;
						}

						// if(Avatar_ID==2 && XUpdated==10 && YUpdated ==10){
						// 	int* x = NULL;
						// 	*x = 1;
						// }

						AvatarBoy.x = XUpdated;
						AvatarBoy.y = YUpdated;
					}


					returnMoveVal = determineMove(mainMaze, XUpdated, YUpdated,AvatarBoy.LastMoveDirection);
					
					if(convergeBool==0){
						int connections=0;

						for(int i=1; i<totalAvatars; i++){
							if(Avatar_ID!=0)break;
							

							int XAV = ntohl(AvatarRetMsg->avatar_turn.Pos[i].x);
							int YAV =ntohl(AvatarRetMsg->avatar_turn.Pos[i].y);
							
								
							int BFSvisited[100][100];
							for(int k=0;k<100;++k){
								for(int s=0;s<100;++s){
									BFSvisited[k][s]=0;
								}
							}

							if(BFS(XUpdated, YUpdated,XAV, YAV, Avatar_ID ,0, BFSvisited)==1){
								printf("%d\n", Avatar_ID);
								connections++;
							}

						}

						if(connections==totalAvatars-1){
							targetX = XUpdated;
							targetY = YUpdated;
							convergeBool=1;
							BFSdone =1;

						}
							
					}

					if(convergeBool==1){
						if(BFSdone==0){
							int BFSvisited[100][100];
							for(int k=0;k<100;++k){
								for(int s=0;s<100;++s){
									BFSvisited[k][s]=0;
								}
							}
							if(BFS(XUpdated, YUpdated,targetX, targetY, Avatar_ID ,0, BFSvisited)==1){

								BFSdone = 1;
							}
							else{
								printf("shouldn't be happening\n");
								int *x = NULL;
								*x = 1;
							}
						}
						int moveIndex = pathFinder[Avatar_ID][0];
						printf("FOUND CONVERGENCE!!\n");

						returnMoveVal = pathFinder[Avatar_ID][moveIndex];
						printf("MOved Avatar_ID:%d Move %d\n", Avatar_ID, returnMoveVal );
						pathFinder[Avatar_ID][0]=moveIndex + 1;
					}

				}

				else{

					if(AvatarBoy.LastMoveDirection == M_WEST){

						mainMaze->grid[XUpdated][YUpdated].Nmoves[M_EAST]=M_EAST+10;
						mainMaze->grid[XUpdated+1][YUpdated].Nmoves[M_WEST]=M_WEST+10;
					}
					else if(AvatarBoy.LastMoveDirection == M_EAST){
						
						mainMaze->grid[XUpdated][YUpdated].Nmoves[M_WEST]=M_WEST+10;
						mainMaze->grid[XUpdated-1][YUpdated].Nmoves[M_EAST]=M_EAST+10;
					}
					else if(AvatarBoy.LastMoveDirection == M_NORTH){
						
						mainMaze->grid[XUpdated][YUpdated].Nmoves[M_SOUTH]=M_SOUTH+10;
						mainMaze->grid[XUpdated][YUpdated+1].Nmoves[M_NORTH]=M_NORTH+10;
					}
					else if(AvatarBoy.LastMoveDirection == M_SOUTH){
						
						mainMaze->grid[XUpdated][YUpdated].Nmoves[M_NORTH]=M_NORTH+10;
						mainMaze->grid[XUpdated][YUpdated-1].Nmoves[M_SOUTH]=M_SOUTH+10;
					}

					// else{

					if(convergeBool==1){
						returnMoveVal = M_NULL_MOVE;
					} else {
						returnMoveVal = determineMove(mainMaze, XUpdated, YUpdated,AvatarBoy.LastMoveDirection);
					}
					

				}
			}

			if(Avatar_ID==0 && convergeBool==1){
				returnMoveVal = M_NULL_MOVE;
			}
			//printf(" This Value is returned: %d\n", returnMoveVal );

			//============================================================================================

			if((moveMessage(Avatar_ID, returnMoveVal, AvatarSocket))<=0){
				printf("Error in movement\n");
				//pthread_mutex_unlock(&mutex);
				exit(15);
			}

			AvatarBoy.LastMoveDirection = returnMoveVal;
			//printf("intendedMove %d\n", returnMoveVal);

			//printf("WHY DO WE NEVER GET HERE %d\n", AvatarBoy.LastMoveDirection );

			if((AvRec = recv(AvatarSocket,AvatarMoveRetMsg,sizeof(AM_Message),0)) <= 0){
				printf("Server returned an unsavory message after Move message given\n");
				continue;
			}

			if(ntohl(AvatarMoveRetMsg->type)==AM_TOO_MANY_MOVES){
				printf("Too Many fucking moves baby!");
				exit(9);
			}

			//evaluateMessage(AvatarMoveRetMsg);
			if(ntohl(AvatarMoveRetMsg->type)==AM_MAZE_SOLVED){
				printf("Maze Solved At MazePort: %d\n", mazePort);
				
				pthread_mutex_lock(&mutex);
				if (mazeprintflag == 0){  // Only print it once....
					printf("Printing Maze to file....\n");
					print_maze_tofile(mainMaze,"maze.dat", mainMaze->x_max, mainMaze->y_max);
					printf("done\n");
					mazeprintflag = 1;
				}
				pthread_mutex_unlock(&mutex);
				
				exit(1);
			}
			pthread_mutex_lock(&mutex);
			asciiDisplay(mainMaze);	
			pthread_mutex_unlock(&mutex);
		}

	}

	pthread_exit(NULL);
}

//=====================================Helper Functions ==================================================

/*
 * A function to determine the best move for the avatar to make based on breadcrumbs and wall knowledge.
 *
 */

int determineMove(Maze* maze, int x, int y, int LastMoveDirection){
		//printf("Maze transfers? %d\n", maze->grid[2][3].up );
		
		if(LastMoveDirection==M_NORTH){
			if (maze->grid[x][y].left != -1) {
				//printf("Maze transfers? %d\n", maze->grid[x][y].up );
				return M_WEST;
			}
			else if (maze->grid[x][y].up != -1) return M_NORTH;
			else if (maze->grid[x][y].right != -1) return M_EAST;
			else return M_SOUTH;
		}
		else if(LastMoveDirection==M_EAST){
			if (maze->grid[x][y].up != -1) {
				return M_NORTH;
			}
			else if (maze->grid[x][y].right != -1) return M_EAST;
			else if (maze->grid[x][y].down != -1) return M_SOUTH;
			else return M_WEST;
		}

		else if(LastMoveDirection==M_SOUTH){
			if (maze->grid[x][y].right != -1) {
				//printf("Maze transfers? %d\n", maze->grid[x][y].up );
				return M_EAST;
			}
			else if (maze->grid[x][y].down != -1) return M_SOUTH;
			else if (maze->grid[x][y].left != -1) return M_WEST;
			else return M_NORTH;
		}

		else if(LastMoveDirection==M_WEST){
			if (maze->grid[x][y].down != -1) {
				//printf("Maze transfers? %d\n", maze->grid[x][y].up );
				return M_SOUTH;
			}
			else if (maze->grid[x][y].left != -1) return M_WEST;
			else if (maze->grid[x][y].up != -1) return M_NORTH;
			else return M_EAST;
		}
		else if(LastMoveDirection==M_NULL_MOVE){
			return M_NULL_MOVE;
		}
		else{
			return M_NORTH;
		}
}

//========================================================================================================
int moveMessage(int Avatar_ID, int MoveDirection, int AvatarSocket){
	int returnVal = -1;
	AM_Message *MoveMsg;
	MoveMsg = (struct AM_Message *)calloc(1,(sizeof(AM_Message)));
	MoveMsg->type = htonl(AM_AVATAR_MOVE);
	MoveMsg->avatar_move.AvatarId = htonl(Avatar_ID); //gets the first argument
	MoveMsg->avatar_move.Direction = htonl(MoveDirection);

	returnVal = send(AvatarSocket,MoveMsg,(sizeof(AM_Message)),0);
	return returnVal;
}

//-------------------------- Determine presence ---------------------------------------------


int p(int base, int exp) {
    if (exp == 0)
        return 1;
    else if (exp % 2)
        return base * p(base, exp - 1);
    else {
        int temp = p(base, exp / 2);
        return temp * temp;
    }
}

int determPresence(int Avatar_ID, int binary){
	int x = binary/(p(10,Avatar_ID));
	x = x%10;
	return x; 
}

int BFS(int x, int y, int GoalX, int GoalY, int Avatar_ID, int level, int BFSvisited[][100]){
	if(level==0){
		printf("Avatar_ID for BFS BFS %d \n", Avatar_ID);
	}

	++level;

	if(BFSvisited[x][y]==1) return 0;
	else BFSvisited[x][y]=1;

	if(x==GoalX && y==GoalY){
		//printf("Khizer fucked up level %d\n", level);
		return 1;
	}

	//printf("he found the error too fuck yead\n");

	int* moves=mainMaze->grid[x][y].Nmoves;
	
	for(int r=0; r<4; r++){
		if(moves[r]==0){
			//printf("move continued \n");
			continue;
		}

		int move = moves[r]%10;
		//printf("move is %d\n", move);
		fflush(stdout);
		if(move==M_NORTH) {
			if(BFS(x,y-1,GoalX,GoalY, Avatar_ID,level,BFSvisited)){
				//printf("level%d move %d\n",level, move);
				pathFinder[Avatar_ID][level]=move;
				return 1;
			}
		}
		else if(move==M_SOUTH) {
			if(BFS(x,y+1,GoalX,GoalY, Avatar_ID,level,BFSvisited)){
				//printf("level%d move %d\n",level, move);
				pathFinder[Avatar_ID][level]=move;
				return 1;

			}
		}

		else if(move==M_WEST) {
			if(BFS(x-1,y,GoalX,GoalY, Avatar_ID,level,BFSvisited)){
				//printf("level%d move %d\n",level, move);
				pathFinder[Avatar_ID][level]=move;
				return 1;
			}
		}

		else if(move==M_EAST) {
			if(BFS(x+1,y,GoalX,GoalY, Avatar_ID,level,BFSvisited)){
				//printf("level%d move %d\n",level, move);
				pathFinder[Avatar_ID][level]=move;
				return 1;
			}
		}

	}

	if(level == 1 && convergeBool==1){
		//printf("oh shiiiiiiiiiiiiitttttt\n");
		//raise(SIGINT);
	}
	return 0;
}