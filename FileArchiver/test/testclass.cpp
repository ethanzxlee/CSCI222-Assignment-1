#include "testclass.h"
#include "FileArchiver.h"

//Connecting sql
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

#include <fstream>
#include <iostream>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(testclass);

testclass::testclass() {
}

testclass::~testclass() {
}

void testclass::testfiletransfer()
{
    FileArchiver first;
  
    
    sql::Driver* driver = get_driver_instance();
    sql::Connection* connection = driver->connect("tcp://127.0.0.1:3306", "root", "*******"); //Enter your password for ******
    sql::PreparedStatement* statement = NULL;
    connection->setSchema("FileArchiver");
    
   
    
    
    bool checkfileexist =  false;
    
    std::string filename = "test.txt";
    
    std::ofstream outfile (filename.c_str());
    
    for(int i = 0; i < 1000000; i++)
    {
        outfile << "abcdefgh " << std::endl;
    }
    
    outfile.close();
    
    checkfileexist = first.exists(filename);
    
    if(checkfileexist == false)
    {
        CPPUNIT_ASSERT(checkfileexist);
    }
}


