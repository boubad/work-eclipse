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
            const std::type_info &tt = vfirst.type();
            if ((tt != typeid (float)) && (tt != typeid (double))
                    && (tt != typeid (long)) && (tt != typeid (int))
                    && (tt != typeid (unsigned char))) {
                continue;
            }
            if (nRows < 1) {
                nRows = n;
            }
            if ((nRows < 1) || (nRows > vv.size())) {
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
};
/////////////////////////////////////////////
size_t GenroutsTest::m_nrows = 0;
size_t GenroutsTest::m_ncols = 0;
std::vector<float> GenroutsTest::m_vecdata;
std::valarray<float> GenroutsTest::m_valdata;
std::vector<std::string> GenroutsTest::m_varnames;
std::string GenroutsTest::m_filename("./data/testdata.tsv");
std::string GenroutsTest::m_outfilename("./testresults/genrouts.txt");
std::unique_ptr<float> GenroutsTest::m_odata;
////////////////////////////////////////////

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

