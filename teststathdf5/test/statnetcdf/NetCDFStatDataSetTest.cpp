#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <netcdfstatdataset.h>
#include <sstream>
////////////////////////////
#include "../DataFixture.h"

/////////////////////////////////
using namespace statdata;
/////////////////////////////

class NetCDFStatDataSetTest : public ::testing::Test {
public:

    NetCDFStatDataSetTest() {
    }

    virtual ~NetCDFStatDataSetTest() {
    }

    virtual void SetUp() {
        this->m_datamap.clear();
        bool bClassic = true;
        this->m_oset.reset(new NetCDFStatDataSetFile(m_ncfilename,
                statdata::mode_replace, bClassic));
        NetCDFStatDataSetFile *g = this->m_oset.get();
        ASSERT_TRUE(g != nullptr);
        bool bRet = g->is_open();
        ASSERT_TRUE(bRet);
        DataFixture *pData = &(this->m_ofixture);
        std::string varname(VAR_INDIVS_NAME);
        NetCDFVariable oVarNames;
        bRet = g->add_indiv_variable(varname, statdata::typeString, oVarNames);
        ASSERT_TRUE(bRet);
        bRet = g->set_indivsids_variable(varname);
        ASSERT_TRUE(bRet);
        bRet = g->set_indivsnames_variable(varname);
        ASSERT_TRUE(bRet);
        const size_t nRows = pData->rows();
        const size_t nCols = pData->cols();
        const std::vector<std::string> &rownames = pData->rownames();
        ASSERT_TRUE(nRows > 0);
        bRet = g->write_indiv_data(varname, rownames);
        ASSERT_TRUE(bRet);
        std::vector<boost::any> v0(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            v0[i] = rownames[i];
        }
        this->m_datamap[varname] = v0;
        const std::vector<std::string> &varnames = pData->colnames();
        const std::vector<float> &initialdata = pData->initial_data();
        for (size_t icol = 0; icol < nCols; ++icol) {
            std::string vname = varnames[icol];
            NetCDFVariable var;
            bRet = g->add_indiv_variable(vname, statdata::typeFloat, var);
            ASSERT_TRUE(bRet);
            std::vector<float> vv(nRows);
            std::vector<boost::any> va(nRows);
            for (size_t i = 0; i < nRows; ++i) {
                vv[i] = initialdata[i * nCols + icol];
                va[i] = initialdata[i * nCols + icol];
            }
            bRet = g->write_indiv_data(vname, vv);
            ASSERT_TRUE(bRet);
            this->m_datamap[vname] = va;
        } // icol
        bRet = g->is_open();
        ASSERT_TRUE(bRet);
    }// SetUp

    virtual void TearDown() {
        NetCDFStatDataSetFile *g = this->m_oset.get();
        if (g != nullptr) {
            if (g->is_open()) {
                bool bRet = g->close();
                ASSERT_TRUE(bRet);
            }
            this->m_oset.reset();
        }// g

    }

protected:
    std::unique_ptr<NetCDFStatDataSetFile> m_oset;
    DataFixture m_ofixture;
    std::map<std::string, std::vector<boost::any> > m_datamap;
    //
    static std::string m_ncfilename;
    static const char *DIM_INDIVS;
    static const char *VAR_INDIVS_NAME;

};
std::string NetCDFStatDataSetTest::m_ncfilename("./testresults/my_statdataset.nc");
const char *NetCDFStatDataSetTest::DIM_INDIVS = "indivs";
const char *NetCDFStatDataSetTest::VAR_INDIVS_NAME = "names";
///////////////////////////////////////

