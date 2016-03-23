
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



#include "amazing-2.h"

// Private Prototypes:
int determineMove(Maze* m, int x, int y,int LastMoveDirection);
int moveMessage(int Avatar_ID, int MoveDirection, int AvatarSocket);
// int p(int base, int exp);
// int determPresence(int Avatar_ID, int binary);
int BFS(int x, int y, int GoalX, int GoalY, int Avatar_ID, int level, int BFSvisited[][100]);



/*
 * A function to run as the controller for a single avatar.  initiated as a thread and runs until maze is solved.
 */
void *avdaemon(void* threadIdOrAvatarId){


	// Tuple center;
	// center.x = mainMaze->x_max / 2;
	// center.y = mainMaze->y_max / 2;
	// printf("Maze Center %d, %d   grid[2][1]: %d\n", center.x, center.y, mainMaze->grid[2][1].up);

	//================================ TO keep track of the previous moves ========================

	Tuple AvatarBoy;
	AvatarBoy.x = -1;
	AvatarBoy.y = -1;
	AvatarBoy.LastMoveDirection = -1;

	Tuple prev; // used for moving the node in the ascii graphics.
	prev.x = 0;
	prev.y = 0;


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

	// Variables to keep track of the updated X Y positions:
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

	free(AvatarInitMsg);

	printf("mazePort %d: \n", mazePort);


	//Keep running the Avatars around as long as the server keeps guiding us
	while ((AvRec=recv(AvatarSocket,AvatarRetMsg,sizeof(AM_Message),0))>0 && AreWeDone==0){

		//if there is an error go to cleanup
		if(AvRec<=0 || ntohl(AvatarRetMsg->type)==AM_ERROR_MASK){
			printf("Avatar communication is down\n");
			//pthread_mutex_unlock(&mutex);
			AreWeDone = 1;
			continue;
		}

		//if the maze is solved One Avatar should print that to the file and then go to clean up
		if(ntohl(AvatarRetMsg->type)==AM_MAZE_SOLVED && Avatar_ID==0){

				// Print to the logfile once with the number of moves, and hash.
			int nAvatars = ntohl(AvatarRetMsg->maze_solved.nAvatars);
			int Difficulty = ntohl(AvatarRetMsg->maze_solved.Difficulty);
			int moves = ntohl(AvatarRetMsg->maze_solved.nMoves);
			int hash = ntohl(AvatarRetMsg->maze_solved.Hash);

			fprintf(outfile, "Solved with-- # of Avatars:%d Diff:%d  Moves:%d Hash:%d\n",nAvatars,Difficulty,moves,hash);
			//fclose(outfile);

			printf("Printing Maze to file....\n");
			print_maze_tofile(mainMaze,"logfiles/maze.dat", mainMaze->x_max, mainMaze->y_max);
			printf("done\n");
		
			AreWeDone = 1;
			continue;
		}

		//Get all the info from the return message and save it:

		turnNumber = ntohl(AvatarRetMsg->avatar_turn.TurnId);




		// Make a move if it is the correct turn
		if (Avatar_ID == turnNumber){
			//pthread_mutex_lock(&mutex);

			//=========== Get the updated position =====================
			XUpdated = ntohl(AvatarRetMsg->avatar_turn.Pos[Avatar_ID].x);
			YUpdated =ntohl(AvatarRetMsg->avatar_turn.Pos[Avatar_ID].y);


			// Update the logfile for all of the Avatars current position:
			if(Avatar_ID==0){
				for(int e=0; e<totalAvatars; e++){
					int XtoWrite = ntohl(AvatarRetMsg->avatar_turn.Pos[e].x);
					int YtoWrite =ntohl(AvatarRetMsg->avatar_turn.Pos[e].y);
					fprintf(outfile, "Avatar_ID: %d, x :%d, y: %d\n", e, XtoWrite, YtoWrite);					
				}

			}

			

			//Update the the draw position of the avatar accordingly:

			mainMaze->grid[prev.x][prev.y].visited = -1;
			mainMaze->grid[XUpdated][YUpdated].visited = Avatar_ID;
			prev.x = XUpdated;
			prev.y = YUpdated;

			// if(determPresence(Avatar_ID, mainMaze->grid[XUpdated][YUpdated].binary)==0){
			// 	mainMaze->grid[XUpdated][YUpdated].binary += p(10,Avatar_ID);
			// }

			
			//=============================================================

			// If prev move failed, update maze with edge information.
		
			int atTarget = (XUpdated == targetX) && (YUpdated == targetY);
			
			//If the position is not changed add the walls accordingly and reorient the position:
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
			//If it is the first move start towards North:
			else if(AvatarBoy.LastMoveDirection==-1){
				AvatarBoy.x = XUpdated;
				AvatarBoy.y = YUpdated;
				AvatarBoy.LastMoveDirection=M_NORTH;
			}
			//If you were able to change the position then either fill up the ends of the maze and //
			// also update the maze with allowed directions in and out of it's mazeNode:
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

				AvatarBoy.x = XUpdated;
				AvatarBoy.y = YUpdated;
			}

			//Normally just keep following the LeftHand rule to move:
			returnMoveVal = determineMove(mainMaze, XUpdated, YUpdated,AvatarBoy.LastMoveDirection);
			
			//If convergence hasn't been found:
			if(convergeBool==0){
				int connections=0;

				for(int i=1; i<totalAvatars; i++){ //For each of the avatars check if Avatar 0  can find a connecting path:
					if(Avatar_ID!=0)break;
					

					int XAV = ntohl(AvatarRetMsg->avatar_turn.Pos[i].x);
					int YAV =ntohl(AvatarRetMsg->avatar_turn.Pos[i].y);
					
						
					int BFSvisited[100][100];
					for(int k=0;k<100;++k){
						for(int s=0;s<100;++s){
							BFSvisited[k][s]=0;
						}
					}

					if(BFS(XUpdated, YUpdated,XAV, YAV, Avatar_ID ,0, BFSvisited)==1){ //if it can then:
						connections++;
					}

				}

				if(connections==totalAvatars-1){
					targetX = XUpdated;
					targetY = YUpdated;
					convergeBool=1; //Announce that there is an available convergence point for all:
					BFSdone =1; //set BFS done so Avatar 0 doesn't do that again:
					atTarget = 1; //set Avatar 0  as the target

				}
					
			}

			if(convergeBool==1){ //if the convergence has been found
				if(BFSdone==0){ //Find path from the other Avatars:
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
						printf("BFS shouldn't be FAILING.\n");
						AreWeDone = 1;
						continue;
					}
				}

				if(!atTarget){ //And if the Avatar hasn't reached the target it should keep pulling out moves from the path FInder:
					int moveIndex = pathFinder[Avatar_ID][0];

					returnMoveVal = pathFinder[Avatar_ID][moveIndex];
					pathFinder[Avatar_ID][0]=moveIndex + 1;
				} else {
					returnMoveVal = M_NULL_MOVE;
				}
			}

		
			//printf(" This Value is returned: %d\n", returnMoveVal );

			//============================================================================================

			if((moveMessage(Avatar_ID, returnMoveVal, AvatarSocket))<=0){ //send a move message based on the direction determined
				printf("Error in movement\n");
				//pthread_mutex_unlock(&mutex);
				AreWeDone = 1;
				continue;
			}

			AvatarBoy.LastMoveDirection = returnMoveVal;


			if((AvRec = recv(AvatarSocket,AvatarMoveRetMsg,sizeof(AM_Message),0)) <= 0){
				printf("Server returned an unsavory message after Move message given\n");
				AreWeDone = 1;
				continue;
			}

			if(ntohl(AvatarMoveRetMsg->type)==AM_TOO_MANY_MOVES){ //Call out the ALgorithm on too many moves:
				printf("Too Many moves baby!");
				AreWeDone = 1;
				continue;
			}

			if(Avatar_ID==0){ //Avatar 0 to draw the graphics:
				asciiDisplay(mainMaze);	
			}
		}

	}


	free(AvatarRetMsg);
	free(AvatarMoveRetMsg);


	return NULL;
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

