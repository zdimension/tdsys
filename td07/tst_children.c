#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

volatile pid_t aine, cadet;

void handler1(int code)
{
    kill(aine, SIGUSR2);
    puts("Fin cadet");
    exit(0);
}

void handler2(int code)
{
    puts("Fin aîné");
    exit(0);
}

int main()
{
    struct sigaction sa = {0};

    puts("Lancement");
    if (!(aine = fork()))
    {
        puts("Début aîné");
        sa.sa_handler = handler2;
        sigaction(SIGUSR2, &sa, NULL);
        for(;;);
    }
    else
    {
        if (!(cadet = fork()))
        {
            puts("Début cadet");
            sa.sa_handler = handler1;
            sigaction(SIGUSR1, &sa, NULL);
            for(;;);
        }
        else
        {
            puts("Attente 2s");
            sleep(2);
            kill(cadet, SIGUSR1);
            puts("Attente enfants");
            while (wait(NULL) > 0);
        }
    }
}