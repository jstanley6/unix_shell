#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./process/process.h"
#include "./tokenize/makeArgs.h"
#include "linkedlist/linkedList.h"
#include "linkedlist/listUtils.h"
#include "alias/alias.h"


void command(char *s, LinkedList *history_list, LinkedList *alias_list, int addtoHistory); // 1 to add to hist
void msshrc(LinkedList *alias_list, LinkedList *history, FILE *fin);

int main() {

    char s[MAX];
    FILE *fp = NULL;
    LinkedList *history = linkedList();
    LinkedList *alias_list = linkedList();

    // Default globals
    HISTCOUNT = 100;
    HISTFILECOUNT = 1000;
    PATH_SET = 0;
    getcwd(DIR, MAX);
    getcwd(STARTPATH, MAX);
    strcat(MSSHHISTLOC, STARTPATH);
    strcat(MSSHHISTLOC, "/.msshrc_history");

    fp = fopen(".msshrc_history", "r");
    if (fp != NULL) {
        int count = histcount(fp);
        if (count > 0)
            buildHistory(history, count, fp, readFile_History);
    }
    if (fp != NULL)
        fclose(fp);

    msshrc(history, alias_list, fp);

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

    while (history->size > HISTFILECOUNT)
        removeFirst(history, cleanType);

    // get the args stdin //
    printf("command?: ");
    fgets(s, MAX, stdin);
    strip(s);


    while (strcmp(s, "exit") != 0) {
        getAliasCommand(s, alias_list);
        command(s, history, alias_list, 1);
        fflush(stdin);
        printf("command?: ");
        fgets(s, MAX, stdin);
        strip(s);
    }// end while


    clearList(alias_list, clean_alias);
    clearList(history, cleanType);
    free(history);
    free(alias_list);
    // write to history file on exit
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

void command(char *s, LinkedList *history_list, LinkedList *alias_list, int addtoHistory) {

    static char lastcmd[MAX] = "";
    int argc, pipeCount, preCount = 0, postCount = 0, ex=1;
    char **argv = NULL, s2[MAX], s3[MAX], s4[MAX], s5[MAX], s6[MAX], s7[MAX],
            **prePipe = NULL, **postPipe = NULL, *alias_name = NULL,
            *alias_cmd = NULL, *token;

    // duplicate the input string ////////
    strcpy(s2, s); // duplicate the captured string
    strcpy(s3, s);
    strcpy(s4, s);
    strcpy(s5, s);
    strcpy(s6, s);
    strcpy(s7, s);

    pipeCount = containsPipe(s); // check for pipes
    argc = makeargs(s, &argv); // tokenize the args

    // update history_list info ///////////////
    if (argc > 0) {
        if (addtoHistory) {
            if (history_list->size >= HISTFILECOUNT)
                removeFirst(history_list, cleanType);
            addLast(history_list, buildNode_Type(buildType_Args(argc, argv)));
            writeHistory(history_list);
        }
        ///////////////////////////////////////////////////////////////////////

        if (s2[0] == '!') {

            history *ret_hist;
            char number[MAX];
            char new_s[MAX];
            int i;

            if (s2[1] == '!') { // doble bang
                if (s2[2] == '\0') {
                    command(lastcmd, history_list, alias_list, 0);
                    ex=0;
                }
            }
            else {  // single bang
                if (s2[1] != ' ') {
                    if (s2[1] == '\0')
                        ex=0;
                    i = 1;

                    while (i <= strlen(s2) - 1)
                        number[i - 1] = s2[i++];
                    number[i - 1] = '\0';
                    if (isNum(number)) {
                        ret_hist = (history *) getItem(history_list, atoi(number));
                        if (ret_hist != NULL) {
                            i = 0;
                            while (i < ret_hist->argc) {
                                strcat(new_s, ret_hist->argv[i++]);
                                strcat(new_s, " ");
                            }
                            if (new_s[0] != '!' && new_s[1] != '!')
                                command(new_s, history_list, alias_list, 0);
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

        strcpy(lastcmd, s2);
        // if CD command //
        if (strcmp(argv[0], "cd") == 0) {
            if (argc == 2)
                cd(argv[1]);
            return;
        }

        // PATH SHITFUCKERY //
        if (strcmp(s6, "echo \"$PATH\"") == 0) {
            printf("%s\n", PATH);
            return;
        }

        if (argc == 1) {
            token = strtok(s6, "=");
            if (strcmp("PATH", token) == 0) {
                token = strtok(NULL, "\n\0");
                if (token != NULL) {
                    char pathstr[MAX] = "$PATH";
                    strcpy(PATH, token);
                    setenv("PATH", PATH, 1);
                    PATH_SET = 1;
                    return;
                }
                else if (s7[4] == '=') {
                    PATH_SET = 1;
                    strcpy(PATH, "");
                    return;
                }

            }
        }

        // CHECK IF INPUT IS ALIAS //
        int alias_type = checkFor_alias(s3, &alias_name, &alias_cmd);

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
            if (strcmp(s, "history") == 0) { // see if history_list requested
                argc = makeargs(s, &argv);
                printHistory(history_list, HISTCOUNT, printType);
                clean(argc, argv);
            }
            else { // else pipe this shit
                if (pipeCount > 0 && ex) {
                    prePipe = parsePrePipe(s2, &preCount);
                    postPipe = parsePostPipe(s2, &postCount);
                    pipeItToFile(prePipe, postPipe, "out.txt");
                    clean(preCount, prePipe);
                    clean(postCount, postPipe);
                }// end if pipeCount

                else {
                    argc = makeargs(s2, &argv);
                    if (argc != -1)
                        forkIt(argv);

                    clean(argc, argv);
                    argv = NULL;
                }
            }
        }
    }
    else
        (clean(argc, argv));


}