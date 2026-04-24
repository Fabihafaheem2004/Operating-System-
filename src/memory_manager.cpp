/**
 * @file memory_manager.cpp
 * @brief Cross-platform memory mapping implementation
 */

#include "../include/memory_manager.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
#endif

// ============================================================================
// Memory Allocation
// ============================================================================

void* allocateMemory(size_t size) {
    void* ptr = nullptr;
    
#ifdef _WIN32
    ptr = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        ptr = nullptr;
    }
#endif
    
    if (ptr == nullptr) {
        std::fprintf(stderr, "Error: Memory allocation failed\n");
    }
    
    return ptr;
}

void freeMemory(void* ptr, size_t size) {
    if (ptr == nullptr) return;
    
#ifdef _WIN32
    VirtualFree(ptr, size, MEM_RELEASE);
#else
    munmap(ptr, size);
#endif
}

// ============================================================================
// Memory Mapping
// ============================================================================

bool mapFileToMemory(const char* filename, size_t fileSize, void** mappedData) {
    if (mappedData == nullptr || fileSize == 0) {
        std::fprintf(stderr, "Error: Invalid parameters for memory mapping\n");
        return false;
    }
    
    *mappedData = nullptr;
    
#ifdef _WIN32
    // Open file for reading
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hFile == INVALID_HANDLE_VALUE) {
        std::fprintf(stderr, "Error: Failed to open file for mapping: %s\n", filename);
        return false;
    }
    
    // Create file mapping object
    HANDLE hMapping = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READONLY,
        0,
        0,
        NULL
    );
    
    if (hMapping == NULL) {
        std::fprintf(stderr, "Error: Failed to create file mapping\n");
        CloseHandle(hFile);
        return false;
    }
    
    // Map view of file
    void* pData = MapViewOfFile(
        hMapping,
        FILE_MAP_READ,
        0,
        0,
        fileSize
    );
    
    if (pData == NULL) {
        std::fprintf(stderr, "Error: Failed to map view of file\n");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return false;
    }
    
    // Copy to allocated memory for consistent interface
    void* buffer = VirtualAlloc(NULL, fileSize + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (buffer == NULL) {
        std::fprintf(stderr, "Error: Failed to allocate memory\n");
        UnmapViewOfFile(pData);
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return false;
    }
    
    std::memcpy(buffer, pData, fileSize);
    static_cast<char*>(buffer)[fileSize] = '\0';
    
    *mappedData = buffer;
    
    // Cleanup
    UnmapViewOfFile(pData);
    CloseHandle(hMapping);
    CloseHandle(hFile);
    
#else
    // Open file for reading
    int fd = open(filename, O_RDONLY);
    
    if (fd < 0) {
        std::perror("Error: Failed to open file for mapping");
        return false;
    }
    
    // Allocate memory
    void* buffer = mmap(NULL, fileSize + 1, PROT_READ | PROT_WRITE, 
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (buffer == MAP_FAILED) {
        std::perror("Error: Failed to allocate memory");
        close(fd);
        return false;
    }
    
    // Read file into mapped memory
    ssize_t bytesRead = read(fd, buffer, fileSize);
    
    if (bytesRead < 0) {
        std::perror("Error: Failed to read file");
        munmap(buffer, fileSize + 1);
        close(fd);
        return false;
    }
    
    static_cast<char*>(buffer)[bytesRead] = '\0';
    *mappedData = buffer;
    
    close(fd);
#endif
    
    std::printf("Memory mapping successful\n");
    return true;
}

void unmapFileFromMemory(void* addr, size_t size) {
    if (addr == nullptr) return;
    
#ifdef _WIN32
    VirtualFree(addr, size, MEM_RELEASE);
#else
    munmap(addr, size);
#endif
}