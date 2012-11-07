#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <anacompo.h>
#include <csvreader.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
//////////////////////////////
using namespace statdata;
///////////////////////////////////

class AnacompoTest : public ::testing::Test {
public:
    typedef double MyType;
public:

    AnacompoTest() {
    }

    virtual ~AnacompoTest() {
    }
protected:

    static void SetUpTestCase() {
        std::map<std::string, std::vector<boost::any> > oMap;
        bool bRet = CSVReader::read_csv_file(m_filename, oMap);
        ASSERT_TRUE(bRet);
        size_t nRows = 0;
        std::vector<std::string> varnames;
        std::vector<std::vector<double> > oArray;
        double dInit = 0.0;
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
            }// i
            if (vfirst.empty()) {
                continue;
            }
            const std::type_info &tt = vfirst.type();
            if ((tt == typeid (float)) || (tt == typeid (double)) ||
                    (tt == typeid (int)) || (tt == typeid (long))) {
                if (nRows < 1) {
                    nRows = n;
                }
                if ((nRows > 0) && (nRows <= vv.size())) {
                    std::vector<double> vz(nRows, dInit);
                    for (size_t i = 0; i < nRows; ++i) {
                        const boost::any &v = vv[i];
                        if (!v.empty()) {
                            double x = 0.0;
                            if (Value::get_value(v, x)) {
                                vz[i] = x;
                            }
                        }
                    }// i
                    oArray.push_back(vz);
                    varnames.push_back(varname);
                }// nRows
            }// found
        }// it
        ASSERT_TRUE(nRows > 2);
        size_t nCols = oArray.size();
        ASSERT_TRUE(nCols > 2);
        ASSERT_TRUE(nRows > nCols);
        size_t nTotal = (size_t) (nRows * nCols);
        m_nrows = nRows;
        m_ncols = nCols;
        m_names = varnames;
        m_vecdata.resize(nTotal);
        ASSERT_TRUE(m_vecdata.size() == nTotal);
        m_valdata.resize(nTotal);
        ASSERT_TRUE(m_valdata.size() == nTotal);
        for (size_t ivar = 0; ivar < nCols; ++ivar) {
            const std::vector<double> &vv = oArray[ivar];
            ASSERT_TRUE(vv.size() == nRows);
            for (size_t irow = 0; irow < nRows; ++irow) {
                size_t k = (size_t) (irow * nCols + ivar);
                double fval = vv[irow];
                m_vecdata[k] = fval;
                m_valdata[k] = fval;
            }// irow
        }// ivar
    }//SetUpTestCase

    static void TearDownTestCase() {
        m_vecdata.clear();
        m_names.clear();
    }//TearDownTestCase
protected:
    static size_t m_nrows;
    static size_t m_ncols;
    static std::vector<MyType> m_vecdata;
    static std::valarray<MyType> m_valdata;
    static std::string m_filename;
    static std::vector<std::string> m_names;
};
size_t AnacompoTest::m_nrows = 0;
size_t AnacompoTest::m_ncols = 0;
std::vector<AnacompoTest::MyType> AnacompoTest::m_vecdata;
std::valarray<AnacompoTest::MyType> AnacompoTest::m_valdata;
std::string AnacompoTest::m_filename("./data/testdata.tsv");
std::vector<std::string> AnacompoTest::m_names;
///////////////////////////////////////
using namespace statdata;
//////////////////////////////////////

TEST_F(AnacompoTest, test_compute_vector_1) {
    const size_t nr = m_nrows;
    const size_t nv = m_ncols;
    const std::vector<double> &data = m_vecdata;
    size_t nFacts = 0;
    std::vector<double> oMeans, oStds, oCorr, oVals, oFreq, oVecs, oVars, oInds;
    bool bRet = IntraEigenSolver<double>::compute_anacompo(nr, nv, data, oMeans,
            oStds, oCorr, nFacts, oFreq, oVals, oVecs, oVars, oInds);
    ASSERT_TRUE(bRet);
    ASSERT_TRUE(nFacts > 0);
    ASSERT_TRUE(nFacts <= nv);
    ASSERT_TRUE(oMeans.size() == nv);
    ASSERT_TRUE(oStds.size() == nv);
    ASSERT_TRUE(oCorr.size() == (size_t) (nv * nv));
    for (size_t i = 0; i < nv; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            double x1 = oCorr[i * nv + j];
            double x2 = oCorr[j * nv + i];
            ASSERT_DOUBLE_EQ(x1, x2);
            if (i == j) {
                ASSERT_DOUBLE_EQ(x1, (double) 1.0);
            }// j
        }// j
    }// i
    ASSERT_TRUE(oFreq.size() == nFacts);
    for (size_t i = 0; i < nFacts; ++i) {
        double x = oFreq[i];
        ASSERT_TRUE((x > 0.0) && (x < 1.0));
    }// i
    ASSERT_TRUE(nFacts == oVals.size());
    for (size_t i = 0; i < nFacts; ++i) {
        double x = oVals[i];
        ASSERT_TRUE(x > 0.0);
    }// i
    ASSERT_TRUE((size_t) (nFacts * nv) == oVecs.size());
    ASSERT_TRUE((size_t) (nFacts * nv) == oVars.size());
    ASSERT_TRUE((size_t) (nFacts * nr) == oInds.size());
} // test_compute_eigen_vrctor

