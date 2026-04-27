/**
 * @file wrapper.h
 * @brief Cross-platform OS wrapper header for Healthcare Signal Monitoring System
 * 
 * This header provides cross-platform abstractions for OS-level operations
 * including file I/O, memory management, process creation, and IPC.
 */

#ifndef WRAPPER_H
#define WRAPPER_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <random>

#ifdef _WIN32
    #define PLATFORM_NAME "Windows"
    #include <windows.h>
    #include <io.h>
    
    typedef HANDLE FileHandle;
    typedef HANDLE ProcessHandle;
    const FileHandle INVALID_FILE_HANDLE = INVALID_HANDLE_VALUE;
    const int INVALID_FD = -1;
#else
    #define PLATFORM_NAME "Linux"
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/shm.h>
    #include <signal.h>
    
    typedef int FileHandle;
    typedef pid_t ProcessHandle;
    const FileHandle INVALID_FILE_HANDLE = -1;
    const int INVALID_FD = -1;
#endif

struct ECGSample {
    double time;
    double amplitude;
};

struct FFTResult {
    std::vector<double> magnitudes;
    std::vector<double> frequencies;
    int chunkId;
};

struct PerformanceMetrics {
    double fileReadTime;
    double memoryMapTime;
    double fftTime;
    double processCreationTime;
    double ipcTime;
    double totalTime;
};

constexpr int ECG_DATA_POINTS = 1024;
constexpr double SAMPLE_RATE = 100.0;
constexpr double TIME_STEP = 1.0 / SAMPLE_RATE;
constexpr int NUM_PROCESSES = 4;
constexpr int FFT_CHUNK_SIZE = ECG_DATA_POINTS / NUM_PROCESSES;

FileHandle OS_OpenFile(const char* filename, bool readOnly = true);
void OS_CloseFile(FileHandle hFile);
bool OS_ReadFile(FileHandle hFile, char* buffer, size_t bufferSize);
bool OS_WriteFile(FileHandle hFile, const char* data, size_t length);
bool OS_FileExists(const char* filename);
size_t OS_GetFileSize(FileHandle hFile);

void* OS_AllocateMemory(size_t size);
void OS_FreeMemory(void* ptr, size_t size);
void* OS_MapFile(FileHandle hFile, size_t size);
void OS_UnmapFile(void* addr, size_t size);

ProcessHandle OS_CreateProcess(const char* programPath, const char* args[]);
void OS_WaitForProcess(ProcessHandle handle);
pid_t OS_Fork();
int OS_Wait(int* status);

#ifdef _WIN32
    typedef struct {
        HANDLE readPipe;
        HANDLE writePipe;
    } PipeHandles;
#else
    typedef struct {
        int readFd;
        int writeFd;
    } PipeHandles;
#endif

bool OS_CreatePipe(PipeHandles* pipes);
bool OS_WriteToPipe(PipeHandles* pipes, const void* data, size_t size);
bool OS_ReadFromPipe(PipeHandles* pipes, void* buffer, size_t size);
void OS_ClosePipe(PipeHandles* pipes);

#ifdef _WIN32
    typedef HANDLE SharedMemoryHandle;
#else
    typedef struct {
        int shmId;
        void* addr;
    } SharedMemoryHandle;
#endif

SharedMemoryHandle OS_CreateSharedMemory(size_t size, const char* name);
void* OS_AttachSharedMemory(SharedMemoryHandle handle, size_t size);
void OS_DetachSharedMemory(SharedMemoryHandle handle, void* addr);
void OS_DestroySharedMemory(SharedMemoryHandle handle);

void GenerateECGData(const char* filename, int numPoints);
std::vector<ECGSample> ParseECGData(const char* data, size_t dataSize, int& count);

void FFT(std::vector<double>& real, std::vector<double>& imag);

#endif