#include <stddef.h>
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

DWORD WINAPI fn_thread(LPVOID arg)
{
    int val = *(int*)arg;
	
    if (val < 0)
    {
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        printf("PID %lu, TID %lu\n", GetCurrentProcessId(), GetCurrentThreadId());
        Sleep(val * 1000);
    }

    return 0;
}

#define N (2)

int main(int argc, char* argv[])
{
    if (argc <= N)
    {
        fprintf(stderr, "Pas assez d'arguments (requis: %d)\n", N);
        exit(1);
    }

    printf("PID parent : %lu\n", GetCurrentProcessId());

    HANDLE th[N];
    int nums[N];
    for (int i = 0; i < N; i++)
    {
        nums[i] = (int)strtol(argv[i + 1], NULL, 10);
        th[i] = CreateThread(
            NULL,
            0,
            fn_thread,
            &nums[i],
            0,
            NULL);
    }
	
    for (int i = 0; i < N; i++)
    {
        printf("Sortie enfant #%d : %lu\n", i, WaitForSingleObject(th[i], INFINITE));
    }
}