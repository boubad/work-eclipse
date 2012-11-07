#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <anacompo.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
//////////////////////////////

class StatEigenTest : public ::testing::Test {
public:
    typedef double MyType;
public:

    StatEigenTest() {
    }

    virtual ~StatEigenTest() {
    }
protected:

    static void SetUpTestCase() {
        const size_t n = m_st_n;
        ASSERT_TRUE(n > 2);
        size_t nn = (size_t) (n * n);
        m_vecdata.resize(nn);
        ASSERT_TRUE(nn == m_vecdata.size());
        m_valdata.resize(nn);
        ASSERT_TRUE(nn == m_valdata.size());
        m_odata.reset(new MyType[nn]);
        MyType *pData = m_odata.get();
        ASSERT_TRUE(pData != nullptr);
        const MyType *pSrc = &(TEST_DATA[0]);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                size_t k1 = (size_t) (i * n + j);
                MyType v = pSrc[k1];
                size_t k2 = (size_t) (j * n + i);
                m_vecdata[k1] = v;
                m_valdata[k1] = v;
                pData[k1] = v;
                m_vecdata[k2] = v;
                m_valdata[k2] = v;
                pData[k2] = v;
            }// j
        }// i
    }//SetUpTestCase

    static void TearDownTestCase() {
        m_vecdata.clear();
        m_odata.reset();
    }//TearDownTestCase
protected:
    static const size_t m_st_n;
    static const MyType TEST_DATA[];
    static std::vector<MyType> m_vecdata;
    static std::valarray<MyType> m_valdata;
    static std::unique_ptr<MyType> m_odata;
};
const size_t StatEigenTest::m_st_n = 4;
const StatEigenTest::MyType StatEigenTest::TEST_DATA[] = {1.0,
    1 / 2.0, 1 / 3.0, 1 / 4.0, 1 / 2.0, 1 / 3.0, 1 / 4.0,
    1 / 5.0, 1 / 3.0, 1 / 4.0, 1 / 5.0, 1 / 6.0, 1 / 4.0, 1 / 5.0, 1
    / 6.0, 1 / 7.0};
std::vector<StatEigenTest::MyType> StatEigenTest::m_vecdata;
std::valarray<StatEigenTest::MyType> StatEigenTest::m_valdata;
std::unique_ptr<StatEigenTest::MyType> StatEigenTest::m_odata;
///////////////////////////////////////
using namespace statdata;
//////////////////////////////////////

TEST_F(StatEigenTest, test_compute_eigen_vector) {
    const size_t n = m_st_n;
    ASSERT_TRUE(n > 2);
    std::vector<MyType> oVals, oVecs;
    size_t nFacts = 0;
    const std::vector<MyType> &oData = m_vecdata;
    bool bRet = IntraEigenSolver<MyType>::compute_eigen(n, oData, nFacts, oVals, oVecs);
    ASSERT_TRUE(bRet);
    ASSERT_TRUE(nFacts > 0);
    //
    ASSERT_TRUE(nFacts == oVals.size());
    ASSERT_TRUE(oVecs.size() == (size_t) (n * nFacts));
#ifdef MYTOTO
    std::cout << std::endl;
    std::cout << std::endl << "====== EigenSolver: test_compute_eigen_vector =======";
    std::cout << std::endl;
    std::cout << "Nb. Factors: " << nFacts << std::endl;
    std::cout << "Eigen values:" << std::endl;
    for (size_t i = 0; i < nFacts; ++i) {
        std::cout << "\t," << oVals[i];
    } // i
    std::cout << std::endl << "Eigen vectors";
    std::cout << std::endl;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < nFacts; ++j) {
            std::cout << "\t" << oVecs[i * nFacts + j];
        } // j
        std::cout << std::endl;
    } // i
    std::cout << std::endl;
#endif
} // test_compute_eigen_vrctor

TEST_F(StatEigenTest, test_compute_eigen_valarray) {
    const size_t n = m_st_n;
    ASSERT_TRUE(n > 2);
    std::valarray<MyType> oVals, oVecs;
    size_t nFacts = 0;
    const std::valarray<MyType> &oData = m_valdata;
    bool bRet = IntraEigenSolver<MyType>::compute_eigen(n, oData, nFacts, oVals, oVecs);
    ASSERT_TRUE(bRet);
    ASSERT_TRUE(nFacts > 0);
    //
    ASSERT_TRUE(nFacts == oVals.size());
    ASSERT_TRUE(oVecs.size() == (size_t) (n * nFacts));
#ifdef MYTOTO
    std::cout << std::endl;
    std::cout << std::endl << "====== EigenSolver: test_compute_eigen_valarray =======";
    std::cout << std::endl;
    std::cout << "Nb. Factors: " << nFacts << std::endl;
    std::cout << "Eigen values:" << std::endl;
    for (size_t i = 0; i < nFacts; ++i) {
        std::cout << "\t," << oVals[i];
    } // i
    std::cout << std::endl << "Eigen vectors";
    std::cout << std::endl;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < nFacts; ++j) {
            std::cout << "\t" << oVecs[i * nFacts + j];
        } // j
        std::cout << std::endl;
    } // i
    std::cout << std::endl;
#endif
} // test_compute_eigen_valarray

TEST_F(StatEigenTest, test_compute_eigen_ptr) {
    const size_t n = m_st_n;
    ASSERT_TRUE(n > 2);
    std::unique_ptr<MyType> oVals(new MyType[n]);
    MyType *pVals = oVals.get();
    ASSERT_TRUE(pVals != nullptr);
    std::unique_ptr<MyType> oVecs(new MyType[n * n]);
    MyType *pVecs = oVecs.get();
    ASSERT_TRUE(pVecs != nullptr);
    size_t nFacts = 0;
    const MyType *pData = m_odata.get();
    ASSERT_TRUE(pData != nullptr);
    bool bRet = IntraEigenSolver<MyType>::compute_eigen(n, pData, nFacts, pVals, pVecs);
    ASSERT_TRUE(bRet);
    ASSERT_TRUE(nFacts > 0);
    //
#ifdef MYTOTO
    std::cout << std::endl;
    std::cout << std::endl << "====== EigenSolver: test_compute_eigen_ptr =======";
    std::cout << std::endl;
    std::cout << "Nb. Factors: " << nFacts << std::endl;
    std::cout << "Eigen values:" << std::endl;
    for (size_t i = 0; i < nFacts; ++i) {
        std::cout << "\t," << pVals[i];
    } // i
    std::cout << std::endl << "Eigen vectors";
    std::cout << std::endl;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < nFacts; ++j) {
            std::cout << "\t" << pVecs[i * nFacts + j];
        } // j
        std::cout << std::endl;
    } // i
    std::cout << std::endl;
#endif
} // test_compute_eigen_ptr
#else


#endif // NO_GTEST

