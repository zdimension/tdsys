#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"

void* fn_thread(void* arg)
{
    sleep(10);
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Pas assez d'arguments (requis: 1)\n");
        exit(1);
    }

    int N = (int) strtol(argv[1], NULL, 10);

    struct timespec vartime = timer_start();

    pthread_t th[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&th[i], NULL, fn_thread, NULL);
    }

    long time = timer_end(vartime);

    printf("Time taken for creating %d threads (nanoseconds): %ld\n", N, time);
    printf("Time taken for creating %d threads (milliseconds): %ld\n", N, time / NANO_TO_MILLI);

    for (int i = 0; i < N; i++)
    {
        void* code;
        pthread_join(th[i], &code);
    }
}