/*
 * GenroutsTest.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: boubad
 */
#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <genrouts.h>
#include <csvreader.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <map>
#include <fstream>
#include <memory>
#include <algorithm>
//////////////////////////////
using namespace statdata;
//////////////////////////////////////

class GenroutsTest : public ::testing::Test {
public:

    GenroutsTest() {
    }

    virtual ~GenroutsTest() {
    }
protected:

    static void SetUpTestCase() {
        std::map<std::string, std::vector<boost::any> > oMap;
        bool bRet = CSVReader::read_csv_file(m_filename, oMap);
        ASSERT_TRUE(bRet);
        size_t nRows = 0;
        m_varnames.clear();
        std::vector<std::vector<float> > oArray;
        float fInit = (float) 0.0f;
        for (auto it = oMap.begin(); it != oMap.end(); ++it) {
            std::string varname = (*it).first;
            const std::vector<boost::any> &vv = (*it).second;
            size_t n = vv.size();
            boost::any vfirst;
            for (size_t i = 0; i < n; ++i) {
                const boost::any &v = vv[i];
                if (!v.empty()) {
                    vfirst = v;
                    break;
                }
            } // i
            if (vfirst.empty()) {
                continue;
            }
            if (nRows < 1) {
                nRows = n;
            }
            if ((nRows < 1) || (nRows > vv.size())) {
                continue;
            }
            const std::type_info &tt = vfirst.type();
            if (tt == typeid(std::string)){
                m_rownames.resize(nRows);
                for (size_t i = 0; i < nRows; ++i){
                    std::string s;
                    Value::get_value(vv[i],s);
                    m_rownames[i] = s;
                }// i
                continue;
            }// string
            if ((tt != typeid (float)) && (tt != typeid (double))
                    && (tt != typeid (long)) && (tt != typeid (int))
                    && (tt != typeid (unsigned char))) {
                continue;
            }
            std::vector<float> vz(nRows, fInit);
            for (size_t i = 0; i < nRows; ++i) {
                const boost::any &v = vv[i];
                if (!v.empty()) {
                    float x = (float) 0.0;
                    if (Value::get_value(v, x)) {
                        vz[i] = x;
                    }
                }
            } // i
            oArray.push_back(vz);
            m_varnames.push_back(varname);
        } // it
        ASSERT_TRUE(nRows > 3);
        size_t nCols = oArray.size();
        ASSERT_TRUE(nCols > 0);
        ASSERT_TRUE(nRows > nCols);
        size_t nTotal = (size_t) (nRows * nCols);
        m_nrows = nRows;
        m_ncols = nCols;
        m_vecdata.resize(nTotal);
        ASSERT_TRUE(m_vecdata.size() == nTotal);
        m_valdata.resize(nTotal);
        ASSERT_TRUE(m_valdata.size() == nTotal);
        m_odata.reset(new float[nTotal]);
        float *pData = m_odata.get();
        ASSERT_TRUE(pData != nullptr);
        for (size_t ivar = 0; ivar < nCols; ++ivar) {
            const std::vector<float> &vv = oArray[ivar];
            ASSERT_TRUE(vv.size() == nRows);
            for (size_t irow = 0; irow < nRows; ++irow) {
                size_t k = (size_t) (irow * nCols + ivar);
                float fval = vv[irow];
                m_vecdata[k] = fval;
                m_valdata[k] = fval;
                pData[k] = fval;
            } // irow
        } // ivar
        ASSERT_EQ(m_nrows, m_rownames.size());
        {
            std::ofstream out(m_outfilename);
            if (out.is_open()) {
                out.close();
            }
        }
    } //SetUpTestCase

    static void TearDownTestCase() {
        m_vecdata.clear();
        m_varnames.clear();
        m_valdata.resize(1);
        m_odata.reset();
    } //TearDownTestCase

    virtual void SetUp() {
        bool bExists = false;
        {
            std::ifstream in(m_outfilename);
            bExists = in.is_open();
            in.close();
        }
        if (bExists) {
            m_out.reset(
                    new std::ofstream(m_outfilename,
                    std::ios::out | std::ios::app));
        } else {
            m_out.reset(new std::ofstream(m_outfilename));
        }
        std::ofstream *p = m_out.get();
        ASSERT_TRUE(p != nullptr);
        ASSERT_TRUE(p->is_open());
    } // SetUp

