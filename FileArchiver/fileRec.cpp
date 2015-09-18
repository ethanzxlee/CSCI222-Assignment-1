#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/prepared_statement.h"

#include "fileRec.h"
#include "helperFuncs.h"
#include "versionRec.h"

void fileRec::createData(const std::string& fileP, const std::string& temporaryPath,
             const std::string& comment, sql::Connection *dbc)
{
    filePath = fileP;
    tempPath = temporaryPath;
    dbcon = dbc;
    modifyTime = getFileModifyTime(filePath);
    length = fileSize(filePath);
    fileHash = calculateFileHash(filePath);
    currentHash = fileHash;
    blockHashes = calculateFileBlockHashes(filePath);
    numVersions = 1;
    refNumber = 0;
    comments.push_back(comment);
}

void fileRec::createExisting(const std::string& file, sql::Connection *db)
{
    filePath = file;
    dbcon = db;
    const char* getfilerec = "select * from filerec where filename=?";
    sql::PreparedStatement *pstmt = NULL;
    sql::ResultSet *rs = NULL;

    pstmt = dbcon->prepareStatement(getfilerec);
    pstmt->setString(1, filePath);
    rs = pstmt->executeQuery();
    bool haveFileRec = rs->next();
    if (!haveFileRec) {
        delete rs;
        delete pstmt;
        return;
    }
    
    filePath = rs->getString(1);
    fileHash = rs->getUInt64(2);
    currentHash = rs->getUInt64(3);
    refNumber = rs->getInt(4);
    numVersions = rs->getInt(5);
    length = rs->getInt(6);
    modifyTime = rs->getInt(7);    
}

void fileRec::saveToDatabase()
{
    const char* putFileRec = "insert into filerec values(?,?,?,?,?,?,?,?)";
    
    sql::PreparedStatement *pstmt = NULL;  
    pstmt = dbcon->prepareStatement(putFileRec);
    pstmt->setString(1, filePath);
    pstmt->setUInt64(2, currentHash);
    pstmt->setUInt64(3, fileHash);
    pstmt->setInt(4, refNumber);
    pstmt->setInt(5, numVersions);
    pstmt->setInt(6, length);
    pstmt->setInt(7, modifyTime);

    std::ifstream ins(tempPath.c_str());
    pstmt->setBlob(8, &ins);

    pstmt->executeUpdate();
    delete pstmt;
    
    saveBlockHashes();
    
    // Need to create an original versionRec because it contains the comment for this version
    // There will be no blktable for the original version, because there is no changes
    versionRec original;
    original.createData(filePath, 0, length, modifyTime, fileHash, comments[0]);
    original.saveToDatabase(dbcon);
}

void fileRec::saveBlockHashes()
{
    const char* putfileblkhashes = "insert into fileblkhashes values(?,?,?,?)";
    
    sql::PreparedStatement *pstmt = NULL;  
    pstmt = dbcon->prepareStatement(putfileblkhashes);
    for (int i = 0; i < blockHashes.size(); ++i) {
        pstmt->setInt(1, NULL);    
        pstmt->setString(2, filePath);
        pstmt->setInt(3, i);
        pstmt->setUInt64(4, blockHashes[i]);
        pstmt->executeUpdate();
    }
    delete pstmt;
}

std::vector<versionRec> fileRec::returnVector(const std::string&filePath, sql::Connection *dbcon)
{
    return returnVector(filePath, numVersions, dbcon);
}

std::vector<versionRec> fileRec::returnVector(const std::string&filePath, int endVersion, sql::Connection *dbcon)
{
    createExisting(filePath, dbcon);
    
    std::vector<versionRec> returnRec;
    
    for (int i = refNumber; i <= endVersion; ++i) {
        versionRec temp;
        temp.createExisting(filePath, i, dbcon);
        returnRec.push_back(temp);
    }
    return returnRec;
}

/*
 * TODO: is this still needed at all?
 * It also may not have a valid dbcon.
 */
fileRec* fileRec::getFile(const std::string& filePath) throw(const char*)
{        
    fileRec* returnFile = new fileRec;
    returnFile->createExisting(filePath, dbcon);
    return returnFile;
}

sql::Connection *fileRec::dbcon = NULL;
