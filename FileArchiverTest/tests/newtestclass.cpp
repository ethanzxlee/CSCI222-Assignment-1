/*
 * File:   newtestclass.cpp
 * Author: zhexian
 *
 * Created on 25/09/2015, 2:36:49 PM
 */

#include "newtestclass.h"
#include "../../FileArchiver/FileArchiver.h"

CPPUNIT_TEST_SUITE_REGISTRATION(newtestclass);

newtestclass::newtestclass() {
}

newtestclass::~newtestclass() {
}

void newtestclass::setUp() {
}

void newtestclass::tearDown() {
}

void newtestclass::testMethod() {
    FileArchiver fa;
    
    CPPUNIT_ASSERT(true);
}

void newtestclass::testFailedMethod() {
    CPPUNIT_ASSERT(false);
}

