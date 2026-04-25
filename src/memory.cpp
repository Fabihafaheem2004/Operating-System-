#include <windows.h>   // Windows API for memory management
#include <stdio.h>     // printf

int main()
{
    // allocate virtual memory block of 1024 bytes
    char* mem = (char*) VirtualAlloc(
        NULL,
        1024,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    // store string in allocated memory
    sprintf(mem, "Memory allocated using VirtualAlloc");

    // print the stored message
    printf("%s\n", mem);

    // free allocated memory
    VirtualFree(mem, 0, MEM_RELEASE);

    return 0;
}