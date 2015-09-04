/* 
 * File:   extraFunctions.h
 * Author: dllyd
 *
 * Created on September 3, 2015, 1:51 AM
 */

#ifndef HELPER_FUNCS_H
#define HELPER_FUNCS_H
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "MurMurHash3.h"

/**
 * Can be either 4096 or 8192
 */
const int BLOCK_SIZE = 4096;

/**
 * Arbitrarily chosen
 */
const uint32_t SEED_VALUE = 42;

struct Block {
    int blockNum;
    uint32_t hash;
    std::size_t length;
    char* bytes;
};

/**
 * Calculates the hash of the entire file
 * @param filePath The path to the file
 * @return the hash of the entire file
 */
uint32_t calculateFileHash(const std::string& filePath);

/**
 * Gets the modification time of a file
 * @param filePath The path to the file
 * @return The last modification time as time since epoch
 */
long int getFileModifyTime(const std::string& filePath);

/**
 * Calculates the size of a file in bytes
 * @param filePath The path to the file
 * @return The size of the file in bytes
 */
std::size_t fileSize(const std::string& filePath);

/**
 * Calculates the hash for each block of size BLOCK_SIZE
 * @param filePath The path to the file
 * @return A vector containing the hash of each block
 */
std::vector<uint32_t> calculateFileBlockHashes(const std::string& filePath);

#endif  /* HELPER_FUNCS_H */
