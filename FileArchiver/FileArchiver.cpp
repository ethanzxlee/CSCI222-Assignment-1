/* 
 * File:   FileArchiver.cpp
 * Author: zhexian
 * 
 * Created on 1 September 2015, 3:22 PM
 */

#include "FileArchiver.h"
#include "helperFuncs.h"

FileArchiver::FileArchiver() throw (sql::SQLException) {
    sql::Connection* connection = connectDB(true);
    connection->close();
    delete connection;
}

FileArchiver::~FileArchiver() {
}

bool FileArchiver::differs(const std::string& filePath) {
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

bool FileArchiver::exists(const std::string& filePath) {
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

void FileArchiver::insertNew(const std::string& filePath, const std::string& comment) {
    boost::uuids::random_generator generator;
    boost::uuids::uuid uniqueId = generator();
    std::string tempFilePath = "/tmp/" + boost::uuids::to_string(uniqueId);

    if (compressFile(filePath, tempFilePath)) {        
        fileRec* newFileRec = new fileRec();
        newFileRec->createData(filePath, tempFilePath, comment);
        
        sql::Connection* connection = connectDB(false);
        newFileRec->save(connection);
        connection->close();

        delete newFileRec;        
        delete connection;
        std::remove(tempFilePath.c_str());
    };
}

void FileArchiver::update(const std::string& filePath, const std::string& comment) {
    std::string retrievedZipFilePath = "";
    std::string retrievedUnzipFilePath = "";
    
    sql::Connection* connection = connectDB(false);
    retrieveFile(filePath, retrievedZipFilePath, connection);
    
    decompressFile(retrievedZipFilePath, retrievedUnzipFilePath);
    std::remove(retrievedZipFilePath.c_str());
    
    // TODO create a version rec here
    // versionnum
    // length
    // mtsec
    // mtnsec
    // ovhash
    
    std::vector<Block> differentBlocks;
    std::vector<uint32_t> fileBlockHashes = calculateFileBlockHashes(filePath);
    std::vector<uint32_t> retrievedFileBlockHashes = calculateFileBlockHashes(retrievedUnzipFilePath);
    
    int fileHashCount = fileBlockHashes.size();
    int retrievedFileHashCount = retrievedFileBlockHashes.size();
    int minHashCount = fileHashCount > retrievedFileHashCount ? retrievedFileHashCount : fileHashCount;
    int maxHashCount = fileHashCount > retrievedFileHashCount ? fileHashCount : retrievedFileHashCount;

    for (int i = 0; i < maxHashCount; i++) {
        if (i < minHashCount) {
            if (fileBlockHashes.at(i) != retrievedFileBlockHashes.at(i)) {
                Block block;
                block.blockNum = i;
                block.hash = fileBlockHashes.at(i);
                block.length = BLOCK_SIZE; 
                
                // TODO compress block and set the data;


                differentBlocks.push_back(block);
            }
        }
        else {
            Block block;
                block.blockNum = i;
                block.hash = fileBlockHashes.at(i);
                block.length = BLOCK_SIZE; 


                differentBlocks.push_back(block);
        }
    }
    std::remove(retrievedUnzipFilePath.c_str());
    connection->close();
    delete connection;
}

void FileArchiver::retrieveFile(const std::string& filePath, const std::string& destinationFilePath, const int versionNum) {
    sql::Connection* connection = connectDB(true);
    retrieveFile(filePath, destinationFilePath, versionNum, connection);
    connection->close();
    delete connection;
}

void FileArchiver::retrieveFile(const std::string& filePath, const std::string& destinationFilePath, sql::Connection* connection) {
    const char* sql_getLatestVersionNum = "SELECT currentversion FROM `filerec` WHERE filename = ?";
    sql::PreparedStatement* statement = connection->prepareStatement(sql_getLatestVersionNum);
    statement->setString(1, filePath);
    sql::ResultSet* result = statement->executeQuery();
    if (result->next()) {
        retrieveFile(filePath, destinationFilePath, result->getInt(1), connection);
    }
    delete result;
    delete statement;
}

void FileArchiver::retrieveFile(const std::string& filePath, const std::string& destinationFilePath, const int versionNum, sql::Connection* connection) {   
    const char* selectBlob = "SELECT filedata, currentversion FROM filerec WHERE filename = ?";
    sql::PreparedStatement* statement = connection->prepareStatement();
    statement->setString(1, filePath);
    sql::ResultSet* result = statement->executeQuery();

    if (result->next()) {
        boost::uuids::random_generator generator;
        boost::uuids::uuid uniqueId = generator();
        std::string tempFilePath = "/tmp/" + boost::uuids::to_string(uniqueId);
        std::ofstream tempFile(destinationFilePath.c_str(), std::ofstream::ate);

        if (tempFile.is_open()) {
            std::istream* blobStream = result->getBlob(1);
            tempFile << blobStream->rdbuf();
            tempFile.close();
            
            if (compressFile(tempFile, destinationFilePath)) {
                std::remove(tempFilePath.c_str());
                std::ofstream destinationFile(destinationFilePath.c_str(), std::ofstream::ate);
                const std::vector<versionRec> allVersions = versionRec::get(connection, versionNum); // TODO not sure what the function would be called
                
                for (std::vector<versionRec>::const_iterator version = allVersions.begin(); version != allVersions.end(); ++version) {
                    const std::vector<Block> allBlocks = version->getBlocks(); // TODO not sure what the function would be called
                   
                    for (std::vector<Block>::const_iterator block = allBlocks.begin(); block != allBlocks.end(); ++block) {
                        int blockNum = *block->blockNum;
                        std::string zippedBlockPath = "/tmp/zip" + blockNum;
                        std::string unzippedBlockPath = "/tmp/unzip" + blockNum;
                        
                        std::ofstream zippedBlock(zippedBlockPath.c_str());
                        zippedBlock << *block->bytes;
                        zippedBlock.close();

                        if (decompressFile(zippedBlockPath.c_str(), unzippedBlockPath.c_str())) {
                            std::ifstream unzippedBlock(unzippedBlockPath.c_str());
                            if (unzippedBlock.is_open()) {
                                destinationFile.seekp(blockNum);
                                destinationFile << unzippedBlock.rdbuf();
                                unzippedBlock.close();
                            }
                            std::remove(unzippedBlockPath.c_str());
                        }
                        std::remove(zippedBlockPath.c_str());
                    }
                }
                destinationFile.close();
            }
            else {
                std::remove(tempFilePath.c_str());
            }
        }
    }
    delete statement;
    delete result;
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

bool FileArchiver::compressFile(const std::string& source, const std::string& destination) {
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

bool FileArchiver::decompressFile(const std::string& source, const std::string& destination) {
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
