/* 
 * File:   versionRec.cpp
 * Author: dllyd
 * 
 * Created on August 31, 2015, 5:41 AM
 */
#include <cstring>
#include <sstream>

#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/prepared_statement.h"

#include "versionRec.h"
#include "helperFuncs.h"

void versionRec::createData(const std::string& filePath, const int& versionNumber,
                            const std::size_t& length, const long int& modifyTime, 
                            const uint32_t& fileHash, const std::string& comment)
{
    fileRef = filePath;
    this->versionNumber = versionNumber;
    this->length = length;  
    this->modifyTime = modifyTime;
    this->fileHash = fileHash;
    this->comment = comment;
    blocks.clear();
}

void versionRec::createExisting(const std::string& filePath, const int& version, 
                                sql::Connection *dbcon)
{
    fileRef = filePath;
    versionNumber = version;
    this->dbcon = dbcon;
    
    const char* getversionrec = "select * from versionrec where fileref=? AND versionnum=?";
    sql::PreparedStatement *pstmt = NULL;
    sql::ResultSet *rs = NULL;
    pstmt = this->dbcon->prepareStatement(getversionrec);
    pstmt->setString(1, fileRef);
    pstmt->setInt(2, versionNumber);
   
    rs = pstmt->executeQuery();
    
    bool haveFileRec = rs->next();
    if (!haveFileRec) {
        delete rs;
        delete pstmt;
        return;
    }
    
    this->idVersionRec = rs->getInt(1);
    this->length = rs->getInt(4);
    this->modifyTime = rs->getInt(5);
    this->fileHash = rs->getUInt64(6);
    this->comment = rs->getString(7);
    
    getExistingBlocks();
}

void versionRec::saveToDatabase(sql::Connection *dbcon)
{    
    this->dbcon = dbcon;
    const char* putversionrec = "insert into versionrec values(?,?,?,?,?,?,?)";
   
    sql::PreparedStatement *pstmt = NULL;
    pstmt = dbcon->prepareStatement(putversionrec);
    pstmt->setInt(1, NULL);
    pstmt->setString(2, fileRef);
    pstmt->setInt(3, versionNumber);
    pstmt->setInt(4, length);
    pstmt->setInt(5, modifyTime);
    pstmt->setUInt64(6, fileHash);
    pstmt->setString(7, comment);
    
    pstmt->executeUpdate();
    delete pstmt;
    
    const char* getid = "SELECT LAST_INSERT_ID()";
    sql::ResultSet *rs = NULL;
    pstmt = dbcon->prepareStatement(getid);
    rs = pstmt->executeQuery();
    rs->next();
    idVersionRec = rs->getInt(1);
        
    saveBlocks();
}

void versionRec::saveBlocks()
{
    if (blocks.size() > 0) 
    {
        const char* putblktable = "insert into blktable values(?,?,?,?,?,?)";
        sql::PreparedStatement *pstmt = NULL;
        pstmt = dbcon->prepareStatement(putblktable);
        for (int i = 0; i < blocks.size(); ++i) 
        {
            pstmt->setInt(1, NULL);
            pstmt->setInt(2, idVersionRec);
            pstmt->setInt(3, blocks[i].length);
            pstmt->setInt(4, blocks[i].blockNum);
            pstmt->setInt(5, blocks[i].hash);
            
            std::istringstream  str(blocks[i].bytes);
            pstmt->setBlob(6, &str);
            pstmt->executeUpdate();
        }
        delete pstmt;  
    }
}

void versionRec::getExistingBlocks()
{
    const char* getblktable = "select * from blktable where version=?"; 
    sql::PreparedStatement *pstmt = NULL;
    sql::ResultSet *rs = NULL;
    pstmt = this->dbcon->prepareStatement(getblktable);
    pstmt->setInt(1, idVersionRec);
    rs = pstmt->executeQuery();
    
    bool haveblktable = rs->next();
    if (!haveblktable) {
        delete rs;
        delete pstmt;
        return;
    }
    
    Block block;
    do {
        block.length = rs->getInt(3);
        block.blockNum = rs->getInt(4);
        block.hash = rs->getUInt64(5);
        rs->getBlob(6)->read(block.bytes, block.length);
        blocks.push_back(block);
    } while(rs->next());
}





