/* ========================================================================== */
/* File: funcFile.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Query Engine
 *
 * Author: Ira Ray Jenkins - jenkins@cs.dartmouth.edu
 * Compiler/Co-Author : Khizar Hussain
 * Date: Tue Apr 29, 2014
 *
 * These functions are provided for the students to simplify the manipulation
 * of files and directories. In addition to the provided functions are some functions
 * written by Khizar Hussain to help with Query Engine component. 
 *
 *      Thus Functions are borrowed from file.c provided by professor earlier, written
 *      specifically for Query Engine and borrowed from some web.c files provided.
 *
 */
/* ========================================================================== */
#define _DEFAULT_SOURCE                          // scandir,
                                             // students shouldn't take advantage
                                             // of the bsd extensions, but
                                             // it makes it easier to test file
                                             // types

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>                          // free, calloc
#include <string.h>                          // strlen, strcpy
#include <dirent.h>                          // scandir, alphasort
#include <stdio.h>

#include <sys/stat.h>                        // stat functionality

// ---------------- Local includes  e.g., "file.h"
#include "hashtable.h"
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
static int SelectFile(const struct dirent *entry);

// ---------------- Public functions

int IsDir(const char *path)
{
    struct stat statbuf;                     // stat buffer

    // make sure we have a path
    if(!path) return 0;

    // if we can stat it, and it's a directory
    return (!stat(path, &statbuf) && S_ISDIR(statbuf.st_mode));
}

int IsFile(const char *path)
{
    struct stat statbuf;                     // stat buffer

    // make sure we have a path
    if(!path) return 0;

    // if we can stat it, and it's a regular file
    return (!stat(path, &statbuf) && S_ISREG(statbuf.st_mode));
}

int GetFilenamesInDir(const char *dir, char ***filenames)
{
    struct dirent **namelist;                // list of dir entries from scandir
    int num_ents;                            // number of regular files found
    int status;                              // return status

    // make sure we have a directory path to work with
    if(!dir) { return -1; }

    // scan the directory for regular files
    status = num_ents = scandir(dir, &namelist, SelectFile, alphasort);
    if(num_ents < 0) {                       // something went wrong scanning
        return -1;
    }

    // allocate the filenames array
    *filenames = calloc(num_ents, sizeof(char*));
    if(!*filenames) {
        status = -1;
        goto cleanup;                        // sorry Dijkstra
    }

    // copy all the filenames and free the dirents
    for(int i = 0; i < num_ents; i++) {
        // allocate a filename large enough to contain the name
        (*filenames)[i] = calloc(strlen(namelist[i]->d_name) + 1, sizeof(char));
        if(!(*filenames)[i]) {
            status = -1;
            goto cleanup;                    // sorry Dijkstra
        }

        // copy the filename
        strcpy((*filenames)[i], namelist[i]->d_name);
    }

cleanup:
    if(namelist) {
        for(int i = 0; i < num_ents; i++) {
            if(namelist[i])
                free(namelist[i]);
        }
        free(namelist);
    }

    return status;
}

//=================================================================================
//=======The metho tests if the query given is words only and not some unsavvy characters =====
int checkAgainstNonAlpha(char *words){
    for(int o=0; o<strlen(words);++o){
            if(isdigit(words[o]) || !isalpha(words[o])){
                printf("Only letters are expected among arguments\n");
                //should this be an exit or a return or a goto cleanup?
                //exit(1);
                return -1;  
            } 
        }
    return 1;
}
//==================== Borrowed NOrmalize Function ===============================

void NormalizeWord(char *word)
{
    for(char *ptr = word; *ptr; ++ptr)
        *ptr = tolower(*ptr);
}

// ========================== Check and process query String ======================

char *processQueryString(char *query){
    //if it's just an empty line prompt for arguments;
    if(strcmp(query,"\n")==0){
        printf("Please provide valid arguments\n");
        return NULL;
    }

    if(query[0]==' '){
        printf("Spaces don't count as arguments. Please provide valid arguments\n");
        return NULL;
    }

    //* Assumption: User will not enter a line break character in the query */

    //remove end of the line:
    char *actQuery;
    actQuery = strtok(query,"\n");

    if(!actQuery){
        printf("Please provide a valid query\n");
        return NULL;
    }

    //Now break the query provided down and check for Operators Validity on the query:

    char **checkQ=calloc(strlen(actQuery)+1,sizeof(char*)); //to add all the tokens to an array for testing
    //separate tokens based on space:

    char *token;

    token = strtok(actQuery," ");
    int l = 0;

    //Add all the tokens to an array:
    while(token){
        checkQ[l] = calloc(strlen(token)+1,sizeof(char));
        strcpy(checkQ[l],token);
        //printf("%s\n",checkQ[l]);
        token = strtok(NULL," ");
        l += 1;
    }

    //Run tests on this array, making sure AND and OR are used as binary operators:
    //printf("%s\n", checkQ[0]);

    if ((strcmp(checkQ[0], "OR") == 0 || (strcmp(checkQ[l-1], "OR") == 0 || strcmp(checkQ[0], "AND") == 0)  || strcmp(checkQ[l-1], "AND") == 0)) {
            printf("Query can't start or end with Logical operators \n");
            goto cleanup;
            //return NULL;
    }

    // if(strcmp(checkQ[0],"''")==0 || checkQ[0]==NULL){
    //  printf("Can't start with a space only. Please provide some valid arguments\n");
    //  return NULL;
    // }

    //check for successive operators:
    for(int k=0;k<l-1;k++){
            if((strcmp(checkQ[k],"OR")==0 || strcmp(checkQ[k],"AND")==0) && (strcmp(checkQ[k+1],"OR")==0 || strcmp(checkQ[k+1],"AND")==0)){
            printf("Two Logical operators cannot follow each other immediately! Please input a correct Query\n");
            goto cleanup;
            //return NULL;
        }
    }

    //If everything checks out, return a character buffer for the program to work with:
    char *returnBuffer = calloc(MAX_QUERY*2,sizeof(char));

    if(checkAgainstNonAlpha(checkQ[0])==-1) return NULL;
    
    NormalizeWord(checkQ[0]);
    strcpy(returnBuffer, checkQ[0]);


    int last = 0;

    for(int n=1;n<l;n++){
        if(checkAgainstNonAlpha(checkQ[n])==-1) return NULL;
        if(strcmp(checkQ[n],"AND")==0){
            ;
        }
        else if(strcmp(checkQ[n],"OR")==0){
            strcat(returnBuffer,"\n");
            last = 1;
        }
        else{
            if(last!=1){
                strcat(returnBuffer," ");
            }
            NormalizeWord(checkQ[n]);
            strcat(returnBuffer,checkQ[n]);
            last = 0;
        }
    }
    //printf("shit\n");
    for(int n=0;n<l;++n){
        free(checkQ[n]);
    }
    free(checkQ);
    
    strcat(returnBuffer,"\n");
    return returnBuffer;


    cleanup:
        for(int n=0;n<l;++n){
            free(checkQ[n]);
        }
        free(checkQ);
    // if(returnBuffer)free(returnBuffer);
    return NULL;
    
    
}

