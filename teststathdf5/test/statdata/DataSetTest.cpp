#include <gtest/gtest.h>
/////////////////////////////
#include <dataset.h>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <fstream>
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

protected:

    virtual void SetUp() {
    }

    virtual void TearDown() {

    }

    static void SetUpTestCase() {
        size_t nInds = m_st_nbindivs;
        m_oset.reset(new DataSet((size_t) 0, nInds));
        DataSet *pSet = m_oset.get();
        ASSERT_TRUE(pSet != nullptr);
        ASSERT_EQ(nInds, pSet->nb_indivs());
        ASSERT_EQ(3, pSet->nb_vars());
        m_stbooldata.resize(nInds);
        {
            for (size_t i = 0; i < nInds; ++i) {
                m_stbooldata[i] = ((::rand() % 2) == 0) ? true : false;
            }// i
            std::string sid("booldata");
            Variable *pVar = pSet->add_variable(sid, sid);
            ASSERT_TRUE(pVar != nullptr);
            pVar->set_type(statdata::typeBool);
            bool bRet = pSet->set_data(sid, m_stbooldata);
            ASSERT_TRUE(bRet);
        }
        {
            m_stshortdata.resize(nInds);
            for (size_t i = 0; i < nInds; ++i) {
                m_stshortdata[i] = (short) (::rand() % 100);
            }// i
            std::string sid("shortdata");
            Variable *pVar = pSet->add_variable(sid, sid);
            ASSERT_TRUE(pVar != nullptr);
            pVar->set_type(statdata::typeShort);
            bool bRet = pSet->set_data(sid, m_stshortdata);
            ASSERT_TRUE(bRet);
        }
        {
            m_stintdata.resize(nInds);
            for (size_t i = 0; i < nInds; ++i) {
                m_stintdata[i] = (int) (::rand() % 1000);
            }// i
            std::string sid("intdata");
            Variable *pVar = pSet->add_variable(sid, sid);
            ASSERT_TRUE(pVar != nullptr);
            pVar->set_type(statdata::typeInt);
            bool bRet = pSet->set_data(sid, m_stintdata);
            ASSERT_TRUE(bRet);
        }
        {
            m_stlongdata.resize(nInds);
            for (size_t i = 0; i < nInds; ++i) {
                m_stlongdata[i] = (long) (::rand() % 10000);
            }// i
            std::string sid("longdata");
            Variable *pVar = pSet->add_variable(sid, sid);
            ASSERT_TRUE(pVar != nullptr);
            pVar->set_type(statdata::typeLong);
            bool bRet = pSet->set_data(sid, m_stlongdata);
            ASSERT_TRUE(bRet);
        }
        {
            m_stfloatdata.resize(nInds);
            for (size_t i = 0; i < nInds; ++i) {
                m_stfloatdata[i] = (float) (::rand() % 20);
            }// i
            std::string sid("floatdata");
            Variable *pVar = pSet->add_variable(sid, sid);
            ASSERT_TRUE(pVar != nullptr);
            pVar->set_type(statdata::typeFloat);
            bool bRet = pSet->set_data(sid, m_stfloatdata);
            ASSERT_TRUE(bRet);
        }
        {
            m_stdoubledata.resize(nInds);
            for (size_t i = 0; i < nInds; ++i) {
                m_stdoubledata[i] = (double) (::rand() % 50);
            }// i
            std::string sid("doubledata");
            Variable *pVar = pSet->add_variable(sid, sid);
            ASSERT_TRUE(pVar != nullptr);
            pVar->set_type(statdata::typeDouble);
            bool bRet = pSet->set_data(sid, m_stdoubledata);
            ASSERT_TRUE(bRet);
        }
        {
            m_ststringdata.resize(nInds);
            for (size_t i = 0; i < nInds; ++i) {
                int ival = ::rand() % 5;
                std::stringstream os;
                os << "GR" << (ival + 1);
                m_ststringdata[i] = os.str();
            }// i
            std::string sid("stringdata");
            Variable *pVar = pSet->add_variable(sid, sid);
            ASSERT_TRUE(pVar != nullptr);
            pVar->set_type(statdata::typeString);
            bool bRet = pSet->set_data(sid, m_ststringdata);
            ASSERT_TRUE(bRet);
        }
        ASSERT_EQ(10, pSet->nb_vars());
    }//SetUpTestCase

    static void TearDownTestCase() {
        m_oset.reset();
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
    static std::unique_ptr<DataSet> m_oset;
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
std::unique_ptr<DataSet> DataSetTest::m_oset;
///////////////////////////////////////

TEST_F(DataSetTest, saveArchiveTest) {
    DataSet *pSet = m_oset.get();
    ASSERT_TRUE(pSet != nullptr);
    bool bRet = pSet->save_archive_file(m_starchivename);
    ASSERT_TRUE(bRet);
}//saveArchiveTest
TEST_F(DataSetTest, readArchiveTest) {
    std::string filename = m_starchivename;
    {
        std::ifstream in(filename.c_str());
        if (!in.is_open()) {
            DataSet *pSet = m_oset.get();
            ASSERT_TRUE(pSet != nullptr);
            bool bRet = pSet->save_archive_file(m_starchivename);
            ASSERT_TRUE(bRet);
        }
    }
    DataSet oSet;
    bool bRet = oSet.import_archive_file(filename);
    ASSERT_TRUE(bRet);
}//saveArchiveTest

TEST_F(DataSetTest, saveCSVTest) {
    DataSet *pSet = m_oset.get();
    ASSERT_TRUE(pSet != nullptr);
    bool bRet = pSet->save_csv_file(m_stfilename);
    ASSERT_TRUE(bRet);
}//saveArchiveTest
