#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <dataset.h>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <fstream>
/////////////////////////////
using namespace statdata;
//////////////////////////////////////
class DataSetTest: public ::testing::Test {
public:

	DataSetTest() {
	}

	virtual ~DataSetTest() {
	}
protected:

protected:

	virtual void SetUp() {
	}

	virtual void TearDown() {

	}

	static void SetUpTestCase() {
		size_t nInds = m_st_nbindivs;
		m_oset.reset(new DataSet((size_t) 0, nInds));
		DataSet *pSet = m_oset.get();
		ASSERT_TRUE(pSet != nullptr);
		ASSERT_EQ(nInds, pSet->nb_indivs());
		ASSERT_EQ(3, pSet->nb_vars());
		m_stbooldata.resize(nInds);
		{
			for (size_t i = 0; i < nInds; ++i) {
				m_stbooldata[i] = ((::rand() % 2) == 0) ? true : false;
			} // i
			std::string sid("booldata");
			Variable *pVar = pSet->add_variable(sid, sid);
			ASSERT_TRUE(pVar != nullptr);
			pVar->set_type(statdata::typeBool);
			pVar->set_num_var(true);
			bool bRet = pSet->set_data(sid, m_stbooldata);
			ASSERT_TRUE(bRet);
		}
		{
			m_stshortdata.resize(nInds);
			for (size_t i = 0; i < nInds; ++i) {
				m_stshortdata[i] = (short) (::rand() % 100);
			} // i
			std::string sid("shortdata");
			Variable *pVar = pSet->add_variable(sid, sid);
			ASSERT_TRUE(pVar != nullptr);
			pVar->set_type(statdata::typeShort);
			pVar->set_num_var(true);
			bool bRet = pSet->set_data(sid, m_stshortdata);
			ASSERT_TRUE(bRet);
		}
		{
			m_stintdata.resize(nInds);
			for (size_t i = 0; i < nInds; ++i) {
				m_stintdata[i] = (int) (::rand() % 1000);
			} // i
			std::string sid("intdata");
			Variable *pVar = pSet->add_variable(sid, sid);
			ASSERT_TRUE(pVar != nullptr);
			pVar->set_type(statdata::typeInt);
			pVar->set_num_var(true);
			bool bRet = pSet->set_data(sid, m_stintdata);
			ASSERT_TRUE(bRet);
		}
		{
			m_stlongdata.resize(nInds);
			for (size_t i = 0; i < nInds; ++i) {
				m_stlongdata[i] = (long) (::rand() % 10000);
			} // i
			std::string sid("longdata");
			Variable *pVar = pSet->add_variable(sid, sid);
			ASSERT_TRUE(pVar != nullptr);
			pVar->set_type(statdata::typeLong);
			bool bRet = pSet->set_data(sid, m_stlongdata);
			ASSERT_TRUE(bRet);
		}
		{
			m_stfloatdata.resize(nInds);
			for (size_t i = 0; i < nInds; ++i) {
				m_stfloatdata[i] = (float) (::rand() % 20);
			} // i
			std::string sid("floatdata");
			Variable *pVar = pSet->add_variable(sid, sid);
			ASSERT_TRUE(pVar != nullptr);
			pVar->set_type(statdata::typeFloat);
			pVar->set_num_var(true);
			bool bRet = pSet->set_data(sid, m_stfloatdata);
			ASSERT_TRUE(bRet);
		}
		{
			m_stdoubledata.resize(nInds);
			for (size_t i = 0; i < nInds; ++i) {
				m_stdoubledata[i] = (double) (::rand() % 50);
			} // i
			std::string sid("doubledata");
			Variable *pVar = pSet->add_variable(sid, sid);
			ASSERT_TRUE(pVar != nullptr);
			pVar->set_type(statdata::typeDouble);
			pVar->set_num_var(true);
			bool bRet = pSet->set_data(sid, m_stdoubledata);
			ASSERT_TRUE(bRet);
		}
		{
			m_ststringdata.resize(nInds);
			for (size_t i = 0; i < nInds; ++i) {
				int ival = ::rand() % 5;
				std::stringstream os;
				os << "GR" << (ival + 1);
				m_ststringdata[i] = os.str();
			} // i
			std::string sid("stringdata");
			Variable *pVar = pSet->add_variable(sid, sid);
			ASSERT_TRUE(pVar != nullptr);
			pVar->set_type(statdata::typeString);
			pVar->set_categ_var(true);
			bool bRet = pSet->set_data(sid, m_ststringdata);
			ASSERT_TRUE(bRet);
		}
		ASSERT_EQ(10, pSet->nb_vars());
	} //SetUpTestCase

