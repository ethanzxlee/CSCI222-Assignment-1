/* 
 * File:   FileArchiver.cpp
 * Author: zhexian
 * 
 * Created on 1 September 2015, 3:22 PM
 */

#include "FileArchiver.h"

FileArchiver::FileArchiver() throw (sql::SQLException) {
    DB_HOSTNAME = "tcp://127.0.0.1:3306";
    DB_USERNAME = "root";
    DB_PASSWORD = "1qaz2wsxmko0nji9";
    DB_SCHEMA = "FileArchiver";

    sql::Connection* connection = connectDB(true);
    connection->close();
    delete connection;
}

bool FileArchiver::differs(const std::string& filePath) {
    bool differs = true;
    boost::uuids::random_generator generator;
    boost::uuids::uuid uniqueId = generator();
    std::string tempFilePath = "/tmp/" + boost::uuids::to_string(uniqueId);

    if (compressFile(filePath, tempFilePath)) {
        sql::Connection* connection = connectDB();

        const char* sql_selectLatestHash = "SELECT curhash FROM filerec WHERE filename = ?";
        sql::PreparedStatement* statement = connection->prepareStatement(sql_selectLatestHash);
        statement->setString(1, filePath);
        sql::ResultSet* result = statement->executeQuery();

        if (result->next()) {
            uint32_t dbHash = result->getInt(1);
            uint32_t localHash = calculateFileHash(tempFilePath);
            differs = dbHash == localHash;
        }

        connection->close();
        delete connection;
        delete statement;
        delete result;
        std::remove(tempFilePath.c_str());
    }

    return differs;
}

bool FileArchiver::exists(const std::string& filePath) {
    bool exists = false;
    sql::Connection* connection = connectDB();

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
        sql::Connection* connection = connectDB();
        fileRec newFileRec;
        newFileRec.createData(filePath, tempFilePath, comment, connection);
        newFileRec.saveToDatabase();
        connection->close();
        delete connection;
        std::remove(tempFilePath.c_str());
    };
}

void FileArchiver::update(const std::string& filePath, const std::string& comment) {
    boost::uuids::random_generator generator;
    boost::uuids::uuid uniqueId = generator();
    std::string retrievedFilePath = "/tmp/" + boost::uuids::to_string(uniqueId);
    sql::Connection* connection = connectDB();
    int currentVersion;

    const char* sql_getLatestVersionNum = "SELECT currentversion FROM `filerec` WHERE filename = ?";
    sql::PreparedStatement* statement = connection->prepareStatement(sql_getLatestVersionNum);
    statement->setString(1, filePath);
    sql::ResultSet* result = statement->executeQuery();
    if (result->next()) {
        currentVersion = result->getInt(1);
        retrieveFile(filePath, retrievedFilePath, currentVersion, connection);
    }
    delete statement;
    delete result;

    std::size_t newerLength = fileSize(filePath);
    long newerModifyTime = getFileModifyTime(filePath);
    uint32_t newerFileHash = calculateFileHash(filePath);
    versionRec newerVersion;
    newerVersion.createData(filePath, currentVersion + 1, newerLength, newerModifyTime, newerFileHash, comment);

    std::ifstream newerFile(filePath.c_str(), std::ifstream::binary);
    std::vector<uint32_t> newerFileBlockHashes = calculateFileBlockHashes(filePath);
    std::vector<uint32_t> retrievedFileBlockHashes = calculateFileBlockHashes(retrievedFilePath);

    int newerFileHashCount = newerFileBlockHashes.size();
    int retrievedFileHashCount = retrievedFileBlockHashes.size();

    for (int i = 0; i < newerFileHashCount; i++) {
        if ((i < retrievedFileHashCount && newerFileBlockHashes.at(i) != retrievedFileBlockHashes.at(i)) || i >= retrievedFileHashCount) {
            std::size_t blockLength = BLOCK_SIZE * (i + 1) > newerLength ? newerLength - BLOCK_SIZE * i : BLOCK_SIZE;

            char* blockBytes = new char[blockLength];
            newerFile.seekg(BLOCK_SIZE * i);
            newerFile.read(blockBytes, blockLength);

            // Compressing a block   
            QByteArray qBlockBytes(blockBytes, blockLength);
            QByteArray qBlockBytesCompressed = qCompress(qBlockBytes);
   
            Block block;
            block.blockNum = i;
            block.bytes = new char[qBlockBytesCompressed.size()];
            memcpy(block.bytes, qBlockBytesCompressed.data(), qBlockBytesCompressed.size());
            block.hash = newerFileBlockHashes.at(i);
            block.length = qBlockBytesCompressed.size();
            newerVersion.addBlock(block);
            
            delete blockBytes;
        }
    }
    newerVersion.saveToDatabase(connection);
    newerFile.close();
    std::remove(retrievedFilePath.c_str());
    connection->close();
    delete connection;
}