TEST_F(AnacompoTest, test_compute_vector_2) {
    const size_t nr = m_nrows;
    const size_t nv = m_ncols;
    const std::vector<double> &data = m_vecdata;
    size_t nFacts = 0;
    std::vector<double> oVars, oInds;
    bool bRet = IntraEigenSolver<double>::compute_anacompo(nr, nv, data, nFacts,
            oVars, oInds);
    ASSERT_TRUE(bRet);
    ASSERT_TRUE(nFacts > 0);
    ASSERT_TRUE(nFacts <= nv);
    ASSERT_TRUE((size_t) (nFacts * nv) == oVars.size());
    ASSERT_TRUE((size_t) (nFacts * nr) == oInds.size());
} // test_compute_eigen_vrctor_2

TEST_F(AnacompoTest, test_compute_valarray_1) {
    const size_t nr = m_nrows;
    const size_t nv = m_ncols;
    const std::valarray<double> &data = m_valdata;
    size_t nFacts = 0;
    std::valarray<double> oMeans, oStds, oCorr, oVals, oFreq, oVecs, oVars, oInds;
    bool bRet = IntraEigenSolver<double>::compute_anacompo(nr, nv, data, oMeans,
            oStds, oCorr, nFacts, oFreq, oVals, oVecs, oVars, oInds);
    ASSERT_TRUE(bRet);
    ASSERT_TRUE(nFacts > 0);
    ASSERT_TRUE(nFacts <= nv);
    ASSERT_TRUE(oMeans.size() == nv);
    ASSERT_TRUE(oStds.size() == nv);
    ASSERT_TRUE(oCorr.size() == (size_t) (nv * nv));
    for (size_t i = 0; i < nv; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            double x1 = oCorr[i * nv + j];
            double x2 = oCorr[j * nv + i];
            ASSERT_DOUBLE_EQ(x1, x2);
            if (i == j) {
                ASSERT_DOUBLE_EQ(x1, (double) 1.0);
            }// j
        }// j
    }// i
    ASSERT_TRUE(oFreq.size() == nFacts);
    for (size_t i = 0; i < nFacts; ++i) {
        double x = oFreq[i];
        ASSERT_TRUE((x > 0.0) && (x < 1.0));
    }// i
    ASSERT_TRUE(nFacts == oVals.size());
    for (size_t i = 0; i < nFacts; ++i) {
        double x = oVals[i];
        ASSERT_TRUE(x > 0.0);
    }// i
    ASSERT_TRUE((size_t) (nFacts * nv) == oVecs.size());
    ASSERT_TRUE((size_t) (nFacts * nv) == oVars.size());
    ASSERT_TRUE((size_t) (nFacts * nr) == oInds.size());
} // test_compute_eigen_vrctor

TEST_F(AnacompoTest, test_compute_valarray_2) {
    const size_t nr = m_nrows;
    const size_t nv = m_ncols;
    const std::valarray<double> &data = m_valdata;
    size_t nFacts = 0;
    std::valarray<double> oVars, oInds;
    bool bRet = IntraEigenSolver<double>::compute_anacompo(nr, nv, data, nFacts,
            oVars, oInds);
    ASSERT_TRUE(bRet);
    ASSERT_TRUE(nFacts > 0);
    ASSERT_TRUE(nFacts <= nv);
    ASSERT_TRUE((size_t) (nFacts * nv) == oVars.size());
    ASSERT_TRUE((size_t) (nFacts * nr) == oInds.size());
} // test_compute_eigen_vrctor_2


#else


#endif // NO_GTEST


