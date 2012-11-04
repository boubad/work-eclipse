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

    CSVReaderTest():m_sfilename(TEST_SFILENAME),
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
TEST_F(CSVReaderTest,readStringTest)
{
    std::map<std::string,std::vector<boost::any> > oAr;
    bool bRet = statdata::CSVReader::read_csv_file(m_sfilename, oAr);
    ASSERT_TRUE(bRet);
    ASSERT_FALSE(oAr.empty());
    size_t nInds = 0;
    for (auto it = oAr.begin(); it != oAr.end(); ++it){
        const std::string &varname = (*it).first;
        ASSERT_FALSE(varname.empty());
        const std::vector<boost::any> &vv = (*it).second;
        ASSERT_FALSE(vv.empty());
        if (nInds == 0){
            nInds = vv.size();
        } else {
            ASSERT_EQ(nInds, vv.size());
        }
    }// it
}// readStringTest
TEST_F(CSVReaderTest,readWStringTest)
{
    std::map<std::wstring,std::vector<boost::any> > oAr;
    bool bRet = statdata::CSVReader::read_csv_file(m_wfilename, oAr);
    ASSERT_TRUE(bRet);
    ASSERT_FALSE(oAr.empty());
    size_t nInds = 0;
    for (auto it = oAr.begin(); it != oAr.end(); ++it){
        const std::wstring &varname = (*it).first;
        ASSERT_FALSE(varname.empty());
        const std::vector<boost::any> &vv = (*it).second;
        ASSERT_FALSE(vv.empty());
        if (nInds == 0){
            nInds = vv.size();
        } else {
            ASSERT_EQ(nInds, vv.size());
        }
    }// it
}// readStringTest
///////////////////////////////////////
