/*
 * hdf5object.cpp
 *
 *  Created on: 10 ao��t 2011
 *      Author: boubad
 */

#include "../include/hdf5object.h"
////////////////////////////
#ifdef _MSC_VER
#include <hdf5/hdf5.h>
#else
#include <hdf5.h>
#endif
////////////////////////////////////
namespace statdata {
namespace hdf5 {
////////////////////////////////////////////
const size_t HDF5Object::NAME_BUFFER_SIZE = 255;
////////////////////////////////////////////
HDF5Object::HDF5Object(HDF5Object *pParent /* = nullptr */) :
		m_parent(pParent) {
}
HDF5Object::HDF5Object(HDF5Object *pParent, const std::string &name) :
		m_parent(pParent), m_name(name) {
	assert(this->m_parent != nullptr);
	assert(!this->m_name.empty());
}
HDF5Object::HDF5Object(const HDF5Object &other) :
		m_parent(other.m_parent), m_id(other.m_id), m_name(other.m_name) {
}
HDF5Object & HDF5Object::operator=(const HDF5Object &other) {
	if (this != &other) {
		this->m_parent = other.m_parent;
		this->m_id = other.m_id;
		this->m_name = other.m_name;
	}
	return (*this);
}
HDF5Object::~HDF5Object() {
}
bool HDF5Object::is_valid(void) const {
	bool bRet = false;
	if (!this->m_id.empty()) {
		bRet = (boost::any_cast<hid_t>(this->m_id) >= 0);
	}
	return (bRet);
} // is_valid
bool HDF5Object::get_name(std::string &s) const {
	if (this->is_valid()) {
		s = this->m_name;
		return (true);
	}
	return false;
} // get_name
///////////////////////////////////////////
bool HDF5Object::get_groups_names(std::vector<std::string> &oNames) const {
	oNames.clear();
	if (!this->is_valid()) {
		return (false);
	}
	hsize_t n = 0;
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (::H5Gget_num_objs(id, &n) < 0) {
		return (false);
	}
	char buf[HDF5Object::NAME_BUFFER_SIZE + 1];
	for (size_t i = 0; i < n; ++i) {
		if (::H5Gget_objtype_by_idx(id, i) == H5G_GROUP) {
			int nx = (int) ::H5Gget_objname_by_idx(id, i, buf,
					HDF5Object::NAME_BUFFER_SIZE);
			if (nx >= 0) {
				buf[nx] = (char) 0;
				std::string s = buf;
				oNames.push_back(s);
			}
		} // group
	} // i
	return (true);
} // get_groups_names
bool HDF5Object::get_datasets_names(std::vector<std::string> &oNames) const {
	oNames.clear();
	if (!this->is_valid()) {
		return (false);
	}
	hsize_t n = 0;
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (::H5Gget_num_objs(id, &n) < 0) {
		return (false);
	}
	char buf[HDF5Object::NAME_BUFFER_SIZE + 1];
	for (size_t i = 0; i < n; ++i) {
		if (::H5Gget_objtype_by_idx(id, i) == H5G_DATASET) {
			int nx = (int) ::H5Gget_objname_by_idx(id, i, buf,
					HDF5Object::NAME_BUFFER_SIZE);
			if (nx >= 0) {
				buf[nx] = (char) 0;
				std::string s = buf;
				oNames.push_back(s);
			}
		} // group
	} // i
	return (true);
} // get_datasets_names
bool HDF5Object::get_attributes_names(std::vector<std::string> &oNames) const {
	oNames.clear();
	if (!this->is_valid()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	int n = ::H5Aget_num_attrs(id);
	if (n < 0) {
		return (true);
	}
	char buf[HDF5Object::NAME_BUFFER_SIZE + 1];
	for (int i = 0; i < n; ++i) {
		int nz = (int) ::H5Aget_name_by_idx(id, ".", H5_INDEX_CRT_ORDER,
				H5_ITER_INC, i, buf, HDF5Object::NAME_BUFFER_SIZE, H5P_DEFAULT);
		if (nz >= 0) {
			buf[nz] = (char) 0;
			std::string s = buf;
			oNames.push_back(s);
		}
	} // i
	return (true);
} // get_attributes_names
bool HDF5Object::has_attribute(const std::string &name) const {
	assert(!name.empty());
	if (this->is_valid()) {
		hid_t id = boost::any_cast<hid_t>(this->m_id);
		return (::H5Aexists(id, name.c_str()) > 0);
	}
	return (false);
} // has_attribute
///////////////////////////////////////////
} // namespace hdf5
} /* namespace statdata */
