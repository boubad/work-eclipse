/*
 * ArrangeTest.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: boubad
 */
#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <arrangedesc.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
//////////////////////////////
#include "../DataFixture.h"
/////////////////////////////

class ArrangeTest : public ::testing::Test {
public:

    ArrangeTest() {
    }

    virtual ~ArrangeTest() {
    }
protected:

    static void SetUpTestCase() {
        DataFixture oFixture;
        m_n = oFixture.rows();
        m_distances = oFixture.row_distances();
    }

    static void TearDownTestCase() {
        m_distances.clear();
        m_n = 0;
    }
protected:
    static size_t m_n;
    static std::vector<float> m_distances;
};
size_t ArrangeTest::m_n;
std::vector<float> ArrangeTest::m_distances;
///////////////////////////////////////
using namespace statdata;
//////////////////////////////////////

TEST_F(ArrangeTest, first_index_minpath_test) {
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    std::vector<short> oIndex;
    float crit = 0.0f;
    for (short iStart = 0; iStart < (short) n; ++iStart) {
        statdata::SingleMinPath(n, dist, iStart, oIndex, crit);
        //std::cout << std::endl << crit;
        //for (size_t i = 0; i < n; ++i) {
        //	std::cout << "\t" << oIndex[i];
        //}
    }
    //std::cout << std::endl;
} // first_index_minpath_test

TEST_F(ArrangeTest, serial_minpath_test) {
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    std::vector<short> oIndex;
    float crit = 0.0f;
    statdata::SerialMinPath(n, dist, oIndex, crit);
    //std::cout << std::endl << crit;
    //for (size_t i = 0; i < n; ++i) {
    //	std::cout << "\t" << oIndex[i];
    //}
    //std::cout << std::endl;
} // serial_minpath_test
#else
////////////////////////////////
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
///////////////////////////////////
#include "../DataFixture.h"
///////////////////////////////////////
#include <arrangedesc.h>
////////////////////////////////////

class ArrangeTest : public CppUnit::TestFixture {
public:
    ArrangeTest();
    virtual ~ArrangeTest();
public:
    CPPUNIT_TEST_SUITE(ArrangeTest);
    CPPUNIT_TEST(first_index_minpath_test);
    CPPUNIT_TEST(serial_minpath_test);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp(void);
    void tearDown(void);
protected:
    void first_index_minpath_test(void);
    void serial_minpath_test(void);
private:
    size_t m_n;
    std::vector<float> m_distances;
};
//////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION(ArrangeTest);
////////////////////////////////////////////
using namespace statdata;
//////////////////////////////////

ArrangeTest::ArrangeTest() : m_n(0) {
}

ArrangeTest::~ArrangeTest() {
}

void ArrangeTest::setUp(void) {
    DataFixture oFixture;
    size_t n = oFixture.rows();
    CPPUNIT_ASSERT(n > 2);
    this->m_n = n;
    this->m_distances = oFixture.row_distances();
    CPPUNIT_ASSERT(this->m_distances.size() >= (size_t) (n * n));
} // setUp

void ArrangeTest::tearDown(void) {
    this->m_n = 0;
    this->m_distances.clear();
} // tearDown

void ArrangeTest::first_index_minpath_test(void) {
    const std::vector<float> &dist = this->m_distances;
    const size_t n = this->m_n;
    std::vector<short> oIndex;
    float crit = 0.0f;
    for (short iStart = 0; iStart < (short) n; ++iStart) {
        statdata::SingleMinPath(n, dist, iStart, oIndex, crit);
        //std::cout << std::endl << crit;
        //for (size_t i = 0; i < n; ++i) {
        //	std::cout << "\t" << oIndex[i];
        //}
    }
    //std::cout << std::endl;
}//first_index_minpath_test

void ArrangeTest::serial_minpath_test(void) {
    const std::vector<float> &dist = this->m_distances;
    const size_t n = this->m_n;
    std::vector<short> oIndex;
    float crit = 0.0f;
    statdata::SerialMinPath(n, dist, oIndex, crit);
    //std::cout << std::endl << crit;
    //for (size_t i = 0; i < n; ++i) {
    //	std::cout << "\t" << oIndex[i];
    //}
    //std::cout << std::endl;
}// serial_minpath_test
#endif // NO_GTEST
