/*							-*- coding: utf-8 -*-
 *
 * redir.c	-- Redirection fichiers
 *
 * Erick Gallesio (2012-03-09)
 * Stéphane Lavirotte (2017/03/23)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void usage(char* progname)
{
    fprintf(stderr, "Usage: %s direction file cmd arg ...\n", progname);
    fprintf(stderr, "       direction = R or W\n");
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        usage(*argv);
    }

    const char* file = argv[2];
    int fd, str;

    switch (argv[1][0])
    {

        case 'R':
        case 'r':
            fd = open(file, O_RDONLY);
            str = 0;
            break;
        case 'W':
        case 'w':
            fd = open(file, O_WRONLY | O_CREAT, 0600);
            str = 1;
            break;
        default:
            usage(*argv);
    }

    if (fd == -1)
    {
        exit(1);
    }

    dup2(fd, str);

    execvp(argv[3], &argv[3]);
}
