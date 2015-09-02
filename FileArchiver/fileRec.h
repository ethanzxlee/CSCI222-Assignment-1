#ifndef FILE_REC_H
#define FILE_REC_H
#include <string>
#include <vector>

#include "mysql_connection.h"
#include "mysql_driver.h"

const int BLOCK_SIZE = 4096;

class fileRec {
public:
//Setters
    /*
     * takes the full filepath and will set the name, length, filehash, tempName based on the filename and
     * numVersions = 1
     * NOTE: was setFilePath
    */
    void createData(const std::string& filePath);

    void setFileName(const std::string& x) { fileName = x; }
    
    /*
     * Pretty sure these commented out ones are not needed because createData
     * sets them.
     */
    //void setTempName(const std::string& x) { tempName = x; }
    //void setModifyTime(const std::string& x) { modifyTime = x; }
    //void setLength(const std::string& x) { length = x; }
    //void setFileHash(const std::string& x) { fileHash = x; }
    void setNumVersions(const int& x) { numVersions = x; }
    void setRefNumber(const int& x) { refNumber = x; }
    
//Add to collections
    //void addBlockHashes(const int& x) { blockHashes.push_back(x); }
    void addToVersionIds(const int& x) { versionIds.push_back(x); }
    void addToComments(const std::string& x) { comments.push_back(x); }

//Getters
    std::string getFilename() { return fileName; }
    std::string getTempName() { return tempName; }
    long int getModifyTime() { return modifyTime; }
    std::size_t getLength() { return length; }
    int getNumVersions() { return numVersions; }
    
    /*
     * This is hash of the original file
     */
    std::string getFileHash() { return fileHash; }
    
    /*
     * So this is the hash of the most recently updated version of the file
     */
    std::string getCurrentHash() { return currentHash; }
    
    int getRefNumber() { return refNumber; }
    std::vector<std::string> getBlockHashes() { return blockHashes; }
    std::vector<int> getVersionIds() { return versionIds; }
    std::vector<std::string> getComments() { return comments; updateComment(); }

//Other
    /*
     * Reads in the specified file with the full filepath e.g. /home/username/file.txt 
     * and then returns a fileRec with all private members filled out. 
     * It gets this from the database, returns NULL if it doesn't exist.
     * Example usage:
     * fileRec* someFile = getFile("/home/username/file.txt")
    */
    static fileRec* getFile(const std::string& filePath) throw(const char*);

    /*
     * Updates the database with all the current values of everything,
    */
    void saveToDatabase(sql::Connection* connection, bool closeConnection);
    
    /*
     * Will update TABLE comments with a new comment
     */
    void updateComment();

    /*
     * connectToDatabase should be called once at the beginning like.
     * fileRec::connectToDatabase();
     */
    static void connectToDatabase();
    
    /*
     * closeDatabase() should be called once at the end
     * fileRec::closeDatabase();
     */
    static void closeDatabase();
    
    /**
     * Check if the specified fileRec exists in the database
     * @param filePath The path of file to be checked
     * @return true if the fileRec exists
     */
    static bool exists(std::string filePath, sql::Connection* connection, bool closeConnection);
private:
    std::string fileName;
    std::string filePath;
    std::string tempName;
    long int modifyTime;		//Unix timestamp, may change to string
    std::size_t length;
    int numVersions;
    std::string fileHash;		//Hash of the entire original file in 1 go
    std::string currentHash;        //Hash of the most recently saved version
    int refNumber;	// 0 for the original file

    /*
     * These are hashes that are taken from size 4096 chunks out of the file
     * There should be ceil(length / 4096) of them for the original file
     * NOTE: not sure how this works when a file is updated yet
    */
    std::vector<std::string> blockHashes;		


    std::vector<int> versionIds;
    std::vector<std::string> comments;
private:
    void calculateFileHash();
    void calculateFileBlockHashes();
    std::size_t fileSize(std::string filename);

    //Database variables
    static bool invalid;
    static sql::Connection *dbcon;
    static sql::Driver *driver;
};

#endif /* FILE_REC_H */