	static void TearDownTestCase() {
		m_oset.reset();
		m_stbooldata.clear();
		m_stshortdata.clear();
		m_stintdata.clear();
		m_stlongdata.clear();
		m_stfloatdata.clear();
		m_stdoubledata.clear();
		m_ststringdata.clear();
	} //TearDownTestCase
protected:
	static size_t m_st_nbindivs;
	static std::vector<bool> m_stbooldata;
	static std::vector<short> m_stshortdata;
	static std::vector<int> m_stintdata;
	static std::vector<long> m_stlongdata;
	static std::vector<float> m_stfloatdata;
	static std::vector<double> m_stdoubledata;
	static std::vector<std::string> m_ststringdata;
	static std::string m_starchivename;
	static std::string m_stfilename;
	static std::string m_stimportfilename;
	static std::unique_ptr<DataSet> m_oset;
};
size_t DataSetTest::m_st_nbindivs = 100;
std::vector<bool> DataSetTest::m_stbooldata;
std::vector<short> DataSetTest::m_stshortdata;
std::vector<int> DataSetTest::m_stintdata;
std::vector<long> DataSetTest::m_stlongdata;
std::vector<float> DataSetTest::m_stfloatdata;
std::vector<double> DataSetTest::m_stdoubledata;
std::vector<std::string> DataSetTest::m_ststringdata;
std::string DataSetTest::m_stfilename("testresults/my_dataset.txt");
std::string DataSetTest::m_starchivename("testresults/my_archive.txt");
std::string DataSetTest::m_stimportfilename("./data/Groupes_GTE1.tsv");
std::unique_ptr<DataSet> DataSetTest::m_oset;
///////////////////////////////////////
TEST_F(DataSetTest, miscVarTest) {
	DataSet *pSet = m_oset.get();
	ASSERT_TRUE(pSet != nullptr);
	//
	size_t nc = pSet->nb_vars();
	ASSERT_TRUE(nc > 0);
	size_t nr = pSet->nb_indivs();
	ASSERT_TRUE(nr > 0);
	std::vector<std::string> varnames;
	pSet->get_variables_ids(varnames);
	ASSERT_EQ(nc, varnames.size());
	//
	std::string xname;
	bool vRet = pSet->get_ids_variable(xname);
	ASSERT_TRUE(vRet);
	ASSERT_TRUE(!xname.empty());
	std::vector<std::string> ids;
	vRet = pSet->get_ids(ids);
	ASSERT_TRUE(vRet);
	ASSERT_EQ(nr, ids.size());
	//
	vRet = pSet->get_names_variable(xname);
	ASSERT_TRUE(vRet);
	ASSERT_TRUE(!xname.empty());
	std::vector<std::string> names;
	vRet = pSet->get_names(names);
	ASSERT_TRUE(vRet);
	ASSERT_EQ(nr, names.size());
	//
	vRet = pSet->get_weights_variable(xname);
	ASSERT_TRUE(vRet);
	ASSERT_TRUE(!xname.empty());
	std::vector<float> weights;
	vRet = pSet->get_weights(weights);
	ASSERT_TRUE(vRet);
	ASSERT_EQ(nr, weights.size());
	//
	for (size_t ivar = 0; ivar < nc; ++ivar) {
		int iIndex = (int) ivar;
		const std::string &varname = varnames[ivar];
		const Variable *pVar1 = pSet->get_variable(varname);
		ASSERT_TRUE(pVar1 != nullptr);
		const Variable *pVar2 = pSet->get_variable(iIndex);
		ASSERT_TRUE(pVar2 != nullptr);
		ASSERT_EQ(pVar1, pVar2);
		int iIndex2 = pSet->get_variable_index(varname);
		ASSERT_EQ(iIndex, iIndex2);
		std::vector<boost::any> *pdata = pSet->variable_data(varname);
		ASSERT_TRUE(pdata != nullptr);
		ASSERT_EQ(nr, pdata->size());
		if (pVar1->get_type() == typeString) {
			std::vector<std::string> vv;
			vRet = pSet->get_data(varname, vv);
			ASSERT_TRUE(vRet);
			ASSERT_EQ(nr, vv.size());
		} else if (pVar1->get_type() == typeBool) {
			std::vector<bool> vv;
			vRet = pSet->get_data(varname, vv);
			ASSERT_TRUE(vRet);
			ASSERT_EQ(nr, vv.size());
		} else {
			std::vector<float> vv;
			vRet = pSet->get_data(varname, vv);
			ASSERT_TRUE(vRet);
			ASSERT_EQ(nr, vv.size());
		}
		statdata::DataType tt = pSet->get_variable_type(varname);
		ASSERT_EQ(pVar1->get_type(), tt);
	} // ivar
	  //
	{
		std::vector<double> vdata(nr);
		for (size_t i = 0; i < nr; ++i) {
			vdata[i] = (double) (::rand() % 50);
		} // i
		std::string sid("testdata");
		Variable *pVar = pSet->add_variable(sid, sid);
		ASSERT_TRUE(pVar != nullptr);
		pVar->set_type(statdata::typeDouble);
		pVar->set_num_var(true);
		bool bRet = pSet->set_data(sid, vdata);
		ASSERT_TRUE(bRet);
		const Variable *pVar1 = pSet->get_variable(sid);
		ASSERT_TRUE(pVar1 != nullptr);
		size_t n = pSet->nb_vars();
		size_t nExpected = (size_t) (nc + 1);
		ASSERT_EQ(nExpected, n);
		int iIndex = pSet->get_variable_index(sid);
		const Variable *pVar2 = pSet->get_variable(iIndex);
		ASSERT_TRUE(pVar2 != nullptr);
		ASSERT_EQ(pVar1, pVar2);
		bRet = pSet->remove_variable(sid);
		ASSERT_TRUE(bRet);
		n = pSet->nb_vars();
		ASSERT_EQ(nc, n);
	}
} //miscVarTest
TEST_F(DataSetTest, miscIndTest) {
	DataSet *pSet = m_oset.get();
	ASSERT_TRUE(pSet != nullptr);
	//
	size_t nc = pSet->nb_vars();
	ASSERT_TRUE(nc > 0);
	size_t nr = pSet->nb_indivs();
	ASSERT_TRUE(nr > 0);
	std::vector<std::string> indnames;
	pSet->get_indivs_ids(indnames);
	ASSERT_EQ(nr, indnames.size());
	for (size_t irow = 0; irow < nr; ++irow) {
		int iIndex = (int) irow;
		const std::string &sid = indnames[irow];
		const Individu *pInd1 = pSet->get_indiv(sid);
		ASSERT_TRUE(pInd1 != nullptr);
		const Individu *pInd2 = pSet->get_indiv(iIndex);
		ASSERT_TRUE(pInd2 != nullptr);
		ASSERT_EQ(pInd1, pInd2);
		int iIndex2 = pSet->get_indiv_index(sid);
		ASSERT_EQ(iIndex, iIndex2);
		std::map<std::string, boost::any> oMap1;
		bool bRet = pSet->indiv_data(sid, oMap1);
		ASSERT_TRUE(bRet);
		ASSERT_EQ(nc, oMap1.size());
		std::map<std::string, boost::any> oMap2;
		bRet = pSet->indiv_data(iIndex, oMap2);
		ASSERT_TRUE(bRet);
		ASSERT_EQ(nc, oMap2.size());
	} // ivar
	  //
	{
		std::string sid("testind");
		Individu *pInd = pSet->add_indiv(sid, sid);
		ASSERT_TRUE(pInd != nullptr);
		const Individu *pInd1 = pSet->get_indiv(sid);
		ASSERT_TRUE(pInd1 != nullptr);
		size_t n = pSet->nb_indivs();
		size_t nExpected = (size_t) (nr + 1);
		ASSERT_EQ(nExpected, n);
		int iIndex = pSet->get_indiv_index(sid);
		const Individu *pInd2 = pSet->get_indiv(iIndex);
		ASSERT_TRUE(pInd2 != nullptr);
		ASSERT_EQ(pInd1, pInd2);
		bool bRet = pSet->remove_indiv(sid);
		ASSERT_TRUE(bRet);
		n = pSet->nb_indivs();
		ASSERT_EQ(nr, n);
	}
} //miscIndTest
////////////////////////////////////////////
#ifndef NO_SERIALIZATION
TEST_F(DataSetTest, saveArchiveTest) {
	DataSet *pSet = m_oset.get();
	ASSERT_TRUE(pSet != nullptr);
	bool bRet = pSet->save_archive_file(m_starchivename);
	ASSERT_TRUE(bRet);
} //saveArchiveTest

