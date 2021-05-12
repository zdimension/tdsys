#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#define	FILENAME "nohup.out"

int main(int argc, char* argv[])
{
    if (isatty(STDIN_FILENO))
    {
        int fd = open("/dev/null", O_WRONLY);
        dup2(fd, STDIN_FILENO);
        if (fd >= STDERR_FILENO)
            close(fd);
    }

    if (isatty(STDOUT_FILENO))
    {
        int fd = open(FILENAME, O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
        dup2(fd, STDOUT_FILENO);
        if (fd >= STDERR_FILENO)
            close(fd);
    }

    if (isatty(STDERR_FILENO))
    {
        dup2(STDOUT_FILENO, STDERR_FILENO);
    }

    struct sigaction sa = {0};
    sa.sa_handler = SIG_IGN;
    sigaction(SIGHUP, &sa, NULL);

    execvp(argv[1], &argv[1]);
}