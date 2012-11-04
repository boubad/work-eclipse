
#include <gtest/gtest.h>
/////////////////////////////
#include <omp_arrange.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
//////////////////////////////
#include "DataFixture.h"
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
