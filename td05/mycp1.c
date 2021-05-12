#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"

void print_usage()
{
    puts("Usage:");
    puts("mycp1 fic1 fic2");
    puts("mycp1 fic1 fic2 ... dir");
    exit(1);
}

void copy(const char* src, const char* dst)
{
    int f1 = open(src, O_RDONLY);
    int f2 = open(dst, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

    char buffer[BUFSIZ];
    ssize_t n;

    while ((n = read(f1, buffer, BUFSIZ)) > 0)
    {
        if (write(f2, buffer, n) != n)
        {
            puts("write failed\n");
            exit(1);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        print_usage();
    }

    const char* last = argv[argc - 1];

    struct stat st;

    for (int i = 1; i < argc - 1; i++)
    {
        if (stat(argv[i], &st) || !S_ISREG(st.st_mode))
            exit(1);
    }

    if (stat(last, &st) == -1 || !S_ISDIR(st.st_mode))
    {
        if (argc != 3)
            print_usage();

        copy(argv[1], argv[2]);
    }
    else
    {
        for (int i = 1; i < argc - 1; i++)
        {
            char buf[1024];
            snprintf(buf, 1024, "%s/%s", last, get_basename(argv[i]));
            copy(argv[i], buf);
        }
    }
}