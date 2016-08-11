/**
 * @file history.c
 * @author Josh Cotes
 * @date 14 Jan 2016
 */

#include <semaphore.h>
#include "history.h"
#include "../linkedlist/genericData.h"

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
void *buildType_Args(int argc, char **argv) {

    if (argc <= 0 || *argv == NULL)
        exit(-99);
    history *s_hist = calloc(1, sizeof(history));
    s_hist->argc = argc;
    s_hist->argv = argv;
    return s_hist;
}

/**
 * @brief Prints the specific types contents.
 *
 * The printType function is called via function pointer
 * Its focus is to print the specific type formatted for
 * the type
 *
 * @param passedIn - The void * passed in representing the specific data
 *
 * @warning - The passed in void * passedIn is checked - exit(-99) if NULL
 */
void printType(void *passedIn) {

    if (passedIn == NULL)
        exit(-99);

    history *s_hist;
    s_hist = (history *) passedIn;
    int counter;

    for (counter = 0; counter < s_hist->argc; counter++) {
        printf("%s ", *(s_hist->argv + counter));
    }
    printf("\n");
}

/**
 * @brief Cleans up all dynamically allocated memory for the specific data type
 *
 * Cleans up and frees all the dynamically allocated memory for the
 * specific data type.  Each pointer in the specific data type is set
 * to NULL after it has been freed.
 *
 * @param passedIn - The void * passed in representing the specific data
 *
 * @warning - The passed in void * passedIn is checked - exit(-99) if NULL
 */
void cleanType(void *passedIn) {

    if (passedIn == NULL)
        exit(-99);

    history *s_hist = passedIn;
    if (s_hist->argc == 0)
        return;

    int i = 0;
    for (i = 0; i < s_hist->argc; i++) {
        free(*(s_hist->argv + i));
        *(s_hist->argv + i) = NULL;
    }
    free(s_hist->argv);
    s_hist->argv = NULL;
    free(s_hist);
    s_hist = NULL;
    passedIn = NULL;
}

int histcount(FILE *fin) {

    int count = 0;
    int ch;
    int charcount = 0;

    while (!feof(fin)) {
        charcount++;
        ch = fgetc(fin);
        if (ch == '\n' && charcount > 1) {
            count++;
            charcount = 0;
        }
        if (ch == '\n' && charcount == 1) {
            charcount = 0;
        }

    }


    rewind(fin);
    return count;
}

void *readFile_History(FILE *fin) {

    char string_buffer[MAX];
    fgets(string_buffer, MAX, fin);
    void *node;
    char **argv;
    int argc;
    strip(string_buffer);
    if (strcmp(string_buffer, "") != 0)
        argc = makeargs(string_buffer, &argv);
    else
        return NULL;
    return buildType_Args(argc, argv);
}
