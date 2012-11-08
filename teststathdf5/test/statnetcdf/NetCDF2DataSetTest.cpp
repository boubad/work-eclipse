#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <dataset.h>
#include <netcdfstatdataset.h>
/////////////////////////
#include <sstream>
#include <memory>
#include <cstdlib>
#include <fstream>
/////////////////////////////
using namespace statdata;
//////////////////////////////////////

class NetCDF2DataSetTest : public ::testing::Test {
public:

    NetCDF2DataSetTest() {
    }

    virtual ~NetCDF2DataSetTest() {
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
            pVar->set_type(statdata::typeInt);
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
            m_stweightdata.resize(nInds);
            for (size_t i = 0; i < nInds; ++i) {
                m_stweightdata[i] = (float)1.0f;
            }// i
            std::string sid("weights");
            Variable *pVar = pSet->add_variable(sid, sid);
            ASSERT_TRUE(pVar != nullptr);
            pVar->set_type(statdata::typeFloat);
            pVar->set_weight_var(true);
            bool bRet = pSet->set_data(sid, m_stweightdata);
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
        {
             NetCDFStatDataSetFile oFile(m_ncfilename,*pSet,true);
             ASSERT_TRUE(oFile.is_open());
             bool bRet = oFile.close();
             ASSERT_TRUE(bRet);
        }
    }//SetUpTestCase

