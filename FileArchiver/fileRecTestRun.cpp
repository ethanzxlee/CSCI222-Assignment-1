/*
 * Just for quickly testing fileRec and versionRec
 * I commented out main.cpp when using this
*/
#include "FileArchiver.h"
#include "fileRec.h"
#include "versionRec.h"
#include "helperFuncs.h"
#include <string>

#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/prepared_statement.h"

using namespace std;


static const char* dataBaseStr = "tcp://127.0.0.1:3306";
static const char* dataBaseUserName = "root";
static const char* dataBaseUserPassword = "mT+qLs$4vAyv6m9L";
static const char* dataBaseSchema = "FileArchiver";

int main()
{
    //Temp database connection here
//    bool invalid = true;
//    sql::Connection* dbcon = NULL;
//    sql::Driver* driver = NULL;
//    driver = get_driver_instance();
//    try {
//        dbcon = driver->connect(dataBaseStr, dataBaseUserName, dataBaseUserPassword);
//    } catch(sql::SQLException &e) {
//        e.what();
//        return 1;
//    } catch (...) {
//        std::cerr << "Error in connection to database" << std::endl;
//    }
//    dbcon->setSchema(dataBaseSchema);
//    std::cout << "Connected to database" << std::endl;
//    invalid = false;
    
    
    
//    fileRec myRec;
//    string filename = "/home/dllyd/Desktop/CSCI222/datafile1";
//    string filename1 = "/home/dllyd/netbeans-8.0.2/uninstall.sh";
//    string comment = "This is the comment";
//    
//    myRec.createData(filename1, filename1, comment, dbcon);
//    myRec.saveToDatabase();
//    
//    versionRec temp;
//    temp.createExisting(filename1, 0, dbcon);
//    Block block;
//    block.blockNum = 0;
//    block.bytes = "something";
//    block.hash = 1;
//    block.length = BLOCK_SIZE;
//    temp.addBlock(block);
//    temp.saveBlocks();
//    
//    
//    vector<versionRec> versions = myRec.returnVector(filename1, dbcon);
//    cout << versions.size() << endl;
//    cout << versions[0].getHash() << endl;
//    cout << versions[0].getBlocks().size() << endl;
    
    //FileArchiver fileArchiver;
    //fileArchiver.insertNew("/home/zhexian/Desktop/Link to Google Drive/data", "Comment for adding data");
    //fileArchiver.update("/home/zhexian/Desktop/Link to Google Drive/data", "update comment");
    //fileArchiver.retrieveFile("/home/zhexian/Desktop/Link to Google Drive/data", "/home/zhexian/Desktop/Link to Google Drive/finally", 1);
    
    //fileArchiver.insertNew("/home/zhexian/v1.jpg", "This is the first comment! :)");
    //fileArchiver.update("/home/zhexian/v1.jpg", "The image has been modified.");
    //fileArchiver.retrieveFile("/home/zhexian/v1.jpg", "/home/zhexian/IWantItBack", 0);
    
    
    return 0;
}