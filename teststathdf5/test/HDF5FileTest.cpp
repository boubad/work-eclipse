/*
 * HDF5FileTest.cpp
 *
 *  Created on: 21 oct. 2012
 *      Author: boubad
 */
#include <gtest/gtest.h>
/////////////////////////////
#include <hdf5store.h>
#include <sstream>
#include <memory>
///////////////////////////
using namespace statdata;
using namespace statdata::hdf5;
/////////////////////////////
class HDF5FileTest: public ::testing::Test {
public:
	HDF5FileTest() :
			m_filename("testresults/mytest.h5"), m_grpname("g_initial"),m_datasetname("testdataset") {
	}
	virtual ~HDF5FileTest() {
	}
protected:
	virtual void SetUp() {
		this->m_ofile.reset(new HDF5File());
		HDF5File *pFile = this->m_ofile.get();
		ASSERT_TRUE(pFile != nullptr);
		bool bRet = pFile->create(this->m_filename);
		ASSERT_TRUE(bRet);
		{
			HDF5Group oGroup(pFile, this->m_grpname, true);
		}
		{
			HDF5Dataset att(pFile, this->m_datasetname);
			size_t n = 10;
			std::vector<int> data(n);
			for (size_t i = 0; i < n; ++i) {
				data[i] = i + 1;
			}
			bRet = att.write_vector(data);
			ASSERT_TRUE(bRet);
		}
	} // SetUp
	virtual void TearDown() {
		this->m_ofile.reset();
	} // TearDown
protected:
	std::string m_filename;
	std::string m_grpname;
	std::string m_datasetname;
	std::unique_ptr<HDF5File> m_ofile;
};
///////////////////////////////////////
TEST_F(HDF5FileTest,testExists) {
	HDF5File *pFile = this->m_ofile.get();
	ASSERT_TRUE(pFile != nullptr);
	bool bRet = pFile->exists();
	ASSERT_TRUE(bRet);
} // testExists
TEST_F(HDF5FileTest,testGetName) {
	HDF5File *pFile = this->m_ofile.get();
	ASSERT_TRUE(pFile != nullptr);
	std::string name;
	bool bRet = pFile->get_name(name);
	ASSERT_TRUE(bRet);
	ASSERT_FALSE(name.empty());
	const char * p = name.c_str();
	ASSERT_TRUE(p != nullptr);
} // testGetName
TEST_F(HDF5FileTest,testGetGroupesNames) {
	HDF5File *pFile = this->m_ofile.get();
	ASSERT_TRUE(pFile != nullptr);
	std::vector<std::string> names;
	bool bRet = pFile->get_groups_names(names);
	ASSERT_TRUE(bRet);
	ASSERT_FALSE(names.empty());
	bool bFound = false;
	for (auto it = names.begin(); it != names.end(); ++it) {
		const std::string &s = *it;
		if (s == this->m_grpname) {
			bFound = true;
			break;
		}
	} // it
	ASSERT_TRUE(bFound);
} // testGetGroupesNames
TEST_F(HDF5FileTest,testGetDatasetNames) {
	HDF5File *pFile = this->m_ofile.get();
	ASSERT_TRUE(pFile != nullptr);
	std::vector<std::string> names;
	bool bRet = pFile->get_datasets_names(names);
	ASSERT_TRUE(bRet);
	ASSERT_FALSE(names.empty());
	bool bFound = false;
	for (auto it = names.begin(); it != names.end(); ++it) {
		const std::string &s = *it;
		if (s == this->m_datasetname) {
			bFound = true;
			break;
		}
	} // it
	ASSERT_TRUE(bFound);
} // testGetDatasetNames
///////////////////////////////////////
