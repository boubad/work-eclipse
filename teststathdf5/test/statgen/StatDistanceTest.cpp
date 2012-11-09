#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <statdistance.h>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <fstream>
/////////////////////////////
using namespace statdata;
//////////////////////////////////////

class StatDistanceTest : public ::testing::Test {
public:

    StatDistanceTest() {
    }

    virtual ~StatDistanceTest() {
    }
protected:

protected:

    static void SetUpTestCase() {
        size_t nInds = m_st_nbindivs;
        ASSERT_TRUE(nInds > 1);
        size_t nCols = m_st_nbvars;
        ASSERT_TRUE(nCols > 0);
        size_t nTotal = (size_t) (nInds * nCols);
        m_stbooldata.resize(nTotal);
         m_stboolvaldata.resize(nTotal);
        for (size_t i = 0; i < nTotal; ++i) {
            m_stbooldata[i] = ((::rand() % 2) == 0) ? true : false;
            m_stboolvaldata[i] = m_stbooldata[i];
        }// i
        ASSERT_TRUE(m_stbooldata.size() == nTotal);
        ASSERT_TRUE(m_stboolvaldata.size() == nTotal);
        m_stfloatdata.resize(nTotal);
        m_stfloatvaldata.resize(nTotal);
        for (size_t i = 0; i < nTotal; ++i) {
            m_stfloatdata[i] = (float) (::rand() % 20);
            m_stfloatvaldata[i] = m_stfloatdata[i];
        }// i
        ASSERT_TRUE(m_stfloatdata.size() == nTotal);
        ASSERT_TRUE(m_stfloatvaldata.size() == nTotal);
        {
            std::ofstream out(m_outfilename);
            if (out.is_open()) {
                out.close();
            }
        }
    }//SetUpTestCase

    static void TearDownTestCase() {
        m_stbooldata.clear();
        m_stfloatdata.clear();
    }//TearDownTestCase

    virtual void SetUp() {
        bool bExists = false;
        {
            std::ifstream in(m_outfilename);
            bExists = in.is_open();
            in.close();
        }
        if (bExists) {
            m_out.reset(new std::ofstream(m_outfilename, std::ios::out | std::ios::app));
        } else {
            m_out.reset(new std::ofstream(m_outfilename));
        }
        std::ofstream *p = m_out.get();
        ASSERT_TRUE(p != nullptr);
        ASSERT_TRUE(p->is_open());
    }// SetUp

    virtual void TearDown() {
        std::ofstream *p = m_out.get();
        if (p != nullptr) {
            if (p->is_open()) {
                p->close();
            }
            m_out.reset();
        }
    }// TearDown
protected:
    std::unique_ptr<std::ofstream> m_out;
    //////////////////////////////
    static size_t m_st_nbindivs;
    static size_t m_st_nbvars;
    static std::vector<bool> m_stbooldata;
    static std::vector<float> m_stfloatdata;
    static std::valarray<bool> m_stboolvaldata;
    static std::valarray<float> m_stfloatvaldata;
    static std::string m_outfilename;
};
size_t StatDistanceTest::m_st_nbindivs = 100;
size_t StatDistanceTest::m_st_nbvars = 10;
std::vector<bool> StatDistanceTest::m_stbooldata;
std::vector<float> StatDistanceTest::m_stfloatdata;
std::valarray<bool> StatDistanceTest::m_stboolvaldata;
std::valarray<float> StatDistanceTest::m_stfloatvaldata;
std::string StatDistanceTest::m_outfilename("./testresults/my_distances.txt");
///////////////////////////////////////
TEST_F(StatDistanceTest, test_quant_distances_valarray) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_st_nbvars;
    ASSERT_TRUE(nCols > 1);
    const size_t nRows = m_st_nbindivs;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<float> &alldata = m_stfloatvaldata;
    //
    statdata::DistanceType oTypes[] = {
       q_squared,
        q_module,
        q_maximum,
        q_divergence,
        q_khi2
    };
    size_t nTypes = sizeof(oTypes) / sizeof(statdata::DistanceType);
    os << std::endl << "====== TEST QUANT DISTANCES VALARRAY=========" << std::endl;
    for (size_t i = 0; i < nTypes; ++i){
        statdata::DistanceType dtype = oTypes[i];
       QuantArrayDistFunc<float,float> oFunc(nRows,nCols,alldata,dtype);
        os << "Distance type\t" << ((int)dtype);
        for (size_t j = 0; j < nRows; ++j){
            for (size_t k = 0; k <= j; ++k){
                float d = oFunc(j,k);
                //ASSERT_TRUE(d >= 0.0f);
                if (k > 0){
                    os << "\t";
                }
                os << d;
            }// k
            os << std::endl;
        }// j
        os << std::endl;
    }// i
    os << std::endl;
}//saveArchiveTest

TEST_F(StatDistanceTest, test_bool_distances_valarray) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_st_nbvars;
    ASSERT_TRUE(nCols > 1);
    const size_t nRows = m_st_nbindivs;
    ASSERT_TRUE(nRows > 0);
    const std::valarray<bool> &alldata = m_stboolvaldata;
    //
    statdata::DistanceType oTypes[] = {
        b_russel_rao,
        b_jaccard,
        b_dice,
        b_sokal_sneath_2,
        b_kulczinsky_1,
        b_kulczinsky_2,
        b_ochiai,
        b_simpson,
        b_kochen_wong,
        b_sokal_michener,
        b_sokal_sneath_1,
        b_rogers_tanimoto,
        b_sokal_sneath_3,
        b_sokal_sneath_4,
        b_sokal_sneath_5,
        b_roux_1,
        b_roux_2,
        b_hamann,
        b_yule,
        b_phi_de_pearson
    };
    size_t nTypes = sizeof(oTypes) / sizeof(statdata::DistanceType);
    os << std::endl << "====== TEST BOOL DISTANCES VALARRAY=========" << std::endl;
    for (size_t i = 0; i < nTypes; ++i){
        statdata::DistanceType dtype = oTypes[i];
        BoolArrayDistFunc<float> oFunc(nRows,nCols,alldata,dtype);
        os << "Distance type\t" << ((int)dtype);
        for (size_t j = 0; j < nRows; ++j){
            for (size_t k = 0; k <= j; ++k){
                float d = oFunc(j,k);
                //ASSERT_TRUE(d >= 0.0f);
                if (k > 0){
                    os << "\t";
                }
                os << d;
            }// k
            os << std::endl;
        }// j
        os << std::endl;
    }// i
    os << std::endl;
}//saveArchiveTest


#else

#endif // NO_GTEST

