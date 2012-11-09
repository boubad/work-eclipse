#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <univarstatdesc.h>
#include <stattest.h>
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

class UnivarStatDescTest : public ::testing::Test {
public:

    UnivarStatDescTest() {
    }

    virtual ~UnivarStatDescTest() {
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
            }// i
            if (vfirst.empty()) {
                continue;
            }
            const std::type_info &tt = vfirst.type();
            if ((tt != typeid (float)) && (tt != typeid (double)) &&
                    (tt != typeid (long)) && (tt != typeid (int)) &&
                    (tt != typeid (unsigned char))) {
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
            }// i
            oArray.push_back(vz);
            m_varnames.push_back(varname);
        }// it
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
        for (size_t ivar = 0; ivar < nCols; ++ivar) {
            const std::vector<float> &vv = oArray[ivar];
            ASSERT_TRUE(vv.size() == nRows);
            for (size_t irow = 0; irow < nRows; ++irow) {

                size_t k = (size_t) (irow * nCols + ivar);
                float fval = vv[irow];
                m_vecdata[k] = fval;
                m_valdata[k] = fval;
            }// irow
        }// ivar
        {
            std::ofstream out(m_outfilename);
            if (out.is_open()) {
                out.close();
            }
        }
    }//SetUpTestCase

    static void TearDownTestCase() {

        m_vecdata.clear();
        m_varnames.clear();
        m_valdata.resize(1);
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
    static const double TEST_DATA[];
};
/////////////////////////////////////////////
size_t UnivarStatDescTest::m_nrows = 0;
size_t UnivarStatDescTest::m_ncols = 0;
std::vector<float> UnivarStatDescTest::m_vecdata;
std::valarray<float> UnivarStatDescTest::m_valdata;
std::vector<std::string> UnivarStatDescTest::m_varnames;
std::string UnivarStatDescTest::m_filename("./data/testdata.tsv");
std::string UnivarStatDescTest::m_outfilename("./testresults/univarstatdesc.txt");
const double UnivarStatDescTest::TEST_DATA[] = {
    3.0, 7.0, 9.0, 4.0, 11.0, 9.0, 6.0, 10.0, 5.0, 17.0, 7.0, 8.0
};
////////////////////////////////////////////

TEST_F(UnivarStatDescTest, test_clearvalues_ptr) {
    const size_t n = sizeof (TEST_DATA) / sizeof (double);
    const double *pData = &(TEST_DATA[0]);
    //
    std::map<size_t, double> oMap;
    UnivarStatDesc::clear_values(n, pData, oMap);
    size_t nIndex = 9;
    ASSERT_TRUE(!oMap.empty());
    ASSERT_TRUE(oMap.find(nIndex) != oMap.end());
    double valexpected = 17.0;
    double val = oMap[nIndex];
    ASSERT_DOUBLE_EQ(valexpected, val);
}// test_clearvalues_ptr

TEST_F(UnivarStatDescTest, test_clearvalues_vector) {
    const size_t n = sizeof (TEST_DATA) / sizeof (double);
    const double *pData = &(TEST_DATA[0]);
    //
    std::vector<double> data(pData, pData + n);
    std::map<size_t, double> oMap;
    UnivarStatDesc::clear_values(data, oMap);
    size_t nIndex = 9;
    ASSERT_TRUE(!oMap.empty());
    ASSERT_TRUE(oMap.find(nIndex) != oMap.end());
    double valexpected = 17.0;
    double val = oMap[nIndex];
    ASSERT_DOUBLE_EQ(valexpected, val);
}// test_clearvalues_vector

TEST_F(UnivarStatDescTest, test_clearvalues_valarray) {
    const size_t n = sizeof (TEST_DATA) / sizeof (double);
    const double *pData = &(TEST_DATA[0]);
    //
    std::valarray<double> data(pData, n);
    std::map<size_t, double> oMap;
    UnivarStatDesc::clear_values(data, oMap);
    size_t nIndex = 9;
    ASSERT_TRUE(!oMap.empty());
    ASSERT_TRUE(oMap.find(nIndex) != oMap.end());
    double valexpected = 17.0;
    double val = oMap[nIndex];
    ASSERT_DOUBLE_EQ(valexpected, val);
}// test_clearvalues_valarray

