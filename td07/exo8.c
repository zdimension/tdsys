#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int code)
{
    printf("SIGINT reçu, PID = %d\n", getpid());
}

int main()
{
    printf("PID parent = %d\n", getpid());

    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGSEGV, &sa, NULL);

    if (!fork())
    {
        execlp("./exo8_enfant.exe", "exo8_enfant.exe", NULL);
    }

    while(1);
}