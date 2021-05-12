#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int mypopen(const char* command)
{
    int fds[2];
    pipe(fds);

    int pid;
    if ((pid = fork()))
    {
        waitpid(pid, NULL, 0);
        close(fds[1]);
    }
    else
    {
        dup2(fds[1], STDOUT_FILENO);
        execl("/bin/sh", "sh", "-c", command, NULL);
    }

    return fds[0];
}

int main()
{
    int fd = mypopen("ls -l");

    char ch[128];
    ssize_t total = 0;
    ssize_t n;
    while ((n = read(fd, ch, sizeof(ch))) > 0)
    {
        total += n;
        write(STDOUT_FILENO, ch, n);
    }
    close(fd);

    printf("Caractères lus: %zd\n", total);
}