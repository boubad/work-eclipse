/*
 * hdf5statdataset2.cpp
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */
#include "../../include/hdf5/hdf5dataset.h"
#include "../../include/hdf5/hdf5file.h"
#include "../../include/hdf5/hdf5group.h"
#include "../../include/hdf5/hdf5statindivtype.h"
#include "../../include/hdf5/hdf5statcorreltype.h"
///////////////////////////////////
#include "helpers.h"
///////////////////////////////////
namespace statdata {
namespace hdf5 {
///////////////////////////////////////////////
bool HDF5Dataset::read(std::vector<statdata::StatCorrel> &data,
		std::valarray<size_t> &dims) {
	data.clear();
	if (!this->is_valid()) {
		if (!this->open()) {
			return (false);
		}
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	//
	hid_t ytype = ::H5Dget_type(id);
	if (ytype < 0) {
		return (false);
	}
	hid_t hspace = ::H5Dget_space(id);
	if (hspace < 0) {
		::H5Tclose(ytype);
		return (false);
	}
	H5S_class_t cl = ::H5Sget_simple_extent_type(hspace);
	if (cl != H5S_SIMPLE) {
		::H5Tclose(ytype);
		::H5Sclose(hspace);
		return (false);
	}
	int nRank = ::H5Sget_simple_extent_ndims(hspace);
	if (nRank < 1) {
		::H5Tclose(ytype);
		::H5Sclose(hspace);
		return (false);
	}
	hsize_t odims1[H5S_MAX_RANK];
	hsize_t odims2[H5S_MAX_RANK];
	if (::H5Sget_simple_extent_dims(hspace, odims1, odims2) < 0) {
		::H5Sclose(hspace);
		return (false);
	}
	::H5Sclose(hspace);
	size_t nTotal = 1;
	dims.resize(nRank);
	for (int i = 0; i < nRank; ++i) {
		size_t c = (size_t) odims1[i];
		nTotal = (size_t) (nTotal * c);
		dims[i] = c;
	} //i
	std::unique_ptr<statcorrel_t> oBuf(new statcorrel_t[nTotal]);
	statcorrel_t *pBuf = oBuf.get();
	if (pBuf == nullptr) {
		::H5Tclose(ytype);
		return (false);
	}
	if (::H5Dread(id, ytype, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf) < 0) {
		::H5Tclose(ytype);
		return (false);
	}
	::H5Tclose(ytype);
	data = std::vector<StatCorrel>(nTotal);
	for (size_t i = 0; i < nTotal; ++i) {
		data[i] = pBuf[i];
	} // i
	return (true);
} // read
bool HDF5Dataset::write(const std::vector<StatCorrel> &data,
		const std::valarray<size_t> &oCount) {
	size_t n = data.size();
	if (n < 1) {
		return (false);
	}
	size_t nDims = oCount.size();
	if ((nDims < 1) || (nDims > H5S_MAX_RANK)) {
		return (false);
	}
	size_t nTotal = 1;
	for (size_t i = 0; i < nDims; ++i) {
		size_t c = oCount[i];
		if (c < 1) {
			return (false);
		}
		nTotal = (size_t) (nTotal * c);
	}
	if (nTotal > n) {
		return (false);
	}
	if (!this->is_valid()) {
		if (!this->open()) {
			if (this->m_parent == nullptr) {
				return (false);
			}
			if (!this->m_parent->is_valid()) {
				return (false);
			}
			if (this->m_name.empty()) {
				return (false);
			}
			std::unique_ptr<HDF5StatCorrelDataType> ooType;
			HDF5Group *pGroup = dynamic_cast<HDF5Group *>(this->m_parent);
			if (pGroup != nullptr) {
				ooType.reset(new HDF5StatCorrelDataType(pGroup));
			} else {
				HDF5File *pFile = dynamic_cast<HDF5File *>(this->m_parent);
				if (pFile != nullptr) {
					ooType.reset(new HDF5StatCorrelDataType(pFile));
				} else {
					return (false);
				}
			}
			hid_t proplist = ::H5Pcreate(H5P_DATASET_CREATE);
			if (proplist < 0) {
				return (false);
			}
			hsize_t pStart[H5S_MAX_RANK];
			hsize_t pEnd[H5S_MAX_RANK];
			hsize_t chunks[H5S_MAX_RANK];
			for (size_t i = 0; i < nDims; ++i) {
				hsize_t size = (hsize_t) oCount[i];
				if (size < 1) {
					size = 1;
				}
				pStart[i] = size;
				pEnd[i] = H5S_UNLIMITED;
			} // i
			int nx = 1;
			for (int i = (int) (nDims - 1); i >= 0; --i) {
				chunks[i] = (hsize_t) nx;
				nx = (int) (nx * pStart[i]);
			} // i
			if (::H5Pset_chunk(proplist, nDims, chunks) < 0) {
				::H5Pclose(proplist);
				return (false);
			}
			hid_t hspace = ::H5Screate(H5S_SIMPLE);
			if (hspace < 0) {
				::H5Pclose(proplist);
				return (false);
			}
			if (::H5Sset_extent_simple(hspace, nDims, pStart, pEnd) < 0) {
				::H5Pclose(proplist);
				::H5Sclose(hspace);
				return (false);
			}
			const char *pName = this->m_name.c_str();
			hid_t plink = ::H5Pcreate(H5P_LINK_CREATE);
			hid_t p2 = ::H5Pcreate(H5P_DATASET_ACCESS);
			hid_t pid = boost::any_cast<hid_t>(this->m_parent->m_id);
			hid_t id = -1;
			const boost::any &xid = ooType.get()->id();
			hid_t etype = boost::any_cast<hid_t>(xid);
			if ((plink >= 0) && (p2 >= 0)) {
				id = ::H5Dcreate2(pid, pName, etype, hspace, plink, proplist,
						p2);
			}
			if (plink >= 0) {
				::H5Pclose(plink);
			}
			if (p2 >= 0) {
				::H5Pclose(p2);
			}
			::H5Pclose(proplist);
			::H5Sclose(hspace);
			if (id >= 0) {
				this->m_id = boost::any(id);
			}
		} // not opened
	}
	if (!this->is_valid()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id < 0) {
		return (false);
	}
	hid_t hspace = ::H5Dget_space(id);
	if (hspace < 0) {
		return (false);
	}
	H5S_class_t cl = ::H5Sget_simple_extent_type(hspace);
	if (cl != H5S_SIMPLE) {
		::H5Sclose(hspace);
		return (false);
	}
	size_t nRank = ::H5Sget_simple_extent_ndims(hspace);
	if ((nRank < 1) || (nRank > nDims)) {
		::H5Sclose(hspace);
		return (false);
	}
	hsize_t pStart[H5S_MAX_RANK];
	hsize_t pEnd[H5S_MAX_RANK];
	if (::H5Sget_simple_extent_dims(hspace, pStart, pEnd) < 0) {
		::H5Sclose(hspace);
		return (false);
	}
	hsize_t start_mem[H5S_MAX_RANK];
	hsize_t start[H5S_MAX_RANK];
	hsize_t stride[H5S_MAX_RANK];
	hsize_t count[H5S_MAX_RANK];
	hsize_t block[H5S_MAX_RANK];
	hsize_t pStart_mem[H5S_MAX_RANK];
	hsize_t pEnd_mem[H5S_MAX_RANK];
	for (size_t i = 0; i < nRank; ++i) {
		start[i] = 0;
		stride[i] = 1;
		block[i] = 1;
		count[i] = (hsize_t) oCount[i];
		start_mem[i] = 0;
		pStart_mem[i] = (hsize_t) oCount[i];
		pEnd_mem[i] = (hsize_t) oCount[i];
	}
	hid_t hspace_mem = ::H5Screate(H5S_SIMPLE);
	if (hspace_mem < 0) {
		::H5Sclose(hspace);
		return (false);
	}
	if (::H5Sset_extent_simple(hspace_mem, nRank, pStart_mem, pEnd_mem) < 0) {
		::H5Sclose(hspace);
		::H5Sclose(hspace_mem);
		return (false);
	}
	if (::H5Sselect_hyperslab(hspace, H5S_SELECT_SET, start, stride, count,
			block) < 0) {
		::H5Sclose(hspace);
		::H5Sclose(hspace_mem);
		return (false);
	}
	if (::H5Sselect_hyperslab(hspace_mem, H5S_SELECT_SET, start_mem, stride,
			count, block) < 0) {
		::H5Sclose(hspace);
		::H5Sclose(hspace_mem);
		return (false);
	}
	hid_t htransf = ::H5Pcreate(H5P_DATASET_XFER);
	if (htransf < 0) {
		::H5Sclose(hspace);
		::H5Sclose(hspace_mem);
		return (false);
	}
	std::unique_ptr<statdata::statcorrel_t> oBuf(
			new statdata::statcorrel_t[nTotal]);
	statdata::statcorrel_t *pBuf = oBuf.get();
	for (size_t i = 0; i < n; ++i) {
		const StatCorrel &ind = data[i];
		::memcpy(&(pBuf[i]), ind.data(), sizeof(statdata::statcorrel_t));
	} // i
	hid_t ytype = ::H5Dget_type(id);
	if (ytype < 0) {
		return (false);
	}
	herr_t iRet = ::H5Dwrite(id, ytype, hspace_mem, hspace, htransf, pBuf);
	::H5Sclose(hspace);
	::H5Sclose(hspace_mem);
	::H5Pclose(htransf);
	return (iRet >= 0);
} // write
///////////////////////////////////////////////
bool HDF5Dataset::write(const std::vector<statdata::StatIndiv> &oIndivs) {
	size_t n = oIndivs.size();
	if (n < 1) {
		return (false);
	}
	if (!this->is_valid()) {
		if (!this->open()) {
			if (this->m_parent == nullptr) {
				return (false);
			}
			if (!this->m_parent->is_valid()) {
				return (false);
			}
			if (this->m_name.empty()) {
				return (false);
			}
			std::unique_ptr<HDF5StatIndivDataType> ooType;
			HDF5Group *pGroup = dynamic_cast<HDF5Group *>(this->m_parent);
			if (pGroup != nullptr) {
				ooType.reset(new HDF5StatIndivDataType(pGroup));
			} else {
				HDF5File *pFile = dynamic_cast<HDF5File *>(this->m_parent);
				if (pFile != nullptr) {
					ooType.reset(new HDF5StatIndivDataType(pFile));
				} else {
					return (false);
				}
			}
			hid_t proplist = ::H5Pcreate(H5P_DATASET_CREATE);
			if (proplist < 0) {
				return false;
			}
			hsize_t pStart[1];
			pStart[0] = (hsize_t) n;
			hsize_t pEnd[1];
			pEnd[0] = H5S_UNLIMITED;
			hsize_t chunks[1];
			chunks[0] = 1;
			if (::H5Pset_chunk(proplist, 1, chunks) < 0) {
				::H5Pclose(proplist);
				return (false);
			}
			hid_t hspace = ::H5Screate(H5S_SIMPLE);
			if (hspace < 0) {
				::H5Pclose(proplist);
				return (false);
			}
			if (::H5Sset_extent_simple(hspace, 1, pStart, pEnd) < 0) {
				::H5Pclose(proplist);
				::H5Sclose(hspace);
				return (false);
			}
			const char *pName = this->m_name.c_str();
			hid_t plink = ::H5Pcreate(H5P_LINK_CREATE);
			hid_t p2 = ::H5Pcreate(H5P_DATASET_ACCESS);
			hid_t pid = boost::any_cast<hid_t>(this->m_parent->m_id);
			hid_t id = -1;
			const boost::any &xid = ooType.get()->id();
			hid_t etype = boost::any_cast<hid_t>(xid);
			if ((plink >= 0) && (p2 >= 0)) {
				id = ::H5Dcreate2(pid, pName, etype, hspace, plink, proplist,
						p2);
			}
			if (plink >= 0) {
				::H5Pclose(plink);
			}
			if (p2 >= 0) {
				::H5Pclose(p2);
			}
			::H5Pclose(proplist);
			::H5Sclose(hspace);
			if (id >= 0) {
				this->m_id = boost::any(id);
			} else {
				return (false);
			}
		} // not opened
	}
	if (!this->is_valid()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id < 0) {
		return (false);
	}
	hid_t hspace = ::H5Dget_space(id);
	if (hspace < 0) {
		return (false);
	}
	H5S_class_t cl = ::H5Sget_simple_extent_type(hspace);
	if (cl != H5S_SIMPLE) {
		::H5Sclose(hspace);
		return (false);
	}
	size_t nRank = ::H5Sget_simple_extent_ndims(hspace);
	if (nRank != 1) {
		::H5Sclose(hspace);
		return (false);
	}
	hsize_t pStart[H5S_MAX_RANK];
	hsize_t pEnd[H5S_MAX_RANK];
	if (::H5Sget_simple_extent_dims(hspace, pStart, pEnd) < 0) {
		::H5Sclose(hspace);
		return (false);
	}
	hsize_t start_mem[1];
	hsize_t start[1];
	hsize_t stride[1];
	hsize_t count[1];
	hsize_t block[1];
	hsize_t pStart_mem[1];
	hsize_t pEnd_mem[1];
	start[0] = 0;
	stride[0] = 1;
	block[0] = 1;
	count[0] = (hsize_t) n;
	start_mem[0] = 0;
	pStart_mem[0] = (hsize_t) n;
	pEnd_mem[0] = (hsize_t) n;
	hid_t hspace_mem = ::H5Screate(H5S_SIMPLE);
	if (hspace_mem < 0) {
		::H5Sclose(hspace);
		return (false);
	}
	if (::H5Sset_extent_simple(hspace_mem, nRank, pStart_mem, pEnd_mem) < 0) {
		::H5Sclose(hspace);
		::H5Sclose(hspace_mem);
		return (false);
	}
	if (::H5Sselect_hyperslab(hspace, H5S_SELECT_SET, start, stride, count,
			block) < 0) {
		::H5Sclose(hspace);
		::H5Sclose(hspace_mem);
		return (false);
	}
	if (::H5Sselect_hyperslab(hspace_mem, H5S_SELECT_SET, start_mem, stride,
			count, block) < 0) {
		::H5Sclose(hspace);
		::H5Sclose(hspace_mem);
		return (false);
	}
	hid_t htransf = ::H5Pcreate(H5P_DATASET_XFER);
	if (htransf < 0) {
		::H5Sclose(hspace);
		::H5Sclose(hspace_mem);
		return (false);
	}
	std::unique_ptr<statdata::statindiv_t> oBuf(new statdata::statindiv_t[n]);
	statdata::statindiv_t *pBuf = oBuf.get();
	for (size_t i = 0; i < n; ++i) {
		const StatIndiv &ind = oIndivs[i];
		::memcpy(&(pBuf[i]), ind.data(), sizeof(statdata::statindiv_t));
	} // i
	hid_t ytype = ::H5Dget_type(id);
	if (ytype < 0) {
		return (false);
	}
	herr_t iRet = ::H5Dwrite(id, ytype, hspace_mem, hspace, htransf, pBuf);
	return (iRet >= 0);
} // write
bool HDF5Dataset::read(std::vector<statdata::StatIndiv> &data) {
	data.clear();
	if (!this->is_valid()) {
		if (!this->open()) {
			return (false);
		}
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	//
	hid_t ytype = ::H5Dget_type(id);
	if (ytype < 0) {
		return (false);
	}
	hid_t hspace = ::H5Dget_space(id);
	if (hspace < 0) {
		::H5Tclose(ytype);
		return (false);
	}
	H5S_class_t cl = ::H5Sget_simple_extent_type(hspace);
	if (cl != H5S_SIMPLE) {
		::H5Tclose(ytype);
		::H5Sclose(hspace);
		return (false);
	}
	int nRank = ::H5Sget_simple_extent_ndims(hspace);
	if (nRank < 1) {
		::H5Tclose(ytype);
		::H5Sclose(hspace);
		return (false);
	}
	hsize_t odims1[H5S_MAX_RANK];
	hsize_t odims2[H5S_MAX_RANK];
	if (::H5Sget_simple_extent_dims(hspace, odims1, odims2) < 0) {
		::H5Sclose(hspace);
		return (false);
	}
	::H5Sclose(hspace);
	size_t nTotal = 1;
	for (int i = 0; i < nRank; ++i) {
		size_t c = (size_t) odims1[i];
		nTotal = (size_t) (nTotal * c);
	} //i
	std::unique_ptr<statdata::statindiv_t> oBuf(
			new statdata::statindiv_t[nTotal]);
	statindiv_t *pBuf = oBuf.get();
	if (pBuf == nullptr) {
		::H5Tclose(ytype);
		return (false);
	}
	if (::H5Dread(id, ytype, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf) < 0) {
		::H5Tclose(ytype);
		return (false);
	}
	::H5Tclose(ytype);
	data = std::vector<statdata::StatIndiv>(nTotal);
	for (size_t i = 0; i < nTotal; ++i) {
		data[i] = pBuf[i];
	} // i
	return (true);
} // read
//////////////////////////////////
} // namespace hdf5
} // namespace statdata
