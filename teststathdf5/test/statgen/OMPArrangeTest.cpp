#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <omp_arrange.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
//////////////////////////////
#include "../DataFixture.h"
/////////////////////////////

class OMPArrangeTest : public ::testing::Test
{
public:

    OMPArrangeTest()
    {
    }

    virtual ~OMPArrangeTest()
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
size_t OMPArrangeTest::m_n;
std::vector<float> OMPArrangeTest::m_distances;
///////////////////////////////////////
using namespace statdata;
//////////////////////////////////////
TEST_F(OMPArrangeTest, MultiTest)
{
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    std::vector<short> oIndex;
    float crit = 0.0f;
    statdata::OMPMinPath(n, dist, oIndex, crit);
    /*
    std::cout << std::endl << crit;
    for (size_t i = 0; i < n; ++i) {
        std::cout << "\t" << oIndex[i];
    }
    std::cout << std::endl;
     * */
} // MultiTest
#else
////////////////////////////////
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
///////////////////////////////////
#include "../DataFixture.h"
///////////////////////////////////////
#include <omp_arrange.h>
////////////////////////////////////

class OMPArrangeTest : public CppUnit::TestFixture {
public:
    OMPArrangeTest();
    virtual ~OMPArrangeTest();
public:
    CPPUNIT_TEST_SUITE(OMPArrangeTest);
    CPPUNIT_TEST(minpath_test);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp(void);
    void tearDown(void);
protected:
    void minpath_test(void);
private:
    size_t m_n;
    std::vector<float> m_distances;
};
//////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION(OMPArrangeTest);
////////////////////////////////////////////
using namespace statdata;
//////////////////////////////////

OMPArrangeTest::OMPArrangeTest():m_n(0) {
}

OMPArrangeTest::~OMPArrangeTest() {
}

void OMPArrangeTest::setUp(void) {
    DataFixture oFixture;
    this->m_n = oFixture.rows();
    CPPUNIT_ASSERT(this->m_n > 2);
    this->m_distances = oFixture.row_distances();
    CPPUNIT_ASSERT(this->m_distances.size() >= (size_t)(this->m_n * this->m_n));
} // setUp

void OMPArrangeTest::tearDown(void) {
    this->m_n = 0;
    this->m_distances.clear();
} // tearDown

void OMPArrangeTest::minpath_test(void) {
   const std::vector<float> &dist = this->m_distances;
    const size_t n = this->m_n;
    std::vector<short> oIndex;
    float crit = 0.0f;
    statdata::OMPMinPath(n, dist, oIndex, crit);
    /*
    std::cout << std::endl << crit;
    for (size_t i = 0; i < n; ++i) {
        std::cout << "\t" << oIndex[i];
    }
    std::cout << std::endl;
     * */
}//minpath_test
#endif // NO_GTEST
