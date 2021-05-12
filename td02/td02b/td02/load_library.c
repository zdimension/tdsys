#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

static void* libptr = NULL;
static void (* funcptr)(int[], int);

void load_library(char* library_name)
{
	if (libptr)
	{
		dlclose(libptr);
	}

	if (!(libptr = dlopen(library_name, RTLD_LAZY)) || !(funcptr = dlsym(libptr, "sort")))
	{
		fprintf(stderr, "Unable to load sort library: %s\n", dlerror());
		exit(1);
	}
}

void sort(int list[], int size)
{
	funcptr(list, size);
}