TEST_F(UnivarStatDescTest, test_reject_ptr) {
    const size_t n = sizeof (TEST_DATA) / sizeof (double);
    const double *pData = &(TEST_DATA[0]);
    //
    double valr = 17.0;
    double valn = 11.0;
    bool bRet = UnivarStatDesc::must_reject(n, pData, valr);
    ASSERT_TRUE(bRet);
    bRet = UnivarStatDesc::must_reject(n, pData, valn);
    ASSERT_TRUE(!bRet);
}// test_reject_ptr

TEST_F(UnivarStatDescTest, test_reject_vector) {
    const size_t n = sizeof (TEST_DATA) / sizeof (double);
    const double *pData = &(TEST_DATA[0]);
    //
    double valr = 17.0;
    double valn = 11.0;
    std::vector<double> data(pData, pData + n);
    bool bRet = UnivarStatDesc::must_reject(data, valr);
    ASSERT_TRUE(bRet);
    bRet = UnivarStatDesc::must_reject(data, valn);
    ASSERT_TRUE(!bRet);
}// test_reject_vector

TEST_F(UnivarStatDescTest, test_reject_valarray) {
    const size_t n = sizeof (TEST_DATA) / sizeof (double);
    const double *pData = &(TEST_DATA[0]);
    //
    double valr = 17.0;
    double valn = 11.0;
    std::valarray<double> data(pData, n);
    bool bRet = UnivarStatDesc::must_reject(data, valr);
    ASSERT_TRUE(bRet);
    bRet = UnivarStatDesc::must_reject(data, valn);
    ASSERT_TRUE(!bRet);
}// test_reject_valarray

TEST_F(UnivarStatDescTest, test_compute_vector) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::vector<float> &alldata = m_vecdata;
    os << std::endl << "===== TEST UNIVARSTATDESC COMPUTE VECTOR ======" << std::endl;
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        }// i
        os << "VARIABLE\t" << names[ivar] << std::endl;
        UnivarStatDesc oDesc;
        bool bRet = oDesc.compute(data);
        ASSERT_TRUE(bRet);
        os << std::endl << "UnivarStatDesc: ====== Test compute stats" << std::endl;
        os << "Count:\t" << oDesc.size() << std::endl;
        os << "Min:\t" << oDesc.min_value() << std::endl;
        os << "Max:\t" << oDesc.max_value() << std::endl;
        os << "Mean:\t" << oDesc.mean() << std::endl;
        os << "Median:\t" << oDesc.median() << std::endl;
        os << "Variance:\t" << oDesc.variance() << std::endl;
        os << "Std-Dev:\t" << oDesc.standard_deviation() << std::endl;
        os << "Skewness:\t" << oDesc.skewness() << std::endl;
        os << "Kurtosis:\t" << oDesc.kurtosis() << std::endl;
        os << "Quant05:\t" << oDesc.quant05() << std::endl;
        os << "Quant10:\t" << oDesc.quant10() << std::endl;
        os << "Quant25:\t" << oDesc.quant25() << std::endl;
        os << "Quant75:\t" << oDesc.quant75() << std::endl;
        os << "Quant90:\t" << oDesc.quant90() << std::endl;
        os << "Quant95:\t" << oDesc.quant95() << std::endl;
        os << std::endl;
    }// ivar
}// test_compute_vector

