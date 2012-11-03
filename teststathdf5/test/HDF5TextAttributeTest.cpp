/*
 * HDF5TextAttributeTest.cpp
 *
 *  Created on: 20 oct. 2012
 *      Author: boubad
 */
#include <gtest/gtest.h>
/////////////////////////////
#include <hdf5store.h>
#include <sstream>
/////////////////////////////
class HDF5TextAttributeTest : public ::testing::Test {
public:
	HDF5TextAttributeTest():m_filename("mytest.h5"),m_grpname("g_initial"){}
	virtual ~HDF5TextAttributeTest(){}
 protected:
  std::string m_filename;
  std::string m_grpname;
};
///////////////////////////////////////
using namespace statdata;
using namespace statdata::hdf5;
//////////////////////////////////////
TEST_F(HDF5TextAttributeTest,ScalarAttribute)
{
	std::string v("0123456789");
	HDF5File oFile;
	bool bRet = oFile.create(this->m_filename);
	ASSERT_TRUE(bRet);
	HDF5Group oGroup(&oFile, this->m_grpname, true);
	std::string name("att_string_scalar");
	HDF5Attribute att(&oGroup,name);
	bRet = att.write_scalar(v);
	ASSERT_TRUE(bRet);
	std::string actual;
	bRet = att.read_scalar(actual);
	ASSERT_TRUE(bRet);
	ASSERT_EQ(v, actual);
}/* Scalar Attribute */
TEST_F(HDF5TextAttributeTest,VectorAttribute)
{
	size_t n = 10;
	std::vector<std::string> values(n);
	for (size_t i = 0; i < n; ++i){
		std::stringstream os;
		os << "val" << (i+1);
		values[i] = os.str();
	}// i
	HDF5File oFile;
	bool bRet = oFile.create(this->m_filename);
	ASSERT_TRUE(bRet);
	HDF5Group oGroup(&oFile, this->m_grpname, true);
	std::string name("att_string_vector");
	HDF5Attribute att(&oGroup,name);
	bRet = att.write_vector(values);
	ASSERT_TRUE(bRet);
	std::vector<std::string> actual;
	bRet = att.read_vector(actual);
	ASSERT_TRUE(bRet);
	ASSERT_EQ(n, actual.size());
	for (size_t i = 0; i < n; ++i){
		std::string v1 = values[i];
		std::string v2 = actual[i];
		ASSERT_EQ(v1, v2);
	}// i
}/* Vector Attribute */
TEST_F(HDF5TextAttributeTest,VectorDataset)
{
	size_t n = 10;
	std::vector<std::string> values(n);
	for (size_t i = 0; i < n; ++i){
		std::stringstream os;
		os << "val" << (i+1);
		values[i] = os.str();
	}// i
	HDF5File oFile;
	bool bRet = oFile.create(this->m_filename);
	ASSERT_TRUE(bRet);
	HDF5Group oGroup(&oFile, this->m_grpname, true);
	std::string name("dataset_string_vector");
	HDF5Dataset att(&oGroup,name);
	bRet = att.write_vector(values);
	ASSERT_TRUE(bRet);
	std::vector<std::string> actual;
	bRet = att.read_vector(actual);
	ASSERT_TRUE(bRet);
	ASSERT_EQ(n, actual.size());
	for (size_t i = 0; i < n; ++i){
		std::string v1 = values[i];
		std::string v2 = actual[i];
		ASSERT_EQ(v1, v2);
	}// i
}/* Vector Dataset */
TEST_F(HDF5TextAttributeTest,ArrayDataset)
{
	size_t nr = 5;
	size_t nc = 3;
	size_t n = (size_t)(nr * nc);
	std::vector<std::string> values(n);
	for (size_t i = 0; i < nr; ++i){
		for (size_t j = 0; j < nc; ++j){
			size_t k = i * nc + j;
			std::stringstream os;
			os << "val" << (k+1);
			values[k] = os.str();
		}// j
	}// i
	HDF5File oFile;
	bool bRet = oFile.create(this->m_filename);
	ASSERT_TRUE(bRet);
	HDF5Group oGroup(&oFile, this->m_grpname, true);
	std::string name("dataset_string_array");
	HDF5Dataset att(&oGroup,name);
	bRet = att.write_array(values,nr,nc);
	ASSERT_TRUE(bRet);
	std::vector<std::string> actual;
	size_t nra = 0;
	size_t nca = 0;
	bRet = att.read_array(actual,nra,nca);
	ASSERT_TRUE(bRet);
	ASSERT_EQ(nr, nra);
	ASSERT_EQ(nc, nca);
	ASSERT_EQ(n, actual.size());
	for (size_t i = 0; i < nr; ++i){
		for (size_t j = 0; j < nc; ++j){
			size_t k = i * nc + j;
			std::string v1 = values[k];
			std::string v2 = actual[k];
			ASSERT_EQ(v1, v2);
		}// j
	}// i
}/* Array Dataset */



