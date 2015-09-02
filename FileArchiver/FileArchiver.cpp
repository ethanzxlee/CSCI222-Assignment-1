/* 
 * File:   FileArchiver.cpp
 * Author: zhexian
 * 
 * Created on 1 September 2015, 3:22 PM
 */


#include <cppconn/exception.h>

#include "FileArchiver.h"

FileArchiver::FileArchiver() {
    sql::Connection* connection = connectDB(true);
    delete connection;
}

FileArchiver::FileArchiver(const FileArchiver& orig) {
}

FileArchiver::~FileArchiver() {
}

bool FileArchiver::differs(std::string filename) {
    bool differs = false;
    if (exists(filename)) {
        std::string compressFilename = filename.append(".tmp");
        if (compressFile(filename, compressFilename)) {
            fileRec* localFile = new fileRec();
            localFile->createData(compressFilename);
            fileRec* remoteFile = fileRec::getFile(filename);
            differs = remoteFile->getCurrentHash() == localFile->getCurrentHash();
            delete remoteFile;
            delete localFile;
            std::remove(compressFilename.c_str());
        };
    }
    return differs;
}

bool FileArchiver::exists(std::string filename) {
    sql::Connection* connection = connectDB(false);
    bool exists = fileRec::exists(filename, connection, true);
    delete connection;
    return exists;
}

void FileArchiver::insertNew(std::string filename, std::string comment) {
    sql::Connection* connection = connectDB(false);
    if (!fileRec::exists(filename, connection, false)) {
        boost::uuids::random_generator generator;
        boost::uuids::uuid uniqueId = generator();
        const char* uniqueIdCount = "SELECT count(*) FROM `blobtable` WHERE tempname = ?";
        sql::PreparedStatement* statement = connection->prepareStatement(uniqueIdCount);
        statement->setString(1, boost::uuids::to_string(uniqueId));
        sql::ResultSet* result = statement->executeQuery();

        while (result->next() && result->getInt(1) > 0) {
            delete result;
            delete statement;
            uniqueId = generator();
            statement = connection->prepareStatement(uniqueIdCount);
            statement->setString(1, boost::uuids::to_string(uniqueId));
            result = statement->executeQuery();
        }

        if (statement != NULL) {
            delete statement;
        }
        if (result != NULL) {
            delete result;
        }

        std::string tempName = "/tmp/" + boost::uuids::to_string(uniqueId);

        if (compressFile(filename, tempName)) {
            fileRec compressFileRec;
            compressFileRec.createData(tempName);
            compressFileRec.setRefNumber(0);
            compressFileRec.setFileName(filename);
            compressFileRec.saveToDatabase(connection, false);

            std::ifstream tempFile(tempName.c_str());
            const char* insertBlob = "INSERT INTO `blobtable` VALUES (?, ?)";

            statement = connection->prepareStatement(insertBlob);
            statement->setString(1, boost::uuids::to_string(uniqueId));
            statement->setBlob(2, &tempFile);
            statement->execute();

            delete statement;
            tempFile.close();
            std::remove(tempName.c_str());
        };
        connection->close();
        delete connection;
    }
}

void FileArchiver::update(std::string filename, std::string comment) {
    if (exists(filename)) {

    }
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