    static void TearDownTestCase() {
        m_oset.reset();
        m_stbooldata.clear();
        m_stshortdata.clear();
        m_stintdata.clear();
        m_stlongdata.clear();
        m_stfloatdata.clear();
        m_stweightdata.clear();
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
    static std::vector<float> m_stweightdata;
    static std::vector<double> m_stdoubledata;
    static std::vector<std::string> m_ststringdata;
    static std::string m_ncfilename;
    static std::unique_ptr<DataSet> m_oset;
};
size_t NetCDF2DataSetTest::m_st_nbindivs = 100;
std::vector<bool> NetCDF2DataSetTest::m_stbooldata;
std::vector<short> NetCDF2DataSetTest::m_stshortdata;
std::vector<int> NetCDF2DataSetTest::m_stintdata;
std::vector<long> NetCDF2DataSetTest::m_stlongdata;
std::vector<float> NetCDF2DataSetTest::m_stfloatdata;
std::vector<float> NetCDF2DataSetTest::m_stweightdata;
std::vector<double> NetCDF2DataSetTest::m_stdoubledata;
std::vector<std::string> NetCDF2DataSetTest::m_ststringdata;
std::string NetCDF2DataSetTest::m_ncfilename("./testresults/my_dataset_2.nc");
std::unique_ptr<DataSet> NetCDF2DataSetTest::m_oset;
///////////////////////////////////////

TEST_F(NetCDF2DataSetTest, readNetCDFTest) {
    NetCDFStatDataSetFile oFile(m_ncfilename);
    ASSERT_TRUE(oFile.is_open());
    DataSet oSet;
    bool bRet = oFile.read(oSet);
    ASSERT_TRUE(bRet);
}//saveArchiveTest

#else
////////////////////////////////
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
///////////////////////////////////
#include <dataset.h>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <fstream>
/////////////////////////////////
using namespace statdata;
/////////////////////////////////////

class NetCDF2DataSetTest : public CppUnit::TestFixture {
public:
    NetCDF2DataSetTest();
    virtual ~NetCDF2DataSetTest();
public:
    CPPUNIT_TEST_SUITE(NetCDF2DataSetTest);
    CPPUNIT_TEST(saveArchiveTest);
    CPPUNIT_TEST(readArchiveTest);
    CPPUNIT_TEST(saveCSVTest);
    CPPUNIT_TEST(readCSVTest);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp(void);
    void tearDown(void);
protected:
    void saveArchiveTest(void);
    void readArchiveTest(void);
    void saveCSVTest(void);
    void readCSVTest(void);
private:
    size_t m_st_nbindivs;
    std::vector<bool> m_stbooldata;
    std::vector<short> m_stshortdata;
    std::vector<int> m_stintdata;
    std::vector<long> m_stlongdata;
    std::vector<float> m_stfloatdata;
    std::vector<double> m_stdoubledata;
    std::vector<std::string> m_ststringdata;
    std::string m_stfilename;
    std::string m_starchivename;
    std::string m_stimportfilename;
    std::unique_ptr<DataSet> m_oset;
};
//////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION(NetCDF2DataSetTest);
////////////////////////////////////////////

NetCDF2DataSetTest::NetCDF2DataSetTest() : m_st_nbindivs(100),
m_stfilename("testresults/my_dataset.txt"),
m_starchivename("testresults/my_archive.txt"),
m_stimportfilename("./data/Groupes_GTE1.tsv") {
}

NetCDF2DataSetTest::~NetCDF2DataSetTest() {

}

void NetCDF2DataSetTest::setUp(void) {
    size_t nInds = this->m_st_nbindivs;
    CPPUNIT_ASSERT(nInds > 0);
    this->m_oset.reset(new DataSet((size_t) 0, nInds));
    DataSet *pSet = this->m_oset.get();
    CPPUNIT_ASSERT(pSet != nullptr);
    CPPUNIT_ASSERT(nInds == pSet->nb_indivs());
    CPPUNIT_ASSERT(3 == pSet->nb_vars());
    this->m_stbooldata.resize(nInds);
    CPPUNIT_ASSERT(this->m_stbooldata.size() == nInds);
    {
        for (size_t i = 0; i < nInds; ++i) {
            this->m_stbooldata[i] = ((::rand() % 2) == 0) ? true : false;
        }// i
        std::string sid("booldata");
        Variable *pVar = pSet->add_variable(sid, sid);
        CPPUNIT_ASSERT(pVar != nullptr);
        pVar->set_type(statdata::typeBool);
        bool bRet = pSet->set_data(sid, m_stbooldata);
        CPPUNIT_ASSERT(bRet);
    }
    {
        this->m_stshortdata.resize(nInds);
        CPPUNIT_ASSERT(this->m_stshortdata.size() == nInds);
        for (size_t i = 0; i < nInds; ++i) {
            this->m_stshortdata[i] = (short) (::rand() % 100);
        }// i
        std::string sid("shortdata");
        Variable *pVar = pSet->add_variable(sid, sid);
        CPPUNIT_ASSERT(pVar != nullptr);
        pVar->set_type(statdata::typeShort);
        bool bRet = pSet->set_data(sid, m_stshortdata);
        CPPUNIT_ASSERT(bRet);
    }
    {
        this->m_stintdata.resize(nInds);
        CPPUNIT_ASSERT(this->m_stintdata.size() == nInds);
        for (size_t i = 0; i < nInds; ++i) {
            this->m_stintdata[i] = (int) (::rand() % 1000);
        }// i
        std::string sid("intdata");
        Variable *pVar = pSet->add_variable(sid, sid);
        CPPUNIT_ASSERT(pVar != nullptr);
        pVar->set_type(statdata::typeInt);
        bool bRet = pSet->set_data(sid, m_stintdata);
        CPPUNIT_ASSERT(bRet);
    }
    {
        this->m_stlongdata.resize(nInds);
        CPPUNIT_ASSERT(this->m_stlongdata.size() == nInds);
        for (size_t i = 0; i < nInds; ++i) {
            this->m_stlongdata[i] = (long) (::rand() % 10000);
        }// i
        std::string sid("longdata");
        Variable *pVar = pSet->add_variable(sid, sid);
        CPPUNIT_ASSERT(pVar != nullptr);
        pVar->set_type(statdata::typeLong);
        bool bRet = pSet->set_data(sid, m_stlongdata);
        CPPUNIT_ASSERT(bRet);
    }
    {
        this->m_stfloatdata.resize(nInds);
        CPPUNIT_ASSERT(this->m_stfloatdata.size() == nInds);
        for (size_t i = 0; i < nInds; ++i) {
            this->m_stfloatdata[i] = (float) (::rand() % 20);
        }// i
        std::string sid("floatdata");
        Variable *pVar = pSet->add_variable(sid, sid);
        CPPUNIT_ASSERT(pVar != nullptr);
        pVar->set_type(statdata::typeFloat);
        bool bRet = pSet->set_data(sid, m_stfloatdata);
        CPPUNIT_ASSERT(bRet);
    }
    {
        this->m_stdoubledata.resize(nInds);
        CPPUNIT_ASSERT(this->m_stdoubledata.size() == nInds);
        for (size_t i = 0; i < nInds; ++i) {
            this->m_stdoubledata[i] = (double) (::rand() % 50);
        }// i
        std::string sid("doubledata");
        Variable *pVar = pSet->add_variable(sid, sid);
        CPPUNIT_ASSERT(pVar != nullptr);
        pVar->set_type(statdata::typeDouble);
        bool bRet = pSet->set_data(sid, m_stdoubledata);
        CPPUNIT_ASSERT(bRet);
    }
    {
        this->m_ststringdata.resize(nInds);
        CPPUNIT_ASSERT(this->m_ststringdata.size() == nInds);
        for (size_t i = 0; i < nInds; ++i) {
            int ival = ::rand() % 5;
            std::stringstream os;
            os << "GR" << (ival + 1);
            this->m_ststringdata[i] = os.str();
        }// i
        std::string sid("stringdata");
        Variable *pVar = pSet->add_variable(sid, sid);
        CPPUNIT_ASSERT(pVar != nullptr);
        pVar->set_type(statdata::typeString);
        bool bRet = pSet->set_data(sid, m_ststringdata);
        CPPUNIT_ASSERT(bRet);
    }
    CPPUNIT_ASSERT(10 == pSet->nb_vars());
}

void NetCDF2DataSetTest::tearDown(void) {
    this->m_oset.reset();
    this->m_stbooldata.clear();
    this->m_stshortdata.clear();
    this->m_stintdata.clear();
    this->m_stlongdata.clear();
    this->m_stfloatdata.clear();
    this->m_stdoubledata.clear();
    this->m_ststringdata.clear();
}

void NetCDF2DataSetTest::saveArchiveTest(void) {
    DataSet *pSet = this->m_oset.get();
    CPPUNIT_ASSERT(pSet != nullptr);
    bool bRet = pSet->save_archive_file(this->m_starchivename);
    CPPUNIT_ASSERT(bRet);
}

void NetCDF2DataSetTest::readArchiveTest(void) {
    std::string filename = this->m_starchivename;
    {
        std::ifstream in(filename.c_str());
        if (!in.is_open()) {
            DataSet *pSet = this->m_oset.get();
            CPPUNIT_ASSERT(pSet != nullptr);
            bool bRet = pSet->save_archive_file(filename);
            CPPUNIT_ASSERT(bRet);
        }
    }
    DataSet oSet;
    bool bRet = oSet.import_archive_file(filename);
    CPPUNIT_ASSERT(bRet);
}

void NetCDF2DataSetTest::saveCSVTest(void) {
    DataSet *pSet = this->m_oset.get();
    CPPUNIT_ASSERT(pSet != nullptr);
    bool bRet = pSet->save_csv_file(this->m_stfilename);
    CPPUNIT_ASSERT(bRet);
}

void NetCDF2DataSetTest::readCSVTest(void) {
    std::string filename = this->m_stimportfilename;
    std::ifstream in(filename.c_str());
    CPPUNIT_ASSERT(in.is_open());
    DataSet oSet;
    bool bRet = oSet.import_csv_file(filename);
    CPPUNIT_ASSERT(bRet);
}
#endif // NO_GTEST

