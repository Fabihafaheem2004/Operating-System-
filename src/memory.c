#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#endif

int main()
{
#ifdef _WIN32
    char* mem = (char*) VirtualAlloc(NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    char* mem = (char*) malloc(1024);
#endif

    sprintf(mem, "Memory allocated successfully");

    printf("%s\n", mem);

#ifdef _WIN32
    VirtualFree(mem, 0, MEM_RELEASE);
#else
    free(mem);
#endif

    return 0;
}