#include <windows.h>
#include <stdio.h>

int main()
{
    STARTUPINFO si = { sizeof(si) };   // startup info for process
    PROCESS_INFORMATION pi;            // stores process details

    // create notepad process
    CreateProcess(
        "notepad.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    // wait until notepad closes
    WaitForSingleObject(pi.hProcess, INFINITE);

    printf("Process finished\n");      // output message

    // close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}