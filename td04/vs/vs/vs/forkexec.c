#include <stddef.h>
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    TCHAR path[] = TEXT("C:\\Windows\\System32\\tree.com");

    if (!CreateProcess(NULL,
        path, 
        NULL, 
        NULL, 
        FALSE,
        0,    
        NULL, 
        NULL, 
        &si,  
        &pi)  
        )
    {
        printf("CreateProcess failed (%08lx).\n", GetLastError());
        return 1;
    }
    
    WaitForSingleObject(pi.hProcess, INFINITE);
	
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
