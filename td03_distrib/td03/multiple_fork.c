#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pids[10];
    for (int i = 0; i < 10; i++)
    {
        if ((pids[i] = fork()) < 0)
        {
            fprintf(stderr, "fork");
            abort();
        }
        else if (!pids[i])
        {
            for (int j = 0; j < 10; j++)
            {
                printf("%d\n", i);
                sleep(1);
            }
            exit(0);
        }
    }
    while (wait(NULL) != -1);
}