    virtual void TearDown() {
        std::ofstream *p = m_out.get();
        if (p != nullptr) {
            if (p->is_open()) {
                p->close();
            }
            m_out.reset();
        }
    } // TearDown
protected:
    //
    std::unique_ptr<std::ofstream> m_out;
    //
    static size_t m_nrows;
    static size_t m_ncols;
    static std::vector<float> m_vecdata;
    static std::valarray<float> m_valdata;
    static std::vector<std::string> m_varnames;
    static std::string m_filename;
    static std::string m_outfilename;
    static std::unique_ptr<float> m_odata;
    static std::vector<std::string> m_rownames;
};
/////////////////////////////////////////////
size_t GenroutsTest::m_nrows = 0;
size_t GenroutsTest::m_ncols = 0;
std::vector<float> GenroutsTest::m_vecdata;
std::valarray<float> GenroutsTest::m_valdata;
std::vector<std::string> GenroutsTest::m_varnames;
std::vector<std::string> GenroutsTest::m_rownames;
std::string GenroutsTest::m_filename("./data/testdata.tsv");
std::string GenroutsTest::m_outfilename("./testresults/genrouts.txt");
std::unique_ptr<float> GenroutsTest::m_odata;
//////////////////////////////////////////////
TEST_F(GenroutsTest, test_writeindexedarray_valarray_string) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    const std::vector<std::string> & oVarNames = m_varnames;
    const std::vector<std::string> & oIndNames = m_rownames;
    //
    std::valarray<short> oColIndexes(nCols),oRowIndexes(nRows);
    std::vector<short> var1(nCols), var2(nRows);
    for (size_t i = 0; i < nCols; ++i){
        var1[i] = (short)i;
    }// i
    std::random_shuffle(var1.begin(),var1.end());
    for (size_t i = 0; i < nCols; ++i){
        oColIndexes[i] = var1[i];
    }// i
    for (size_t i = 0; i < nRows; ++i){
        var2[i] = (short)i;
    }// i
    std::random_shuffle(var2.begin(),var2.end());
    for (size_t i = 0; i < nRows; ++i){
        oRowIndexes[i] = var2[i];
    }// i
    //
    std::stringstream stream;
    statdata::WriteIndexedArray(stream, nRows, nCols,
    alldata,
    oIndNames,
    oVarNames,
    oRowIndexes,
    oColIndexes);
    std::string ss = stream.str();
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    (*pout) << std::endl <<  ss << std::endl;
} // test_writeindexedarray_valarray_string
TEST_F(GenroutsTest, test_writeindexedarray_vector_string) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    const std::vector<std::string> & oVarNames = m_varnames;
    const std::vector<std::string> & oIndNames = m_rownames;
    //
    std::vector<short> oColIndexes(nCols),oRowIndexes(nRows);
    for (size_t i = 0; i < nCols; ++i){
        oColIndexes[i] = (short)i;
    }// i
    std::random_shuffle(oColIndexes.begin(),oColIndexes.end());
    for (size_t i = 0; i < nRows; ++i){
        oRowIndexes[i] = (short)i;
    }// i
    std::random_shuffle(oRowIndexes.begin(),oRowIndexes.end());
    //
    std::stringstream stream;
    statdata::WriteIndexedArray(stream, nRows, nCols,
    alldata,
    oIndNames,
    oVarNames,
    oRowIndexes,
    oColIndexes);
    std::string ss = stream.str();
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    (*pout) << std::endl <<  ss << std::endl;
} // test_writeindexedarray_vectot_string
//////////////////////////////////////////////////
TEST_F(GenroutsTest, test_writeindexedarray_valarray_wstring) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    const std::vector<std::string> & oVars = m_varnames;
    const std::vector<std::string> & oInds = m_rownames;
    //
    std::vector<std::wstring> oVarNames(nCols), oIndNames(nRows);
    std::valarray<short> oColIndexes(nCols),oRowIndexes(nRows);
    std::vector<short> var1(nCols), var2(nRows);
    for (size_t i = 0; i < nCols; ++i){
        var1[i] = (short)i;
        std::wstring wr;
        const std::string &s = oVars[i];
        if (!s.empty()){
            std::wstring s0(s.length(),L' ');
            std::copy(s.begin(),s.end(),s0.begin());
            wr = s0;
        }
        oVarNames[i] = wr;
    }// i
    std::random_shuffle(var1.begin(),var1.end());
    for (size_t i = 0; i < nCols; ++i){
        oColIndexes[i] = var1[i];
    }// i
    for (size_t i = 0; i < nRows; ++i){
        var2[i] = (short)i;
        std::wstring wr;
        const std::string &s = oInds[i];
        if (!s.empty()){
            std::wstring s0(s.length(),L' ');
            std::copy(s.begin(),s.end(),s0.begin());
            wr = s0;
        }
        oIndNames[i] = wr;
    }// i
    std::random_shuffle(var2.begin(),var2.end());
    for (size_t i = 0; i < nRows; ++i){
        oRowIndexes[i] = var2[i];
    }// i
    //
    std::wstringstream stream;
    statdata::WriteIndexedArray(stream, nRows, nCols,
    alldata,
    oIndNames,
    oVarNames,
    oRowIndexes,
    oColIndexes);
    std::wstring ss = stream.str();
    std::string sx(ss.length(),' ');
    std::copy(ss.begin(), ss.end(), sx.begin());
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    (*pout) << std::endl <<  sx << std::endl;
} // test_writeindexedarray_valarray_wstring
TEST_F(GenroutsTest, test_writeindexedarray_vector_wstring) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    const std::vector<std::string> & oVars = m_varnames;
    const std::vector<std::string> & oInds = m_rownames;
    //
    std::vector<std::wstring> oVarNames(nCols), oIndNames(nRows);
    std::vector<short> oColIndexes(nCols),oRowIndexes(nRows);
    for (size_t i = 0; i < nCols; ++i){
        oColIndexes[i] = (short)i;
        std::wstring wr;
        const std::string &s = oVars[i];
        if (!s.empty()){
            std::wstring s0(s.length(),L' ');
            std::copy(s.begin(),s.end(),s0.begin());
            wr = s0;
        }
        oVarNames[i] = wr;
    }// i
    std::random_shuffle(oColIndexes.begin(),oColIndexes.end());
    for (size_t i = 0; i < nRows; ++i){
        oRowIndexes[i] = (short)i;
        std::wstring wr;
        const std::string &s = oInds[i];
        if (!s.empty()){
            std::wstring s0(s.length(),L' ');
            std::copy(s.begin(),s.end(),s0.begin());
            wr = s0;
        }
        oIndNames[i] = wr;
    }// i
    std::random_shuffle(oRowIndexes.begin(),oRowIndexes.end());
    //
    std::wstringstream stream;
    statdata::WriteIndexedArray(stream, nRows, nCols,
    alldata,
    oIndNames,
    oVarNames,
    oRowIndexes,
    oColIndexes);
    std::wstring ss = stream.str();
    std::string sx(ss.length(),' ');
    std::copy(ss.begin(), ss.end(), sx.begin());
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    (*pout) << std::endl <<  sx << std::endl;
} // test_writeindexedarray_vector_wstring
///////////////////////////////////////////////
TEST_F(GenroutsTest, test_recode_ptr) {
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    size_t n = (size_t)(m_nrows * m_ncols);
    ASSERT_TRUE(n > 0);
    short rMin = (short)150;
    short rMax = (short)500;
    //
    std::unique_ptr<short> oDest(new short[n]);
    //
    short *pDest = oDest.get();
    ASSERT_TRUE(pDest != nullptr);
    //
    bool bRet = statdata::Recode(n,alldata, pDest, rMax,rMin);
    ASSERT_TRUE(bRet);
    //
    for (size_t i = 0; i < n; ++i){
        short val = pDest[i];
        ASSERT_TRUE(val >= rMin);
        ASSERT_TRUE(val <= rMax);
    }// i
} // test_recode_ptr
TEST_F(GenroutsTest, test_recode_valarray) {
    const std::valarray<float> &alldata = m_valdata;
    size_t n = (size_t)(m_nrows * m_ncols);
    ASSERT_TRUE(n > 0);
    short rMin = (short)150;
    short rMax = (short)500;
    //
    std::valarray<short> oDest;
    //
    bool bRet = statdata::Recode(alldata, oDest, rMax,rMin);
    ASSERT_TRUE(bRet);
    ASSERT_EQ(n,oDest.size());
    //
    for (size_t i = 0; i < n; ++i){
        short val = oDest[i];
        ASSERT_TRUE(val >= rMin);
        ASSERT_TRUE(val <= rMax);
    }// i
} // test_recode_valarray
TEST_F(GenroutsTest, test_recode_vector) {
    const std::vector<float> &alldata = m_vecdata;
    size_t n = (size_t)(m_nrows * m_ncols);
    ASSERT_TRUE(n > 0);
    short rMin = (short)150;
    short rMax = (short)500;
    //
    std::vector<short> oDest;
    //
    bool bRet = statdata::Recode(alldata, oDest, rMax,rMin);
    ASSERT_TRUE(bRet);
    ASSERT_EQ(n,oDest.size());
    //
    for (size_t i = 0; i < n; ++i){
        short val = oDest[i];
        ASSERT_TRUE(val >= rMin);
        ASSERT_TRUE(val <= rMax);
    }// i
} // test_recode_vector
//////////////////////////////////////////////////
TEST_F(GenroutsTest, test_compute_distances_array_ptr) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    //
    size_t nTotal = (size_t)(nCols * nRows);
    size_t nnv = (size_t)(nCols * nCols);
    size_t nnr = (size_t)(nRows * nRows);
    std::unique_ptr<float> oDest(new float[nTotal]);
    std::unique_ptr<float> oRowDist(new float[nnr]);
    std::unique_ptr<float> oColDist(new float[nnv]);
    //
    float *pRowDist = oRowDist.get();
    ASSERT_TRUE(pRowDist != nullptr);
    float *pColDist = oColDist.get();
    ASSERT_TRUE(pColDist != nullptr);
    float *pDest = oDest.get();
    ASSERT_TRUE(pDest != nullptr);
    //
    bool bRet = statdata::Compute_Distances(nRows, nCols, alldata, pDest,
            pRowDist,pColDist);
    ASSERT_TRUE(bRet);
    //
    float fZero = 0.0f;
    for (size_t ivar = 0; ivar < nCols; ++ivar){
        for (size_t jvar = 0; jvar <= ivar; ++jvar){
            float f1 = pColDist[ivar * nCols + jvar];
            float f2 = pColDist[jvar * nCols + ivar];
            ASSERT_FLOAT_EQ(f1, f2);
            ASSERT_TRUE(f1 >= fZero);
            if (ivar == jvar){
                ASSERT_EQ(fZero, f1);
            }
        }// jvar
    }// ivar
    for (size_t irow = 0; irow < nRows; ++irow){
        for (size_t jrow = 0; jrow <= irow; ++jrow){
            float f1 = pRowDist[irow * nRows + jrow];
            float f2 = pRowDist[jrow * nRows + irow];
            ASSERT_FLOAT_EQ(f1, f2);
            ASSERT_TRUE(f1 >= fZero);
            if (irow == jrow){
                ASSERT_EQ(fZero, f1);
            }
        }// jrow
    }// irow
} // test_normalize_array_ptr
TEST_F(GenroutsTest, test_compute_distances_valarray) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    //
    size_t nTotal = (size_t)(nCols * nRows);
    size_t nnv = (size_t)(nCols * nCols);
    size_t nnr = (size_t)(nRows * nRows);
    std::valarray<float> oDest;
    std::valarray<float> oRowDist;
    std::valarray<float> oColDist;
    //
    bool bRet = statdata::Compute_Distances(nRows, nCols, alldata, oDest,
            oRowDist,oColDist);
    ASSERT_TRUE(bRet);
    ASSERT_EQ(nTotal, oDest.size());
    ASSERT_EQ(nnv, oColDist.size());
    ASSERT_EQ(nnr, oRowDist.size());
    //
    float fZero = 0.0f;
    for (size_t ivar = 0; ivar < nCols; ++ivar){
        for (size_t jvar = 0; jvar <= ivar; ++jvar){
            float f1 = oColDist[ivar * nCols + jvar];
            float f2 = oColDist[jvar * nCols + ivar];
            ASSERT_FLOAT_EQ(f1, f2);
            ASSERT_TRUE(f1 >= fZero);
            if (ivar == jvar){
                ASSERT_EQ(fZero, f1);
            }
        }// jvar
    }// ivar
    for (size_t irow = 0; irow < nRows; ++irow){
        for (size_t jrow = 0; jrow <= irow; ++jrow){
            float f1 = oRowDist[irow * nRows + jrow];
            float f2 = oRowDist[jrow * nRows + irow];
            ASSERT_FLOAT_EQ(f1, f2);
            ASSERT_TRUE(f1 >= fZero);
            if (irow == jrow){
                ASSERT_EQ(fZero, f1);
            }
        }// jrow
    }// irow
} // test_normalize_array_valarray
TEST_F(GenroutsTest, test_compute_distances_vector) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    //
    size_t nTotal = (size_t)(nCols * nRows);
    size_t nnv = (size_t)(nCols * nCols);
    size_t nnr = (size_t)(nRows * nRows);
    std::vector<float> oDest;
    std::vector<float> oRowDist;
    std::vector<float> oColDist;
    //
    bool bRet = statdata::Compute_Distances(nRows, nCols, alldata, oDest,
            oRowDist,oColDist);
    ASSERT_TRUE(bRet);
    ASSERT_EQ(nTotal, oDest.size());
    ASSERT_EQ(nnv, oColDist.size());
    ASSERT_EQ(nnr, oRowDist.size());
    //
    float fZero = 0.0f;
    for (size_t ivar = 0; ivar < nCols; ++ivar){
        for (size_t jvar = 0; jvar <= ivar; ++jvar){
            float f1 = oColDist[ivar * nCols + jvar];
            float f2 = oColDist[jvar * nCols + ivar];
            ASSERT_FLOAT_EQ(f1, f2);
            ASSERT_TRUE(f1 >= fZero);
            if (ivar == jvar){
                ASSERT_EQ(fZero, f1);
            }
        }// jvar
    }// ivar
    for (size_t irow = 0; irow < nRows; ++irow){
        for (size_t jrow = 0; jrow <= irow; ++jrow){
            float f1 = oRowDist[irow * nRows + jrow];
            float f2 = oRowDist[jrow * nRows + irow];
            ASSERT_FLOAT_EQ(f1, f2);
            ASSERT_TRUE(f1 >= fZero);
            if (irow == jrow){
                ASSERT_EQ(fZero, f1);
            }
        }// jrow
    }// irow
} // test_normalize_array_vector
////////////////////////////////////////////
TEST_F(GenroutsTest, test_normalize_array_ptr) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    //
    size_t nn = (size_t)(nCols * nRows);
    std::unique_ptr<float> oWeights(new float[nCols]);
    std::unique_ptr<float> oDest(new float[nn]);
    //
    float *pWeights = oWeights.get();
    ASSERT_TRUE(pWeights != nullptr);
    float *pDest = oDest.get();
    ASSERT_TRUE(pDest != nullptr);
    //
    bool bRet = statdata::Normalize_Array(nRows,nCols,alldata, pDest,pWeights);
    ASSERT_TRUE(bRet);
    //
    float fOne = 1.0f;
    for (size_t ivar1 = 0; ivar1 < nCols; ++ivar1){
        float f = pWeights[ivar1];
        ASSERT_TRUE(f > 0.0f);
        ASSERT_TRUE(f < fOne);
    }// ivar
} // test_normalize_array_ptr
TEST_F(GenroutsTest, test_normalize_array_valarray) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    //
    size_t nn = (size_t)(nCols * nRows);
    std::valarray<float> oWeights;
    std::valarray<float> oDest;
    //
    bool bRet = statdata::Normalize_Array(nRows,nCols,alldata, oDest,oWeights);
    ASSERT_TRUE(bRet);
    ASSERT_EQ(nCols, oWeights.size());
    ASSERT_EQ(nn, oDest.size());
    //
    float fOne = 1.0f;
    for (size_t ivar1 = 0; ivar1 < nCols; ++ivar1){
        float f = oWeights[ivar1];
        ASSERT_TRUE(f > 0.0f);
        ASSERT_TRUE(f < fOne);
    }// ivar
} // test_normalize_array_valarray
TEST_F(GenroutsTest, test_normalize_array_vector) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    //
    size_t nn = (size_t)(nCols * nRows);
    std::vector<float> oWeights;
    std::vector<float> oDest;
    //
    bool bRet = statdata::Normalize_Array(nRows,nCols,alldata, oDest,oWeights);
    ASSERT_TRUE(bRet);
    ASSERT_EQ(nCols, oWeights.size());
    ASSERT_EQ(nn, oDest.size());
    //
    float fOne = 1.0f;
    for (size_t ivar1 = 0; ivar1 < nCols; ++ivar1){
        float f = oWeights[ivar1];
        ASSERT_TRUE(f > 0.0f);
        ASSERT_TRUE(f < fOne);
    }// ivar
} // test_normalize_array_vector
/////////////////////////////////////////////
TEST_F(GenroutsTest, test_correlations_ptr) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    //
    size_t nn = (size_t)(nCols * nCols);
    std::unique_ptr<float> oMeans(new float[nCols]);
    std::unique_ptr<float> oStds(new float[nCols]);
    std::unique_ptr<float> oCorr(new float[nn]);
    //
    float *pMeans = oMeans.get();
    ASSERT_TRUE(pMeans != nullptr);
    float *pStds = oStds.get();
    ASSERT_TRUE(pStds != nullptr);
    float *pCorr = oCorr.get();
    ASSERT_TRUE(pCorr != nullptr);
    //
    statdata::Correlations(nRows,nCols,alldata,pMeans,pStds,pCorr);
    //
    float fOne = 1.0f;
    for (size_t ivar1 = 0; ivar1 < nCols; ++ivar1){
        ASSERT_TRUE(pStds[ivar1] > 0.0f);
        for (size_t ivar2 = 0; ivar2 <= ivar1; ++ivar2){
            float f1 = pCorr[ivar1 * nCols + ivar2];
            float f2 = pCorr[ivar2 * nCols + ivar1];
            ASSERT_FLOAT_EQ(f1,f2);
            ASSERT_TRUE((float)std::abs(f1) <= fOne);
            if (ivar1 == ivar2){
                ASSERT_FLOAT_EQ(fOne, f1);
            }
        }// ivar2
    }// ivar
} // test_correlations_ptr
TEST_F(GenroutsTest, test_correlations_valarray) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> alldata = m_valdata;
    //
    size_t nn = (size_t)(nCols * nCols);
    std::valarray<float> oMeans;
    std::valarray<float> oStds;
    std::valarray<float> oCorr;
    //
    statdata::Correlations(nRows,nCols,alldata,oMeans,oStds,oCorr);
    ASSERT_EQ(nCols, oMeans.size());
    ASSERT_EQ(nCols, oStds.size());
    ASSERT_EQ(nn, oCorr.size());
    //
    float fOne = 1.0f;
    for (size_t ivar1 = 0; ivar1 < nCols; ++ivar1){
        ASSERT_TRUE(oStds[ivar1] > 0.0f);
        for (size_t ivar2 = 0; ivar2 <= ivar1; ++ivar2){
            float f1 = oCorr[ivar1 * nCols + ivar2];
            float f2 = oCorr[ivar2 * nCols + ivar1];
            ASSERT_FLOAT_EQ(f1,f2);
            ASSERT_TRUE((float)std::abs(f1) <= fOne);
            if (ivar1 == ivar2){
                ASSERT_FLOAT_EQ(fOne, f1);
            }
        }// ivar2
    }// ivar
} // test_correlations_valarray
TEST_F(GenroutsTest, test_correlations_vector) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> alldata = m_vecdata;
    //
    size_t nn = (size_t)(nCols * nCols);
    std::vector<float> oMeans;
    std::vector<float> oStds;
    std::vector<float> oCorr;
    //
    statdata::Correlations(nRows,nCols,alldata,oMeans,oStds,oCorr);
    ASSERT_EQ(nCols, oMeans.size());
    ASSERT_EQ(nCols, oStds.size());
    ASSERT_EQ(nn, oCorr.size());
    //
    float fOne = 1.0f;
    for (size_t ivar1 = 0; ivar1 < nCols; ++ivar1){
        ASSERT_TRUE(oStds[ivar1] > 0.0f);
        for (size_t ivar2 = 0; ivar2 <= ivar1; ++ivar2){
            float f1 = oCorr[ivar1 * nCols + ivar2];
            float f2 = oCorr[ivar2 * nCols + ivar1];
            ASSERT_FLOAT_EQ(f1,f2);
            ASSERT_TRUE((float)std::abs(f1) <= fOne);
            if (ivar1 == ivar2){
                ASSERT_FLOAT_EQ(fOne, f1);
            }
        }// ivar2
    }// ivar
} // test_correlations_vector
///////////////////////////////////////////////
TEST_F(GenroutsTest, test_sort_1_ptr) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::unique_ptr<float> data(new float[nRows]);
        float *pData = data.get();
        ASSERT_TRUE(pData != nullptr);
        for (size_t i = 0; i < nRows; ++i) {
            pData[i] = alldata[i * nCols + ivar];
        } // i
        std::unique_ptr<short> oRes(new short[nRows]);
        short *pRes = oRes.get();
        ASSERT_TRUE(pRes != nullptr);
        statdata::Sort(nRows, pData, pRes);
        for (size_t i = 0; i < nRows; ++i) {
            short ii = pRes[i];
            ASSERT_TRUE(ii >= 0);
            ASSERT_TRUE(ii < (short) nRows);
            if (i > 0) {
                float f1 = pData[pRes[i - 1]];
                float f2 = pData[pRes[i]];
                ASSERT_TRUE(f1 <= f2);
            }// i
        } // i
    } // ivar
} // test_sort_1_ptr
TEST_F(GenroutsTest, test_rank_ptr) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::unique_ptr<float> data(new float[nRows]);
        float *pData = data.get();
        ASSERT_TRUE(pData != nullptr);
        for (size_t i = 0; i < nRows; ++i) {
            pData[i] = alldata[i * nCols + ivar];
        } // i
        std::unique_ptr<short> oRes(new short[nRows]);
        short *pRes = oRes.get();
        ASSERT_TRUE(pRes != nullptr);
        statdata::Rank(nRows, pData, pRes);
        for (size_t i = 0; i < nRows; ++i) {
            short ii = pRes[i];
            ASSERT_TRUE(ii > 0);
            ASSERT_TRUE(ii <= (short) nRows);
            if (i > 0) {
                short r1 = pRes[i - 1];
                short r2 = pRes[i];
                float f1 = pData[i - 1];
                float f2 = pData[i];
                if (r1 < r2){
                    ASSERT_TRUE(f1 < f2);
                } else if (r1 > r2){
                    ASSERT_TRUE(f1 > f2);
                } else {
                    ASSERT_FLOAT_EQ(f1,f2);
                }
            }// i
        } // i
    } // ivar
} // test_rank_ptr
TEST_F(GenroutsTest, test_sort_2_ptr) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::unique_ptr<float> data(new float[nRows]);
        float *pData = data.get();
        ASSERT_TRUE(pData != nullptr);
        for (size_t i = 0; i < nRows; ++i) {
            pData[i] = alldata[i * nCols + ivar];
        } // i
        statdata::Sort(nRows, pData);
        for (size_t i = 0; i < nRows; ++i) {
            if (i > 0) {
                float f1 = pData[i - 1];
                float f2 = pData[i];
                ASSERT_TRUE(f1 <= f2);
            }// i
        } // i
    } // ivar
} // test_sort_2_ptr
/////////////////////////////////////////////
TEST_F(GenroutsTest, test_sort_1_valarray) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::valarray<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        std::valarray<short> oRes;
        statdata::Sort(data, oRes);
        ASSERT_EQ(nRows, oRes.size());
        for (size_t i = 0; i < nRows; ++i) {
            short ii = oRes[i];
            ASSERT_TRUE(ii >= 0);
            ASSERT_TRUE(ii < (short) nRows);
            if (i > 0) {
                float f1 = data[oRes[i - 1]];
                float f2 = data[oRes[i]];
                ASSERT_TRUE(f1 <= f2);
            }// i
        } // i
    } // ivar
} // test_sort_1_valarray
TEST_F(GenroutsTest, test_rank_valarray) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::valarray<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        std::valarray<short> oRes;
        statdata::Rank(data, oRes);
        ASSERT_EQ(nRows, oRes.size());
        for (size_t i = 0; i < nRows; ++i) {
            short ii = oRes[i];
            ASSERT_TRUE(ii > 0);
            ASSERT_TRUE(ii <= (short) nRows);
            if (i > 0) {
                short r1 = oRes[i - 1];
                short r2 = oRes[i];
                float f1 = data[i - 1];
                float f2 = data[i];
                if (r1 < r2){
                    ASSERT_TRUE(f1 < f2);
                } else if (r1 > r2){
                    ASSERT_TRUE(f1 > f2);
                } else {
                    ASSERT_FLOAT_EQ(f1,f2);
                }
            }// i
        } // i
    } // ivar
} // test_rank_valarray
TEST_F(GenroutsTest, test_sort_2_valarray) {
     size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::valarray<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        statdata::Sort(data);
        for (size_t i = 0; i < nRows; ++i) {
            if (i > 0) {
                float f1 = data[i - 1];
                float f2 = data[i];
                ASSERT_TRUE(f1 <= f2);
            }// i
        } // i
    } // ivar
} // test_sort_2_valarray
/////////////////////////////////////////////////
TEST_F(GenroutsTest, test_sort_1_vector) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        std::vector<short> oRes;
        statdata::Sort(data, oRes);
        ASSERT_EQ(nRows, oRes.size());
        for (size_t i = 0; i < nRows; ++i) {
            short ii = oRes[i];
            ASSERT_TRUE(ii >= 0);
            ASSERT_TRUE(ii < (short) nRows);
            if (i > 0) {
                float f1 = data[oRes[i - 1]];
                float f2 = data[oRes[i]];
                ASSERT_TRUE(f1 <= f2);
            }// i
        } // i
    } // ivar
} // test_sort_1_vector
TEST_F(GenroutsTest, test_sort_2_vector) {
     size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        statdata::Sort(data);
        for (size_t i = 0; i < nRows; ++i) {
            if (i > 0) {
                float f1 = data[i - 1];
                float f2 = data[i];
                ASSERT_TRUE(f1 <= f2);
            }// i
        } // i
    } // ivar
} // test_sort_2_vector
TEST_F(GenroutsTest, test_rank_vector) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        std::vector<short> oRes;
        statdata::Rank(data, oRes);
        ASSERT_EQ(nRows, oRes.size());
        for (size_t i = 0; i < nRows; ++i) {
            short ii = oRes[i];
            ASSERT_TRUE(ii > 0);
            ASSERT_TRUE(ii <= (short) nRows);
            if (i > 0) {
                short r1 = oRes[i - 1];
                short r2 = oRes[i];
                float f1 = data[i - 1];
                float f2 = data[i];
                if (r1 < r2){
                    ASSERT_TRUE(f1 < f2);
                } else if (r1 > r2){
                    ASSERT_TRUE(f1 > f2);
                } else {
                    ASSERT_FLOAT_EQ(f1,f2);
                }
            }// i
        } // i
    } // ivar
} // test_rank_vector
//////////////////////////////////////////////