TEST_F(DataSetTest, readArchiveTest) {
	std::string filename = m_starchivename;
	{
		DataSet *pSet = m_oset.get();
		ASSERT_TRUE(pSet != nullptr);
		bool bRet = pSet->save_archive_file(filename);
		ASSERT_TRUE(bRet);
	}
	DataSet *pSet = m_oset.get();
	ASSERT_TRUE(pSet != nullptr);
	DataSet oSet;
	bool bRet = oSet.import_archive_file(filename);
	ASSERT_TRUE(bRet);
	std::vector<std::string> categ_varnames;
	oSet.get_categ_vars_ids(categ_varnames);
	ASSERT_TRUE(!categ_varnames.empty());
	for (auto it = categ_varnames.begin(); it != categ_varnames.end(); ++it) {
		const std::string &varname = *it;
		std::vector<std::string> vec1;
		bRet = oSet.get_data(varname, vec1);
		ASSERT_TRUE(bRet);
		std::vector<std::string> vec2;
		bRet = pSet->get_data(varname, vec2);
		ASSERT_TRUE(bRet);
		size_t n = vec1.size();
		ASSERT_EQ(n, vec2.size());
		for (size_t i = 0; i < n; ++i) {
			ASSERT_EQ(vec1[i], vec2[i]);
		} //
	} // it
	std::vector<std::string> num_varnames;
	oSet.get_num_vars_ids(num_varnames);
	ASSERT_TRUE(!num_varnames.empty());
	for (auto it = num_varnames.begin(); it != num_varnames.end(); ++it) {
		const std::string &varname = *it;
		const Variable *pv = oSet.get_variable(varname);
		ASSERT_TRUE(pv != nullptr);
		statdata::DataType tt = pv->get_type();
		if ((tt == typeShort) || (tt == typeInt) || (tt == typeLong)
				|| (tt == typeFloat) || (tt == typeDouble)) {
			std::vector<float> vec1;
			bRet = oSet.get_data(varname, vec1);
			ASSERT_TRUE(bRet);
			std::vector<float> vec2;
			bRet = pSet->get_data(varname, vec2);
			ASSERT_TRUE(bRet);
			size_t n = vec1.size();
			ASSERT_EQ(n, vec2.size());
			for (size_t i = 0; i < n; ++i) {
				ASSERT_FLOAT_EQ(vec1[i], vec2[i]);
			} //
		} // tt
	} // it
} //saveArchiveTest
#endif // NO_SERIALIZATION
TEST_F(DataSetTest, saveCSVTest) {
	DataSet *pSet = m_oset.get();
	ASSERT_TRUE(pSet != nullptr);
	bool bRet = pSet->save_csv_file(m_stfilename);
	ASSERT_TRUE(bRet);
} //saveArchiveTest
TEST_F(DataSetTest, readCSVTest) {
	std::string filename = m_stfilename;
	{
		DataSet *pSet = m_oset.get();
		ASSERT_TRUE(pSet != nullptr);
		bool bRet = pSet->save_csv_file(filename);
		ASSERT_TRUE(bRet);
	}
	DataSet *pSet = m_oset.get();
	ASSERT_TRUE(pSet != nullptr);
	DataSet oSet;
	bool bRet = oSet.import_csv_file(filename);
	ASSERT_TRUE(bRet);
	std::vector<std::string> categ_varnames;
	oSet.get_categ_vars_ids(categ_varnames);
	ASSERT_TRUE(!categ_varnames.empty());
	for (auto it = categ_varnames.begin(); it != categ_varnames.end(); ++it) {
		const std::string &varname = *it;
		std::vector<std::string> vec1;
		bRet = oSet.get_data(varname, vec1);
		ASSERT_TRUE(bRet);
		std::vector<std::string> vec2;
		bRet = pSet->get_data(varname, vec2);
		ASSERT_TRUE(bRet);
		size_t n = vec1.size();
		ASSERT_EQ(n, vec2.size());
		for (size_t i = 0; i < n; ++i) {
			ASSERT_EQ(vec1[i], vec2[i]);
		} //
	} // it
	std::vector<std::string> num_varnames;
	oSet.get_num_vars_ids(num_varnames);
	ASSERT_TRUE(!num_varnames.empty());
	for (auto it = num_varnames.begin(); it != num_varnames.end(); ++it) {
		const std::string &varname = *it;
		const Variable *pv = oSet.get_variable(varname);
		ASSERT_TRUE(pv != nullptr);
		statdata::DataType tt = pv->get_type();
		if ((tt == typeShort) || (tt == typeInt) || (tt == typeLong)
				|| (tt == typeFloat) || (tt == typeDouble)) {
			std::vector<float> vec1;
			bRet = oSet.get_data(varname, vec1);
			ASSERT_TRUE(bRet);
			std::vector<float> vec2;
			bRet = pSet->get_data(varname, vec2);
			ASSERT_TRUE(bRet);
			size_t n = vec1.size();
			ASSERT_EQ(n, vec2.size());
			for (size_t i = 0; i < n; ++i) {
				ASSERT_FLOAT_EQ(vec1[i], vec2[i]);
			} //
		} // tt
	} // it
} //readCSVTest
#else
////////////////////////////////
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
///////////////////////////////////
#include <dataset.h>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <fstream>
/////////////////////////////////
using namespace statdata;
/////////////////////////////////////

