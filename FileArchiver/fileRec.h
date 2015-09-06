#ifndef FILE_REC_H
#define FILE_REC_H
#include <string>
#include <vector>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "MurMurHash3.h"
#include "versionRec.h"

class fileRec {
public:
    /**
     * Fills in this fileRec with all information based on the tempPath
     * Still needs saveToDatabase to be called to commit it to database
     * @param filePath The filepath to the original file being added
     * @param tempPath The filepath to the temporary zipped file, this will
     * be added to the database as the blob
     * @param comment This is the comment that will be associated with this version
     * of the file in the original versionrec table
     * @param dbconn A valid connection to the database
    */
    void createData(const std::string& filePath, const std::string& tempPath,
                    const std::string& comment, sql::Connection *dbconn);
    
    /**
     * Used to create and populate a fileRec with data that already exists in the db
     * @param filePath The path to the original file
     * @param dbcon A valid connection to the database
     */
    void createExisting(const std::string& filePath, sql::Connection *dbcon);
    
    /**
     * Gives back a vector containing all versionRecs that are associated with this 
     * filePath.
     * @param filePath Path to the original file
     * @param dbconn A valid connection to the database
     * @return 
     */
    std::vector<versionRec> returnVector(const std::string& filePath, sql::Connection *dbcon);
    
    /**
     * Saves whats in memory to the database, assuming that this is a new file (the original)
     */
    void saveToDatabase();
    
    /**
     * Setters
     * NOTE: not sure which ones are needed atm
     */
    void setTempPath(const std::string& x) { tempPath = x; }
    void setModifyTime(const long int& x) { modifyTime = x; }
    void setLength(const long int& x) { length = x; }
    void setFileHash(const long int& x) { fileHash = x; }
    void setNumVersions(const int& x) { numVersions = x; }
    void setRefNumber(const int& x) { refNumber = x; }
    void addToVersionIds(const int& x) { versionIds.push_back(x); }
    void addToComments(const std::string& x) { comments.push_back(x); }

    /**
     * Getters
     * NOTE: not sure which ones are needed atm
     */
    std::string getTempPath() { return tempPath; }
    long int getModifyTime() { return modifyTime; }
    std::size_t getLength() { return length; }
    int getNumVersions() { return numVersions; }
    uint32_t getFileHash() { return fileHash; }
    uint32_t getCurrentHash() { return currentHash; }
    int getRefNumber() { return refNumber; }
    std::vector<uint32_t> getBlockHashes() { return blockHashes; }
    std::vector<int> getVersionIds() { return versionIds; }
    std::vector<std::string> getComments() { return comments; }

    
    /*
     * NOTE: Not sure if still needed, or if it is it needs to take in a sql con
     * @param filePath The filepath to the original version of the file requested
     * @return  A new fileRec* with all members filled out
    */
    static fileRec* getFile(const std::string& filePath) throw(const char*);
private:
    std::string filePath;
    std::string tempPath;
    long int modifyTime;
    std::size_t length;
    int numVersions;
    uint32_t fileHash;              //Hash of the entire original file
    uint32_t currentHash;           //Hash of the most recently saved version
    int refNumber;

    std::vector<uint32_t> blockHashes;      
    std::vector<int> versionIds;
    std::vector<std::string> comments;
private:
    void saveBlockHashes();

    static sql::Connection *dbcon;
};

#endif /* FILE_REC_H */