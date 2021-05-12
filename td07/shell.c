#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <setjmp.h>
#include <stdbool.h>

static char* const ARG_DELIM = " \t\r\a\n";

volatile pid_t child;

static sigjmp_buf env;
#define INTRECEIVED 42

int my_system(char* const* args)
{
    if (!(child = fork()))
    {
        exit(execvp(args[0], args));
    }
    int status;
    wait(&status);
    child = -1;
    return status;
}

void handler(int code)
{
    if (child != -1)
    {
        kill(SIGINT, child);
        putchar('\n');
    }
    else
    {
        siglongjmp(env, INTRECEIVED);
    }
}

int main()
{
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigaction(SIGINT, &sa, NULL);

    char* line = NULL;

    size_t len;
    while (1)
    {
        if (sigsetjmp(env, true) == INTRECEIVED)
        {
            putchar('\n');
        }

        printf("$ ");

        if (getline(&line, &len, stdin) == -1) break;

        int nargs = 0;
        char** args = NULL;
        char* p = strtok(line, ARG_DELIM);

        if (!p)
            continue;

        for (;;)
        {
            if (!(args = realloc(args, sizeof(char*) * ++nargs)))
                abort();
            args[nargs - 1] = p;
            if (!p)
                break;
            p = strtok(NULL, ARG_DELIM);
        }

        if (!strcmp(args[0], "cd"))
        {
            chdir(getenv("HOME"));
        }
        else if (!strcmp(args[0], "exit"))
        {
            exit(0);
        }
        else
        {
            if (my_system(args) == -1)
            {
                perror(args[0]);
            }
        }

        free(args);
    }

    free(line);
}