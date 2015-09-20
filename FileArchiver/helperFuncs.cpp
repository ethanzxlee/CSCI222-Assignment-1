/* 
 * File:   helperFuncs.cpp
 * Author: dllyd
 * 
 * Created on September 3, 2015, 1:51 AM
 */
#include <boost/filesystem.hpp>
#include <boost/crc.hpp>

#include "helperFuncs.h"

uint32_t calculateFileHash(const std::string& filePath)
{      
    boost::crc_32_type result;
    std::ifstream ifs(filePath.c_str(), std::ios::binary);

    if (ifs.good())
    {
	do
	{
            char buffer[BLOCK_SIZE];
            ifs.read(buffer, BLOCK_SIZE);
            result.process_bytes(buffer, ifs.gcount());
	} while (ifs.good());
    }
    return result.checksum();
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
    std::ifstream ifs(filePath.c_str(), std::ios::binary);
    std::vector<uint32_t> blockHashes;
    char buffer[BLOCK_SIZE];
   
    while (ifs.read(buffer, BLOCK_SIZE))
    {
        boost::crc_32_type result;
        result.process_bytes(buffer, ifs.gcount());
        blockHashes.push_back(result.checksum());
    }

    boost::crc_32_type result;
    result.process_bytes(buffer, ifs.gcount());    
    blockHashes.push_back(result.checksum());

    return blockHashes;
}
