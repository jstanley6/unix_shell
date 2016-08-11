#include <errno.h>
#include <fcntl.h>
#include "pipes.h"
#include "../tokenize/makeArgs.h"
#include "../history/history.h"

int containsPipe(char *s) {
    if (s == NULL)
        exit(-99);

    unsigned long s_length;
    unsigned int n_pipes;
    n_pipes = 0;
    s_length = strlen(s); // get s length

    while (s_length-- > 0) // super lambda pew pew
        if (s[s_length] == '|')
            n_pipes++;
    return n_pipes;
}

char **parsePrePipe(char *s, int *preCount) {

    if (s == NULL || preCount == NULL)
        exit(-99);

    unsigned long numof_chars;
    unsigned long s_length;
    unsigned long i;
    char *prepipe_string;
    char **prePipe;

    s_length = strlen(s);

    // count characters up to '|' //
    for (numof_chars = 0; numof_chars < s_length && s[numof_chars] != '|'; numof_chars++);

    prepipe_string = calloc(numof_chars + 1, sizeof(char));

    // copy the prepipe chars to the prepipe string//
    for (i = 0; i < numof_chars; prepipe_string[i] = s[i], i++);

    *preCount = makeargs(prepipe_string, &prePipe);
    free(prepipe_string);

    return prePipe;
}

char **parsePostPipe(char *s, int *postCount) {

    unsigned long i;
    unsigned long s_length;
    char *postPipe_string;
    char **postPipe;

    s_length = strlen(s);

    for (i = 0; i < s_length && s[i] != '|'; i++);
    postPipe_string = s + i + 1;
    *postCount = makeargs(postPipe_string, &postPipe);

    return postPipe;
}

void pipeIt(char **prePipe, char **postPipe) {

    if (prePipe == NULL || postPipe == NULL)
        exit(-99);

    pid_t pid;
    int status;
    pid = fork();


    if (pid == 0) { // in child

        pid_t pid2;
        int fd[2];

        if (pipe(fd) < 0) {
            printf("Pipe Failure\n");
            exit(-1);
        }

        pid2 = fork();
        if (pid2 == 0) { // in grandchild

            char cwd[MAX];
            getcwd(cwd, MAX);
            if (strcmp(cwd, DIR) != 0)
                chdir(DIR);

            int save_stdout = dup(1); // to restore for displaying error
            close(fd[0]);
            dup2(fd[1], 1);
            close(fd[1]);
            if (execvp(*prePipe, prePipe) < 0) {
                dup2(save_stdout, 1); // restore the stdout
                printf("%s: command not found\n", *prePipe);

            }
            exit(errno);
        }
        else { // in child
            waitpid(pid2, &status, 0);
            close(fd[1]);
            close(0);
            dup(fd[0]);
            close(fd[0]);

            if (execvp(*postPipe, postPipe) < 0)
                printf("%s: command not found\n", *postPipe);
            exit(errno);
        }
    }

    else { // in first parent
        waitpid(pid, &status, 0);

    }
}

void pipeItToFile(char **prePipe, char **postPipe, char *fname) {

    // int save_stdout = dup(1);
    int nfp = 1;
    if(fname != NULL){
        nfp = open(fname, O_WRONLY|O_CREAT);
    }

    if (prePipe == NULL || postPipe == NULL)
        exit(-99);

    pid_t pid;
    int status;
    pid = fork();
    int fd[2];

    if (pid == 0) { // in child

        pid_t pid2;

        if (pipe(fd) < 0) {
            printf("Pipe Failure\n");
            exit(-1);
        }

        pid2 = fork();
        if (pid2 == 0) { // in grandchild

            chdir(DIR);
            int save_stdout = dup(1); // to restore for displaying error
            close(fd[0]);
            dup2(fd[1], 1);
            close(fd[1]);

            if (execvp(*prePipe, prePipe) < 0) {
                dup2(save_stdout, 1); // restore the stdout
                printf("%s: command not found\n", *prePipe);
            }
            exit(errno);
        }
        else { // in child

            waitpid(pid2, &status, 0);
            close(fd[1]);
            close(0);
            dup(fd[0]);
            close(fd[0]);

            if (execvp(*postPipe, postPipe) < 0) {
                printf("%s: command not found\n", *postPipe);
            }

            dup2(2, 1); // restore the stdout
            close(nfp);
            exit(errno);
        }
    }

    else // in first parent
        waitpid(pid, &status, 0);
}
