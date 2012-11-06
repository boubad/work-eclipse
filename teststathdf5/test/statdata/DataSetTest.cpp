#include <gtest/gtest.h>
/////////////////////////////
#include <dataset.h>
#include <sstream>
#include <memory>
/////////////////////////////
using namespace statdata;
//////////////////////////////////////

class DataSetTest : public ::testing::Test {
public:

    DataSetTest() {
    }

    virtual ~DataSetTest() {
    }
protected:
    std::unique_ptr<DataSet> m_oset;
protected:

    virtual void SetUp() {
        m_oset.reset(new DataSet((size_t)0, (size_t)0));
        DataSet *pSet = m_oset.get();
        ASSERT_TRUE(pSet != nullptr);
    }

    virtual void TearDown() {
        m_oset.reset();
    }

    static void SetUpTestCase() {
    }//SetUpTestCase

    static void TearDownTestCase() {
        m_stbooldata.clear();
        m_stshortdata.clear();
        m_stintdata.clear();
        m_stlongdata.clear();
        m_stfloatdata.clear();
        m_stdoubledata.clear();
        m_ststringdata.clear();
    }//TearDownTestCase
protected:
    static size_t m_st_nbindivs;
    static std::vector<bool> m_stbooldata;
    static std::vector<short> m_stshortdata;
    static std::vector<int> m_stintdata;
    static std::vector<long> m_stlongdata;
    static std::vector<float> m_stfloatdata;
    static std::vector<double> m_stdoubledata;
    static std::vector<std::string> m_ststringdata;
    static std::string m_starchivename;
    static std::string m_stfilename;
    static std::string m_stimportfilename;
};
size_t DataSetTest::m_st_nbindivs = 100;
std::vector<bool> DataSetTest::m_stbooldata;
std::vector<short> DataSetTest::m_stshortdata;
std::vector<int> DataSetTest::m_stintdata;
std::vector<long> DataSetTest::m_stlongdata;
std::vector<float> DataSetTest::m_stfloatdata;
std::vector<double> DataSetTest::m_stdoubledata;
std::vector<std::string> DataSetTest::m_ststringdata;
std::string DataSetTest::m_stfilename("testresults/my_dataset.txt");
std::string DataSetTest::m_starchivename("testresults/my_archive.txt");
std::string DataSetTest::m_stimportfilename("./data/Groupes_GTE1.tsv");
///////////////////////////////////////

