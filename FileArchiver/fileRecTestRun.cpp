/*
 * Just for quickly testing fileRec
 * I commented out main.cpp when using this
*/
#include "fileRec.h"
#include <string>
using namespace std;

int ma0in()
{
    fileRec::connectToDatabase();
    fileRec myRec;
    string filename = "/home/zhexian/image.jpg";
    myRec.createData(filename);
    myRec.setRefNumber(0);
//    myRec.saveToDatabase();
    
    
    fileRec* newRec = fileRec::getFile(filename);
    
    
    
    fileRec::closeDatabase();
    //cin.get(); 
            
    return 0;
}