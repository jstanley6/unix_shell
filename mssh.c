#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./process/process.h"
#include "./tokenize/makeArgs.h"
#include "linkedlist/linkedList.h"
#include "linkedlist/listUtils.h"
#include "alias/alias.h"


void command(char *inputString, LinkedList *historyList, LinkedList *alias_list, int addtoHistory); // 1 to add to hist
void msshrc(LinkedList *alias_list, LinkedList *history, FILE *fin);

void setGlobals();

int main() {

    char userInput[MAX];
    FILE *fp = NULL;
    LinkedList *historyList = linkedList();
    LinkedList *aliasList = linkedList();

    setGlobals();

    // read the msshrc history and build the historyList structure //
    fp = fopen(".msshrc_history", "r");
    if (fp != NULL) {
        int historyCount = histCount(fp);
        if (historyCount > 0)
            buildHistory(historyList, historyCount, fp, readFile_History);
    }
    if (fp != NULL)
        fclose(fp);

    msshrc(historyList, aliasList, fp);

    if (!PATH_SET) {
        char *pathbuf;
        size_t n;
        n = confstr(_CS_PATH, NULL, (size_t) 0);
        if ((pathbuf = malloc(n)) == NULL) abort();
        confstr(_CS_PATH, pathbuf, n);
        strcpy(PATH, pathbuf);
        free(pathbuf);
        pathbuf = NULL;
        PATH_SET = 1;
    }

    while (historyList->size > HISTFILECOUNT)
        removeFirst(historyList, cleanType);

    // get the first command from stdin and saves to userInput//
    printf("command?: ");
    fgets(userInput, MAX, stdin);
    strip(userInput);

    // keep taking commands from stdin until "exit" //
    while (strcmp(userInput, "exit") != 0) {

        getAliasCommand(userInput, aliasList); //
        command(userInput, historyList, aliasList, 1);
        fflush(stdin);
        printf("command?: ");
        fgets(userInput, MAX, stdin);
        strip(userInput);
    }// end while


    clearList(aliasList, clean_alias);
    clearList(historyList, cleanType);
    free(historyList);
    free(aliasList);
    // write to historyList file on exit
    return 0;

}// end main


void msshrc(LinkedList *history, LinkedList *alias_list, FILE *fin) {

    // run msshrc shit //
    if (alias_list == NULL || history == NULL)
        exit(-99);

    char buffer[MAX];
    char tokbuffer[MAX];
    char *token;

    fin = fopen(".msshrc", "r");

    if (fin == NULL) {
        return;
    }

    while (fgets(buffer, MAX, fin) != NULL) {
        strcpy(tokbuffer, buffer);
        token = strtok(tokbuffer, ":=\n\0");

        if (strcmp(token, "HISTCOUNT") == 0)
            HISTCOUNT = atoi(strtok(NULL, "=\n\0"));

        else if (strcmp(token, "HISTFILECOUNT") == 0)
            HISTFILECOUNT = atoi(strtok(NULL, "=\n\0"));

        else if (strcmp(token, "PATH") == 0) {
            token = strtok(NULL, ":");

            if (strcmp(token, "$PATH") != 0) {
                strip(buffer);
                printf("%s: command not found\n", buffer);
            }

            token = strtok(NULL, "\n\0");
            strcpy(PATH, token);
            PATH_SET = 1;
        }
        else {
            strip(buffer);
            command(buffer, history, alias_list, 0);
        }
    }
    fclose(fin);
}

void cd(char *dir) {

    char *d = DIR;
    char temp[MAX];

    if (strcmp(dir, "..") == 0) {
        unsigned long i = strlen(DIR);
        while (i-- > 0)
            if (DIR[i] == '/') {
                if (i > 0)
                    DIR[i] = '\0';
                else
                    DIR[1] = '\0';
                break;
            }
    }
    else if (strcmp(dir, "~") == 0) {
        strcpy(DIR, getenv("HOME"));
        chdir(DIR);
    }
    else if (strcmp(dir, "/") == 0) {
        DIR[0] = '/';
        DIR[1] = '\0';
    }
    else {
        if (dir[0] != '/') {
            strcpy(temp, DIR);
            if (strcmp(DIR, "/") != 0)
                strcat(temp, "/");
            strcat(temp, dir);
        }
        else(strcpy(temp, dir));

        if (chdir(temp) == 0)
            strcpy(DIR, temp);
        else
            printf("cd: %s: No such file or directory\n", dir);

    }
    printf("%s\n", DIR);
}

int isNum(char *num) {
    int i = 0;
    while (i < strlen(num)) {
        if (num[i] != '0' &&
            num[i] != '1' &&
            num[i] != '2' &&
            num[i] != '3' &&
            num[i] != '4' &&
            num[i] != '5' &&
            num[i] != '6' &&
            num[i] != '7' &&
            num[i] != '8' &&
            num[i] != '9')
            return 0;
        i++;
    }
    return 1;
}