class DataSetTest : public CppUnit::TestFixture {
public:
	DataSetTest();
	virtual ~DataSetTest();
public:
	CPPUNIT_TEST_SUITE(DataSetTest);
	CPPUNIT_TEST(saveArchiveTest);
	CPPUNIT_TEST(readArchiveTest);
	CPPUNIT_TEST(saveCSVTest);
	CPPUNIT_TEST(readCSVTest);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp(void);
	void tearDown(void);
protected:
	void saveArchiveTest(void);
	void readArchiveTest(void);
	void saveCSVTest(void);
	void readCSVTest(void);
private:
	size_t m_st_nbindivs;
	std::vector<bool> m_stbooldata;
	std::vector<short> m_stshortdata;
	std::vector<int> m_stintdata;
	std::vector<long> m_stlongdata;
	std::vector<float> m_stfloatdata;
	std::vector<double> m_stdoubledata;
	std::vector<std::string> m_ststringdata;
	std::string m_stfilename;
	std::string m_starchivename;
	std::string m_stimportfilename;
	std::unique_ptr<DataSet> m_oset;
};
//////////////////////////////////
CPPUNIT_TEST_SUITE_REGISTRATION(DataSetTest);
////////////////////////////////////////////

DataSetTest::DataSetTest() : m_st_nbindivs(100),
m_stfilename("testresults/my_dataset.txt"),
m_starchivename("testresults/my_archive.txt"),
m_stimportfilename("./data/Groupes_GTE1.tsv") {
}

