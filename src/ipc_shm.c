#include <windows.h>
#include <stdio.h>

#define SIZE 1000

int main() {

    // 1. Create file mapping object (shared memory)
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use system paging file
        NULL,
        PAGE_READWRITE,
        0,
        SIZE * sizeof(float),
        "ECGSharedMemory"
    );

    if (hMapFile == NULL) {
        printf("Could not create file mapping (%lu)\n", GetLastError());
        return 1;
    }

    // 2. Map view of file into address space
    float *signal = (float *) MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        SIZE * sizeof(float)
    );

    if (signal == NULL) {
        printf("Could not map view of file (%lu)\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // 3. Write ECG dummy data (replace later with file input)
    for (int i = 0; i < 10; i++) {
        signal[i] = i * 0.25;
    }

    // 4. Read & print shared memory
    printf("Windows Shared Memory ECG Data:\n");

    for (int i = 0; i < 10; i++) {
        printf("%f\n", signal[i]);
    }

    // 5. Cleanup
    UnmapViewOfFile(signal);
    CloseHandle(hMapFile);

    return 0;
}