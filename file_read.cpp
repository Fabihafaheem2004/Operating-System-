#include <windows.h>   // Windows API for file handling
#include <stdio.h>     // printf

int main()
{
    // open the file in read mode
    HANDLE file = CreateFile(
        "data\\ecg.csv",   // file path (Windows style)
        GENERIC_READ,      // read permission
        0,                 // no sharing
        NULL,
        OPEN_EXISTING,     // open only if file exists
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // check if file opened successfully
    if(file == INVALID_HANDLE_VALUE)
    {
        printf("File open error\n");
        return 1;
    }

    char buffer[1024];   // buffer to store file data
    DWORD bytesRead;     // number of bytes read

    // read file content into buffer
    ReadFile(file, buffer, sizeof(buffer)-1, &bytesRead, NULL);

    // add null terminator for safe printing
    buffer[bytesRead] = '\0';

    // print file content
    printf("%s", buffer);

    // close the file handle
    CloseHandle(file);

    return 0;
}