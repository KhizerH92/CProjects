#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>

//Function to Verify Mapfiles
//Exits on failure with an error message
void verifyMapfile(char* fileToCheck){
	FILE* file = fopen(fileToCheck, "r");

	if (file == NULL){
		printf("map invalid");
		exit(1);
	}
	
	char line[10] = {0};

	int filecounter=0;

	while(fgets(line,sizeof(line),file)!=NULL){
		++filecounter;

		if(strlen(line)<3){ fclose(file); exit(1);}

		if(line[0]==' ' || line[1]!=' ' || line[2]==' '){
			printf("Mapfile format is wrong");
			exit(1);
		} 
		if(line[3] != '\n' && line[3]!= '\r' && line[3]!=0 && line[3]!=' '){
			printf("Mapfile format is wrong");
			exit(1);	
		}

		memset(line,0,10);

	}

	fclose(file);
	if(!filecounter) {printf("map invalid"); exit(1);}
}

// creates a map using the map file
// using arrays

char* mapCreator(int reverse, char *mapfile){

	verifyMapfile(mapfile);

	FILE* file = fopen(mapfile, "r");
	char *map = malloc(1000);
	char line[10];

	if (file == NULL){return NULL;}

	while(fgets(line,sizeof(line),file)){
		if(reverse==0){
			int indexVal = line[2];
			map[indexVal] = line[0];
		}
		else{
			int indexVal = line[0];
			map[indexVal] = line[2];
		}
	}
	fclose(file);

	return map;
}

//Extra credit reverse file contents function

void reverseEverything(char* t, long c){
	for(long i = 0; i < c/2; ++i){
		char temp = t[i];
		t[i] = t[c-i-1];
		t[c-i-1] = temp;
	}		
}


//Encrypts or decrypts depending on switches using maps

void EncDecFunc(char *mapFile, char *line, int reverseOrNot){
		
	char* mapping = mapCreator(reverseOrNot, mapFile);

	int i = 0;
	while(line[i] != 0){
		if(isalpha(line[i]) && islower(line[i])){
			//printf("Do we ever get here");
			//printf("%c\n", line[i] );
			int indexValue = (int)line[i];
			if(mapping[indexValue]!=0){
				line[i]=mapping[indexValue];
				i++;
			}
		}
		else{
			i++;
		}
	}
	//return lineOutPut;
    		
}

