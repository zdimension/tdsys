#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static char* const ARG_DELIM = " \t\r\a\n";

int main(int argc, char* argv[])
{
    char* line = NULL;

    size_t len;
    size_t read;
    while ((read = getline(&line, &len, stdin)) != -1)
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

        if (!fork())
        {
            if (!strcmp(args[0], "cd"))
            {
                chdir(getenv("HOME"));
            }
            else
            {
                if (execvp(args[0], args) == -1)
                {
                    perror(args[0]);
                }
            }
            exit(0);
        }
        free(args);
    }

    free(line);
}