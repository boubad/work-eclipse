/*
 * ArrangeTest.cpp
 *
 *  Created on: Oct 30, 2012
 *      Author: boubad
 */
#include <gtest/gtest.h>
/////////////////////////////
#include <arrangedesc.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
//////////////////////////////
#include "DataFixture.h"
/////////////////////////////

class ArrangeTest : public ::testing::Test
{
public:

    ArrangeTest()
    {
    }

    virtual ~ArrangeTest()
    {
    }
protected:

    static void SetUpTestCase()
    {
        DataFixture oFixture;
        m_n = oFixture.rows();
        m_distances = oFixture.row_distances();
    }

    static void TearDownTestCase()
    {
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

TEST_F(ArrangeTest, firstIndexTest)
{
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    std::vector<short> oIndex;
    float crit = 0.0f;
    for (short iStart = 0; iStart < (short) n; ++iStart)
    {
        statdata::SingleMinPath(n, dist, iStart, oIndex, crit);
        //std::cout << std::endl << crit;
        //for (size_t i = 0; i < n; ++i) {
        //	std::cout << "\t" << oIndex[i];
        //}
    }
    //std::cout << std::endl;
} // firstIndexTest

TEST_F(ArrangeTest, SerialTest)
{
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
} // SerialTest