TEST_F(NetCDFStatDataSetTest, test_read_data) {
    NetCDFStatDataSetFile *g = this->m_oset.get();
    ASSERT_TRUE(g != nullptr);
    ASSERT_TRUE(g->is_open());
    const DataFixture *pData = &(this->m_ofixture);
    //
    size_t nLen = 0;
    bool bRet = g->get_indivs_size(nLen);
    ASSERT_TRUE(bRet);
    const size_t nRows = pData->rows();
    ASSERT_TRUE(nRows == nLen);
    std::string idsname;
    bRet = g->get_indivsids_variable(idsname);
    ASSERT_TRUE(bRet);
    std::string idsvarname(VAR_INDIVS_NAME);
    ASSERT_TRUE(idsname == idsvarname);
    NetCDFVariables vars;
    bRet = g->get_indivs_variables(vars);
    ASSERT_TRUE(bRet);
    const size_t m = vars.size();
    for (size_t i = 0; i < m; ++i) {
        const NetCDFVariable &v = vars[i];
        size_t nStart = 0;
        size_t nCount = 0;
        std::string vname = v.name;
        auto it = this->m_datamap.find(vname);
        ASSERT_TRUE(it != this->m_datamap.end());
        const std::vector<boost::any> &vv = (*it).second;
        ASSERT_TRUE(vv.size() == nRows);
        if (v.type == statdata::typeString) {
            std::vector<std::string> vals;
            bRet = g->read_indiv_data(v.name, vals, nStart, nCount);
            ASSERT_TRUE(bRet);
            ASSERT_TRUE(nStart == 0);
            ASSERT_TRUE(nCount == nRows);
            for (size_t i = 0; i < nRows; ++i) {
                std::string s1;
                ASSERT_TRUE(Value::get_value(vv[i], s1));
                std::string s2 = vals[i];
                ASSERT_TRUE(s1 == s2);
            }// i
        } else if (v.type == statdata::typeFloat) {
            std::vector<float> vals;
            bRet = g->read_indiv_data(v.name, vals, nStart, nCount);
            ASSERT_TRUE(bRet);
            ASSERT_TRUE(nStart == 0);
            ASSERT_TRUE(nCount == nRows);
            for (size_t i = 0; i < nRows; ++i) {
                float v1;
                ASSERT_TRUE(Value::get_value(vv[i], v1));
                float v2 = vals[i];
                ASSERT_FLOAT_EQ(v1, v2);
            }// i
        }
    } // i
}/* test_read_data */

TEST_F(NetCDFStatDataSetTest, test_get_data) {
    NetCDFStatDataSetFile *g = this->m_oset.get();
    ASSERT_TRUE(g != nullptr);
    ASSERT_TRUE(g->is_open());
    const DataFixture *pData = &(this->m_ofixture);
    //
    const std::vector<std::string> &rownames = pData->rownames();
    const size_t nRows = pData->rows();
    ASSERT_TRUE(nRows > 0);
    //
    std::map<size_t, boost::any> oIndexMap;
    bool bRet = g->get_indivsids(oIndexMap);
    ASSERT_TRUE(bRet);
    for (auto it = oIndexMap.begin(); it != oIndexMap.end(); ++it) {
        size_t iIndex = (*it).first;
        ASSERT_TRUE((iIndex >= 0) && (iIndex < nRows));
        std::string sid;
        const boost::any &vi = (*it).second;
        ASSERT_TRUE(Value::get_value(vi, sid));
        std::string sy = rownames[iIndex];
        ASSERT_TRUE(sid == sy);
        std::map<std::string, boost::any> xMap;
        bRet = g->get_indiv_data(vi, xMap);
        ASSERT_TRUE(bRet);
        for (auto jt = xMap.begin(); jt != xMap.end(); ++jt) {
            const std::string &ss = (*jt).first;
            auto kt = this->m_datamap.find(ss);
            ASSERT_TRUE(kt != this->m_datamap.end());
            const std::vector<boost::any> &v0 = (*kt).second;
            ASSERT_TRUE(iIndex < v0.size());
            const boost::any &x0 = v0[iIndex];
            const boost::any &vx = (*jt).second;
            statdata::DataType tt = Value::get_type(x0);
            ASSERT_TRUE(tt == Value::get_type(vx));
            if (tt == statdata::typeString) {
                std::string s1, s2;
                ASSERT_TRUE(Value::get_value(x0, s1));
                ASSERT_TRUE(Value::get_value(vx, s2));
                ASSERT_EQ(s1, s2);
            } else if (tt == statdata::typeFloat) {
                float s1, s2;
                ASSERT_TRUE(Value::get_value(x0, s1));
                ASSERT_TRUE(Value::get_value(vx, s2));
                ASSERT_FLOAT_EQ(s1, s2);
            }
        } // jt
    } // it
}
#else
#endif // NO_GTEST

