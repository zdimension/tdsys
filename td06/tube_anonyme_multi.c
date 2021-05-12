#include <unistd.h>

// buf doit être de longueur PAIRE
void ecrireAlphabet(int fd, const char* buf, int delay)
{
    while (*buf)
    {
        write(fd, buf, 2);
        buf += 2;
        sleep(delay);
    }
}

int main()
{
    int fds[2];
    pipe(fds);

    if (!fork())
    {
        if (fork())
        {
            ecrireAlphabet(fds[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1);
        }
        else
        {
            ecrireAlphabet(fds[1], "abcdefghijklmnopqrstuvwxyz", 2);
        }
    }
    else
    {
        char ch[3];
        ssize_t n;
        while ((n = read(fds[0], ch, sizeof(ch))) > 0)
            write(STDOUT_FILENO, ch, n);
    }
}