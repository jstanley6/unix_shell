//
// Created by josh on 1/26/16.
//

#ifndef LAB8_ALIAS_H
#define LAB8_ALIAS_H

#include "../linkedlist/linkedList.h"

struct alias {
    char *alias_name;
    char *command;
};
typedef struct alias Alias;

void getAliasCommand(char *s, LinkedList *theList);
void *buildType_Alias(char *name, char *command);

int checkFor_alias(char *s, char **alias, char **command);

int comparator(const void *v1, const void *v2);

void printType_alias(void *passedin);

void clean_alias(void *removedata);

Node *buildNode_Alias(void *passedin);

#endif //LAB8_ALIAS_H

