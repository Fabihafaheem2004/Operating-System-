/**
 * @file file_manager.h
 * @brief Cross-platform file management header for ECG data handling
 * 
 * Provides functions for CSV generation and file I/O using system calls.
 */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <cstddef>
#include <vector>

// ECG Data Structure
struct ECGSample {
    double time;
    double amplitude;
};

// Constants
constexpr int ECG_DATA_POINTS = 1024;
constexpr double TIME_STEP = 0.01;

/**
 * @brief Generate ECG CSV data file
 * @param filename Path to the CSV file
 * @param numPoints Number of data points to generate (default: 1024)
 * @return true if generation successful, false otherwise
 */
bool generateECGData(const char* filename, int numPoints = ECG_DATA_POINTS);

/**
 * @brief Read file contents using system calls
 * @param filename Path to the file
 * @param buffer Buffer to store file contents
 * @param bufferSize Size of the buffer
 * @return Number of bytes read, or -1 on error
 */
int readFile(const char* filename, char* buffer, size_t bufferSize);

/**
 * @brief Get file size
 * @param filename Path to the file
 * @return File size in bytes, or -1 on error
 */
long getFileSize(const char* filename);

/**
 * @brief Parse CSV data and extract amplitude values
 * @param csvData Raw CSV data string
 * @param amplitudes Vector to store parsed amplitude values
 * @return Number of values parsed
 */
int parseCSVData(const char* csvData, std::vector<double>& amplitudes);

/**
 * @brief Close file handle
 * @param fd File descriptor/handle
 */
void closeFile(int fd);

#endif // FILE_MANAGER_H