#include "fileRec.h"
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "md5.h"
#include <cstring>
#include <iostream>
#include <fstream>

#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/prepared_statement.h"

// Will eventually be changed to my ip
const char* dataBaseStr = "tcp://127.0.0.1:3306";	
const char* dataBaseUserName = "root";		//Should probably also change?
const char* dataBaseUserPassword = "1qaz2wsxmko0nji9";
const char* dataBaseSchema = "ass2";

const char* noDB = "Unable to open the database";
const char* prepareFailed = "Failure when preparing statement";
const char* bindFailed = "Failure when binding arguments";
const char* writeFailed = "Failure when writing to database";


long int getFileEpoch(std::string path)
{
    struct stat attr;
    stat(path.c_str(), &attr); 
    long int test = static_cast<long int>(attr.st_mtime); 
    return test;
}

/*
 * This calculates all of the block hashes for the fileRec file
 * this function isn't meant to be called outside of this file.
 * Each block is of size 4096 and the hash is added to the blockHashes
 * vector 
 * NOTE: its using md5 from source code I got from 262 might need to ask neil
 * if were allowed to use it?
 */
void fileRec::calculateFileBlockHashes()
{
    MD5 md5;
    std::ifstream file(tempName.c_str());

    unsigned char buffer[BLOCK_SIZE];
    long len;
    while (file.read((char *)buffer, BLOCK_SIZE))
    {
            char *x = md5.digestMemory(buffer, BLOCK_SIZE);
            std::string chunk = x;
            //std::cout << "Chunk: " << x << " " << blockHashes.size() << std::endl;
            blockHashes.push_back(chunk);
    }

    /*
     * TODO: For the last partial block?
     */ 
    char *x = md5.digestMemory(buffer, BLOCK_SIZE);
    std::string chunk = x;
    //std::cout << "Chunk: " << x << " " << blockHashes.size() << std::endl;
    blockHashes.push_back(chunk);
}

/*
 * Calculates the hash of the ENTIRE file in one go
 * shouldn't need to be called outside this file
 */
void fileRec::calculateFileHash()
{
    MD5 md5;
    char fileP[256];
    strcpy(fileP, tempName.c_str());
    fileHash = md5.digestFile(fileP);
    //std::cout << "fileHash: " << fileHash << std::endl;
}

/*
 * Simple function to return the amount of bytes in a file
 */
