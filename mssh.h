//
// Created by kelvinzero on 8/15/16.
//

#ifndef LAB8_MSSH_H
#define LAB8_MSSH_H

#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./process/process.h"
#include "./tokenize/makeArgs.h"
#include "./linkedlist/linkedList.h"
#include "./linkedlist/listUtils.h"
#include "./alias/alias.h"
#include "./systemfile/systemfileUtil.h"
#include "./command/command.h"

void command(char *inputString, LinkedList *historyList, LinkedList *alias_list, int addtoHistory);

#endif //LAB8_MSSH_H
