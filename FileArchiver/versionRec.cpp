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

void versionRec::createData(const std::string& filePath, const int& verNum,
                            const std::size_t& len, const long int& modTime, 
                            const uint32_t& fHash, const std::string& comm)
{
    fileRef = filePath;
    versionNumber = verNum;
    length = len;  
    modifyTime = modTime;
    fileHash = fHash;
    comment = comm;
    blocks.clear();
}

void versionRec::createExisting(const std::string& filePath, const int& version, 
                                sql::Connection *dbc)
{
    fileRef = filePath;
    versionNumber = version;
    dbcon = dbc;
    
    const char* getversionrec = "select * from versionrec where fileref=? AND versionnum=?";
    sql::PreparedStatement *pstmt = NULL;
    sql::ResultSet *rs = NULL;
    pstmt = dbcon->prepareStatement(getversionrec);
    pstmt->setString(1, fileRef);
    pstmt->setInt(2, versionNumber);
   
    rs = pstmt->executeQuery();

    bool haveFileRec = rs->next();
    if (!haveFileRec) {
        delete rs;
        delete pstmt;
        return;
    }
    
    idVersionRec = rs->getInt(1);
    length = rs->getInt(4);
    modifyTime = rs->getInt(5);
    fileHash = rs->getUInt64(6);
    comment = rs->getString(7);
    
    getExistingBlocks();
}

void versionRec::saveToDatabase(sql::Connection *dbc, bool isUpdate)
{    
    dbcon = dbc;

    sql::PreparedStatement *pstmt = NULL;
    if (isUpdate) {
        const char* updatefilerec = "update filerec set nversion = nversion + 1, curhash = ? where filename = ?";
        pstmt = dbcon->prepareStatement(updatefilerec);
        pstmt->setUInt64(1, fileHash);
        pstmt->setString(2, fileRef);
        pstmt->executeUpdate();
        delete pstmt;
    }
    
    const char* putversionrec = "insert into versionrec values(?,?,?,?,?,?,?)";
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
            
            std::istringstream str(std::string(blocks[i].bytes, blocks[i].length));
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
    pstmt = dbcon->prepareStatement(getblktable);
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
        block.bytes = new char[block.length];
        rs->getBlob(6)->read(block.bytes, block.length);
        blocks.push_back(block);
    } while(rs->next());
}
