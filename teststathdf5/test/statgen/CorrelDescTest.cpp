#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <correldesc.h>
#include <csvreader.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <fstream>
//////////////////////////////
using namespace statdata;
///////////////////////////////////

class CorrelDescTest : public ::testing::Test {
public:
    typedef double MyType;
public:

    CorrelDescTest() {
    }

    virtual ~CorrelDescTest() {
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
        m_odata.reset(new MyType[nTotal]);
        MyType *pData = m_odata.get();
        ASSERT_TRUE(pData != nullptr);
        for (size_t ivar = 0; ivar < nCols; ++ivar) {
            const std::vector<double> &vv = oArray[ivar];
            ASSERT_TRUE(vv.size() == nRows);
            for (size_t irow = 0; irow < nRows; ++irow) {
                size_t k = (size_t) (irow * nCols + ivar);
                double fval = vv[irow];
                m_vecdata[k] = fval;
                m_valdata[k] = fval;
                pData[k] = fval;
            }// irow
        }// ivar
    }//SetUpTestCase

    static void TearDownTestCase() {
        m_vecdata.clear();
        m_names.clear();
        m_odata.reset();
    }//TearDownTestCase
protected:
    static size_t m_nrows;
    static size_t m_ncols;
    static std::vector<MyType> m_vecdata;
    static std::valarray<MyType> m_valdata;
    static std::string m_filename;
    static std::vector<std::string> m_names;
    static std::unique_ptr<MyType> m_odata;
    static std::string m_outfilename_vector;
    static std::string m_outfilename_valarray;
};
size_t CorrelDescTest::m_nrows = 0;
size_t CorrelDescTest::m_ncols = 0;
std::vector<CorrelDescTest::MyType> CorrelDescTest::m_vecdata;
std::valarray<CorrelDescTest::MyType> CorrelDescTest::m_valdata;
std::string CorrelDescTest::m_filename("./data/testdata.tsv");
std::vector<std::string> CorrelDescTest::m_names;
std::unique_ptr<CorrelDescTest::MyType> CorrelDescTest::m_odata;
std::string CorrelDescTest::m_outfilename_vector("./testresults/correl_vector.txt");
std::string CorrelDescTest::m_outfilename_valarray("./testresults/correl_valarray.txt");
///////////////////////////////////////
using namespace statdata;
//////////////////////////////////////

TEST_F(CorrelDescTest, test_compute_vector) {
    const size_t nr = m_nrows;
    const size_t nv = m_ncols;
    const std::vector<std::string> & names = m_names;
    const std::vector<MyType> &data = m_vecdata;
    //
    std::ofstream os(m_outfilename_vector.c_str());
    os << std::endl << "============ TEST CORRELATIONS ====" << std::endl;
    //
    CorrelDescs<MyType> oDescs(nr, nv, names, data);
    oDescs.sort();
    for (auto it = oDescs.begin(); it != oDescs.end(); ++it) {
        const CorrelDesc<MyType> &c = *it;
        if (!c.reject_correlation()) {
            os << c << std::endl;
        }
    } // it
    CorrelDescs<MyType> oFirst, oFinal;
    std::string srefname("MATH1");
    os << std::endl << "Filtered by " << srefname << " Correlation" << std::endl;
    for (auto it = names.begin(); it != names.end(); ++it) {
        const std::string &sname1 = *it;
        if (sname1 != srefname) {
            for (auto jt = names.begin(); jt != it; ++jt) {
                const std::string &sname2 = *jt;
                if (sname2 != srefname) {
                    double vc = 0.0;
                    if (oDescs.partial_correlation(sname1, sname2, srefname, vc)) {
                        CorrelDesc<MyType> xc(nr, vc);
                        xc.first_name(sname1);
                        xc.second_name(sname2);
                        oFirst.add(xc);
                        os << xc << std::endl;
                    }
                }// ok
            }// jt
        }// sname1
    }// i
    os << std::endl;
    std::string srefname2("MATH2");
    std::set<std::string> xNames;
    oFirst.get_names(xNames);
    os << std::endl << "Filtered by " << srefname2 << " Correlation" << std::endl;
    for (auto it = xNames.begin(); it != xNames.end(); ++it) {
        const std::string &sname1 = *it;
        if (sname1 != srefname2) {
            for (auto jt = xNames.begin(); jt != it; ++jt) {
                const std::string &sname2 = *jt;
                if (sname2 != srefname2) {
                    double vc = 0.0;
                    if (oFirst.partial_correlation(sname1, sname2, srefname2, vc)) {
                        CorrelDesc<MyType> xc(nr, vc);
                        xc.first_name(sname1);
                        xc.second_name(sname2);
                        oFinal.add(xc);
                        os << xc << std::endl;
                    }
                }// ok
            }// jt
        }// sname1
    }// i
    os << std::endl;
} // test_compute_eigen_vrctor
TEST_F(CorrelDescTest, test_compute_valarray) {
    const size_t nr = m_nrows;
    const size_t nv = m_ncols;
    const std::vector<std::string> & names = m_names;
    const std::valarray<MyType> &data = m_valdata;
    //
    std::ofstream os(m_outfilename_valarray.c_str());
    os << std::endl << "============ TEST CORRELATIONS ====" << std::endl;
    //
    CorrelDescs<MyType> oDescs(nr, nv, names, data);
    oDescs.sort();
    for (auto it = oDescs.begin(); it != oDescs.end(); ++it) {
        const CorrelDesc<MyType> &c = *it;
        if (!c.reject_correlation()) {
            os << c << std::endl;
        }
    } // it
    CorrelDescs<MyType> oFirst, oFinal;
    std::string srefname("MATH1");
    os << std::endl << "Filtered by " << srefname << " Correlation" << std::endl;
    for (auto it = names.begin(); it != names.end(); ++it) {
        const std::string &sname1 = *it;
        if (sname1 != srefname) {
            for (auto jt = names.begin(); jt != it; ++jt) {
                const std::string &sname2 = *jt;
                if (sname2 != srefname) {
                    double vc = 0.0;
                    if (oDescs.partial_correlation(sname1, sname2, srefname, vc)) {
                        CorrelDesc<MyType> xc(nr, vc);
                        xc.first_name(sname1);
                        xc.second_name(sname2);
                        oFirst.add(xc);
                        os << xc << std::endl;
                    }
                }// ok
            }// jt
        }// sname1
    }// i
    os << std::endl;
    std::string srefname2("MATH2");
    std::set<std::string> xNames;
    oFirst.get_names(xNames);
    os << std::endl << "Filtered by " << srefname2 << " Correlation" << std::endl;
    for (auto it = xNames.begin(); it != xNames.end(); ++it) {
        const std::string &sname1 = *it;
        if (sname1 != srefname2) {
            for (auto jt = xNames.begin(); jt != it; ++jt) {
                const std::string &sname2 = *jt;
                if (sname2 != srefname2) {
                    double vc = 0.0;
                    if (oFirst.partial_correlation(sname1, sname2, srefname2, vc)) {
                        CorrelDesc<MyType> xc(nr, vc);
                        xc.first_name(sname1);
                        xc.second_name(sname2);
                        oFinal.add(xc);
                        os << xc << std::endl;
                    }
                }// ok
            }// jt
        }// sname1
    }// i
    os << std::endl;
} // test_compute_valarray



#else


#endif // NO_GTEST



