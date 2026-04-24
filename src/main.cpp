/**
 * @file main.cpp
 * @brief User 1 Module - ECG Signal Processing Entry Point
 * 
 * This module demonstrates:
 * - ECG CSV data generation
 * - File reading using system calls
 * - Memory mapping for data loading
 * - Signal data parsing and display
 */

#include "../include/file_manager.h"
#include "../include/memory_manager.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

int main() {
    const char* filename = "data/ecg.csv";
    
    // Step 1: Generate ECG CSV data
    if (!generateECGData(filename, ECG_DATA_POINTS)) {
        std::fprintf(stderr, "Failed to generate ECG data\n");
        return 1;
    }
    
    // Step 2: Get file size
    long fileSize = getFileSize(filename);
    if (fileSize < 0) {
        std::fprintf(stderr, "Failed to get file size\n");
        return 1;
    }
    
    // Step 3: Read file using system calls
    char* buffer = static_cast<char*>(malloc(fileSize + 1));
    if (buffer == nullptr) {
        std::fprintf(stderr, "Failed to allocate buffer\n");
        return 1;
    }
    
    int bytesRead = readFile(filename, buffer, fileSize + 1);
    if (bytesRead < 0) {
        std::fprintf(stderr, "Failed to read file\n");
        free(buffer);
        return 1;
    }
    
    std::printf("File read successful\n");
    
    // Step 4: Load data using memory mapping
    void* mappedData = nullptr;
    if (!mapFileToMemory(filename, fileSize, &mappedData)) {
        std::fprintf(stderr, "Failed to map file to memory\n");
        free(buffer);
        return 1;
    }
    
    // Step 5: Parse CSV data
    std::vector<double> amplitudes;
    int count = parseCSVData(static_cast<const char*>(mappedData), amplitudes);
    
    if (count <= 0) {
        std::fprintf(stderr, "Failed to parse CSV data\n");
        unmapFileFromMemory(mappedData, fileSize + 1);
        free(buffer);
        return 1;
    }
    
    // Step 6: Print first 10 values
    std::printf("\nFirst 10 values of ECG signal:\n");
    std::printf("Index\tAmplitude\n");
    std::printf("-----\t---------\n");
    
    int printCount = (count < 10) ? count : 10;
    for (int i = 0; i < printCount; ++i) {
        std::printf("%d\t%.6f\n", i, amplitudes[i]);
    }
    
    // Cleanup
    unmapFileFromMemory(mappedData, fileSize + 1);
    free(buffer);
    
    std::printf("\nTotal data points loaded: %d\n", count);
    
    return 0;
}