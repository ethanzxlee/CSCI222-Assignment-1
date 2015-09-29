/*
 * File:   newtestclass.h
 * Author: giritharan
 *
 * Created on 23/09/2015, 9:16:27 PM
 */

#ifndef NEWTESTCLASS_H
#define	NEWTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class newtestclass : public CPPUNIT_NS::TestFixture {
    
    CPPUNIT_TEST_SUITE(newtestclass);
    CPPUNIT_TEST(testfiletransfer); //checking for file upload to sql
    CPPUNIT_TEST(testfileretrieve);
    CPPUNIT_TEST(testfileupdate);
    CPPUNIT_TEST(testsetreference);
    //CPPUNIT_TEST(testcompression);
    //CPPUNIT_TEST(testOrdering);


    CPPUNIT_TEST_SUITE_END();

    public:
	newtestclass();
        virtual ~newtestclass();

    private:
	void testfiletransfer(); //Test for compression and file transfer to SQL
	void testfileretrieve(); //Test for retrieving that version of file
        void testfileupdate(); //Updating the same filepath with modification
        void testsetreference(); //Set referencing for that filepath
        
        //void testcompression(); // Ignore Private. Done by testfiletransfer
        //void testMethod(); // Ignore
        //void testFailedMethod(); // Ignore
};

#endif	/* NEWTESTCLASS_H */

