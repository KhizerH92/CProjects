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
//#define _DEFAULT_SOURCE                          // scandir,
                                             // students shouldn't take advantage
                                             // of the bsd extensions, but
                                             // it makes it easier to test file
                                             // types

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>                          // free, calloc
#include <string.h>                          // strlen, strcpy
#include <dirent.h>                          // scandir, alphasort

#include <sys/stat.h>                        // stat functionality
#include "hashtable.h"
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
//static int SelectFile(const struct dirent *entry);

int IsDir(const char *path);

int IsFile(const char *path);

int GetFilenamesInDir(const char *dir, char ***filenames);
//=================================================================================
//=======The metho tests if the query given is words only and not some unsavvy characters =====
int checkAgainstNonAlpha(char *words);
//==================== Borrowed NOrmalize Function ===============================
void NormalizeWord(char *word);
// ========================== Check and process query String ======================
char *processQueryString(char *query);
// ========================= Check all the strings in the files ===================
 int isStringOk(char *testing);
//========================= ARRAY FORMING FUNCTIONS ==============================

int GetDocumentID(char* fileName);

char* constructPath(char *fileadd, char* mainPath);
 //======================== Rebuild from file function ============================

 int reBuildFromFile(GenHashTable *hashTab, FILE *file);

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
//static int SelectFile(const struct dirent *entry);
