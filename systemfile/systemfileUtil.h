//
// Created by kelvinzero on 8/15/16.
//

#ifndef LAB8_MSSHHANDLER_H
#define LAB8_MSSHHANDLER_H

#include <stdio.h>
#include "../mssh.h"
#include "../linkedlist/linkedList.h"

int countHistory();

void run_msshrc(LinkedList *history, LinkedList *alias_list, FILE *fin);

#endif //LAB8_MSSHHANDLER_H