//=============================PACKAGES AND SENDS THE MOVE MESSAGE TO THE SERVER ================
int moveMessage(int Avatar_ID, int MoveDirection, int AvatarSocket){
	int returnVal = -1;
	AM_Message *MoveMsg;
	MoveMsg = (struct AM_Message *)calloc(1,(sizeof(AM_Message)));
	MoveMsg->type = htonl(AM_AVATAR_MOVE);
	MoveMsg->avatar_move.AvatarId = htonl(Avatar_ID); //gets the first argument
	MoveMsg->avatar_move.Direction = htonl(MoveDirection);

	returnVal = send(AvatarSocket,MoveMsg,(sizeof(AM_Message)),0);
	free(MoveMsg);
	return returnVal;
}

//-------------------------- Determine presence ---------------------------------------------


// int p(int base, int exp) {
//     if (exp == 0)
//         return 1;
//     else if (exp % 2)
//         return base * p(base, exp - 1);
//     else {
//         int temp = p(base, exp / 2);
//         return temp * temp;
//     }
// }

// int determPresence(int Avatar_ID, int binary){
// 	int x = binary/(p(10,Avatar_ID));
// 	x = x%10;
// 	return x; 
// }

// ============================= BFS Method that uses directions allowed on Nodes to find path ==================
int BFS(int x, int y, int GoalX, int GoalY, int Avatar_ID, int level, int BFSvisited[][100]){
	if(level==0){
		// printf("Avatar_ID for BFS BFS %d \n", Avatar_ID);
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
	
	return 0;
}

