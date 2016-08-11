/**
 * @file history.c
 * @author Josh Cotes
 * @date 14 Jan 2016
 */

#ifndef HISTORY_H
#define HISTORY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../linkedlist/genericData.h"
#include "../tokenize/makeArgs.h"
#include "../linkedlist/linkedList.h"

#define MAX 100
#define D_HISTCOUNT 100
#define D_HISTFILECOUNT 1000

int HISTCOUNT;
int HISTFILECOUNT;
int PATH_SET;
char PATH[MAX];
char DIR[MAX];
char STARTPATH[MAX];
char MSSHHISTLOC[MAX];

/**
 * @brief The history data structure.
 *
 * A history contains an int for the number of arguments and a char ** for the actual arguments
 *
 * @note I prefer named structures and then the typedef after the structure
 */
struct hist {
    int argc;
    char **argv;
};

typedef struct hist history;

/**
 * @brief The builds the history data structure
 *
 * Builds the history structure and returns it as a void pointer
 *
 * @param argc - The int for the number of arguments
 * @param argv - The array to put the tokens into
 * @return void * - Representing the data structure history
 *
 * @warning - If there are no tokens or the String is null - exit(-99)
 */
void *buildType_Args(int argc, char **argv);

int histcount(FILE *fin);

void *readFile_History(FILE *fin);



#endif