int main( int argc, char *argv[] ) {
	/* Check for invalid arguments */
	extern char *optarg;
	extern int optind;
	char *encrypter = NULL;
	char *decrypter = NULL;

	//Taking care of help

	for(int i =0; i < argc; ++i){
		if(argv[i][0] != '-') continue;
		if(!strcmp("--help", argv[i])){
						//print your helping ins
			printf(
			"					\n"
			"WELCOME TO DAWG 2.0, DOG\n"
			"-r : reverse\n"
			"-n : number\n"
			"-m [OPTION]: decrypt\n "
			"-M [OPTION]: encrypt\n"
			);
			return 0;
		} //checks for the help function
		
	}




	//implementaiton of getopts for option processing
	int err=0;
	int nflag=0, mflag=0, Mflag = 0, rflag=0;
	int c;
	int todo = 0;
	while((c=getopt(argc,argv, "rnm:M:"))!=-1){
		switch(c){
		case 'n':
			nflag =1;
			break;

		case 'M':
			Mflag = 1;
			if(optarg==NULL){
				printf("Please provide a mapfile for option -M");
				return(0);
			}
			if(!todo) todo = 'M';
			encrypter = optarg; //we know that encrypter is only pointing to same argv as optarg
			break;

		case 'm':
			if(optarg==NULL){
				printf("Please provide a mapfile for option -m ");
				return(0);
			}
			if(!todo) todo = 'm';
			decrypter= optarg;
			mflag = 1;
			break;

		case 'r':
			rflag = 1;
			break;		

		case '?':
			err = 1;

			break;
		}  //chekcs for all the options that are being called or used
	}

	if(err==1){
		printf("an unknown option was provided or there is no filename");
		exit(1); //you exit with a value of 1 for error exits        				// these are both just error conditions if there is a wrong option or
	}
	else if (optind==argc){

		//this is where you wait for stdip
		// you basically process stdip the way you process the rest of the stuff
		// in files later:

		int keepGoing = 1;
		
		int counting=1;
		char* fromStd = malloc(sizeof(char)*1000);
		while(keepGoing){
			
			memset(fromStd,0,sizeof(char)*1000);

			if( !fgets(fromStd, 1000, stdin)){
				break;
			}


			long linsize = strlen(fromStd);


			if(nflag){
				printf("     %d ", counting);
				counting++;
			}
			
			if(rflag){
				reverseEverything(fromStd,linsize-1);
			}

			if(mflag * Mflag){
				if(todo == 'M'){
				
					//printf("%s", EncDecFunc(encrypter,line,1));  One way of doing things
					EncDecFunc(encrypter,fromStd,1);
					EncDecFunc(decrypter,fromStd,0);
				}

				else if(todo == 'm'){
					//printf("%s", EncDecFunc(decrypter,line,0));
					EncDecFunc(decrypter,fromStd,0);
					EncDecFunc(encrypter,fromStd,1);
				}
			}

			else if(todo == 'M'){
					//printf("%s", EncDecFunc(encrypter,line,1));  One way of doing things
					EncDecFunc(encrypter,fromStd,1);
			}

			else if(todo == 'm'){
					//printf("%s", EncDecFunc(decrypter,line,0));
					EncDecFunc(decrypter,fromStd,0);

			}


			printf("%s", fromStd);
			
		}
		free(fromStd);
				// the user is not giving any filename to act on after options
	}

	//adding files if they are not option required files:
	char** filenames = malloc(sizeof(char*)*argc);

	int count = 0;
	for (int i=1; i< argc; ++i){;
		filenames[i] = NULL;
		
		if(!strcmp(argv[i],"-m") || !strcmp(argv[i],"-M")){
			++i;
		}
		else if(argv[i][0]!='-'){
			
			filenames[count]=argv[i];
			++count;
		}  // You are just adding files that need to be acted upon by functions and maps into filenames array (array of pointers)
	}


	// Processing files as inputs based on options:
	char* line = (char*)malloc(BUFSIZ);

	for(int i  =0; i < count; ++i){
		//printf("%s\n", filenames[i]);

		FILE* fp;
		fp = fopen(filenames[i], "r");
		


		if (fp == NULL){
    		printf("file called %s does not exists\n", filenames[i]);
    		continue;
    	}


    	//following code finds file size
		fseek(fp,0L, SEEK_END);
		long fileSize = ftell(fp)+2;
		fseek(fp,0L, SEEK_SET);
		char* text = malloc(fileSize);
		memset(text, 0, fileSize);
		char* original = text;

		fread(text,1,fileSize-2,fp);
		

		if(rflag){
		 	reverseEverything(text, fileSize-2);
		 }

    	int count = 1;
    	char* limit = (text+fileSize-2);



    	while(text < limit){
    		while(*text == '\n' || *text == '\r'){
    			printf("%c", *text);
    			 ++text;
    		}


			int i = 0;    		
    		while(*text != '\n' && *text != '\r' && *text != '\0'){
    			line[i] = *text;
    			++text;
    			++i;
    		}

    		//if(text || )
    	
    		//if todo is M you get the line and encrypt it
    		//then see if 
			if(nflag){
				printf("%d  ", count );
				fflush(stdout);
				count++;
			}

			if(mflag * Mflag){
				if(todo == 'M'){
				
					//printf("%s", EncDecFunc(encrypter,line,1));  One way of doing things
					EncDecFunc(encrypter,line,1);
					EncDecFunc(decrypter,line,0);
				}

				else if(todo == 'm'){
					//printf("%s", EncDecFunc(decrypter,line,0));
					EncDecFunc(decrypter,line,0);
					EncDecFunc(encrypter,line,1);
				}
			}

			else if(todo == 'M'){
					
					//printf("%s", EncDecFunc(encrypter,line,1));  One way of doing things
					EncDecFunc(encrypter,line,1);
			}

			else if(todo == 'm'){
					//printf("%s", EncDecFunc(decrypter,line,0));
					EncDecFunc(decrypter,line,0);

			}

			printf("%s", line);
    		if(text < limit) printf("\n");
			memset(line, 0, BUFSIZ);

		}

		free(original);
	}

	free(line);    	


	free(filenames); //whenver you allocate memeory you need to free it up	
	filenames = NULL; // and then set the allocated pointer to NULL
	return(0);
}