//============================================================================
// Name        : statdatatest.cpp
// Author      : Boubacar Diarra
// Version     :
// Copyright   : boubadiarra@gmail.com
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <hdf5store.h>
////////////////////////////
#include <iostream>
#include <sstream>
//////////////////////////////
using namespace std;
using namespace statdata;
using namespace statdata::hdf5;
////////////////////////////////
bool my_test_statindivs(HDF5Group *pGroup){
	std::string v_name = "statindivs_vector";
	size_t n = 5;
	std::vector<StatIndiv> data(n);
	for (size_t i = 0; i < n; ++i){
		StatIndiv &ind = data[i];
		ind.index(1000 + i);
		ind.set_active(true);
		ind.set_categ(true);
		ind.set_sup(true);
		{
			std::stringstream os;
			os << "IDENT" << (i+1);
			ind.id(os.str());
		}
		{
			std::stringstream os;
			os << "INDIVNAME" << (i + 1);
			ind.name(os.str());
		}
	}// i
	HDF5Dataset att(pGroup, v_name);
	bool bRet = att.write_indivs(data);
	if (!bRet){
		return (false);
	}
	std::vector<StatIndiv> vv;
	bRet = att.read_indivs(vv);
	return bRet;
}// my_test_statindivs
template<typename T>
bool my_test_dataset(HDF5Group *pGroup, T val) {
	boost::any av(val);
	const std::type_info &tt = av.type();
	std::string cname = tt.name();
	std::string v_name = cname + "_ds_vector";
	{
		HDF5Dataset att(pGroup, v_name);
		size_t n = 5;
		std::vector<T> v1(n);
		for (size_t i = 0; i < n; ++i) {
			v1[i] = val;
		}
		bool bRet = att.write_vector(v1);
		if (!bRet) {
			std::cout << "FAIL...\t" << v_name << " write vector" << std::endl;
			return (false);
		}
		std::vector<T> v2;
		bRet = att.read_vector(v2);
		if (!bRet) {
			std::cout << "FAIL...\t" << v_name << " read vector" << std::endl;
			return (false);
		}
		if (v2.size() != n) {
			std::cout << v_name << " : bad vector size " << n << ","
					<< v2.size() << std::endl;
		}
		for (size_t i = 0; i < n; ++i) {
			if (v1[i] != v2[i]) {
				std::cout << "FAIL...\t" + v_name << " mismatch " << v1[i]
						<< " , " << v2[i] << std::endl;
			}
		} // i
	}
	//
	v_name = cname + "_ds_array";
	{
		HDF5Dataset att(pGroup, v_name);
		size_t nr = 5;
		size_t nc = 3;
		size_t nTotal = nr * nc;
		std::vector<T> v1(nTotal);
		for (size_t i = 0; i < nTotal; ++i) {
			v1[i] = val;
		}
		bool bRet = att.write_array(v1, nr, nc);
		if (!bRet) {
			std::cout << "FAIL...\t" << v_name << " write array" << std::endl;
			return (false);
		}
		std::vector<T> v2;
		size_t nr1 = 0;
		size_t nc1 = 0;
		bRet = att.read_array(v2, nr1, nc1);
		if (!bRet) {
			std::cout << "FAIL...\t" << v_name << " read array" << std::endl;
			return (false);
		}
		if (nr != nr1) {
			std::cout << v_name << " : bad row size " << nr << "," << nr1
					<< std::endl;
		}
		if (nc != nc1) {
			std::cout << v_name << " : bad col size " << nc << "," << nc1
					<< std::endl;
		}
	}
	return (true);
} //my_test_dataset
////////////////////////////
template<typename T>
bool my_test_attribute(HDF5Group *pGroup, T val) {
	boost::any av(val);
	const std::type_info &tt = av.type();
	std::string cname = tt.name();
	std::string s_name = cname + "_scalar_att";
	{
		HDF5Attribute att(pGroup, s_name);
		bool bRet = att.write_scalar(val);
		if (!bRet) {
			std::cout << "FAIL...\t" << s_name << " write scalar" << std::endl;
			return (false);
		}
		T actual;
		bRet = att.read_scalar(actual);
		if (!bRet) {
			std::cout << "FAIL...\t" << s_name << " read scalar" << std::endl;
			return (false);
		}
		if (actual != val) {
			std::cout << "FAIL...\t" + s_name << " mismatch " << val << " , "
					<< actual << std::endl;
		}
	}
	std::string v_name = cname + "_vector_att";
	{
		HDF5Attribute att(pGroup, v_name);
		size_t n = 5;
		std::vector<T> v1(n);
		for (size_t i = 0; i < n; ++i) {
			v1[i] = val;
		}
		bool bRet = att.write_vector(v1);
		if (!bRet) {
			std::cout << "FAIL...\t" << v_name << " write vector" << std::endl;
			return (false);
		}
		std::vector<T> v2;
		bRet = att.read_vector(v2);
		if (!bRet) {
			std::cout << "FAIL...\t" << v_name << " read vector" << std::endl;
			return (false);
		}
		if (v2.size() != n) {
			std::cout << v_name << " : bad vector size " << n << ","
					<< v2.size() << std::endl;
		}
		for (size_t i = 0; i < n; ++i) {
			if (v1[i] != v2[i]) {
				std::cout << "FAIL...\t" + v_name << " mismatch " << v1[i]
						<< " , " << v2[i] << std::endl;
			}
		} // i
	}
	return (true);
} // my_test_attribute
////////////////////////////////////////////
int main() {
	std::string filename("mytest.h5");
	HDF5File oFile;
	bool bRet = oFile.create(filename);
	assert(bRet);
	std::string grpname("ginitial");
	HDF5Group oGroup(&oFile, grpname, true);
	//
	// DataType
	{
		my_test_statindivs(&oGroup);
	}
	// Attribute
	my_test_attribute(&oGroup, (signed char) 'S');
	my_test_attribute(&oGroup, (unsigned char) 'U');
	my_test_attribute(&oGroup, (char) 'C');
	my_test_attribute(&oGroup, (short) 10);
	my_test_attribute(&oGroup, (unsigned short) 11);
	my_test_attribute(&oGroup, (int) 100);
	my_test_attribute(&oGroup, (unsigned int) 101);
	my_test_attribute(&oGroup, (long) 1000);
	my_test_attribute(&oGroup, (unsigned long) 1001);
	my_test_attribute(&oGroup, (long long) 10000);
	my_test_attribute(&oGroup, (unsigned long long) 100001);
	my_test_attribute(&oGroup, (float) 50);
	my_test_attribute(&oGroup, (double) 500);
	my_test_attribute(&oGroup, (long double) 5000);
	std::string s = "0123456789";
	my_test_attribute(&oGroup, s);
	std::cout << std::endl << "====================" << std::endl;
	//
	// Dataset
	my_test_dataset(&oGroup, (signed char) 'S');
	my_test_dataset(&oGroup, (unsigned char) 'U');
	my_test_dataset(&oGroup, (char) 'C');
	my_test_dataset(&oGroup, (short) 10);
	my_test_dataset(&oGroup, (unsigned short) 11);
	my_test_dataset(&oGroup, (int) 100);
	my_test_dataset(&oGroup, (unsigned int) 101);
	my_test_dataset(&oGroup, (long) 1000);
	my_test_dataset(&oGroup, (unsigned long) 1001);
	my_test_dataset(&oGroup, (long long) 10000);
	my_test_dataset(&oGroup, (unsigned long long) 100001);
	my_test_dataset(&oGroup, (float) 50);
	my_test_dataset(&oGroup, (double) 500);
	my_test_dataset(&oGroup, (long double) 5000);
	std::string ss = "0123456789";
	my_test_dataset(&oGroup, ss);
	std::cout << std::endl << "====================" << std::endl;
	//
	cout << std::endl << std::endl << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
} // main
