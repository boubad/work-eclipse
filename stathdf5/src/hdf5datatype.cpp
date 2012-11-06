/*
 * hdf5datatype.cpp
 *
 *  Created on: 15 oct. 2012
 *      Author: boubad
 */
#include "../include/hdf5datatype.h"
#include "../include/hdf5file.h"
#include "../include/hdf5group.h"
///////////////////////////////////
#include "helpers.h"
///////////////////////////////
namespace statdata {
namespace hdf5 {

HDF5DataType::HDF5DataType(HDF5File *pFile, const std::string &name,
		bool bCreateIfNotExists, size_t nSize) :
		HDF5Object(pFile, name), m_created(false) {
	this->check(bCreateIfNotExists, nSize);
}
HDF5DataType::HDF5DataType(HDF5Group *pGroup, const std::string &name,
		bool bCreateIfNotExists, size_t nSize) :
		HDF5Object(pGroup, name), m_created(false) {
	this->check(bCreateIfNotExists, nSize);
}
HDF5DataType::HDF5DataType(const HDF5DataType &other) :
		HDF5Object(other), m_created(other.m_created) {
}
HDF5DataType & HDF5DataType::operator=(const HDF5DataType &other) {
	if (this != &other) {
		HDF5Object::operator=(other);
		this->m_created = other.m_created;
	}
	return (*this);
}
HDF5DataType::~HDF5DataType() {


}
bool HDF5DataType::commit(void) {
	if (this->m_id.empty()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id < 0) {
		return (false);
	}
	if (!this->m_created) {
		return (true);
	}
	if (this->m_parent == nullptr) {
		return (false);
	}
	if (this->m_parent->m_id.empty()) {
		return (false);
	}
	hid_t pid = boost::any_cast<hid_t>(this->m_parent->m_id);
	if (pid < 0) {
		return (false);
	}
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
	if (bRet) {
		this->m_created = false;
	}
	return (bRet);
} // commit
bool HDF5DataType::check(bool bCreateIfNotExists, size_t nSize) {
	if (this->is_valid()) {
		return true;
	}
	if (this->m_name.empty()) {
		return (false);
	}
	if (this->m_parent == nullptr) {
		return (false);
	}
	if (this->m_parent->m_id.empty()) {
		return (false);
	}
	hid_t pid = boost::any_cast<hid_t>(this->m_parent->m_id);
	if (pid < 0) {
		return (false);
	}
	const char *pName = this->m_name.c_str();
	hid_t prop = ::H5Pcreate(H5P_DATATYPE_ACCESS);
	if (prop < 0) {
		return (false);
	}
	hid_t id = ::H5Topen2(pid, pName, prop);
	::H5Pclose(prop);
	if (id >= 0) {
		this->m_id = boost::any(id);
		return (true);
	}
	if (!bCreateIfNotExists) {
		return (false);
	}
	if (nSize < 1) {
		return (false);
	}
	id = ::H5Tcreate(H5T_COMPOUND, nSize);
	if (id < 0){
		return (false);
	}
	this->m_id = boost::any(id);
	this->m_created = true;
	return (true);
}
bool HDF5DataType::close(void) {
	if (!this->is_valid()) {
		return (true);
	}
	if (this->m_created) {
		if (!this->commit()) {
			return (false);
		}
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id >= 0) {
		if (::H5Tclose(id) < 0) {
			return (false);
		}
	}
	return (true);
}
bool HDF5DataType::exists(void) const {
	if (this->m_id.empty()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	return (id >= 0);
}
bool HDF5DataType::is_valid(void) const {
	if (this->m_id.empty()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	return (id >= 0);
}
bool HDF5DataType::get_name(std::string &s) const {
	s = this->m_name;
	return (true);
}
bool HDF5DataType::get_groups_names(
		std::vector<std::string> & /*oNames */) const {
	return (false);
}
bool HDF5DataType::get_datasets_names(
		std::vector<std::string> & /*oNames */) const {
	return (false);
}
} /* namespace hdf5 */
//////////////////////////////
} /* namespace statdata */

