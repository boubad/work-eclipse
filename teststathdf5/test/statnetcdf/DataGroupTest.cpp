#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <datagroup.h>
#include <sstream>
#include <fstream>
////////////////////////////
#include "../DataFixture.h"

/////////////////////////////////
using namespace statdata;
/////////////////////////////

class MyDataFixture {
public:
    MyDataFixture();
    ~MyDataFixture();
public:

    const DataFixture & get_data(void) const {
        return (this->m_odata);
    }

    const std::string & ncfilename(void) const {
        return (this->m_ncfilename);
    }

    const char *get_indivs_dim_name(void) const {
        return (DIM_INDIVS);
    }

    const char *get_indivsnames_varname(void) const {
        return (VAR_INDIVS_NAME);
    }
    bool get_dataset_file(statdata::NetCDFDataGroup::NetCDFDataGroupPtr &oPtr,
            statdata::datafile_open_mode mode = statdata::mode_read,
            bool bClassic = true);
private:
    const std::string m_ncfilename;
    static DataFixture m_odata;
    //
    static const char *TEST_NC_FILENAME;
    static const char *DIM_INDIVS;
    static const char *VAR_INDIVS_NAME;
    static const char *ATT_VAR_INDIVS_LONGNAME;
};
//////////////////////////////////
DataFixture MyDataFixture::m_odata;
const char *MyDataFixture::DIM_INDIVS = "indivs";
const char *MyDataFixture::VAR_INDIVS_NAME = "Names";
const char *MyDataFixture::ATT_VAR_INDIVS_LONGNAME = "Noms des individus";
const char *MyDataFixture::TEST_NC_FILENAME = "./testresults/my_datagroup.nc";
//////////////////////////////////////////

MyDataFixture::MyDataFixture() :
m_ncfilename(TEST_NC_FILENAME) {
}

MyDataFixture::~MyDataFixture() {
}

bool MyDataFixture::get_dataset_file(statdata::NetCDFDataGroup::NetCDFDataGroupPtr &oPtr,
        statdata::datafile_open_mode mode /*= statdata::mode_read */,
        bool bClassic /* true */) {
    bool bExists = false;
    {
        std::ifstream in(this->m_ncfilename.c_str());
        bExists = in.is_open();
        if (bExists) {
            in.close();
        }
    }
    if (!bExists) {
        DataFixture oData;
        DataFixture *pData = &oData;
        NetCDFDataGroup oGroup;
        NetCDFDataGroup *g = &oGroup;
        if (!g->open(this->m_ncfilename, statdata::mode_replace, bClassic)) {
            return (false);
        }
        NetCDFDimension oDim;
        std::string dimname(DIM_INDIVS);
        if (!g->create_dimension(oDim, dimname)) {
            return (false);
        }
        NetCDFVariable oNamesVar;
        oNamesVar.name = VAR_INDIVS_NAME;
        oNamesVar.type = typeString;
        oNamesVar.long_name = ATT_VAR_INDIVS_LONGNAME;
        oNamesVar.dims.push_back(oDim);
        if (!g->create_variable(oNamesVar)) {
            return (false);
        }
        const size_t nRows = pData->rows();
        Values xnames(nRows);
        const std::vector<std::string> &rownames = pData->rownames();
        for (size_t i = 0; i < nRows; ++i) {
            xnames[i] = rownames[i];
        }
        std::valarray<size_t> index(1), count(1);
        index[0] = 0;
        count[0] = nRows;
        if (!g->write_data(oNamesVar.name, index, count, xnames)) {
            return (false);
        }
        const size_t nCols = pData->cols();
        const std::vector<std::string> &varnames = pData->colnames();
        const std::vector<std::string> &long_varnames = pData->col_longnames();
        const std::vector<std::string> &units = pData->col_units();
        const std::vector<std::string> &formats = pData->col_cformat();
        float fMissing = std::numeric_limits<float>::quiet_NaN();
        const std::vector<float> &initialdata = pData->initial_data();
        for (size_t icol = 0; icol < nCols; ++icol) {
            std::string vname = varnames[icol];
            NetCDFVariable var;
            var.name = vname;
            var.type = typeFloat;
            var.long_name = long_varnames[icol];
            var.c_format = formats[icol];
            var.dims.push_back(oDim);
            var.missing_value = fMissing;
            var.units = units[icol];
            var.valid_min = 0.0f;
            var.valid_max = 20.0f;
            if (!g->create_variable(var)) {
                return (false);
            }
            Values vv(nRows);
            for (size_t i = 0; i < nRows; ++i) {
                vv[i] = initialdata[i * nCols + icol];
            }
            if (!g->write_data(vname, index, count, vv)) {
                return (false);
            }
        } // icol
        if (!g->close()) {
            return (false);
        }
    } // not_exists
    oPtr.reset(new NetCDFDataGroup(this->m_ncfilename, mode, bClassic));
    NetCDFDataGroup *p = oPtr.get();
    if (p != nullptr) {
        return (p->is_open());
    }
    return false;
} // get_dataset_file
/////////////////////////////////

class DataGroupTest : public ::testing::Test {
public:

    DataGroupTest() {
    }

    virtual ~DataGroupTest() {
    }

    virtual void SetUp() {

    }// SetUp

    virtual void TearDown() {


    }
protected:

    const DataFixture & get_data(void) const {
        static MyDataFixture oFixture;
        return (oFixture.get_data());
    }

