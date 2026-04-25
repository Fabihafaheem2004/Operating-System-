#include <windows.h>
#include <stdio.h>

int main()
{
    HANDLE readPipe, writePipe;  // handles for pipe ends

    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE }; // allow handle inheritance

    // create a pipe
    CreatePipe(&readPipe, &writePipe, &sa, 0);

    char msg[] = "Hello from pipe";

    // write data into pipe
    WriteFile(writePipe, msg, sizeof(msg), NULL, NULL);

    char buffer[100];
    DWORD bytesRead;

    // read data from pipe
    ReadFile(readPipe, buffer, sizeof(buffer), &bytesRead, NULL);

    // display received message
    printf("%s\n", buffer);

    return 0;
}