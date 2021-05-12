#include <unistd.h>

void lecture(int fd)
{
    char ch;
    while(read(fd, &ch, 1) == 1)
        write(STDOUT_FILENO, &ch, 1);
}

int main()
{
    int fds[2];
    pipe(fds);
    
    write(fds[1], "0123456789", 10);
    close(fds[1]);
    lecture(fds[0]);
}