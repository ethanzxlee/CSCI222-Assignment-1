/*
 * File:   newtestclass.h
 * Author: zhexian
 *
 * Created on 25/09/2015, 2:36:49 PM
 */

#ifndef NEWTESTCLASS_H
#define	NEWTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class newtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(newtestclass);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    newtestclass();
    virtual ~newtestclass();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* NEWTESTCLASS_H */