DataSetTest::~DataSetTest() {

}

void DataSetTest::setUp(void) {
	size_t nInds = this->m_st_nbindivs;
	CPPUNIT_ASSERT(nInds > 0);
	this->m_oset.reset(new DataSet((size_t) 0, nInds));
	DataSet *pSet = this->m_oset.get();
	CPPUNIT_ASSERT(pSet != nullptr);
	CPPUNIT_ASSERT(nInds == pSet->nb_indivs());
	CPPUNIT_ASSERT(3 == pSet->nb_vars());
	this->m_stbooldata.resize(nInds);
	CPPUNIT_ASSERT(this->m_stbooldata.size() == nInds);
	{
		for (size_t i = 0; i < nInds; ++i) {
			this->m_stbooldata[i] = ((::rand() % 2) == 0) ? true : false;
		} // i
		std::string sid("booldata");
		Variable *pVar = pSet->add_variable(sid, sid);
		CPPUNIT_ASSERT(pVar != nullptr);
		pVar->set_type(statdata::typeBool);
		bool bRet = pSet->set_data(sid, m_stbooldata);
		CPPUNIT_ASSERT(bRet);
	}
	{
		this->m_stshortdata.resize(nInds);
		CPPUNIT_ASSERT(this->m_stshortdata.size() == nInds);
		for (size_t i = 0; i < nInds; ++i) {
			this->m_stshortdata[i] = (short) (::rand() % 100);
		} // i
		std::string sid("shortdata");
		Variable *pVar = pSet->add_variable(sid, sid);
		CPPUNIT_ASSERT(pVar != nullptr);
		pVar->set_type(statdata::typeShort);
		bool bRet = pSet->set_data(sid, m_stshortdata);
		CPPUNIT_ASSERT(bRet);
	}
	{
		this->m_stintdata.resize(nInds);
		CPPUNIT_ASSERT(this->m_stintdata.size() == nInds);
		for (size_t i = 0; i < nInds; ++i) {
			this->m_stintdata[i] = (int) (::rand() % 1000);
		} // i
		std::string sid("intdata");
		Variable *pVar = pSet->add_variable(sid, sid);
		CPPUNIT_ASSERT(pVar != nullptr);
		pVar->set_type(statdata::typeInt);
		bool bRet = pSet->set_data(sid, m_stintdata);
		CPPUNIT_ASSERT(bRet);
	}
	{
		this->m_stlongdata.resize(nInds);
		CPPUNIT_ASSERT(this->m_stlongdata.size() == nInds);
		for (size_t i = 0; i < nInds; ++i) {
			this->m_stlongdata[i] = (long) (::rand() % 10000);
		} // i
		std::string sid("longdata");
		Variable *pVar = pSet->add_variable(sid, sid);
		CPPUNIT_ASSERT(pVar != nullptr);
		pVar->set_type(statdata::typeLong);
		bool bRet = pSet->set_data(sid, m_stlongdata);
		CPPUNIT_ASSERT(bRet);
	}
	{
		this->m_stfloatdata.resize(nInds);
		CPPUNIT_ASSERT(this->m_stfloatdata.size() == nInds);
		for (size_t i = 0; i < nInds; ++i) {
			this->m_stfloatdata[i] = (float) (::rand() % 20);
		} // i
		std::string sid("floatdata");
		Variable *pVar = pSet->add_variable(sid, sid);
		CPPUNIT_ASSERT(pVar != nullptr);
		pVar->set_type(statdata::typeFloat);
		bool bRet = pSet->set_data(sid, m_stfloatdata);
		CPPUNIT_ASSERT(bRet);
	}
	{
		this->m_stdoubledata.resize(nInds);
		CPPUNIT_ASSERT(this->m_stdoubledata.size() == nInds);
		for (size_t i = 0; i < nInds; ++i) {
			this->m_stdoubledata[i] = (double) (::rand() % 50);
		} // i
		std::string sid("doubledata");
		Variable *pVar = pSet->add_variable(sid, sid);
		CPPUNIT_ASSERT(pVar != nullptr);
		pVar->set_type(statdata::typeDouble);
		bool bRet = pSet->set_data(sid, m_stdoubledata);
		CPPUNIT_ASSERT(bRet);
	}
	{
		this->m_ststringdata.resize(nInds);
		CPPUNIT_ASSERT(this->m_ststringdata.size() == nInds);
		for (size_t i = 0; i < nInds; ++i) {
			int ival = ::rand() % 5;
			std::stringstream os;
			os << "GR" << (ival + 1);
			this->m_ststringdata[i] = os.str();
		} // i
		std::string sid("stringdata");
		Variable *pVar = pSet->add_variable(sid, sid);
		CPPUNIT_ASSERT(pVar != nullptr);
		pVar->set_type(statdata::typeString);
		bool bRet = pSet->set_data(sid, m_ststringdata);
		CPPUNIT_ASSERT(bRet);
	}
	CPPUNIT_ASSERT(10 == pSet->nb_vars());
}

