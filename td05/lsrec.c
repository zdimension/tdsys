#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "util.h"

void print_fileinfo(const char* path)
{
    struct stat fstat;
    stat(path, &fstat);

    int mode = fstat.st_mode;

    char c = '?';

    if (S_ISREG(mode))
        c = '-';
    else if (S_ISDIR(mode))
        c = 'd';
    else if (S_ISBLK(mode))
        c = 'b';
    else if (S_ISCHR(mode))
        c = 'c';
    else if (S_ISFIFO(mode))
        c = 'p';
    else if (S_ISLNK(mode))
        c = 'l';
    else if (S_ISSOCK(mode))
        c = 's';

    char buf[9];
    char* ptr = &buf[8];
    for (int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if (mode & 1) *(ptr--) = "xwr"[j];

    struct passwd* pw = getpwuid(fstat.st_uid);
    struct group* gr = getgrgid(fstat.st_gid);

    char date[32];
    struct tm t;
    localtime_r(&(fstat.st_mtim.tv_sec), &t);

    strftime(date, 31, "%b %d %H:%I", &t);

    printf("%c%.9s %lu %s %s %4ld %s %s\n", c, buf, fstat.st_nlink, pw->pw_name, gr->gr_name, fstat.st_size, date, get_basename(path));
}

void list(const char* path)
{
    if (is_dir(path))
    {
        DIR* directory = opendir(path);

        if (!directory)
            return;

        struct dirent* entry;

        printf("%s:\n", path);

        while ((entry = readdir(directory)))
        {
            char buf[1024];
            snprintf(buf, sizeof(buf), "%s/%s", path, entry->d_name);
            print_fileinfo(buf);
        }

        rewinddir(directory);

        while ((entry = readdir(directory)))
        {
            if (entry->d_type == DT_DIR)
            {
                char buf[1024];
                if (is_dot_dir(entry->d_name))
                    continue;
                snprintf(buf, sizeof(buf), "%s/%s", path, entry->d_name);
                putchar('\n');
                list(buf);
            }
        }

        closedir(directory);
    }
    else
    {
        print_fileinfo(path);
    }
}

int main(int argc, char* argv[])
{
    while(--argc)
        list(*++argv);
}