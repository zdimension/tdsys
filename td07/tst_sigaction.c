#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int code)
{
    static volatile int sigintCount = 0;
    printf("Signal reçu: %s\n", code == SIGSEGV ? "SEGV" : "INT");
    if (code == SIGINT)
    {
        if (++sigintCount == 5)
        {
            puts("5 SIGINT reçus");
            exit(1);
        }
    }
    else
    {
        sigintCount = 0;
    }
}

int main()
{
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    sleep(5);

    puts("Après sleep");
}
