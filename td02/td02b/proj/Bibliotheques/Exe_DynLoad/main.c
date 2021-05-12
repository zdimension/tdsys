#include <Windows.h>
#include <stdio.h>

typedef void(*print_stop)(char*);

int main()
{
	void* hinst = LoadLibrary(L"Bibliotheques.dll");
	
	if (!hinst)
	{
		fprintf(stderr, "Erreur de chargement DLL\n");
		return 1;
	}

	const print_stop func = (print_stop) GetProcAddress(hinst, "PrintStop");
	func("abc");
	FreeLibrary(hinst);

	return 0;
}
