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
#include <chrono>

int main() {
    const char* filename = "data/ecg.csv";
    
    auto totalStart = std::chrono::high_resolution_clock::now();
    
    auto genStart = std::chrono::high_resolution_clock::now();
    if (!generateECGData(filename, ECG_DATA_POINTS)) {
        std::fprintf(stderr, "Failed to generate ECG data\n");
        return 1;
    }
    auto genEnd = std::chrono::high_resolution_clock::now();
    double genTime = std::chrono::duration<double, std::milli>(genEnd - genStart).count();
    
    auto sizeStart = std::chrono::high_resolution_clock::now();
    long fileSize = getFileSize(filename);
    if (fileSize < 0) {
        std::fprintf(stderr, "Failed to get file size\n");
        return 1;
    }
    auto sizeEnd = std::chrono::high_resolution_clock::now();
    double sizeTime = std::chrono::duration<double, std::milli>(sizeEnd - sizeStart).count();
    
    auto readStart = std::chrono::high_resolution_clock::now();
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
    auto readEnd = std::chrono::high_resolution_clock::now();
    double readTime = std::chrono::duration<double, std::milli>(readEnd - readStart).count();
    
    std::printf("File read successful\n");
    
    auto mapStart = std::chrono::high_resolution_clock::now();
    void* mappedData = nullptr;
    if (!mapFileToMemory(filename, fileSize, &mappedData)) {
        std::fprintf(stderr, "Failed to map file to memory\n");
        free(buffer);
        return 1;
    }
    auto mapEnd = std::chrono::high_resolution_clock::now();
    double mapTime = std::chrono::duration<double, std::milli>(mapEnd - mapStart).count();
    
    auto parseStart = std::chrono::high_resolution_clock::now();
    std::vector<double> amplitudes;
    int count = parseCSVData(static_cast<const char*>(mappedData), amplitudes);
    
    if (count <= 0) {
        std::fprintf(stderr, "Failed to parse CSV data\n");
        unmapFileFromMemory(mappedData, fileSize + 1);
        free(buffer);
        return 1;
    }
    auto parseEnd = std::chrono::high_resolution_clock::now();
    double parseTime = std::chrono::duration<double, std::milli>(parseEnd - parseStart).count();
    
    std::printf("\nFirst 10 values of ECG signal:\n");
    std::printf("Index\tAmplitude\n");
    std::printf("-----\t---------\n");
    
    int printCount = (count < 10) ? count : 10;
    for (int i = 0; i < printCount; ++i) {
        std::printf("%d\t%.6f\n", i, amplitudes[i]);
    }
    
    unmapFileFromMemory(mappedData, fileSize + 1);
    free(buffer);
    
    auto totalEnd = std::chrono::high_resolution_clock::now();
    double totalTime = std::chrono::duration<double, std::milli>(totalEnd - totalStart).count();
    
    std::printf("\n========== TIMING RESULTS ==========\n");
    #ifdef _WIN32
    std::printf("Platform: Windows\n");
    #else
    std::printf("Platform: Linux\n");
    #endif
    std::printf("ECG Generation:  %.3f ms\n", genTime);
    std::printf("Get File Size:    %.3f ms\n", sizeTime);
    std::printf("File Read:       %.3f ms\n", readTime);
    std::printf("Memory Mapping:   %.3f ms\n", mapTime);
    std::printf("CSV Parsing:      %.3f ms\n", parseTime);
    std::printf("--------------------------------\n");
    std::printf("TOTAL TIME:      %.3f ms\n", totalTime);
    std::printf("================================\n");
    std::printf("\nTotal data points loaded: %d\n", count);
    
    return 0;
}