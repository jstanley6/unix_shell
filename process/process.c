#include <sys/wait.h>
#include <errno.h>
#include "process.h"
#include "../history/history.h"


void forkIt(char **argv) {

    int status;
    pid_t pid = fork();
    char cwd[MAX];


    if (pid == -1) { // fork failure
        exit(-99);
    }
    else if (pid == 0) { // in the child process

        char cwd[MAX];
        getcwd(cwd, MAX);
        if (strcmp(cwd, DIR) != 0)
            chdir(DIR);

        if (execvp(*argv, argv) < 0) {
            printf("%s: command not found\n", *argv);
        }
        exit(errno);

    }
    else {
        waitpid(pid, &status, 0);
    }
}
