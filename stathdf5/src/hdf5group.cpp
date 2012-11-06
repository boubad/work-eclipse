/*
 * hdf5group.cpp
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */
#include "../include/hdf5group.h"
#include "../include/hdf5file.h"
////////////////////////////
#include "helpers.h"
////////////////////////////////////
namespace statdata {
namespace hdf5 {
///////////////////////////////
HDF5Group::HDF5Group(HDF5Object *pParent /*= nullptr*/) :
		HDF5Object(pParent) {
}
HDF5Group::HDF5Group(HDF5File *pFile, const std::string &name,
		bool bCreateIfNotExists) :
		HDF5Object(pFile, name) {
	this->check_exists(bCreateIfNotExists);
}
HDF5Group::HDF5Group(HDF5Group *pGroup, const std::string &name,
		bool bCreateIfNotExists) :
		HDF5Object(pGroup, name) {
	this->check_exists(bCreateIfNotExists);
}
HDF5Group::HDF5Group(const HDF5Group &other) :
		HDF5Object(other) {
}
HDF5Group & HDF5Group::operator=(const HDF5Group &other) {
	if (this != &other) {
		HDF5Object::operator=(other);
	}
	return (*this);
}
HDF5Group::~HDF5Group() {
	this->close();
}
bool HDF5Group::close(void) {
	if (this->m_id.empty()) {
		return (true);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id < 0) {
		return (true);
	}
	if (::H5Gclose(id) >= 0) {
		this->m_id = boost::any();
		return (true);
	}
	return (false);
}
bool HDF5Group::exists(void) const {
	if (this->is_valid()) {
		return (true);
	}
	const char *pName = this->m_name.c_str();
	HDF5Object *p = this->m_parent;
	assert(p != nullptr);
	assert(p->is_valid());
	hid_t pid = boost::any_cast<hid_t>(p->m_id);
	hid_t temp = ::H5Gopen2(pid, pName, H5P_DEFAULT);
	bool bRet = (temp >= 0);
	if (temp >= 0) {
		::H5Gclose(temp);
	}
	return (bRet);
} // exists
void HDF5Group::check_exists(bool bCreateIfNotExists) {
	if (this->is_valid()) {
		return;
	}
	const char *pName = this->m_name.c_str();
	HDF5Object *p = this->m_parent;
	assert(p != nullptr);
	assert(p->is_valid());
	hid_t pid = boost::any_cast<hid_t>(p->m_id);
	hid_t id = -1;
	if ((id = ::H5Gopen2(pid, pName, H5P_DEFAULT)) < 0) {
		if (bCreateIfNotExists) {
			id = ::H5Gcreate2(pid, pName, H5P_DEFAULT, H5P_DEFAULT,
					H5P_DEFAULT);
		}
	}
	if (id >= 0) {
		this->m_id = boost::any(id);
	}
} // check_exists
bool HDF5Group::set_comment(const std::string &s) {
	if (!this->is_valid()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	return (::H5Oset_comment(id, s.c_str()) >= 0);
} // set_comment
bool HDF5Group::get_comment(std::string &s) {
	s.empty();
	if (!this->is_valid()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	int n = (int) ::H5Oget_comment(id, nullptr, 0);
	if (n < 0) {
		return (false);
	}
	if (n < 256) {
		char buf[256];
		if (::H5Oget_comment(id, buf, n) < 0) {
			return (false);
		}
		buf[n] = (char) 0;
		s = buf;
	} else {
		std::unique_ptr<char> oBuf(new char[n+1]);
		char *pBuf = oBuf.get();
		if (pBuf == nullptr){
			return (false);
		}
		if (::H5Oget_comment(id, pBuf, n) < 0) {
			return (false);
		}
		pBuf[n] = (char) 0;
		s = pBuf;
	}
	return (true);
} // get_comments
} /* namespace hdf5 */
/////////////////////////////////////////
} /* namespace statdata */

