#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

static void* libptr = NULL;
static void (*funcptr)(int[], int);

int load_library(WCHAR* library_name)
{
	if (libptr)
	{
		FreeLibrary(libptr);
	}

	if (!(libptr = LoadLibrary(library_name)) || !(funcptr = (void*)GetProcAddress(libptr, "sort")))
	{
		fprintf(stderr, "Unable to load sort library '%ls': 0x%08x\n", library_name, GetLastError());
		return FALSE;
	}

	return TRUE;
}

void sort(int list[], int size)
{
	funcptr(list, size);
}