void FileArchiver::retrieveFile(const std::string& filePath, const std::string& destinationFilePath, const int versionNum) {
    sql::Connection* connection = connectDB();
    retrieveFile(filePath, destinationFilePath, versionNum, connection);
    connection->close();
    delete connection;
}

void FileArchiver::retrieveFile(const std::string& filePath, const std::string& destinationFilePath, const int versionNum, sql::Connection* connection) {
    const char* selectBlob = "SELECT filedata FROM filerec WHERE filename = ?";
    sql::PreparedStatement* statement = connection->prepareStatement(selectBlob);
    statement->setString(1, filePath);
    sql::ResultSet* result = statement->executeQuery();

    if (result->next()) {
        boost::uuids::random_generator generator;
        boost::uuids::uuid uniqueId = generator();
        std::string tempFileZipPath = "/tmp/" + boost::uuids::to_string(uniqueId);
        std::ofstream tempFileZip(tempFileZipPath.c_str(), std::ofstream::binary);

        if (tempFileZip.is_open()) {
            std::istream* blobStream = result->getBlob(1);
            tempFileZip << blobStream->rdbuf();
            tempFileZip.close();
            
            std::string tempFilePath = "/tmp/" + boost::uuids::to_string(generator());
            std::string modifiedFilePath;
            
            if (decompressFile(tempFileZipPath, tempFilePath)) {
                    std::remove(tempFileZipPath.c_str());
                fileRec existingFileRec;
                std::vector<versionRec> allVersions = existingFileRec.returnVector(filePath, versionNum, connection);

                for (std::vector<versionRec>::iterator version = allVersions.begin(); version != allVersions.end(); ++version) {
                    modifiedFilePath = "/tmp/" + boost::uuids::to_string(generator());   
                    std::size_t tempFileSize = fileSize(tempFilePath.c_str());
                    std::fstream tempFile(tempFilePath.c_str(), std::ios_base::binary | std::ios_base::in);
                    std::fstream modifiedFile(modifiedFilePath.c_str(), std::ios_base::binary | std::ios_base::out);
                    
                    std::vector<Block> allBlocks = version->getBlocks();
                    
                    // Original blocks
                    for (std::size_t i = 0; i < version->getLength() && i < tempFileSize; i += BLOCK_SIZE) {
                        int readSize = version->getLength() - i < BLOCK_SIZE ? version->getLength() - i : BLOCK_SIZE;
                        char* tempBlock = new char[readSize];
                        tempFile.read(tempBlock, readSize);
                        modifiedFile.write(tempBlock, readSize);
                        delete tempBlock;
                    }

                    // Changed blocks
                    for (std::vector<Block>::iterator block = allBlocks.begin(); block != allBlocks.end(); ++block) {
                        int blockNum = (*block).blockNum;

                        // Decompressing a block
                        QByteArray qBlockBytesCompressed((*block).bytes, (*block).length);
                        QByteArray qBlockBytes = qUncompress(qBlockBytesCompressed);
                        
                        modifiedFile.seekp(blockNum * BLOCK_SIZE);
                        modifiedFile.write(qBlockBytes.data(), qBlockBytes.size());
                    }
                    
                    tempFile.close();
                    modifiedFile.close();
                    std::remove(tempFilePath.c_str());
                    tempFilePath = modifiedFilePath;
                }
                
                std::fstream finalFile(modifiedFilePath.c_str(), std::ios_base::binary | std::ios_base::in);
                std::fstream destinationFile(destinationFilePath.c_str(), std::ios_base::binary | std::ios_base::out);
                
                destinationFile << finalFile.rdbuf();
                finalFile.close();
                destinationFile.close();
                std::remove(modifiedFilePath.c_str());
            } 
            else {
                std::remove(tempFileZipPath.c_str());
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

        const char* createTable_filerec = "CREATE TABLE IF NOT EXISTS `FileArchiver`.`filerec` (`filename` VARCHAR(255) NOT NULL, `curlhash` INT(24) UNSIGNED NOT NULL, `ovhash` INT(24) UNSIGNED NOT NULL, `currentversion` INT(11) NOT NULL, `nversion` INT(11) NOT NULL, `length` INT(11) NOT NULL, `mtsec` INT(11) NOT NULL, `filedata` MEDIUMBLOB NOT NULL, PRIMARY KEY (`filename`)) ENGINE = InnoDB;";
        statement = connection->prepareStatement(createTable_filerec);
        statement->execute();
        delete statement;

        const char* createTable_fileblkhashes = "CREATE TABLE IF NOT EXISTS `FileArchiver`.`fileblkhashes` (`idfileblkhashes` INT(24) NOT NULL AUTO_INCREMENT, `fileref` VARCHAR(255) NOT NULL, `blknum` VARCHAR(33) NOT NULL, `hashval` INT(24) UNSIGNED NOT NULL, PRIMARY KEY (`idfileblkhashes`), FOREIGN KEY (`fileref`) REFERENCES filerec(`filename`)) ENGINE = InnoDB;";
        statement = connection->prepareStatement(createTable_fileblkhashes);
        statement->execute();
        delete statement;

        const char* createTable_versionrec = "CREATE TABLE IF NOT EXISTS `FileArchiver`.`versionrec` (`idversionrec` INT(11) NOT NULL AUTO_INCREMENT, `fileref` VARCHAR(255) NOT NULL, `versionnum` INT(11) NOT NULL, `length` INT(11) NOT NULL, `mtsec` INT(11) NOT NULL, `ovhash` INT(24) UNSIGNED NOT NULL, `commenttxt` MEDIUMTEXT NULL, PRIMARY KEY (`idversionrec`), FOREIGN KEY (`fileref`) REFERENCES filerec(`filename`)) ENGINE = InnoDB;";
        statement = connection->prepareStatement(createTable_versionrec);
        statement->execute();
        delete statement;

        const char* createTable_blktable = "CREATE TABLE IF NOT EXISTS `FileArchiver`.`blktable` (`idblktable` INT(11) NOT NULL AUTO_INCREMENT, `version` INT(11) NOT NULL, `length` INT(11) NOT NULL, `blknum` INT(11) NOT NULL, `hash` INT(24) UNSIGNED NOT NULL, `data` MEDIUMBLOB NOT NULL, PRIMARY KEY (`idblktable`), FOREIGN KEY (`version`) REFERENCES versionrec(`idversionrec`)) ENGINE = InnoDB;";
        statement = connection->prepareStatement(createTable_blktable);
        statement->execute();
        delete statement;

    }

    connection->setSchema(DB_SCHEMA);
    return connection;
}

bool FileArchiver::compressFile(const std::string& source, const std::string& destination) {
    std::ifstream sourceFile(source.c_str(), std::ifstream::binary);
    std::ofstream compressedFile(destination.c_str(), std::ofstream::binary);

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
    std::ifstream compressedFile(source.c_str(), std::ifstream::binary);
    std::ofstream destinationFile(destination.c_str(), std::ofstream::binary);

    if (!compressedFile.is_open()) {
        return false;
    }

    if (!destinationFile.good()) {
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
