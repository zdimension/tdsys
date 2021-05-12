#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("PID enfant = %d\n", getpid());

    while (1);
}