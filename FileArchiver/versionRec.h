#ifndef VERSIONREC_H
#define VERSIONREC_H
#include <string>
#include <vector>

#include "mysql_connection.h"
#include "mysql_driver.h"

#include "helperFuncs.h"

class versionRec {
public:
    /**
     * Fill out a versionRec with all data
     * Will still need to manually call addBlock to fill the blocks vector
     */
    void createData(const std::string& filePath, const int& verNum,
                    const std::size_t& length, const long int& modifyTime, 
                    const uint32_t& fileHash,  const std::string& comment);
    
    /**
     * Used to create and populate a versionRec with data that already exists in the db
     * @param filePath The path to the original file
     * @param version The version number of the versionRec you want
     * @param dbcon A valid connection to the database
     */
    void createExisting(const std::string& filePath, const int& version, sql::Connection *dbcon);
        
    /**
     * Saves to the table versionrec and blktable. Assumes its a new version and 
     * member variables are filled, including the blocks vector
     * @param dbcon A valid connection to the mysql database
     */
    void saveToDatabase(sql::Connection *dbcon, bool isUpdate = false);
    
    /**
     * @param b A Block with proper information of the block filled in
     */
    void addBlock(const Block& b) { blocks.push_back(b); }
    
    /**
     * Getters
     */
    std::string getFileReg() { return fileRef; }
    int getVersionNumber() { return versionNumber; }
    std::size_t getLength() { return length; }
    long int getModifyTime() { return modifyTime; }
    uint32_t getHash() { return fileHash; }
    std::vector<Block> getBlocks() { return blocks; }
    int getSymbol(){ return symbolDecision;}
    std::string getComment(){ return comment;}
    
    /**
     * Setters
     */
    void setFileRef(const std::string& x) { fileRef = x; }
    void setVersionNumber(const int& x) { versionNumber = x; }
    void setLength(const std::size_t& x) { length = x; }
    void setModifyTime(const long int& x) { modifyTime = x; }
    void setHash(const uint32_t& x) { fileHash = x; }   
    void setComment(const std::string & x){ comment=x;}
    void setSymbol(int x){ symbolDecision = x;}
    
private:
    void getExistingBlocks();
    void saveBlocks();
    
    int idVersionRec;
    std::string fileRef;
    int versionNumber;
    std::size_t length;
    long int modifyTime;
    uint32_t fileHash;
    std::string comment;
    std::vector<Block> blocks;
    int symbolDecision;
    
    //Database variable
    sql::Connection *dbcon;
};

#endif  /* VERSIONREC_H */

