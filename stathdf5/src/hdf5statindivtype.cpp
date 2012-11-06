/*
 * hdf5statindivtype.cpp
 *
 *  Created on: 17 oct. 2012
 *      Author: boubad
 */
#include "../include/hdf5statindivtype.h"
#include "../include/hdf5file.h"
#include "../include/hdf5group.h"
///////////////////////////////////
#include "helpers.h"
///////////////////////////////
namespace statdata {
namespace hdf5 {
///////////////////////////////////
HDF5StatIndivDataType::HDF5StatIndivDataType(HDF5File *pFile) :
		HDF5DataType(pFile,
				std::string(statdata::StatIndiv::type_name()),
				true, sizeof(statindiv_t)) {
	if (this->is_created()) {
		this->register_datatype();
	}
}
HDF5StatIndivDataType::HDF5StatIndivDataType(HDF5Group *pGroup) :
		HDF5DataType(pGroup, std::string(statdata::StatIndiv::type_name()),
				true, sizeof(statindiv_t)) {
	if (this->is_created()) {
		this->register_datatype();
	}
}
HDF5StatIndivDataType::HDF5StatIndivDataType(const HDF5StatIndivDataType &other) :
		HDF5DataType(other) {
}
HDF5StatIndivDataType & HDF5StatIndivDataType::operator=(
		const HDF5StatIndivDataType &other) {
	if (this != &other) {
		HDF5DataType::operator=(other);
	}
	return (*this);
}
HDF5StatIndivDataType::~HDF5StatIndivDataType() {
	this->close();
}
bool HDF5StatIndivDataType::register_datatype(void) {
	if (this->m_id.empty()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id < 0) {
		return (false);
	}
	if (this->m_parent == nullptr) {
		return (false);
	}
	if (this->m_parent->id().empty()) {
		return (false);
	}
	hid_t pid = boost::any_cast<hid_t>(this->m_parent->id());
	if (pid < 0) {
		return (false);
	}
	hsize_t dims1[1];
	dims1[0] = StatIndiv::id_max_len() + 1;
	int perms[1];
	perms[0] = 0;
	hid_t t1 = ::H5Tarray_create1(H5T_NATIVE_CHAR, 1, dims1, perms);
	if (t1 < 0) {
		return (false);
	}
	hsize_t dims2[1];
	dims2[0] = StatIndiv::name_max_len() + 1;
	hid_t t2 = ::H5Tarray_create1(H5T_NATIVE_CHAR, 1, dims2, perms);
	if (t2 < 0) {
		::H5Tclose(t1);
		return (false);
	}
	::H5Tinsert(id, "index", HOFFSET(statindiv_t,index),H5T_NATIVE_UINT);
	::H5Tinsert(id, "active", HOFFSET(statindiv_t,active),H5T_NATIVE_INT);
	::H5Tinsert(id, "categ", HOFFSET(statindiv_t,categ),H5T_NATIVE_INT);
	::H5Tinsert(id, "supplement", HOFFSET(statindiv_t,supplement),H5T_NATIVE_INT);
	::H5Tinsert(id, "weight", HOFFSET(statindiv_t,weight),H5T_NATIVE_DOUBLE);
	::H5Tinsert(id, "id", HOFFSET(statindiv_t,id),t1);
	::H5Tinsert(id, "name", HOFFSET(statindiv_t,name),t2);
	//
	hid_t plink = ::H5Pcreate(H5P_LINK_CREATE);
	hid_t pdatatypecreate = ::H5Pcreate(H5P_DATATYPE_CREATE);
	hid_t pdatatypeaccess = ::H5Pcreate(H5P_DATATYPE_ACCESS);
	bool bRet = false;
	if ((plink >= 0) && (pdatatypecreate >= 0) && (pdatatypeaccess >= 0)) {
		bRet = (::H5Tcommit2(pid, this->m_name.c_str(), id, plink,
				pdatatypecreate, pdatatypeaccess) >= 0);
	}
	if (plink >= 0) {
		::H5Pclose(plink);
	}
	if (pdatatypecreate >= 0) {
		::H5Pclose(pdatatypecreate);
	}
	if (pdatatypeaccess >= 0) {
		::H5Pclose(pdatatypeaccess);
	}
	::H5Tclose(t1);
	::H5Tclose(t2);
	if (bRet) {
		this->m_created = false;
	}
	return (bRet);
} // register_datatype
////////////////////////////////////////
} // namespace hdf5
} // namespace statdata

