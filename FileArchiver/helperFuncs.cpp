/* 
 * File:   helperFuncs.cpp
 * Author: dllyd
 * 
 * Created on September 3, 2015, 1:51 AM
 */
#include <boost/filesystem.hpp>

#include "helperFuncs.h"

/**
 * TODO: don't allocate space for the entire file
 */
uint32_t calculateFileHash(const std::string& filePath)
{  
    uint32_t hash[4];
    int fileLength = fileSize(filePath);
    std::ifstream file(filePath.c_str(), std::ios::binary);   
    char* key = new char[fileLength];
    file.read(key, fileLength);    
    MurmurHash3_x86_128(key, fileLength, SEED_VALUE, hash);
    delete [] key;

    /*
     * There are 4 hash's only using the first one atm
     */
     return hash[0];
}

std::size_t fileSize(const std::string& filePath)
{
    std::ifstream in(filePath.c_str(), std::ios::ate | std::ios::binary);
    return in.tellg(); 
}

long int getFileModifyTime(const std::string& filePath)
{
    return boost::filesystem::last_write_time(filePath);
}

std::vector<uint32_t> calculateFileBlockHashes(const std::string& filePath)
{
    std::size_t length = fileSize(filePath);
    std::ifstream file(filePath.c_str(), std::ios::binary);
    std::vector<uint32_t> blockHashes;
    uint32_t hash[4];
    char key[BLOCK_SIZE];
   
    while (file.read((char *)key, BLOCK_SIZE))
    {
        MurmurHash3_x86_128(key, BLOCK_SIZE, SEED_VALUE, hash);
     
        /**
         * Currently only using the first hash, there is 4 tho
         */
        blockHashes.push_back(hash[0]);
    }
    
    //This will be the remaining portion that wasn't a full BLOCK_SIZE bytes
    std::size_t extra = length - (BLOCK_SIZE*(length / BLOCK_SIZE));
    MurmurHash3_x86_128(key, extra, SEED_VALUE, hash);
    blockHashes.push_back(hash[0]);

    return blockHashes;
}