TEST_F(UnivarStatDescTest, test_compute_valarray) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::valarray<float> &alldata = m_valdata;
    os << std::endl << "===== TEST UNIVARSTATDESC COMPUTE VALARRAY ======" << std::endl;
    for (size_t ivar = 0; ivar < nCols; ++ivar) {
        std::valarray<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + ivar];
        }// i
        os << "VARIABLE\t" << names[ivar] << std::endl;
        UnivarStatDesc oDesc;
        bool bRet = oDesc.compute(data);
        ASSERT_TRUE(bRet);
        os << std::endl << "UnivarStatDesc: ====== Test compute stats" << std::endl;
        os << "Count:\t" << oDesc.size() << std::endl;
        os << "Min:\t" << oDesc.min_value() << std::endl;
        os << "Max:\t" << oDesc.max_value() << std::endl;
        os << "Mean:\t" << oDesc.mean() << std::endl;
        os << "Median:\t" << oDesc.median() << std::endl;
        os << "Variance:\t" << oDesc.variance() << std::endl;
        os << "Std-Dev:\t" << oDesc.standard_deviation() << std::endl;
        os << "Skewness:\t" << oDesc.skewness() << std::endl;
        os << "Kurtosis:\t" << oDesc.kurtosis() << std::endl;
        os << "Quant05:\t" << oDesc.quant05() << std::endl;
        os << "Quant10:\t" << oDesc.quant10() << std::endl;
        os << "Quant25:\t" << oDesc.quant25() << std::endl;
        os << "Quant75:\t" << oDesc.quant75() << std::endl;
        os << "Quant90:\t" << oDesc.quant90() << std::endl;
        os << "Quant95:\t" << oDesc.quant95() << std::endl;
        os << std::endl;
    }// ivar
}// test_compute_valarray

TEST_F(UnivarStatDescTest, test_series_mediane_vector) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::vector<float> &alldata = m_vecdata;
    //
    typedef StochasticIndependanceMedianTest<>::SignVectorType SignVectorType;
    //
    os << std::endl << "========== TEST SERIES MEDIANE VECTOR ==========" << std::endl;
    os << "VARIABLE\tMU\tTAU\tMUCRIT\tTAUCRIT\tRESULT" << std::endl;
    for (size_t icol = 0; icol < nCols; ++icol) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + icol];
        } // i
        std::random_shuffle(data.begin(), data.end());
        StochasticIndependanceMedianTest<> oTest(data);
        os << names[icol] << "\t";
        os << oTest.series_count() << "\t";
        os << oTest.max_series_length() << "\t";
        os << oTest.series_count_criteria() << "\t";
        os << oTest.max_series_length_criteria() << "\t";
        std::string sresult;
        oTest.result_string(sresult);
        os << sresult << "\t";
        std::string sr = "";
        const SignVectorType &v = oTest.signs();
        for (auto it = v.begin(); it != v.end(); ++it) {
            if (*it) {
                sr += "+";
            } else {
                sr += "-";
            }
        } // it
        os << std::endl;
    } // icol
}// test_series_mediane_vector

TEST_F(UnivarStatDescTest, test_series_mediane_valarray) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::valarray<float> &alldata = m_valdata;
    //
    typedef StochasticIndependanceMedianTest<>::SignVectorType SignVectorType;
    //
    os << std::endl << "========== TEST SERIES MEDIANE VALARRAY ==========" << std::endl;
    os << "VARIABLE\tMU\tTAU\tMUCRIT\tTAUCRIT\tRESULT" << std::endl;
    for (size_t icol = 0; icol < nCols; ++icol) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + icol];
        } // i
        std::random_shuffle(data.begin(), data.end());
        std::valarray<float> xdata(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            xdata[i] = data[i];
        }
        StochasticIndependanceMedianTest<> oTest(xdata);
        os << names[icol] << "\t";
        os << oTest.series_count() << "\t";
        os << oTest.max_series_length() << "\t";
        os << oTest.series_count_criteria() << "\t";
        os << oTest.max_series_length_criteria() << "\t";
        std::string sresult;
        oTest.result_string(sresult);
        os << sresult << "\t";
        std::string sr = "";
        const SignVectorType &v = oTest.signs();
        for (auto it = v.begin(); it != v.end(); ++it) {
            if (*it) {
                sr += "+";
            } else {
                sr += "-";
            }
        } // it
        os << std::endl;
    } // icol
}// test_series_mediane_valarray

