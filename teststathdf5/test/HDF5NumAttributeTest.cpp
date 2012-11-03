/*
 * HDF5NumAttributeTest.cpp
 *
 *  Created on: 20 oct. 2012
 *      Author: boubad
 */
#include <gtest/gtest.h>
/////////////////////////////
#include <hdf5store.h>
/////////////////////////////
template <typename T>
class HDF5NumAttributeTest : public ::testing::Test {
public:
	typedef std::vector<T> Vector;
public:
	HDF5NumAttributeTest():m_filename("mytest.h5"),m_grpname("g_initial"){}
	virtual ~HDF5NumAttributeTest(){}
 protected:
  T value_;
  std::string m_filename;
  std::string m_grpname;
};
typedef ::testing::Types<char, signed char,unsigned char,short,unsigned short,int, unsigned int, long, unsigned long, long long, unsigned long long> MyTypes;
TYPED_TEST_CASE(HDF5NumAttributeTest, MyTypes);
///////////////////////////////////////
using namespace statdata;
using namespace statdata::hdf5;
//////////////////////////////////////
TYPED_TEST(HDF5NumAttributeTest,ScalarAttribute)
{
	TypeParam v = (TypeParam)68;
	boost::any vv(v);
	const std::type_info &tt = vv.type();
	HDF5File oFile;
	bool bRet = oFile.create(this->m_filename);
	ASSERT_TRUE(bRet);
	HDF5Group oGroup(&oFile, this->m_grpname, true);
	std::string name = std::string("att_") + std::string(tt.name()) + std::string("_scalar");
	HDF5Attribute att(&oGroup,name);
	bRet = att.write_scalar(v);
	ASSERT_TRUE(bRet);
	TypeParam actual;
	bRet = att.read_scalar(actual);
	ASSERT_TRUE(bRet);
	ASSERT_EQ(v, actual);
}/* Scalar Attribute */
TYPED_TEST(HDF5NumAttributeTest,VectorAttribute)
{
	size_t n = 10;
	typename TestFixture::Vector values(n);
	for (size_t i = 0; i < n; ++i){
		values[i] = (TypeParam)(68 + i);
	}// i
	TypeParam v = (TypeParam)68;
	boost::any vv(v);
	const std::type_info &tt = vv.type();
	HDF5File oFile;
	bool bRet = oFile.create(this->m_filename);
	ASSERT_TRUE(bRet);
	HDF5Group oGroup(&oFile, this->m_grpname, true);
	std::string name = std::string("att_") + std::string(tt.name()) + std::string("_vector");
	HDF5Attribute att(&oGroup,name);
	bRet = att.write_vector(values);
	ASSERT_TRUE(bRet);
	typename TestFixture::Vector actual;
	bRet = att.read_vector(actual);
	ASSERT_TRUE(bRet);
	ASSERT_EQ(n, actual.size());
	for (size_t i = 0; i < n; ++i){
		TypeParam v1 = values[i];
		TypeParam v2 = actual[i];
		ASSERT_EQ(v1, v2);
	}// i
}/* Vector Attribute */
TYPED_TEST(HDF5NumAttributeTest,VectorDataset)
{
	size_t n = 10;
	typename TestFixture::Vector values(n);
	for (size_t i = 0; i < n; ++i){
		values[i] = (TypeParam)(68 + i);
	}// i
	TypeParam v = (TypeParam)68;
	boost::any vv(v);
	const std::type_info &tt = vv.type();
	HDF5File oFile;
	bool bRet = oFile.create(this->m_filename);
	ASSERT_TRUE(bRet);
	HDF5Group oGroup(&oFile, this->m_grpname, true);
	std::string name = std::string("dataset_") + std::string(tt.name()) + std::string("_vector");
	HDF5Dataset att(&oGroup,name);
	bRet = att.write_vector(values);
	ASSERT_TRUE(bRet);
	typename TestFixture::Vector actual;
	bRet = att.read_vector(actual);
	ASSERT_TRUE(bRet);
	ASSERT_EQ(n, actual.size());
	for (size_t i = 0; i < n; ++i){
		TypeParam v1 = values[i];
		TypeParam v2 = actual[i];
		ASSERT_EQ(v1, v2);
	}// i
}/* Vector VectorDataset */
TYPED_TEST(HDF5NumAttributeTest,ArrayDataset)
{
	size_t nr = 5;
	size_t nc = 3;
	size_t n = (size_t)(nr * nc);
	typename TestFixture::Vector values(n);
	for (size_t i = 0; i < nr; ++i){
		for (size_t j = 0; j < nc; ++j){
		    values[i * nc + j] = (TypeParam)(68 + i * nc + j);
		}
	}// i
	TypeParam v = (TypeParam)68;
	boost::any vv(v);
	const std::type_info &tt = vv.type();
	HDF5File oFile;
	bool bRet = oFile.create(this->m_filename);
	ASSERT_TRUE(bRet);
	HDF5Group oGroup(&oFile, this->m_grpname, true);
	std::string name = std::string("dataset_") + std::string(tt.name()) + std::string("_array");
	HDF5Dataset att(&oGroup,name);
	bRet = att.write_array(values,nr,nc);
	ASSERT_TRUE(bRet);
	typename TestFixture::Vector actual;
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
			TypeParam v1 = values[k];
			TypeParam v2 = actual[k];
			ASSERT_EQ(v1, v2);
		}// j
	}// i
}/* Vector VectorDataset */
