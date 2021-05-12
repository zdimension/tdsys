#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    char ch[128];
    ssize_t n;
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) exit(1);
    while ((n = read(fd, ch, sizeof(ch))) > 0)
        write(STDOUT_FILENO, ch, n);
    close(fd);
}