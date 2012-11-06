
#include <gtest/gtest.h>
/////////////////////////////
#include <treeitem.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
//////////////////////////////
#include "../DataFixture.h"
////////////////////////////////
using namespace statdata;
/////////////////////////////

class TreeItemTest : public ::testing::Test
{
public:
    typedef short IndexType;
    typedef float CriteriaType;
    //
    typedef statdata::TreeItem<CriteriaType, IndexType> TreeItemType;
    //
    typedef TreeItemType::IndexVectorType IndexVectorType;
    typedef TreeItemType::IndexArrayType IndexArrayType;
public:

    TreeItemTest()
    {
    }

    virtual ~TreeItemTest()
    {
    }
protected:

    static void SetUpTestCase()
    {
        DataFixture oFixture;
        size_t n = oFixture.rows();
        m_n = n;
        m_distances = oFixture.row_distances();
        m_names = oFixture.rownames();
        m_valdist.resize(n * n);
        m_odist.reset(new float[n * n]);
        float *p = m_odist.get();
        ASSERT_TRUE(p != nullptr);
        const std::vector<float> &vv = m_distances;
        for (size_t i = 0; i < n; ++i)
        {
            size_t k = i * n + i;
            m_valdist[k] = 0.0f;
            p[k] = 0.0f;
            for (size_t j = 0; j < i; ++j)
            {
                size_t k1 = i * n + j;
                float f = vv[k1];
                m_valdist[k1] = f;
                p[k1] = f;
                size_t k2 = j * n + i;
                m_valdist[k2] = f;
                p[k2] = f;
            }// j
        }// i
    }

    static void TearDownTestCase()
    {
        m_odist.reset();
        m_names.clear();
        m_distances.clear();
        m_n = 0;
    }
protected:
    static size_t m_n;
    static std::vector<float> m_distances;
    static std::vector<std::string> m_names;
    static std::valarray<float> m_valdist;
    static std::unique_ptr<float> m_odist;
};
size_t TreeItemTest::m_n;
std::vector<float> TreeItemTest::m_distances;
std::vector<std::string> TreeItemTest::m_names;
std::valarray<float> TreeItemTest::m_valdist;
std::unique_ptr<float> TreeItemTest::m_odist;
///////////////////////////////////////

TEST_F(TreeItemTest, clusterize_min_vector)
{
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmin;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_min_vector

TEST_F(TreeItemTest, clusterize_min_valarray)
{
    const std::valarray<float> &dist = m_valdist;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmin;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_min_valarray

TEST_F(TreeItemTest, clusterize_min_ptr)
{
    const float *dist = m_odist.get();
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmin;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_min_ptr
///////////////////////////////////

TEST_F(TreeItemTest, clusterize_mean_vector)
{
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmean;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_mean_vector

TEST_F(TreeItemTest, clusterize_mean_valarray)
{
    const std::valarray<float> &dist = m_valdist;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmean;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_mean_valarray

TEST_F(TreeItemTest, clusterize_mean_ptr)
{
    const float *dist = m_odist.get();
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmean;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_mean_ptr
/////////////////////////////////////////////////////

TEST_F(TreeItemTest, clusterize_max_vector)
{
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmax;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_max_vector

TEST_F(TreeItemTest, clusterize_max_valarray)
{
    const std::valarray<float> &dist = m_valdist;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmax;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_max_valarray

TEST_F(TreeItemTest, clusterize_max_ptr)
{
    const float *dist = m_odist.get();
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    statdata::LinkType type = statdata::linkmax;
    TreeItemType::clusterize(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} // clusterize_max_ptr
//////////////////////////////////////////////////

TEST_F(TreeItemTest, clusterize_leaves_vector)
{
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 5;
    statdata::LinkType type = statdata::linkmean;
    TreeItemType::clusterize_leaves(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} //clusterize_leaves_vector

TEST_F(TreeItemTest, clusterize_leaves_valarray)
{
    const std::valarray<float> &dist = m_valdist;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 5;
    statdata::LinkType type = statdata::linkmean;
    TreeItemType::clusterize_leaves(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} //clusterize_leaves_valarray

TEST_F(TreeItemTest, clusterize_leaves_ptr)
{
    const float *dist = m_odist.get();
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    TreeItemType oTree;
    const size_t nClasses = 5;
    statdata::LinkType type = statdata::linkmean;
    TreeItemType::clusterize_leaves(n, dist, oTree, nClasses, type);
    size_t ns = oTree.size();
    ASSERT_EQ(nClasses, ns);
    // oTree.writeTo(std::cout, names);
    // std::cout << std::endl;
} //clusterize_leaves_ptr
//////////////////////////////////////////////////

TEST_F(TreeItemTest, clusterize_data_vector)
{
    const std::vector<float> &dist = m_distances;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    IndexArrayType oAr;
    statdata::LinkType type = statdata::linkmean;
    const size_t nClasses = 5;
    statdata::ClusterizeData(n, dist, oAr, nClasses, type);
    const size_t nx = oAr.size();
    ASSERT_EQ(nClasses, nx);
    for (size_t i = 0; i < nx; ++i)
    {
        const IndexVectorType &v = oAr[i];
        size_t ny = v.size();
        ASSERT_TRUE(ny > 0);
    }// i
} //clusterize_data_vector

TEST_F(TreeItemTest, clusterize_data_valarray)
{
    const std::valarray<float> &dist = m_valdist;
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    IndexArrayType oAr;
    statdata::LinkType type = statdata::linkmean;
    const size_t nClasses = 5;
    statdata::ClusterizeData(n, dist, oAr, nClasses, type);
    const size_t nx = oAr.size();
    ASSERT_EQ(nClasses, nx);
    for (size_t i = 0; i < nx; ++i)
    {
        const IndexVectorType &v = oAr[i];
        size_t ny = v.size();
        ASSERT_TRUE(ny > 0);
    }// i
} //clusterize_data_valarray

TEST_F(TreeItemTest, clusterize_data_ptr)
{
    const float *dist = m_odist.get();
    const size_t n = m_n;
    //const std::vector<std::string> &names = m_names;
    //
    IndexArrayType oAr;
    statdata::LinkType type = statdata::linkmean;
    const size_t nClasses = 5;
    statdata::ClusterizeData(n, dist, oAr, nClasses, type);
    const size_t nx = oAr.size();
    ASSERT_EQ(nClasses, nx);
    for (size_t i = 0; i < nx; ++i)
    {
        const IndexVectorType &v = oAr[i];
        size_t ny = v.size();
        ASSERT_TRUE(ny > 0);
    }// i
} //clusterize_data_ptr
