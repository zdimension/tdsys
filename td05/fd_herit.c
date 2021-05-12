#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

const char* parent = "Parent après fork\n";
const char* enfant = "Enfant\n";

int list_fd()
{
    char buf[64];
    snprintf(buf, 64, "/proc/%i/fd/", getpid());

    int fd_count = 0;
    DIR* dir = opendir(buf);
    struct dirent* dp;
    while ((dp = readdir(dir)))
    {
        if (dp->d_type != DT_DIR)
            fd_count++;
    }
    closedir(dir);

    return fd_count;
}

int main()
{
    int fd;
    DIR* dirp;
    struct dirent* dentry;

    if ((dirp = opendir(".")) == NULL)
        perror("opendir(\".\")");

    fd = open("sortie", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    write(fd, "Parent avant fork\n", 18);

    switch (fork())
    {
        case -1:
            perror("fork");
            abort();
        case 0:
            if ((dentry = readdir(dirp)) == NULL)
                perror("readdir()");
            else
                printf("Répertoire \"%s\" lu\n", dentry->d_name);

            write(fd, enfant, strlen(enfant));

            sleep(2);

            write(fd, enfant, strlen(enfant));

            printf("%d descripteurs dans l'enfant\n", list_fd());

            exit(0);
        default:
            sleep(1);

            write(fd, parent, strlen(parent));

            printf("%d descripteurs dans le parent\n", list_fd());

            wait(NULL);
    }

    // n'arrivera jamais ici
}