void command(char *inputString, LinkedList *historyList, LinkedList *alias_list, int addtoHistory) {

    static char lastcmd[MAX] = "";
    int numberOfArgs, pipeCount, preCount = 0, postCount = 0, ex = 1;

    char **argv = NULL, inputCopy[MAX], inputCopy3[MAX], inputCopy4[MAX],
            inputCopy5[MAX], inputCopy6[MAX], inputCopy7[MAX],
            **prePipe = NULL, **postPipe = NULL, *alias_name = NULL,
            *alias_cmd = NULL, *token;

    // make copies of the input string //
    strcpy(inputCopy, inputString);
    strcpy(inputCopy3, inputString);
    strcpy(inputCopy4, inputString);
    strcpy(inputCopy5, inputString);
    strcpy(inputCopy6, inputString);
    strcpy(inputCopy7, inputString);

    pipeCount = containsPipe(inputString); // check for pipes
    numberOfArgs = makeargs(inputString, &argv); // tokenize the args

    // update historyList info //
    if (numberOfArgs > 0) {
        if (addtoHistory) {
            if (historyList->size >= HISTFILECOUNT)
                removeFirst(historyList, cleanType);
            addLast(historyList, buildNode_Type(buildType_Args(numberOfArgs, argv)));
            writeHistory(historyList);
        }

        // input is at least single bang
        if (inputCopy[0] == '!') {

            S_history *ret_hist;
            char number[MAX];
            char new_s[MAX];
            int i;

            // input is double bang
            if (inputCopy[1] == '!') {
                if (inputCopy[2] == '\0') {
                    command(lastcmd, historyList, alias_list, 0);
                    ex=0;
                }
            } else {  // input single bang
                if (inputCopy[1] != ' ') {
                    if (inputCopy[1] == '\0')
                        ex=0;
                    i = 1;

                    while (i <= strlen(inputCopy) - 1)
                        number[i - 1] = inputCopy[i++];
                    number[i - 1] = '\0';
                    if (isNum(number)) {
                        ret_hist = (S_history *) getItem(historyList, atoi(number));
                        if (ret_hist != NULL) {
                            i = 0;
                            while (i < ret_hist->argc) {
                                strcat(new_s, ret_hist->argv[i++]);
                                strcat(new_s, " ");
                            }
                            if (new_s[0] != '!' && new_s[1] != '!')
                                command(new_s, historyList, alias_list, 0);
                            else
                                printf("%s: event not allowed from ! or !!\n", new_s);
                            ex=0;
                        }
                        else {
                            printf("!%d: event not found\n", atoi(number));
                            ex=0;
                        }
                    }
                }
            }
        }

        strcpy(lastcmd, inputCopy);
        // if CD command //
        if (strcmp(argv[0], "cd") == 0) {
            if (numberOfArgs == 2)
                cd(argv[1]);
            return;
        }

        // PATH SHITFUCKERY //
        if (strcmp(inputCopy6, "echo \"$PATH\"") == 0) {
            printf("%s\n", PATH);
            return;
        }

        if (numberOfArgs == 1) {
            token = strtok(inputCopy6, "=");
            if (strcmp("PATH", token) == 0) {
                token = strtok(NULL, "\n\0");
                if (token != NULL) {
                    char pathstr[MAX] = "$PATH";
                    strcpy(PATH, token);
                    setenv("PATH", PATH, 1);
                    PATH_SET = 1;
                    return;
                } else if (inputCopy7[4] == '=') {
                    PATH_SET = 1;
                    strcpy(PATH, "");
                    return;
                }

            }
        }

        // CHECK IF INPUT IS ALIAS //
        int alias_type = checkFor_alias(inputCopy3, &alias_name, &alias_cmd);

        // ALIAS - ADD TO ALIAS LIST ////////
        if (alias_type == 1) {
            addLast(alias_list, buildNode_Type(buildType_Alias(alias_name, alias_cmd)));
        }
            /////////////////////////////////////////////////////////////////////////

            // REMOVE ALIAS FROM LIST //////////
        else if (alias_type == 2) {
            removeItem(alias_list, buildNode_Type(buildType_Alias(alias_name, NULL)), clean_alias, comparator);
        }

            // PRINT ALIAS LIST ////////////////
        else if (alias_type == 3) {
            printList(alias_list, printType_alias);
        }
            ///////////////////////////////////////////////////////////////////


            //clear alias list/////////////
        else if (alias_type == 4) {
            emptyList(alias_list, clean_alias);
        }

            // IF NOT AN ALIAS ________________________________//
        else if (alias_type != -1) {
            if (strcmp(inputString, "history") == 0) { // see if historyList requested
                numberOfArgs = makeargs(inputString, &argv);
                printHistory(historyList, HISTCOUNT, printType);
                clean(numberOfArgs, argv);
            }
            else { // else pipe this shit
                if (pipeCount > 0 && ex) {
                    prePipe = parsePrePipe(inputCopy, &preCount);
                    postPipe = parsePostPipe(inputCopy, &postCount);
                    pipeItToFile(prePipe, postPipe, "out.txt");
                    clean(preCount, prePipe);
                    clean(postCount, postPipe);
                }// end if pipeCount

                else {
                    numberOfArgs = makeargs(inputCopy, &argv);
                    if (numberOfArgs != -1)
                        forkIt(argv);

                    clean(numberOfArgs, argv);
                    argv = NULL;
                }
            }
        }
    }
    else
        (clean(numberOfArgs, argv));
}

/**
 * @brief Sets the global variables used throughout the shell.
 */
void setGlobals() {

    HISTCOUNT = 100;
    HISTFILECOUNT = 1000;
    PATH_SET = 0;
    getcwd(DIR, MAX); // get current working directory for DIR var
    getcwd(STARTPATH, MAX); // get current working directory for STARTPATH var
    strcat(MSSHHISTLOC, STARTPATH);
    strcat(MSSHHISTLOC, "/.msshrc_history");
}