TEST_F(UnivarStatDescTest, test_series_ascendantes_vector) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::vector<float> &alldata = m_vecdata;
    //
    typedef StochasticIndependanceMedianTest<>::SignVectorType SignVectorType;
    //
    os << std::endl << "========== TEST SERIES ASCENDANTES VECTOR ==========" << std::endl;
    os << "VARIABLE\tMU\tTAU\tMUCRIT\tTAUCRIT\tRESULT" << std::endl;
    for (size_t icol = 0; icol < nCols; ++icol) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + icol];
        } // i
        std::random_shuffle(data.begin(), data.end());
        StochasticIndependanceAscDescTest<> oTest(data);
        os << names[icol] << "\t";
        os << oTest.series_count() << "\t";
        os << oTest.max_series_length() << "\t";
        os << oTest.series_count_criteria() << "\t";
        os << oTest.max_series_length_criteria() << "\t";
        std::string sresult;
        oTest.result_string(sresult);
        os << sresult << "\t";
        std::string sr = "";
        const SignVectorType &v = oTest.signs();
        for (auto it = v.begin(); it != v.end(); ++it) {
            if (*it) {
                sr += "+";
            } else {
                sr += "-";
            }
        } // it
        os << std::endl;
    } // icol
}// test_series_ascendantes_vector

TEST_F(UnivarStatDescTest, test_series_ascendantes_valarray) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::valarray<float> &alldata = m_valdata;
    //
    typedef StochasticIndependanceAscDescTest<>::SignVectorType SignVectorType;
    //
    os << std::endl << "========== TEST SERIES MEDIANE VALARRAY ==========" << std::endl;
    os << "VARIABLE\tMU\tTAU\tMUCRIT\tTAUCRIT\tRESULT" << std::endl;
    for (size_t icol = 0; icol < nCols; ++icol) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + icol];
        } // i
        std::random_shuffle(data.begin(), data.end());
        std::valarray<float> xdata(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            xdata[i] = data[i];
        }
        StochasticIndependanceAscDescTest<> oTest(xdata);
        os << names[icol] << "\t";
        os << oTest.series_count() << "\t";
        os << oTest.max_series_length() << "\t";
        os << oTest.series_count_criteria() << "\t";
        os << oTest.max_series_length_criteria() << "\t";
        std::string sresult;
        oTest.result_string(sresult);
        os << sresult << "\t";
        std::string sr = "";
        const SignVectorType &v = oTest.signs();
        for (auto it = v.begin(); it != v.end(); ++it) {
            if (*it) {
                sr += "+";
            } else {
                sr += "-";
            }
        } // it
        os << std::endl;
    } // icol
}// test_series_ascendantes_valarray

TEST_F(UnivarStatDescTest, test_series_diff_vector) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::vector<float> &alldata = m_vecdata;
    //
    os << std::endl << "========== TEST SERIES DIFF VECTOR  ==========" << std::endl;
    os << "VARIABLE\tCRIT\tMINCRIT\tRESULT" << std::endl;
    for (size_t icol = 0; icol < nCols; ++icol) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + icol];
        } // i
        std::random_shuffle(data.begin(), data.end());
        StochasticIndependanceSquaredDiff oTest(data);
        os << names[icol] << "\t";
        os << oTest.criteria() << "\t";
        os << oTest.min_criteria() << "\t";
        std::string sresult;
        oTest.result_string(sresult);
        os << sresult << std::endl;
    } // icol
    os << std::endl;
}// test_series_diff_vector

TEST_F(UnivarStatDescTest, test_series_diff_valarray) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::valarray<float> &alldata = m_valdata;
    //
    os << std::endl << "========== TEST SERIES DIFF VALARRAY  ==========" << std::endl;
    os << "VARIABLE\tCRIT\tMINCRIT\tRESULT" << std::endl;
    for (size_t icol = 0; icol < nCols; ++icol) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + icol];
        } // i
        std::random_shuffle(data.begin(), data.end());
        std::valarray<float> xdata(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            xdata[i] = data[i];
        }
        StochasticIndependanceSquaredDiff oTest(xdata);
        os << names[icol] << "\t";
        os << oTest.criteria() << "\t";
        os << oTest.min_criteria() << "\t";
        std::string sresult;
        oTest.result_string(sresult);
        os << sresult << std::endl;
    } // icol
    os << std::endl;
}// test_series_diff_valarray

