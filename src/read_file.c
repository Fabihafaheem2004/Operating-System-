#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

int main()
{
#ifdef _WIN32
    HANDLE file = CreateFile("data/ecg.csv", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(file == INVALID_HANDLE_VALUE)
    {
        printf("File open error\n");
        return 1;
    }

    char buffer[1024];
    DWORD bytesRead;

    ReadFile(file, buffer, sizeof(buffer)-1, &bytesRead, NULL);
    buffer[bytesRead] = '\0';

    printf("%s", buffer);
    CloseHandle(file);

#else
    int file = open("data/ecg.csv", O_RDONLY);

    if(file < 0)
    {
        printf("File open error\n");
        return 1;
    }

    char buffer[1024];
    int bytesRead = read(file, buffer, sizeof(buffer)-1);

    buffer[bytesRead] = '\0';
    printf("%s", buffer);

    close(file);
#endif

    return 0;
}