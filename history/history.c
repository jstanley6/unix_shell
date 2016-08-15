/**
 * @file history.c
 * @author Josh Cotes
 * @date 14 Jan 2016
 *
 * @brief Contains tools to create, modify, query, and clean the history data structure
 */

#include <semaphore.h>
#include "history.h"

/**
 * @brief Builds a history node
 *
 * Builds a history structure and returns it as a void pointer
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

    S_history *newHistory = calloc(1, sizeof(S_history));
    newHistory->argc = argc;
    newHistory->argv = argv;
    return newHistory;
}

/**
 * @brief Prints the specific types contents.
 *
 * The printType function is called via function pointer
 * Its focus is to print the specific type formatted for
 * the history type
 *
 * @param passedIn - The void * passed in representing the specific data
 *
 * @warning - The passed in void * passedIn is checked - exit(-99) if NULL
 */
void printType(void *passedIn) {

    if (passedIn == NULL)
        exit(-99);

    S_history *s_hist;
    s_hist = (S_history *) passedIn;
    int counter;

    for (counter = 0; counter < s_hist->argc; counter++) {
        printf("%s ", *(s_hist->argv + counter));
    }
    printf("\n");
}

/**
 * @brief Cleans up all dynamically allocated memory for the history data type
 *
 * Cleans up and frees all the dynamically allocated memory for the
 * history data type.  Each pointer in the specific data type is set
 * to NULL after it has been freed.
 *
 * @param historyIn - The void * passed in representing the specific data
 *
 * @warning - The passed in void * passedIn is checked - exit(-99) if NULL
 */
void cleanType(void *historyIn) {

    if (historyIn == NULL)
        exit(-99);

    S_history *s_hist = historyIn;
    int i;

    if (s_hist->argc == 0)
        return;

    for (i = 0; i < s_hist->argc; i++) {
        free(*(s_hist->argv + i));
        *(s_hist->argv + i) = NULL;
    }

    free(s_hist->argv);
    s_hist->argv = NULL;
    free(s_hist);
    s_hist = NULL;
    historyIn = NULL;
}

/**
 * @brief Counts the lines of the file that the fin parameter points to
 *
 * Counts the number of lines of the file that the fin parameter points to. Ignores blank
 * lines and rewinds the passed in file pointer when done.
 *
 * @param fin - The file pointer
 * @return - The number of non blank lines in the file
 * @warning - The passed in *fin is checked - exit(-99) if NULL
 */
int histCount(FILE *fin) {

    if (fin == NULL)
        exit(-99);

    int lineCount = 0;
    int charHolder;
    int charCount = 0;

    while (!feof(fin)) {
        charCount++;
        charHolder = fgetc(fin);

        if (charHolder == '\n' && charCount > 1) {
            lineCount++;
            charCount = 0;
        }
        if (charHolder == '\n' && charCount == 1) {
            charCount = 0;
        }
    }
    rewind(fin);
    return lineCount;
}

/**
 * @brief Reads the next line of the file pointed to by fin and returns a S_history pointer with the data.
 *
 * Reads the next line of the file and returns a reference to buildType_Args, which returns a new
 * S_history data structure containing the data from the line of the file.
 *
 * @param fin - The file pointer
 * @return - New S_history data structure
 * @warning - The passed in *fin is checked - exit(-99) if NULL
 */
void *readFile_History(FILE *fin) {

    if (fin == NULL)
        exit(-99);

    char stringBuffer[MAX];
    char **argv;
    int argc;

    fgets(stringBuffer, MAX, fin);
    strip(stringBuffer);

    if (strcmp(stringBuffer, "") != 0)
        argc = makeargs(stringBuffer, &argv);
    else
        return NULL;

    return buildType_Args(argc, argv);
}