TEST_F(UnivarStatDescTest, test_skewness_kurtosis_vector) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::vector<float> &alldata = m_vecdata;
    //
    typedef StochasticIndependanceMedianTest<>::SignVectorType SignVectorType;
    //
    os << std::endl << "========== TEST NORMALITY BY SKEWNESS KURTOSIS VECTOR ==========" << std::endl;
    os << "VARIABLE\tSKEW\tKURT\tSKEWDEV\tKURTDEV\tRESULT" << std::endl;
    for (size_t icol = 0; icol < nCols; ++icol) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + icol];
        } // i
        std::random_shuffle(data.begin(), data.end());
        NormalityBySkewnessKurtosisTest oTest(data);
        os << names[icol] << "\t";
        os << oTest.skewness() << "\t";
        os << oTest.kurtosis() << "\t";
        os << oTest.skewness_deviation() << "\t";
        os << oTest.kurtosis_deviation() << "\t";
        std::string sresult;
        oTest.result_string(sresult);
        os << sresult << std::endl;
    } // icol
    os << std::endl;
}// test_skewness_kurtosis_vector

TEST_F(UnivarStatDescTest, test_skewness_kurtosis_valarray) {
    std::ofstream *pout = m_out.get();
    ASSERT_TRUE(pout != nullptr);
    ASSERT_TRUE(pout->is_open());
    std::ofstream &os = *pout;
    const size_t nCols = m_ncols;
    ASSERT_TRUE(nCols > 0);
    const size_t nRows = m_nrows;
    ASSERT_TRUE(nRows > 3);
    const std::vector<std::string> &names = m_varnames;
    const std::valarray<float> &alldata = m_valdata;
    //
    os << std::endl << "========== TEST NORMALITY BY SKEWNESS KURTOSIS VALARRAY ==========" << std::endl;
    os << "VARIABLE\tSKEW\tKURT\tSKEWDEV\tKURTDEV\tRESULT" << std::endl;
    for (size_t icol = 0; icol < nCols; ++icol) {
        std::vector<float> data(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            data[i] = alldata[i * nCols + icol];
        } // i
        std::random_shuffle(data.begin(), data.end());
        std::valarray<float> xdata(nRows);
        for (size_t i = 0; i < nRows; ++i) {
            xdata[i] = data[i];
        }
        NormalityBySkewnessKurtosisTest oTest(xdata);
        os << names[icol] << "\t";
        os << oTest.skewness() << "\t";
        os << oTest.kurtosis() << "\t";
        os << oTest.skewness_deviation() << "\t";
        os << oTest.kurtosis_deviation() << "\t";
        std::string sresult;
        oTest.result_string(sresult);
        os << sresult << std::endl;
    } // icol
    os << std::endl;
}// test_skewness_kurtosis_valarray
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

class UnivarStatDescTest : public CppUnit::TestFixture {
public:
    UnivarStatDescTest();
    virtual ~UnivarStatDescTest();
public:
    CPPUNIT_TEST_SUITE(UnivarStatDescTest);
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
CPPUNIT_TEST_SUITE_REGISTRATION(UnivarStatDescTest);
/////////////////////////////////////////////
const char *UnivarStatDescTest::TEST_SFILENAME = "./data/Groupes_GTE1.tsv";
const wchar_t *UnivarStatDescTest::TEST_WFILENAME = L"./data/Groupes_GTE1.tsv";
////////////////////////////////////////////

UnivarStatDescTest::UnivarStatDescTest() :
m_sfilename(TEST_SFILENAME), m_wfilename(TEST_WFILENAME) {
}

UnivarStatDescTest::~UnivarStatDescTest() {
}

void UnivarStatDescTest::setUp(void) {
} // setUp

void UnivarStatDescTest::tearDown(void) {
} // tearDown

void UnivarStatDescTest::test_read_string(void) {
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

void UnivarStatDescTest::test_read_wstring(void) {
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

