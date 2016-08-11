#include "makeArgs.h"

void clean(int argc, char **argv) {

    int i = 0;
    for (i = 0; i < argc; i++) {
        free(*(argv + i));
        *(argv + i) = NULL;
    }
    free(argv);
}

void printargs(int argc, char **argv) {
    int x;
    for (x = 0; x < argc; x++)
        printf("%s\n", argv[x]);
}// end printargs

int makeargs(char *s, char ***argv) {

    if (s == NULL || argv == NULL)
        return 0;

    unsigned int argc, i;
    char *token = NULL, *rp = NULL, *s_ptr = NULL, *tokens[MAX];
    char delims[5] = {' ', '\t', '\n', '\0'};

    for (argc = 0, s_ptr = s; ; argc++, s_ptr = NULL) {
        token = strtok_r(s_ptr, delims, &rp);

        if (token == NULL)
            break;
        tokens[argc] = token;
    }

    *argv = calloc(argc + 1, sizeof(char *));

    for (i = 0; i < argc; i++) {
        *((*argv) + i) = calloc(strlen(tokens[i]) + 1, sizeof(char));
        strcpy(*((*argv) + i), tokens[i]);
    }
    *((*argv) + i) = '\0';

    if (argc == 0)
        return -1;
    return argc;
}// end makeArgs