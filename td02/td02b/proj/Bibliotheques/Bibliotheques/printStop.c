#include <stdio.h>

_declspec(dllexport) void PrintStop(char* msg)
{
	printf(msg);
	getchar();
}
