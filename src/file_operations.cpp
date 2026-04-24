#include "../include/wrapper.h"
#include <iostream>

// File Open karne ka function
FileHandle OS_OpenFile(const char* filename) {
    #ifdef _WIN32
        // Windows system call: CreateFile
        return CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    #else
        // Linux system call: open
        return open(filename, O_RDWR | O_CREAT, 0644);
    #endif
}

// File mein likhne ka function
void OS_WriteFile(FileHandle hFile, const char* data) {
    #ifdef _WIN32
        DWORD written;
        WriteFile(hFile, data, strlen(data), &written, NULL); // Windows [cite: 38]
    #else
        write(hFile, data, strlen(data)); // Linux [cite: 38]
    #endif
}