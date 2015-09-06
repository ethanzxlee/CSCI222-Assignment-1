/* 
 * File:   FileArchiver.h
 * Author: zhexian
 *
 * Created on 1 September 2015, 3:22 PM
 */

#ifndef FILEARCHIVER_H
#define	FILEARCHIVER_H

#include <string.h>
#include <fstream>
#include <stdio.h>
#include <vector>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 
#include <boost/uuid/uuid_io.hpp> 
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

#include "fileRec.h"
#include "helperFuncs.h"

class FileArchiver {
public:
    /**
     * Constructor
     */
    FileArchiver() throw (sql::SQLException);
    
    /**
     * Destructor
     */
    virtual ~FileArchiver();
    
    /**
     * Check if the specified file is different from the one in the database
     * @param filename The path of the file to be checked
     * @return true if different, false if same 
     */
    bool differs(const std::string& filePath);
    
    /**
     * Check if the file exists in the database
     * @param filename The path of the file to be checked
     * @return true if the file exists in the database
     */
    bool exists(const std::string& filePath);
    
    /**
     * Insert a new file into the database
     * @param filename The path of the file to be inserted to the database
     * @param comment The comment of the insertion
     */
    void insertNew(const std::string& filePath, const std::string& comment);
    
    /**
     * Update the newer version of file to the database
     * @param filename The path of the file to be updated to the database
     * @param comment The comment of the update
     */
    void update(const std::string& filePath, const std::string& comment);

    
    void retrieveFile(const std::string& filePath, const std::string& destinationFilePath, const int versionNum, sql::Connection* connection);
    void retrieveFile(const std::string& filePath, const std::string& destinationFilePath, const int versionNum);
    void retrieveFile(const std::string& filePath, const std::string& destinationFilePath, sql::Connection* connection); 

    void getCurrentVersionNumber(std::string filename);
    
    void getHashOfLastSaved(std::string filename);
    
    void getComment(std::string filename, int versionnum);
    
    void getVersionInfo(std::string filename);
    
    void setReference(std::string filename, int versionnum, std::string comment);
    
    
private:
    const char* DB_HOSTNAME = "tcp://127.0.0.1:3306";
    const char* DB_USERNAME = "root";
    const char* DB_PASSWORD = "1qaz2wsxmko0nji9";
    const char* DB_SCHEMA = "FileArchiver";
    
    
    /**
     * Create a connection to the MySQL database and validate the schema if specified.
     * The connection has to be closed as usual after using it.
     * @param checkSchema true to validate the schema
     * @return A MySQL database connection
     */
    sql::Connection* connectDB(bool checkSchema);
    
    /**
     * Compress a file om disk
     * @param source The path of the source file
     * @param destination The path of the compressed file
     * @return true if the file is compressed successfully
     */
    bool compressFile(const std::string& source, const std::string& destination); 
    
    /**
     * Decompress a file on disk
     * @param source The path of compressed file
     * @param destination The path of decompressed file
     * @return true if the file is decompressed successfully
     */
    bool decompressFile(const std::string& source, const std::string& destination);
};

#endif	/* FILEARCHIVER_H */

