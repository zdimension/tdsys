#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

static char* const ARG_DELIM = " \t\r\a\n";

int my_system(char* const* args)
{
    if (!fork())
    {
        exit(execvp(args[0], args));
    }
    int status;
    wait(&status);
    return status;
}

int main(int argc, char* argv[])
{
    char* line = NULL;

    size_t len;
    size_t read;
    while ((printf("$ "), read = getline(&line, &len, stdin)) != -1)
    {
        int nargs = 0;
        char** args = NULL;
        char* p = strtok(line, ARG_DELIM);

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