void DataSetTest::tearDown(void) {
	this->m_oset.reset();
	this->m_stbooldata.clear();
	this->m_stshortdata.clear();
	this->m_stintdata.clear();
	this->m_stlongdata.clear();
	this->m_stfloatdata.clear();
	this->m_stdoubledata.clear();
	this->m_ststringdata.clear();
}

void DataSetTest::saveArchiveTest(void) {
	DataSet *pSet = this->m_oset.get();
	CPPUNIT_ASSERT(pSet != nullptr);
	bool bRet = pSet->save_archive_file(this->m_starchivename);
	CPPUNIT_ASSERT(bRet);
}

void DataSetTest::readArchiveTest(void) {
	std::string filename = this->m_starchivename;
	{
		std::ifstream in(filename.c_str());
		if (!in.is_open()) {
			DataSet *pSet = this->m_oset.get();
			CPPUNIT_ASSERT(pSet != nullptr);
			bool bRet = pSet->save_archive_file(filename);
			CPPUNIT_ASSERT(bRet);
		}
	}
	DataSet oSet;
	bool bRet = oSet.import_archive_file(filename);
	CPPUNIT_ASSERT(bRet);
}

void DataSetTest::saveCSVTest(void) {
	DataSet *pSet = this->m_oset.get();
	CPPUNIT_ASSERT(pSet != nullptr);
	bool bRet = pSet->save_csv_file(this->m_stfilename);
	CPPUNIT_ASSERT(bRet);
}

void DataSetTest::readCSVTest(void) {
	std::string filename = this->m_stimportfilename;
	std::ifstream in(filename.c_str());
	CPPUNIT_ASSERT(in.is_open());
	DataSet oSet;
	bool bRet = oSet.import_csv_file(filename);
	CPPUNIT_ASSERT(bRet);
}
#endif // NO_GTEST
