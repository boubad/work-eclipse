/*
 * hdf5statcorreltype.cpp
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */
#include "../include/hdf5statcorreltype.h"
#include "../include/hdf5file.h"
#include "../include/hdf5group.h"
///////////////////////////////////
#include "helpers.h"
///////////////////////////////
namespace statdata {
namespace hdf5 {
///////////////////////////////////
HDF5StatCorrelDataType::HDF5StatCorrelDataType(HDF5File *pFile) :
		HDF5DataType(pFile, std::string(statdata::StatCorrel::type_name()),
				true, sizeof(statcorrel_t)) {
	if (this->is_created()) {
		this->register_datatype();
	}
}
HDF5StatCorrelDataType::HDF5StatCorrelDataType(HDF5Group *pGroup) :
		HDF5DataType(pGroup, std::string(statdata::StatCorrel::type_name()),
				true, sizeof(statcorrel_t)) {
	if (this->is_created()) {
		this->register_datatype();
	}
}
HDF5StatCorrelDataType::HDF5StatCorrelDataType(
		const HDF5StatCorrelDataType &other) :
		HDF5DataType(other) {
}
HDF5StatCorrelDataType & HDF5StatCorrelDataType::operator=(
		const HDF5StatCorrelDataType &other) {
	if (this != &other) {
		HDF5DataType::operator=(other);
	}
	return (*this);
}
HDF5StatCorrelDataType::~HDF5StatCorrelDataType() {
	this->close();
}
bool HDF5StatCorrelDataType::register_datatype(void) {
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
	/*
	 * unsigned int count;
	 double corr;
	 double prob;
	 double min_corr;
	 double max_corr;
	 */
	::H5Tinsert(id, "count", HOFFSET(statcorrel_t,count),H5T_NATIVE_UINT);
	::H5Tinsert(id, "corr", HOFFSET(statcorrel_t,corr),H5T_NATIVE_DOUBLE);
	::H5Tinsert(id, "prob", HOFFSET(statcorrel_t,prob),H5T_NATIVE_DOUBLE);
	::H5Tinsert(id, "min_corr", HOFFSET(statcorrel_t,min_corr),H5T_NATIVE_DOUBLE);
	::H5Tinsert(id, "max_corr", HOFFSET(statcorrel_t,max_corr),H5T_NATIVE_DOUBLE);
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
	if (bRet) {
		this->m_created = false;
	}
	return (bRet);
} // register_datatype
////////////////////////////////////////
} // namespace hdf5
} // namespace statdata

