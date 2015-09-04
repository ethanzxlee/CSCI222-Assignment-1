/* 
 * File:   FileArchiver.cpp
 * Author: zhexian
 * 
 * Created on 1 September 2015, 3:22 PM
 */

#include "FileArchiver.h"

FileArchiver::FileArchiver() throw (sql::SQLException) {
    sql::Connection* connection = connectDB(true);
    delete connection;
}

FileArchiver::FileArchiver(const FileArchiver& orig) {
}

FileArchiver::~FileArchiver() {
}

bool FileArchiver::differs(std::string filePath) {
    bool differs = true;
    boost::uuids::random_generator generator;
    boost::uuids::uuid uniqueId = generator();
    std::string tempFilePath = "/tmp/" + boost::uuids::to_string(uniqueId);
    
    if (compressFile(filePath, tempFilePath)) {
        sql::Connection* connection = connectDB(false);

        const char* sql_selectLatestHash = "SELECT curhash FROM filerec WHERE filename = ?";
        sql::PreparedStatement statement = connection->prepareStatement(sql_selectLatestHash);
        statement->setString(1, filePath);
        sql::ResultSet* result = statement->executeQuery();

        if (result->next()) {
            uint32_t dbHash = result->getInt(1);
            uint32_t localHash = calculateFileHash(tempFilePath);
            differs = dbHash == localHash;
        }
        
        connection.close();
        delete connection;
        delete statement; 
        delete result;
        std::remove(tempFilePath.c_str());
    }
    
    return differs;
}

bool FileArchiver::exists(std::string filePath) {
    bool exists = false;
    sql::Connection* connection = connectDB(false); 
    
    const char* sql_checkFileExists = "SELECT count() FROM `filerec` WHERE filename = ?";
    sql::PreparedStatement* statement = connection->prepareStatement(sql_checkFileExists);
    statement->setString(1, filePath);
    
    sql::ResultSet* result = statement->executeQuery();  
    if (result->next()) {
        exists = result->getInt(1) == 1;
    }
    
    connection->close();
    delete connection;
    delete statement;
    delete result;
    
    return exists;
}

void FileArchiver::insertNew(std::string filePath, std::string comment) {
    boost::uuids::random_generator generator;
    boost::uuids::uuid uniqueId = generator();
    std::string tempFilePath = "/tmp/" + boost::uuids::to_string(uniqueId);

    if (compressFile(filename, tempFilePath)) {
        sql::Connection* connection = connectDB(false);
        
        fileRec* newFileRec = new fileRec();
        newFileRec->createData(filePath, tempFilePath, comment);
        newFileRec->save(connection);
        
        delete newFileRec;        
        connection->close();
        delete connection;
        std::remove(tempFilePath.c_str());
    };
}

void FileArchiver::update(std::string filePath, std::string comment) {
    sql::Connection connection = connectDB(false);

    const char* selectBlob = "SELECT filedata, currentversion FROM filerec WHERE filename = ?";
    sql::PreparedStatement* statement = connection->prepareStatement();
    statement->setString(1, filePath);
    sql::ResultSet* result = statement->executeQuery();

    if (result->next()) {
        boost::uuids::random_generator generator;
        boost::uuids::uuid uniqueId1 = generator();
        std::string tempFilePath = "/tmp/" + boost::uuids::to_string(uniqueId1);
        std::ofstream tempFile(tempFilePath.c_str());

        if (tempFile.is_open()) {
            std::istream* blobStream = result->getBlob(1);
            tempFile << blobStream->rdbuf();
            tempFile.close();
            
            boost::uuids::uuid uniqueId2 = generator();
            std::string rebuildFilePath = "/tmp/" + boost::uuids::to_string(uniqueId2);
            
            
            
            std::ofstream file(tempFilePath.c_str(), std::ofstream::app);
            
                
  
        }

    }

    delete statement;
    delete result;

    connection.close();
    delete connection;
}

sql::Connection* FileArchiver::connectDB(bool checkSchema) {
    sql::Driver* driver = get_driver_instance();
    sql::Connection* connection = driver->connect(DB_HOSTNAME, DB_USERNAME, DB_PASSWORD);
    
    if (checkSchema) {
        sql::PreparedStatement* statement = NULL;

        const char* createSchema = "CREATE SCHEMA IF NOT EXISTS FileArchiver";
        statement = connection->prepareStatement(createSchema);
        statement->execute();
        delete statement;

        connection->setSchema(DB_SCHEMA);

        const char* createTableFileRec = "CREATE TABLE IF NOT EXISTS `filerec` (`filename` VARCHAR(255) NOT NULL, `curhash` INT(11), `ovhash` INT(11), `currentversion` INT(11), `nversions` INT(11), `length` INT(11), `mtnsec` INT(11), `mtsec` INT(11), `tempname` VARCHAR(45), PRIMARY KEY(`filename`))";
        statement = connection->prepareStatement(createTableFileRec);
        statement->execute();
        delete statement;

        const char* createTableBlobTable = "CREATE TABLE IF NOT EXISTS `blobtable` (`tempname` VARCHAR(45) NOT NULL, `filedata` MEDIUMBLOB);";
        statement = connection->prepareStatement(createTableBlobTable);
        statement->execute();
        delete statement;
    }

    connection->setSchema(DB_SCHEMA);
    return connection;
}

bool FileArchiver::compressFile(std::string source, std::string destination) {
    std::ifstream sourceFile(source.c_str(), std::ifstream::in);
    std::ofstream compressedFile(destination.c_str(), std::ofstream::out);

    if (!sourceFile.good() || !compressedFile.is_open()) {
        return false;
    }

    boost::iostreams::filtering_streambuf<boost::iostreams::input> compressStream;
    compressStream.push(boost::iostreams::gzip_compressor());
    compressStream.push(sourceFile);
    boost::iostreams::copy(compressStream, compressedFile);
    sourceFile.close();
    compressedFile.close();

    return true;
}

bool FileArchiver::decompressFile(std::string source, std::string destination) {
    std::ifstream compressedFile(source.c_str(), std::ifstream::in);
    std::ofstream destinationFile(destination.c_str(), std::ofstream::out);

    if (!compressedFile.is_open() || !destinationFile.good()) {
        return false;
    }

    boost::iostreams::filtering_streambuf<boost::iostreams::input> decompressStream;
    decompressStream.push(boost::iostreams::gzip_decompressor());
    decompressStream.push(compressedFile);
    boost::iostreams::copy(decompressStream, destinationFile);
    compressedFile.close();
    destinationFile.close();

    return true;
}
