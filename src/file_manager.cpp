/**
 * @file file_manager.cpp
 * @brief Cross-platform file management implementation for ECG data handling
 */

#include "../include/file_manager.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

// ============================================================================
// CSV Generation
// ============================================================================

bool generateECGData(const char* filename, int numPoints) {
#ifdef _WIN32
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hFile == INVALID_HANDLE_VALUE) {
        std::fprintf(stderr, "Error: Failed to create file %s\n", filename);
        return false;
    }
    
    // Write CSV header
    const char* header = "time,amplitude\n";
    DWORD written;
    WriteFile(hFile, header, strlen(header), &written, NULL);
    
    // Generate and write data points
    char buffer[64];
    for (int i = 0; i < numPoints; ++i) {
        double time = i * 0.01;
        double amplitude = std::sin(time) + (std::rand() % 100) / 1000.0;
        int len = std::snprintf(buffer, sizeof(buffer), "%.4f,%.6f\n", time, amplitude);
        WriteFile(hFile, buffer, len, &written, NULL);
    }
    
    CloseHandle(hFile);
#else
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (fd < 0) {
        std::perror("Error: Failed to create file");
        return false;
    }
    
    // Write CSV header
    const char* header = "time,amplitude\n";
    write(fd, header, strlen(header));
    
    // Generate and write data points
    char buffer[64];
    for (int i = 0; i < numPoints; ++i) {
        double time = i * 0.01;
        double amplitude = std::sin(time) + (std::rand() % 100) / 1000.0;
        int len = std::snprintf(buffer, sizeof(buffer), "%.4f,%.6f\n", time, amplitude);
        write(fd, buffer, len);
    }
    
    close(fd);
#endif
    
    std::printf("ECG file generated\n");
    return true;
}

// ============================================================================
// File Reading using System Calls
// ============================================================================

long getFileSize(const char* filename) {
#ifdef _WIN32
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
        std::fprintf(stderr, "Error: Failed to open file %s\n", filename);
        return -1;
    }
    
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        CloseHandle(hFile);
        return -1;
    }
    
    CloseHandle(hFile);
    return fileSize.QuadPart;
#else
    struct stat st;
    if (stat(filename, &st) < 0) {
        std::perror("Error: Failed to get file size");
        return -1;
    }
    return st.st_size;
#endif
}

int readFile(const char* filename, char* buffer, size_t bufferSize) {
#ifdef _WIN32
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
        std::fprintf(stderr, "Error: Failed to open file %s\n", filename);
        return -1;
    }
    
    DWORD bytesRead;
    if (!ReadFile(hFile, buffer, bufferSize, &bytesRead, NULL)) {
        std::fprintf(stderr, "Error: Failed to read file\n");
        CloseHandle(hFile);
        return -1;
    }
    
    buffer[bytesRead] = '\0';
    CloseHandle(hFile);
    return static_cast<int>(bytesRead);
#else
    int fd = open(filename, O_RDONLY);
    
    if (fd < 0) {
        std::perror("Error: Failed to open file");
        return -1;
    }
    
    ssize_t bytesRead = read(fd, buffer, bufferSize - 1);
    
    if (bytesRead < 0) {
        std::perror("Error: Failed to read file");
        close(fd);
        return -1;
    }
    
    buffer[bytesRead] = '\0';
    close(fd);
    return static_cast<int>(bytesRead);
#endif
}

// ============================================================================
// CSV Parsing
// ============================================================================

int parseCSVData(const char* csvData, std::vector<double>& amplitudes) {
    amplitudes.clear();
    
    const char* ptr = csvData;
    
    // Skip header line
    while (*ptr && *ptr != '\n') ++ptr;
    if (*ptr == '\n') ++ptr;
    
    // Parse each line
    while (*ptr && *ptr != '\0') {
        // Skip time value
        while (*ptr && *ptr != ',') ++ptr;
        if (*ptr == ',') ++ptr;
        
        // Read amplitude value
        char* endPtr = nullptr;
        double amplitude = std::strtod(ptr, &endPtr);
        
        if (endPtr != ptr) {
            amplitudes.push_back(amplitude);
        }
        
        // Move to next line
        ptr = endPtr;
        while (*ptr && *ptr != '\n') ++ptr;
        if (*ptr == '\n') ++ptr;
    }
    
    return static_cast<int>(amplitudes.size());
}

void closeFile(int fd) {
#ifdef _WIN32
    // fd is not used on Windows as we close handle immediately after read
    (void)fd;
#else
    if (fd >= 0) {
        close(fd);
    }
#endif
}