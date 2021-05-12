#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

#include "sort.h"
#include "load_library.h"
#include "utils.h"
#include "timer.h"

/* ------------------------------------------------------------------------------------
 * Valeurs par défaut des variables globales qui suivent 
 * ------------------------------------------------------------------------------------
 */

#define SIZE 10000
#define FALSE 0
#define TRUE 1

/* ------------------------------------------------------------------------------------
 * Variables globales 
 * ------------------------------------------------------------------------------------
*/

static int Size_Array = SIZE;
static int Verbose = FALSE;

/* ------------------------------------------------------------------------------------
 * Prototypes de fonctions définies plus tard 
 * ------------------------------------------------------------------------------------
*/
static void Scan_Args(int argc, char* argv[]);

static void Usage(const char* execname);

void exec_algorithm(char* libname)
{
    if (Verbose)
    {
        printf("Executing algorithm from %s\n", libname);
    }
    char buf[256];
    sprintf(buf, "./%s.so", libname);
    load_library(buf);

    int list[Size_Array];

    // Initialize a ramdom list of numbers;
    srand(0);
    for (int i = 0; i < Size_Array; i++)
    {
        list[i] = rand() % 1000;
    }

    if (Verbose)
    {
        printf("Array to sort:");
        print_list(list, Size_Array);
    }

    struct timespec vartime = timer_start();
    sort(list, Size_Array);
    long time_elapsed_nanos = timer_end(vartime);

    if (Verbose)
    {
        printf("Time taken for sorting (nanoseconds): ");
        printf("%ld\n", time_elapsed_nanos);

        printf("Sorted array:");
        print_list(list, Size_Array);
    }
}

int main(int argc, char* argv[])
{
    /* Décodage des arguments de la ligne de commande */

    Scan_Args(argc, argv);

    DIR* pwd = opendir(".");
    char* ch1;
    char* ch2;
    struct dirent* dir;
    if (pwd)
    {
        while ((dir = readdir(pwd)))
        {
            ch1 = strtok(dir->d_name, ".");
            ch2 = strtok(NULL, ".");
            if (ch2 && !strcmp(ch2, "so"))
            {
                exec_algorithm(ch1);
            }
        }
    }
}

/* Analyse des arguments 
 * ----------------------
 */
static void Scan_Args(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'h':
                    Usage(argv[0]);
                    exit(1);
                case 's':
                    Size_Array = atoi(argv[++i]);
                    break;
                case 'v':
                    Verbose = TRUE;
                    break;
            }
        }
    }
}

/* Information d'utilisation
 * -------------------------
 */
static void Usage(const char* execname)
{
    fprintf(stderr, "\nUsage: %s [options]\n", execname);
    fprintf(stderr, "\nOptions générales\n"
                    "-----------------\n"
                    "-h\tce message\n"
                    "-s\ttaille du tableau à trier\n"
                    "-v\taffiche plus d'informations\n");
}