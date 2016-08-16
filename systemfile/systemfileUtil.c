/**
 * @file msshHandler.c
 * @author Josh Cotes
 * @date 14 Jan 2016
 *
 * @brief Contains utilities to create, modify, and read the .mssh* files.
 *
 */

#include "systemfileUtil.h"

/**
 * @brief Reads and executes lines from the .msshrc file.
 *
 * Reads .msshrc line by line, sets global variables, and executes shell commands
 *
 * @param history - Linkedlist containing history
 * @param alias_list - Linkedlist containing alias list
 * @param fin -
 */
void run_msshrc(LinkedList *history, LinkedList *alias_list, FILE *fin) {

    if (alias_list == NULL || history == NULL)
        exit(-99);

    fin = fopen(".run_msshrc", "r");
    if (fin == NULL) {
        return;
    }

    char fileBuffer[MAX], tokenBuffer[MAX];
    char *token;

    while (fgets(fileBuffer, MAX, fin) != NULL) {

        strcpy(tokenBuffer, fileBuffer);
        token = strtok(tokenBuffer, ":=\n\0");

        // if HISTCOUNT command in file
        if (strcmp(token, "HISTCOUNT") == 0)
            HISTCOUNT = atoi(strtok(NULL, "=\n\0"));

            // if HISTFILECOUNT command in file
        else if (strcmp(token, "HISTFILECOUNT") == 0)
            HISTFILECOUNT = atoi(strtok(NULL, "=\n\0"));

            // if PATH=$PATH:/path... command in file
        else if (strcmp(token, "PATH") == 0) {
            token = strtok(NULL, ":");

            // if $PATH string incorrect
            if (strcmp(token, "$PATH") != 0) {
                strip(fileBuffer);
                printf("%s: command not found\n", fileBuffer);
            }
            token = strtok(NULL, "\n\0");
            strcpy(PATH, token); // set path if correct
            PATH_SET = 1; // path is set
        }

            // if none of the above, execute as command
        else {
            strip(fileBuffer);
            command(fileBuffer, history, alias_list, 0);
        }
    }
    fclose(fin);
    fin = NULL;
    token = NULL;
}

