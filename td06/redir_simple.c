#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main()
{
    dup2(open("foo", O_WRONLY | O_CREAT, 0600), STDOUT_FILENO);
    execlp("ls", "ls", "-l", NULL);

    return 0;
}