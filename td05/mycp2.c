#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "util.h"

void print_usage()
{
    puts("Usage:");
    puts("mycp2 [-vri] fic1 fic2");
    puts("mycp2 [-vri] fic1 fic2 ... dir");
    exit(EXIT_FAILURE);
}

bool verbose = false, recurse = false, interac = false;

void copy(const char* src, const char* dst)
{
    if (verbose)
        printf("'%s' -> '%s'\n", src, dst);

    if (interac && !access(dst, W_OK))
    {
        printf("mycp2: overwrite '%s'? ", dst);
        char answer;
        scanf("%c", &answer);
        fflush(stdin);
        if (answer != 'y')
            return;
    }

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

void copy_dir(const char* src, const char* dst)
{
    if (verbose)
        printf("'%s' -> '%s'\n", src, dst);

    struct stat st;
    stat(src, &st);
    mkdir(dst, st.st_mode);

    DIR* directory = opendir(src);

    struct dirent* entry;

    while ((entry = readdir(directory)))
    {
        char buf[1024];
        snprintf(buf, sizeof(buf), "%s/%s", src, entry->d_name);
        char buf2[1024];
        snprintf(buf2, sizeof(buf2), "%s/%s", dst, entry->d_name);
        if (entry->d_type == DT_DIR)
        {
            if (is_dot_dir(entry->d_name))
                continue;

            copy_dir(buf, buf2);
        }
        else
        {
            copy(buf, buf2);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        print_usage();
    }

    int opt;
    while ((opt = getopt(argc, argv, "vri")) != -1)
    {
        switch (opt)
        {
            case 'v':
                verbose = true;
                break;
            case 'r':
                recurse = true;
                break;
            case 'i':
                interac = true;
                break;
            default:
                print_usage();
        }
    }

    const char* last = argv[argc - 1];

    struct stat st;

    if (!recurse)
    {
        for (int i = optind; i < argc - 1; i++)
        {
            if (stat(argv[i], &st) || !S_ISREG(st.st_mode))
                exit(1);
        }
    }

    if (stat(last, &st) == -1 || !S_ISDIR(st.st_mode))
    {
        if (argc - optind != 2)
        {
            puts("2 parameters expected");
            print_usage();
        }

        copy(argv[optind], argv[optind + 1]);
    }
    else
    {
        for (int i = optind; i < argc - 1; i++)
        {
            stat(argv[i], &st);

            char buf[1024];
            snprintf(buf, 1024, "%s/%s", last, get_basename(argv[i]));

            if (S_ISDIR(st.st_mode))
            {
                copy_dir(argv[i], buf);
            }
            else
            {
                copy(argv[i], buf);
            }
        }
    }
}