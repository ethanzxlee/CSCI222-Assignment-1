#ifndef TESTCLASS_H
#define	TESTCLASS_H

//By default
#include <cppunit/extensions/HelperMacros.h>

class testclass : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE(testclass);
	CPPUNIT_TEST(testfiletransfer); //checking for file upload to sql
	//CPPUNIT_TEST(testfile);
	//CPPUNIT_TEST(testOrdering);

	CPPUNIT_TEST_SUITE_END();

	public:
		testclass();
	    	virtual ~testclass();

	private:
	    	void testfiletransfer();
	    	//void testsetReference();
};

#endif	/* TESTCLASS_H */
