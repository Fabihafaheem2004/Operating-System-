#include <windows.h>
#include <stdio.h>

#define SIZE 10

int main() {

    // 1. Allocate memory (like mmap/malloc but Windows low-level)
    float *ecg = (float *) VirtualAlloc(
        NULL,
        SIZE * sizeof(float),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (ecg == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Memory allocated using VirtualAlloc\n");

    // 2. Store ECG-like data
    for (int i = 0; i < SIZE; i++) {
        ecg[i] = 0.1f * i;
    }

    // 3. Print data
    printf("ECG Data:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%f\n", ecg[i]);
    }

    // 4. Free memory using VirtualFree
    if (VirtualFree(ecg, 0, MEM_RELEASE)) {
        printf("Memory freed using VirtualFree\n");
    } else {
        printf("VirtualFree failed\n");
    }

    return 0;
}
