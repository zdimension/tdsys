#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    printf("%ld %s\n", (long)getpid(), argv[0]);
    return 0;
}