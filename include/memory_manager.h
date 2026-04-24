/**
 * @file memory_manager.h
 * @brief Cross-platform memory mapping header
 * 
 * Provides functions for memory-mapped file operations.
 */

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstddef>

/**
 * @brief Allocate memory for file content
 * @param size Size of memory to allocate
 * @return Pointer to allocated memory, or nullptr on failure
 */
void* allocateMemory(size_t size);

/**
 * @brief Free allocated memory
 * @param ptr Pointer to memory to free
 * @param size Size of memory block
 */
void freeMemory(void* ptr, size_t size);

/**
 * @brief Map file content into memory
 * @param filename Path to the file
 * @param fileSize Size of the file
 * @param mappedData Output pointer to mapped data
 * @return true if mapping successful, false otherwise
 */
bool mapFileToMemory(const char* filename, size_t fileSize, void** mappedData);

/**
 * @brief Unmap file from memory
 * @param addr Address of mapped memory
 * @param size Size of mapped region
 */
void unmapFileFromMemory(void* addr, size_t size);

#endif // MEMORY_MANAGER_H