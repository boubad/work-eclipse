#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <csvreader.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <map>
//////////////////////////////
using namespace statdata;
//////////////////////////////////////

class CSVReaderTest : public ::testing::Test {
public:

    CSVReaderTest() : m_sfilename(TEST_SFILENAME),
    m_wfilename(TEST_WFILENAME) {
    }

    virtual ~CSVReaderTest() {
    }
protected:
    std::string m_sfilename;
    std::wstring m_wfilename;
    //
    static const char *TEST_SFILENAME;
    static const wchar_t *TEST_WFILENAME;
};
/////////////////////////////////////////////
const char *CSVReaderTest::TEST_SFILENAME = "./data/Groupes_GTE1.tsv";
const wchar_t *CSVReaderTest::TEST_WFILENAME = L"./data/Groupes_GTE1.tsv";
////////////////////////////////////////////

TEST_F(CSVReaderTest, test_read_string) {
    std::map<std::string, std::vector<boost::any> > oAr;
    bool bRet = statdata::CSVReader::read_csv_file(m_sfilename, oAr);
    ASSERT_TRUE(bRet);
    ASSERT_FALSE(oAr.empty());
    size_t nInds = 0;
    for (auto it = oAr.begin(); it != oAr.end(); ++it) {
        const std::string &varname = (*it).first;
        ASSERT_FALSE(varname.empty());
        const std::vector<boost::any> &vv = (*it).second;
        ASSERT_FALSE(vv.empty());
        if (nInds == 0) {
            nInds = vv.size();
        } else {
            ASSERT_EQ(nInds, vv.size());
        }
    }// it
}// test_read_string

TEST_F(CSVReaderTest, test_read_wstring) {
    std::map<std::wstring, std::vector<boost::any> > oAr;
    bool bRet = statdata::CSVReader::read_csv_file(m_wfilename, oAr);
    ASSERT_TRUE(bRet);
    ASSERT_FALSE(oAr.empty());
    size_t nInds = 0;
    for (auto it = oAr.begin(); it != oAr.end(); ++it) {
        const std::wstring &varname = (*it).first;
        ASSERT_FALSE(varname.empty());
        const std::vector<boost::any> &vv = (*it).second;
        ASSERT_FALSE(vv.empty());
        if (nInds == 0) {
            nInds = vv.size();
        } else {
            ASSERT_EQ(nInds, vv.size());
        }
    }// it
}// test_read_wstring
///////////////////////////////////////
#else
////////////////////////////////
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
///////////////////////////////////
#include <csvreader.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <map>
//////////////////////////////
using namespace statdata;
////////////////////////////////////

class CSVReaderTest : public CppUnit::TestFixture {
public:
    CSVReaderTest();
    virtual ~CSVReaderTest();
public:
    CPPUNIT_TEST_SUITE(CSVReaderTest);
    CPPUNIT_TEST(test_read_string);
    CPPUNIT_TEST(test_read_wstring);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp(void);
    void tearDown(void);
protected:
    void test_read_string(void);
    void test_read_wstring(void);
private:
    std::string m_sfilename;
    std::wstring m_wfilename;
    //
    static const char *TEST_SFILENAME;
    static const wchar_t *TEST_WFILENAME;
};
/////////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION(CSVReaderTest);
/////////////////////////////////////////////
const char *CSVReaderTest::TEST_SFILENAME = "./data/Groupes_GTE1.tsv";
const wchar_t *CSVReaderTest::TEST_WFILENAME = L"./data/Groupes_GTE1.tsv";
////////////////////////////////////////////

CSVReaderTest::CSVReaderTest() :
m_sfilename(TEST_SFILENAME), m_wfilename(TEST_WFILENAME) {
}

CSVReaderTest::~CSVReaderTest() {
}

void CSVReaderTest::setUp(void) {
} // setUp

void CSVReaderTest::tearDown(void) {
} // tearDown

void CSVReaderTest::test_read_string(void) {
    std::map<std::string, std::vector<boost::any> > oAr;
    bool bRet = statdata::CSVReader::read_csv_file(m_sfilename, oAr);
    CPPUNIT_ASSERT(bRet);
    CPPUNIT_ASSERT(!oAr.empty());
    size_t nInds = 0;
    for (auto it = oAr.begin(); it != oAr.end(); ++it) {
        const std::string &varname = (*it).first;
        CPPUNIT_ASSERT(!varname.empty());
        const std::vector<boost::any> &vv = (*it).second;
        CPPUNIT_ASSERT(!vv.empty());
        if (nInds == 0) {
            nInds = vv.size();
        } else {
            CPPUNIT_ASSERT(nInds == vv.size());
        }
    }// it
} // test_read_string

void CSVReaderTest::test_read_wstring(void) {
     std::map<std::wstring, std::vector<boost::any> > oAr;
    bool bRet = statdata::CSVReader::read_csv_file(m_wfilename, oAr);
    CPPUNIT_ASSERT(bRet);
    CPPUNIT_ASSERT(!oAr.empty());
    size_t nInds = 0;
    for (auto it = oAr.begin(); it != oAr.end(); ++it) {
        const std::wstring &varname = (*it).first;
        CPPUNIT_ASSERT(!varname.empty());
        const std::vector<boost::any> &vv = (*it).second;
        CPPUNIT_ASSERT(!vv.empty());
        if (nInds == 0) {
            nInds = vv.size();
        } else {
            CPPUNIT_ASSERT(nInds == vv.size());
        }
    }// it
} // test_read_wstring
#endif // NO_GTEST