TEST_F(GenroutsTest, test_inverse_ptr) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::unique_ptr<float> data(new float[nRows]);
        float *pData = data.get();
        ASSERT_TRUE(pData != nullptr);
        for (size_t i = 0; i < nRows; ++i) {
            pData[i] = alldata[i * nCols + ivar];
        } // i
        std::unique_ptr<float> oRes(new float[nRows]);
        float *pRes = oRes.get();
        ASSERT_TRUE(pRes != nullptr);
        statdata::Inverse(nRows, (const float *) pData, pRes);
        for (size_t i = 0; i < nRows; ++i) {
            float f = pData[i];
            float fexpected = (f != 0.0f) ? (float) (1.0 / f) : f;
            float factual = pRes[i];
            ASSERT_FLOAT_EQ(fexpected, factual);
        } // i
    } // ivar
} // test_inverse_ptr

TEST_F(GenroutsTest, test_inverse_vector) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        }// i
        std::vector<float> oRes;
        statdata::Inverse(data, oRes);
        ASSERT_TRUE(oRes.size() == nRows);
        for (size_t i = 0; i < nRows; ++i) {
            float f = data[i];
            float fexpected = (f != 0.0f) ? (float) (1.0 / f) : f;
            float factual = oRes[i];
            ASSERT_FLOAT_EQ(fexpected, factual);
        } // i
    } // ivar
} // test_inverse_vector

