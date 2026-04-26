#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main()
{
#ifdef _WIN32
    HANDLE readPipe, writePipe;
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };

    CreatePipe(&readPipe, &writePipe, &sa, 0);

    char msg[] = "Hello from pipe";
    WriteFile(writePipe, msg, sizeof(msg), NULL, NULL);

    char buffer[100];
    DWORD bytesRead;

    ReadFile(readPipe, buffer, sizeof(buffer), &bytesRead, NULL);
    printf("%s\n", buffer);

#else
    int fd[2];
    pipe(fd);

    char msg[] = "Hello from pipe";
    write(fd[1], msg, sizeof(msg));

    char buffer[100];
    read(fd[0], buffer, sizeof(buffer));

    printf("%s\n", buffer);
#endif

    return 0;
}