//
// Created by josh on 1/26/16.
//

#include <stdlib.h>
#include <string.h>
#include "alias.h"

#define FALSE 0;
#define TRUE 1;

void getAliasCommand(char *s, LinkedList *theList) {

    if (theList == NULL) {
        exit(-99);
    }
    else if (theList->size == 0) {
        return;
    }
    else {
        Node *curr = theList->head;
        do {
            Alias *c_alias = (Alias *) curr->next->data;

            if (strcmp(c_alias->alias_name, s) == 0) {
                strcpy(s, c_alias->command);
                return;
            }
            curr = curr->next;
        } while (curr->next != NULL && curr != NULL);
    }
}

Node *buildNode_Alias(void *passedin) {

    Node *s_newnode;
    s_newnode = calloc(1, sizeof(Node));
    s_newnode->data = passedin;
    s_newnode->prev = NULL;
    s_newnode->next = NULL;
    return s_newnode;
}

void *buildType_Alias(char *name, char *command) {

    if (name == NULL)
        exit(-99);
    Alias *s_alias = calloc(1, sizeof(Alias));
    s_alias->alias_name = name;
    s_alias->command = command;
    return s_alias;
}

int checkFor_alias(char *s, char **alias, char **command) {

    char *rp = NULL;
    char *token = NULL;
    char delims[6] = {'\'', '=', '"', ' ', '\n', '\0'};
    char *s_ptr = NULL;
    char *tokens[2]; // holds the alias and command 0 and 1 respectively

    s_ptr = s;
    token = strtok_r(s_ptr, delims, &rp);
    s_ptr = NULL;
    if (token == NULL)
        return 0;

    else if (strcmp(token, "alias") == 0) { // if this is an alias command

        tokens[0] = strtok_r(s_ptr, delims, &rp); // this should return the alias name
        if (tokens[0] == NULL) {
            return 3;
        }
        // flexible alias format, get alias name and command //
        if (strcmp(tokens[0], "-p") == 0)
            return 3;
        else if (*rp == '\"') { // command in quotes
            rp++;
            tokens[1] = strtok_r(s_ptr, "\"", &rp);
            if (tokens[1] == NULL)
                return 0;
        }
        else if (*rp == '\'') { // command in single quotes
            rp++;
            tokens[1] = strtok_r(s_ptr, "\'", &rp);
            if (tokens[1] == NULL)
                return 0;
        }
        else {           // command not in any quotes
            tokens[1] = strtok_r(s_ptr, "\n\0", &rp);
            if (tokens[1] == NULL)
                return 0;
        }

        // copy tokens and return 1 for alias
        *alias = calloc(strlen(tokens[0]) + 1, sizeof(char));
        strcpy(*alias, tokens[0]);
        *command = calloc(strlen(tokens[1]) + 1, sizeof(char));
        strcpy(*command, tokens[1]);
        return 1;
    } // end 'alias' if //


    else if (strcmp(token, "unalias") == 0) {
        if (strcmp(rp, "") == 0)
            return 3;
        tokens[0] = strtok_r(s_ptr, " ", &rp); // get the alias name
        if (strcmp(tokens[0], "-a") == 0)
            return 4;
        *alias = calloc(strlen(tokens[0]) + 1, sizeof(char));
        strcpy(*alias, tokens[0]);
        return 2;
    }
    else
        return 0;
}

void printType_alias(void *passedin) {

    if (passedin == NULL)
        exit(-99);

    Alias *alias = (Alias *) passedin;
    if (alias->alias_name == NULL || alias->command == NULL)
        return;
    printf("alias %s='%s'\n", alias->alias_name, alias->command);
}

int comparator(const void *v1, const void *v2) {

    if (v1 == NULL || v2 == NULL)
        exit(-99);

    Alias *s_alias1 = (Alias *) v1;
    Alias *s_alias2 = (Alias *) v2;

    return strcmp(s_alias1->alias_name, s_alias2->alias_name);

}

void clean_alias(void *removedata) {
    if (removedata == NULL)
        exit(-99);

    Alias *s_alias = (Alias *) removedata;
    free(s_alias->command);
    s_alias->command = NULL;
    free(s_alias->alias_name);
    s_alias->alias_name = NULL;

    free(s_alias);
    s_alias = NULL;
    removedata = NULL;
}