// ========================= Check all the strings in the files ===================

 int isStringOk(char *testing){
    testing = strdup(testing);

    strtok(testing," ");

    int num = 0;
    int counter=0;

    char* freq = NULL;

    do{
        if(!freq)continue;
        for(int o=0; o<strlen(freq);++o){
            if(!isdigit(freq[o]) && freq[o]!='\n') return -1;
        }

        if(!num)num=atoi(freq);
        
        ++counter;
    }while((freq = strtok(NULL," ")) && *freq!='\n');


    //printf("counter: %d, and num:%d \n", counter, num );
    if( (counter) != (num*2+1)){
        return -1;
    }
    //------doesn't really work ----
    free(testing);
    //---------------------------
    return 1;
}
//========================= ARRAY FORMING FUNCTIONS ==============================

int GetDocumentID(char* fileName){
    for(int i=0; i<strlen(fileName); ++i){
        if(!isdigit(fileName[i])){
            //printf("wrong name");
            return -1;
        }
    }
    int x = atoi(fileName);
    return x;
}

char* constructPath(char *fileadd, char* mainPath){
    //char* path = calloc(1,strlen(fileadd)); works
    char* path = calloc((strlen(fileadd)+10+strlen(mainPath)),sizeof(char));
    int pathLen = strlen(mainPath);
    if(mainPath[pathLen-1]!='/'){
        sprintf(path,"%s/%s", mainPath,fileadd); 
    }
    else{
         sprintf(path,"%s%s", mainPath,fileadd); 
    }
    return path;
}


 //======================== Rebuild from file function ============================


char* Strdupp(char* o){
        char *d =calloc(strlen(o)+2, sizeof(char));
        sprintf(d,"%s", o);
        return d;
}
 int reBuildFromFile(GenHashTable *hashTab, FILE *file){
    char line[20000] = {0};

    while(fgets(line,20000,file)!=NULL){
        
        char *testString = Strdupp(line);


        if(isStringOk(testString)==-1) {
            memset(line,0,20000);
            //---------------
            free(testString);
            //---------------
            continue;}

        char* p = strtok(line," "); 
        char *arrayToadd = Strdupp(p);

       //works
        //printf("%s\n", arrayToadd);


        int frequency = atoi(strtok(NULL," "));
        //printf("%d", frequency);

        for(int l=0; l<frequency; ++l){
            int fileId = atoi(strtok(NULL," "));
            //printf("%d", fileId);
            int occ = atoi(strtok(NULL," "));
            //printf("%d\n", occ);
            for(int m=0; m<occ; ++m){
                //printf("%s %d\n", arrayToadd, fileId);
                GenHashInsert(arrayToadd,hashTab,fileId);
                
                //printf("%s\n", getHashVal(hashTab,arrayToadd)->word);
            }
         }


         
        free(arrayToadd);
        
         //------added later----
         free(testString);
         //-------------------
         memset(line,0,20000);
    }
    return 1;
}

//---------------------------------------------------------------------------------
/*
 * SelectFile - test if a directory entry is a file
 * @entry: directory entry to test
 *
 * Returns non-zero if the directory entry is a regular file; otherwise, 0.
 *
 * Known issues:
 * This function uses the BSD dirent extensions: d_type, DT_REG, and DT_UNKNOWN.
 * Posix doesn't promise that dirent will contain a type, but BSD does. Using
 * these extensions avoids a separate call to stat.
 * Example:
 *    return(entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN);
 *
 * If this function is not working properly, you can replace the return
 * statement with a simpler test, for example to filter out "." and ".." dirs:
 * Example:
 *    return(strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."));
 *
 * If you do not want to filter out *any* entries, then simply use:
 * Example:
 *    return(1);
 *
 * Should have no use outside of this file, thus declared static.
 */
static int SelectFile(const struct dirent *entry){
    return (entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN);
}
