#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

int main()
{
#ifdef _WIN32
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    CreateProcess("notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    WaitForSingleObject(pi.hProcess, INFINITE);

    printf("Process finished\n");

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

#else
    pid_t pid = fork();

    if(pid == 0)
    {
        execl("/usr/bin/gedit", "gedit", NULL); // Linux editor
    }
    else
    {
        wait(NULL);
        printf("Process finished\n");
    }
#endif

    return 0;
}