    bool get_dataset_file(statdata::NetCDFDataGroup::NetCDFDataGroupPtr &oPtr,
            statdata::datafile_open_mode mode = statdata::mode_read,
            bool bClassic = true) {
        return (m_ofixture.get_dataset_file(oPtr, mode, bClassic));
    } // get_dataset_fie

    const char *get_indivs_dim_name(void) const {
        return (m_ofixture.get_indivs_dim_name());
    }

    const char *get_indivsnames_varname(void) const {
        return (m_ofixture.get_indivsnames_varname());
    }
protected:
    static MyDataFixture m_ofixture;

};
/////////////////////////////////////
MyDataFixture DataGroupTest::m_ofixture;
//////////////////////////////////////////////

TEST_F(DataGroupTest, test_read_data) {
    NetCDFDataGroup::NetCDFDataGroupPtr oPtr;
    bool bRet = this->get_dataset_file(oPtr);
    ASSERT_TRUE(bRet);
    NetCDFDataGroup *g = oPtr.get();
    ASSERT_TRUE(g != nullptr);
    const DataFixture *pData = &(this->get_data());
    ASSERT_TRUE(pData != nullptr);
    const size_t nRows = pData->rows();
    const size_t nCols = pData->cols();
    //
    std::valarray<size_t> index(1), count(1);
    Values values;
    std::string varname = this->get_indivsnames_varname();
    index[0] = 0;
    count[0] = 0;
    bRet = g->read_data(varname, index, count, values);
    ASSERT_TRUE(bRet);
    ASSERT_TRUE(index[0] == 0);
    ASSERT_EQ(nRows, count[0]);
    const std::vector<std::string> &names = pData->rownames();
    for (size_t i = 0; i < nRows; ++i) {
        const boost::any &v = values[i];
        ASSERT_TRUE(!v.empty());
        ASSERT_TRUE(v.type() == typeid (std::string));
        std::string ss;
        ASSERT_TRUE(Value::get_value(v, ss));
        const std::string &sx = names[i];
        ASSERT_EQ(sx, ss);
    } // i
    const std::vector<std::string> & vnames = pData->colnames();
    const std::vector<float> &data = pData->initial_data();
    ASSERT_TRUE((data.size() == (size_t) (nRows * nCols)));
    for (size_t icol = 0; icol < nCols; ++icol) {
        const std::string &vname = vnames[icol];
        index[0] = 0;
        count[0] = 0;
        bRet = g->read_data(vname, index, count, values);
        ASSERT_TRUE(bRet);
        ASSERT_TRUE(index[0] == 0);
        ASSERT_TRUE(count[0] == nRows);
        for (size_t i = 0; i < nRows; ++i) {
            float f = data[i * nCols + icol];
            const boost::any &v = values[i];
            float factual;
            ASSERT_TRUE(Value::get_value(v, factual));
            ASSERT_FLOAT_EQ(f, factual);
        } // i
    } // icol
    bRet = g->close();
    ASSERT_TRUE(bRet);
}/* test_read_data */

TEST_F(DataGroupTest, test_create_axis) {
    NetCDFDataGroup::NetCDFDataGroupPtr oPtr;
    bool bRet = this->get_dataset_file(oPtr, statdata::mode_write);
    ASSERT_TRUE(bRet);
    NetCDFDataGroup *g = oPtr.get();
    ASSERT_TRUE(g != nullptr);
    const DataFixture *pData = &(this->get_data());
    ASSERT_TRUE(pData != nullptr);
    const size_t nRows = pData->rows();
    const size_t nCols = pData->cols();
    std::string rowname("rows");
    std::string colname("cols");
    std::string sunits;
    std::string varname("ALLUES");
    std::string longvarname("Moyennes des UE");
    std::string cformat("%.4g");
    float fDelta = 1.0f;
    float fMin = 0.0f;
    float fMaxCol = (float) (nCols - 1);
    float fMaxRow = (float) (nRows - 1);
    const std::vector<float> &data = pData->initial_data();
    bRet = g->create_axis(rowname, sunits, fMin, fMaxRow, fDelta);
    ASSERT_TRUE(bRet);
    bRet = g->create_axis(colname, sunits, fMin, fMaxCol, fDelta);
    ASSERT_TRUE(bRet);
    NetCDFDimension oDimCol, oDimRow;
    bRet = g->get_dimension(rowname, oDimRow);
    ASSERT_TRUE(bRet);
    bRet = g->get_dimension(colname, oDimCol);
    ASSERT_TRUE(bRet);
    NetCDFVariable var;
    var.name = varname;
    var.type = typeFloat;
    var.long_name = longvarname;
    var.c_format = cformat;
    var.dims.push_back(oDimRow);
    var.dims.push_back(oDimCol);
    var.valid_min = 0.0f;
    var.valid_max = 20.0f;
    bRet = g->create_variable(var);
    ASSERT_TRUE(bRet);
    std::valarray<size_t> index(2), count(2);
    index[0] = 0;
    index[1] = 0;
    count[0] = oDimRow.len;
    count[1] = oDimCol.len;
    const size_t nn = data.size();
    Values vv(nn);
    for (size_t i = 0; i < nn; ++i) {
        vv[i] = data[i];
    }
    bRet = g->write_data(varname, index, count, vv);
    bRet = g->close();
    ASSERT_TRUE(bRet);
}

#else
#endif // NO_GTEST