TEST_F(GenroutsTest, test_inverse_valarray) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::valarray<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        std::valarray<float> oRes;
        statdata::Inverse(data, oRes);
        ASSERT_TRUE(oRes.size() == nRows);
        for (size_t i = 0; i < nRows; ++i) {
            float f = data[i];
            float fexpected = (f != 0.0f) ? (float) (1.0 / f) : f;
            float factual = oRes[i];
            ASSERT_FLOAT_EQ(fexpected, factual);
        } // i
    } // ivar
} // test_inverse_valarray
///////////////////////////////////////////

TEST_F(GenroutsTest, test_meanstd_ptr) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const float *alldata = m_odata.get();
    ASSERT_TRUE(alldata != nullptr);
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::unique_ptr<float> data(new float[nRows]);
        float *pData = data.get();
        ASSERT_TRUE(pData != nullptr);
        for (size_t i = 0; i < nRows; ++i) {
            pData[i] = alldata[i * nCols + ivar];
        } // i
        float vMean = 0, vStd = 0;
        bool bRet = statdata::MeanStd(nRows, pData, vMean, vStd);
        ASSERT_TRUE(bRet);
        ASSERT_TRUE(vStd > 0.0f);
    } // ivar
} // test_meanstd_ptr

TEST_F(GenroutsTest, test_meanstd_vector) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::vector<float> &alldata = m_vecdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        float vMean = 0, vStd = 0;
        bool bRet = statdata::MeanStd(data, vMean, vStd);
        ASSERT_TRUE(bRet);
        ASSERT_TRUE(vStd > 0.0f);
    } // ivar
} // test_meanstd_vector

TEST_F(GenroutsTest, test_meanstd_valarray) {
    size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_valdata;
    //
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::valarray<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        } // i
        float vMean = 0, vStd = 0;
        bool bRet = statdata::MeanStd(data, vMean, vStd);
        ASSERT_TRUE(bRet);
        ASSERT_TRUE(vStd > 0.0f);
    } // ivar
} // test_meanstd_valarray
///////////////////////////////////////
#else

#endif // NO_GTEST

