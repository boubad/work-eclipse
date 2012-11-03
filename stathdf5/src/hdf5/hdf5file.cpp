/*
 * hdf5file.cpp
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */
#include "../../include/hdf5/hdf5file.h"
////////////////////////////
#include "helpers.h"
//////////////////////
namespace statdata {
namespace hdf5 {
/////////////////////////////////////
HDF5File::HDF5File() {
}
HDF5File::HDF5File(const std::string &filename, bool bReadOnly /*= true */) {
	this->m_name = filename;
	this->open(bReadOnly);
}
HDF5File::HDF5File(const HDF5File &other) :
		HDF5Object(other) {
}
HDF5File & HDF5File::operator=(const HDF5File &other) {
	if (this != &other) {
		HDF5Object::operator=(other);
	}
	return (*this);
}
bool HDF5File::close(void) {
	if (this->m_id.empty()) {
		return (true);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id < 0) {
		return (true);
	}
	if (::H5Fclose(id) >= 0) {
		this->m_id = boost::any();
		return (true);
	}
	return (false);
} // close
HDF5File::~HDF5File() {
	this->close();
}
bool HDF5File::exists(void) const {
	if (this->is_valid()) {
		return (true);
	}
	if (this->m_name.empty()) {
		return (false);
	}
	hid_t temp = ::H5Fopen(this->m_name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	bool bRet = (temp >= 0);
	if (temp >= 0) {
		::H5Fclose(temp);
	}
	return (bRet);
} // exists
bool HDF5File::create(const std::string &filename) {
	::H5Eset_auto2(H5E_DEFAULT, NULL, NULL);
	if (!this->close()) {
		return (false);
	}
	hid_t id = ::H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT,
			H5P_DEFAULT);
	if (id < 0) {
		return (false);
	}
	this->m_id = boost::any(id);
	return (true);
} // create
bool HDF5File::open(const std::string &filename, bool bReadOnly /*= true */) {
	if (filename.empty()) {
		return (false);
	}
	::H5Eset_auto2(H5E_DEFAULT, NULL, NULL);
	if (!this->close()) {
		return (false);
	}
	this->m_name = filename;
	hid_t id = ::H5Fopen(filename.c_str(),
			(bReadOnly) ? H5F_ACC_RDONLY : H5F_ACC_RDWR, H5P_DEFAULT);
	if (id >= 0) {
		this->m_id = boost::any(id);
	}
	if (bReadOnly) {
		return (id >= 0);
	} else if (id < 0) {
		id = ::H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT,
				H5P_DEFAULT);
	}
	if (id >= 0) {
		this->m_id = boost::any(id);
	}
	return (id >= 0);
} // open
bool HDF5File::open(bool bReadOnly /*= true */) {
	return (this->open(this->m_name, bReadOnly));
} // open
bool HDF5File::get_name(std::string &s) const {
	s.clear();
	if (this->is_valid()) {
		hid_t id = boost::any_cast<hid_t>(this->m_id);
		size_t n = ::H5Fget_name(id, NULL, 0);
		if (n < 1) {
			return (false);
		}
		std::unique_ptr<char> oBuf(new char[n + 1]);
		char *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Fget_name(id, pBuf, n) < 0) {
			return (false);
		}
		s = pBuf;
		HDF5File *pFile = const_cast<HDF5File *>(this);
		pFile->m_name = s;
		return (true);
	}
	return (false);
} // get_name
bool HDF5File::get_attributes_names(std::vector<std::string> &oNames) const {
	oNames.clear();
	return (this->is_valid());
} // get_attributes_names
bool HDF5File::has_attribute(const std::string & /*name */) const {
	return (false);
}
} // namespace hdf5
////////////////////////
} /* namespace statdata */