std::size_t fileRec::fileSize(std::string filename)
{
    std::ifstream in(filename.c_str(), std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

void fileRec::createData(const std::string& filePath)
{
    // Find the filename
    /*
     * Not sure if needed
     */
    std::size_t found = filePath.find_last_of("/\\");
    fileName = filePath.substr(found+1);

    this->filePath = filePath;
    
    /*
     * This is the name of the blob for the first occurance it will be the same as the filepath
     * after that it will need something unique about it maybe just + 1
     */
    this->tempName = filePath;

    /*
     * Get the modification time as a unix timestamp (time since epoch)
     */
    modifyTime = getFileEpoch(filePath);

    /*
     * Get the filehash for the entire file
     */
    calculateFileHash();
    
    /*
     * Since its the first version the currentHash is the same as the original
     */
    currentHash = fileHash;

    /*
     * Get the hashes per chunk
     */
    calculateFileBlockHashes();

    /*
     * Get the size of the file
     */
    length = fileSize(filePath.c_str());
    //std::cout << length << std::endl;

    /*
     * This is the first version, or if not will be updated elsewhere
     */
    numVersions = 1;

    /*
     * This is the original so its 0, if its not it will be updated elsewhere
     */
    refNumber = 0;
}

fileRec* fileRec::getFile(const std::string& filePath) throw(const char*)
{
    if (invalid)
        throw (noDB);
    
    // filename in the database is actually the filepath not just the name
    const char* getFileRec = "select * from filerec where filename=?";
    sql::PreparedStatement *pstmt = NULL;
    sql::ResultSet *rs = NULL;
    
    pstmt = dbcon->prepareStatement(getFileRec);
    pstmt->setString(1, filePath);
    rs = pstmt->executeQuery();
    bool haveFileRec = rs->next();
    if (!haveFileRec) {
        delete rs;
        delete pstmt;
        return NULL;
    }
    
    fileRec* returnFile = new fileRec;
    returnFile->filePath = rs->getString(1);
    returnFile->fileHash = rs->getString(2);
    returnFile->currentHash = rs->getString(3);
    returnFile->refNumber = rs->getInt(4);
    returnFile->numVersions = rs->getInt(5);
    returnFile->length = rs->getInt(6);
    returnFile->modifyTime = rs->getInt(7);  //TODO: fix this
    returnFile->modifyTime = rs->getInt(8);
    returnFile->tempName = rs->getString(9); //TODO: fix this
    returnFile->tempName = rs->getString(10);
 
    std::size_t found = filePath.find_last_of("/\\");
    returnFile->fileName = filePath.substr(found+1);
    
    return returnFile;
}

/*
 * Need to initialise static members
*/
bool fileRec::invalid = true;
sql::Connection *fileRec::dbcon = NULL;
sql::Driver *fileRec::driver = NULL;

void fileRec::connectToDatabase()
{
    invalid = true;
    driver = get_driver_instance();
    try {
            dbcon = driver->connect(dataBaseStr, dataBaseUserName, dataBaseUserPassword);
    } catch(sql::SQLException &e) {
            e.what();
            return;
    }
    dbcon->setSchema(dataBaseSchema);
    std::cout << "Connected to database" << std::endl;
    invalid = false;
}

void fileRec::closeDatabase()
{
    if (!invalid) {
            dbcon->close();
            delete dbcon;
    }
    invalid = true;
}

void fileRec::saveToDatabase(sql::Connection* connection, bool closeConnection)
{
//    if (invalid)
//        throw (noDB);
//    
    //TODO: probably need to update other tables as well
    const char* putFileRec = "insert into filerec values(?,?,?,?,?,?,?,?,?,?)";
    const char* putBlobTable = "insert into blobtable values(?,?)";
    
    /*
     * Update TABLE blobtable, this needs to be done before filerec
     * because of foreign key constraint
     */
    sql::PreparedStatement *pstmt = NULL;
    pstmt = connection->prepareStatement(putBlobTable);
    pstmt->setString(1, tempName);
    
    //NOTE: large files currently break mysql
    std::ifstream ins(filePath.c_str());
    pstmt->setBlob(2, &ins);
    
    pstmt->executeUpdate();
    delete pstmt;
    
    // Update TABLE filerec
    pstmt = NULL;
    pstmt = connection->prepareStatement(putFileRec);
    pstmt->setString(1, filePath);
    pstmt->setString(2, fileHash);     // Hash of the full original
    pstmt->setString(3, currentHash);  // Hash of the full most recent
    pstmt->setInt(4, refNumber);    //Reference number 0 for original
    pstmt->setInt(5, numVersions);  // Number of versions stored
    pstmt->setInt(6, length);       // Length of the reference version
    pstmt->setInt(7, modifyTime);   //TODO: change 7 and 8 
    pstmt->setInt(8, modifyTime);
    pstmt->setString(9, tempName);  //TODO: change 9 and 10 they are the name of the original blob in the blobtable
    pstmt->setString(10, tempName);
    
    pstmt->executeUpdate();
    delete pstmt;
    
    if (closeConnection) {
        connection->close();
    }
    
    //TODO: other tables?
}

void fileRec::updateComment()
{

}

bool fileRec::exists(std::string filePath, sql::Connection* connection, bool closeConnection) {
    bool exists = false;
    const char* checkFileExists = "SELECT filename FROM `filerec` WHERE filename = ?";

    sql::PreparedStatement* statement = connection->prepareStatement(checkFileExists);
    statement->setString(1, filePath);
    sql::ResultSet* result = statement->executeQuery();
    exists = result->rowsCount() > 0;
    
    if (closeConnection) {
        connection->close();
    }

    return exists;
}



