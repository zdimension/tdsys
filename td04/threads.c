#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <syscall.h>

void* fn_thread(const int* arg)
{
    if (*arg < 0)
    {
        pthread_exit((void*) 1);
    }

    for (int i = 0; i < 5; i++)
    {
        printf("PID %d, TID %lu, Linux %ld\n", getpid(), pthread_self(), syscall(SYS_gettid));
        sleep(*arg);
    }

    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    const int N = 2;

    if (argc <= N)
    {
        fprintf(stderr, "Pas assez d'arguments (requis: %d)\n", N);
        exit(1);
    }

    printf("PID parent : %d\n", getpid());

    pthread_t th[N];
    int nums[N];
    for (int i = 0; i < N; i++)
    {
        nums[i] = (int) strtol(argv[i + 1], NULL, 10);
        pthread_create(&th[i], NULL, (void* (*)(void*)) fn_thread, &nums[i]);
    }

    for (int i = 0; i < N; i++)
    {
        void* code;
        pthread_join(th[i], &code);
        printf("Sortie enfant #%d : %d\n", i, (int) code);
    }
}