/*
 * hdf5attribute.cpp
 *
 *  Created on: 13 oct. 2012
 *      Author: boubad
 */
#include "../../include/hdf5/hdf5attribute.h"
#include "../../include/hdf5/hdf5group.h"
#include "../../include/hdf5/hdf5dataset.h"
////////////////////////////
#include "helpers.h"
////////////////////////////////////
namespace statdata {
namespace hdf5 {
////////////////////////////////////////
HDF5Attribute::HDF5Attribute() {
	this->open();
}
HDF5Attribute::HDF5Attribute(HDF5Group *pParent, const std::string &name) :
		HDF5Object(pParent, name) {
	this->open();
}
HDF5Attribute::HDF5Attribute(HDF5Dataset *pParent, const std::string &name) :
		HDF5Object(pParent, name) {
	this->open();
}
HDF5Attribute::~HDF5Attribute() {
	this->close();
}
bool HDF5Attribute::get_info(statdata::DataType &type,
		size_t &nSize, bool &bScalar) const {
	type = typeOther;
	nSize = 0;
	bScalar = false;
	if (!this->is_valid()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id < 0) {
		return (false);
	}
	hid_t ytype = ::H5Aget_type(id);
	if (ytype < 0) {
		return (false);
	}
	hid_t xtype = ::H5Tget_native_type(ytype, H5T_DIR_ASCEND);
	::H5Tclose(ytype);
	if (xtype < 0) {
		return (false);
	}
	Helpers::my_get_type(xtype, type);
	if (type == typeOther) {
		return (false);
	}
	::H5Tclose(xtype);
	hid_t hspace = ::H5Aget_space(id);
	if (hspace < 0) {
		return (false);
	}
	H5S_class_t cl = ::H5Sget_simple_extent_type(hspace);
	if (cl == H5S_SCALAR) {
		bScalar = true;
		nSize = 1;
		::H5Sclose(hspace);
		return (true);
	} else if (cl == H5S_SIMPLE) {
		bScalar = false;
		int nRank = ::H5Sget_simple_extent_ndims(hspace);
		if (nRank != 1) {
			::H5Sclose(hspace);
			return (false);
		}
		hsize_t dims[] = { 0 };
		if (::H5Sget_simple_extent_dims(hspace, dims, nullptr) < 0) {
			::H5Sclose(hspace);
			return (false);
		}
		nSize = (size_t) dims[0];
		::H5Sclose(hspace);
		return (true);
	} else {
		return (false);
		::H5Sclose(hspace);
	}
	return (false);
} // get_info
bool HDF5Attribute::close(void) {
	if (!this->is_valid()) {
		return (true);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (::H5Aclose(id) >= 0) {
		this->m_id = boost::any();
		return (true);
	}
	return (false);
} // close
bool HDF5Attribute::get_groups_names(std::vector<std::string> &oNames) const {
	oNames.clear();
	return (this->is_valid());
}
bool HDF5Attribute::get_datasets_names(std::vector<std::string> &oNames) const {
	oNames.clear();
	return (this->is_valid());
}
bool HDF5Attribute::get_attributes_names(
		std::vector<std::string> &oNames) const {
	oNames.clear();
	return (this->is_valid());
}
bool HDF5Attribute::has_attribute(const std::string & /*name */) const {
	return (false);
}
bool HDF5Attribute::open(void) {
	if (!this->is_valid()) {
		if (this->m_parent == nullptr) {
			return (false);
		}
		if (!this->m_parent->is_valid()) {
			return (false);
		}
		if (this->m_name.empty()) {
			return (false);
		}
		hid_t ip = boost::any_cast<hid_t>(this->m_parent->m_id);
		hid_t id = ::H5Aopen(ip, this->m_name.c_str(), H5P_DEFAULT);
		if (id >= 0) {
			this->m_id = boost::any(id);
		}
		return (id >= 0);
	} else {
		return (true);
	}
} // open
bool HDF5Attribute::exists(void) const {
	if (this->is_valid()) {
		return (true);
	}
	const char *pName = this->m_name.c_str();
	HDF5Object *p = this->m_parent;
	if (p == nullptr) {
		return (false);
	}
	if (!p->is_valid()) {
		return (false);
	}
	assert(p != nullptr);
	assert(p->is_valid());
	hid_t ip = boost::any_cast<hid_t>(p->m_id);
	hid_t temp = ::H5Aopen(ip, pName, H5P_DEFAULT);
	bool bRet = (temp >= 0);
	if (temp >= 0) {
		::H5Aclose(temp);
	}
	return (bRet);
} // exists
bool HDF5Attribute::write_scalar_any(const boost::any &v) {
	if (v.empty()) {
		return (false);
	}
	hid_t id = -1;
	if (!this->is_valid()) {
		if (this->m_parent == nullptr) {
			return (false);
		}
		if (!this->m_parent->is_valid()) {
			return (false);
		}
		if (this->m_name.empty()) {
			return (false);
		}
		hid_t ip = boost::any_cast<hid_t>(this->m_parent->m_id);
		id = ::H5Aopen(ip, this->m_name.c_str(), H5P_DEFAULT);
		if (id >= 0) {
			this->m_id = boost::any(id);
		} else {
			hid_t hspace = ::H5Screate(H5S_SCALAR);
			if (hspace < 0) {
				return (false);
			}
			if (v.type() == typeid(std::string)) {
				std::string s = boost::any_cast<std::string>(v);
				size_t nMax = s.length();
				if (nMax < 1) {
					::H5Sclose(hspace);
					return (false);
				}
				hid_t rtype = ::H5Tcopy(H5T_C_S1);
				if (rtype < 0) {
					::H5Sclose(hspace);
					return (false);
				}
				if (::H5Tset_size(rtype, nMax) < 0) {
					::H5Sclose(hspace);
					::H5Tclose(rtype);
					return (false);
				}
				id = ::H5Acreate2(ip, this->m_name.c_str(), rtype, hspace,
						H5P_DEFAULT, H5P_DEFAULT);
				::H5Sclose(hspace);
				::H5Tclose(rtype);
				if (id >= 0) {
					this->m_id = boost::any(id);
				} else {
					return (false);
				}
			} else {
				hid_t rtype = -1;
				if (!Helpers::my_conv_type(v, rtype)) {
					::H5Sclose(hspace);
					return (false);
				}
				id = ::H5Acreate2(ip, this->m_name.c_str(), rtype, hspace,
						H5P_DEFAULT, H5P_DEFAULT);
				::H5Sclose(hspace);
				if (id >= 0) {
					this->m_id = boost::any(id);
				} else {
					return (false);
				}
			}
		}
	} else {
		id = boost::any_cast<hid_t>(this->m_id);
	}
	//
	herr_t iRet = -1;
	hid_t rtype = -1;
	if (!Helpers::my_conv_type(v, rtype)) {
		return (false);
	}
	if (v.type() == typeid(std::string)) {
		std::string s = boost::any_cast<std::string>(v);
		if (s.empty()) {
			return (false);
		}
		hid_t sid = ::H5Tcopy(H5T_C_S1);
		if (sid < 0) {
			return (false);
		}
		size_t nMax = s.length();
		if (::H5Tset_size(sid, nMax) < 0) {
			::H5Tclose(sid);
			return (false);
		}
		iRet = ::H5Awrite(id, sid, s.c_str());
		::H5Tclose(sid);
	} else {
		const std::type_info &tt = v.type();
		if (tt == typeid(char)) {
			char val = boost::any_cast<char>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(signed char)) {
			signed char val = boost::any_cast<signed char>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(unsigned char)) {
			unsigned char val = boost::any_cast<unsigned char>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(short)) {
			short val = boost::any_cast<short>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(unsigned short)) {
			unsigned short val = boost::any_cast<unsigned short>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(int)) {
			int val = boost::any_cast<int>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(unsigned int)) {
			unsigned int val = boost::any_cast<unsigned int>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(long)) {
			long val = boost::any_cast<long>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(unsigned long)) {
			unsigned long val = boost::any_cast<unsigned long>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(long long)) {
			long long val = boost::any_cast<long long>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(unsigned long long)) {
			unsigned long long val = boost::any_cast<unsigned long long>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(float)) {
			float val = boost::any_cast<float>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(double)) {
			double val = boost::any_cast<double>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		} else if (tt == typeid(long double)) {
			long double val = boost::any_cast<long double>(v);
			iRet = ::H5Awrite(id, rtype, &val);
		}
	}
	return (iRet >= 0);
} // write_scalar_any
bool HDF5Attribute::write_any_vector(const std::vector<boost::any> &oVec) {
	size_t n = oVec.size();
	if (n < 1) {
		return (false);
	}
	hid_t rtype = -1;
	boost::any v;
	for (size_t i = 0; i < n; ++i) {
		const boost::any &vx = oVec[i];
		if (!vx.empty()) {
			v = vx;
			break;
		}
	}
	if (!Helpers::my_conv_type(v, rtype)) {
		return (false);
	}
	int nRank = 1;
	hsize_t pStart[] = { n };
	hsize_t pEnd[] = { n };
	hid_t id = -1;
	if (!this->is_valid()) {
		if (this->m_parent == nullptr) {
			return (false);
		}
		if (!this->m_parent->is_valid()) {
			return (false);
		}
		if (this->m_name.empty()) {
			return (false);
		}
		hid_t ip = boost::any_cast<hid_t>(this->m_parent->m_id);
		id = ::H5Aopen(ip, this->m_name.c_str(), H5P_DEFAULT);
		if (id >= 0) {
			this->m_id = boost::any(id);
		} else {
			hid_t hspace = ::H5Screate(H5S_SIMPLE);
			if (hspace < 0) {
				return (false);
			}
			if (::H5Sset_extent_simple(hspace, nRank, pStart, pEnd) < 0) {
				::H5Sclose(hspace);
				return (false);
			}
			if (v.type() == typeid(std::string)) {
				hid_t sid = ::H5Tcopy(H5T_C_S1);
				if (sid < 0) {
					::H5Sclose(hspace);
					return (false);
				}
				if (::H5Tset_size(sid, H5T_VARIABLE ) < 0) {
					::H5Tclose(sid);
					::H5Sclose(hspace);
					return (false);
				}
				id = ::H5Acreate2(ip, this->m_name.c_str(), sid, hspace,
						H5P_DEFAULT, H5P_DEFAULT);
				::H5Sclose(sid);
			} else {
				id = ::H5Acreate2(ip, this->m_name.c_str(), rtype, hspace,
						H5P_DEFAULT, H5P_DEFAULT);
			}
			::H5Sclose(hspace);
			if (id < 0) {
				return (false);
			}
			this->m_id = boost::any(id);
		}
	}
	hid_t hspace = ::H5Aget_space(id);
	if (hspace < 0) {
		return (false);
	}
	H5S_class_t cl = ::H5Sget_simple_extent_type(hspace);
	if (cl != H5S_SIMPLE) {
		::H5Sclose(hspace);
		return (false);
	}
	if (::H5Sset_extent_simple(hspace, nRank, pStart, pEnd) < 0) {
		::H5Sclose(hspace);
		return (false);
	}
	::H5Sclose(hspace);
	herr_t iRet = -1;
	const std::type_info &tt = v.type();
	if (tt == typeid(std::string)) {
		hid_t sid = ::H5Tcopy(H5T_C_S1);
		if (sid < 0) {
			return (false);
		}
		if (::H5Tset_size(sid, H5T_VARIABLE ) < 0) {
			return (false);
		}
		typedef char *PChar;
		std::unique_ptr<PChar> oBuf(new PChar[n]);
		PChar *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			::H5Tclose(sid);
			return (false);
		}
		std::vector<std::string> oTemp(n);
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			pBuf[i] = nullptr;
			if (!vx.empty()) {
				std::string s = boost::any_cast<std::string>(vx);
				if (!s.empty()) {
					oTemp[i] = s;
					pBuf[i] = (PChar) s.c_str();
				}
			}
		} // i
		iRet = ::H5Awrite(id, sid, pBuf);
		::H5Tclose(sid);
		return (iRet >= 0);
	}
	if (tt == typeid(signed char)) {
		std::unique_ptr<signed char> oBuf(new signed char[n]);
		signed char *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<signed char>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_SCHAR;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_CHAR, p);
	} else if (tt == typeid(unsigned char)) {
		std::unique_ptr<unsigned char> oBuf(new unsigned char[n]);
		unsigned char *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<unsigned char>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_UCHAR;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_UCHAR, p);
	} else if (tt == typeid(char)) {
		std::unique_ptr<char> oBuf(new char[n]);
		char *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<char>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_CHAR;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_CHAR, p);
	} else if (tt == typeid(short)) {
		std::unique_ptr<short> oBuf(new short[n]);
		short *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<short>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_SHORT;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_SHORT, p);
	} else if (tt == typeid(unsigned short)) {
		std::unique_ptr<unsigned short> oBuf(new unsigned short[n]);
		unsigned short *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<unsigned short>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_USHORT;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_USHORT, p);
	} else if (tt == typeid(int)) {
		std::unique_ptr<int> oBuf(new int[n]);
		int *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<int>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_INT;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_INT, p);
	} else if (tt == typeid(unsigned int)) {
		std::unique_ptr<unsigned int> oBuf(new unsigned int[n]);
		unsigned int *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<unsigned int>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_UINT;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_UINT, p);
	} else if (tt == typeid(long)) {
		std::unique_ptr<long> oBuf(new long[n]);
		long *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<long>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_LONG;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_LONG, p);
	} else if (tt == typeid(unsigned long long)) {
		std::unique_ptr<unsigned long long> oBuf(new unsigned long long[n]);
		unsigned long long *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<unsigned long long>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_ULLONG;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_ULLONG, p);
	} else if (tt == typeid(unsigned long)) {
		std::unique_ptr<unsigned long> oBuf(new unsigned long[n]);
		unsigned long *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<unsigned long>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_ULONG;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_ULONG, p);
	} else if (tt == typeid(long long)) {
		std::unique_ptr<long long> oBuf(new long long[n]);
		long long *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<long long>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_LLONG;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_LLONG, p);
	} else if (tt == typeid(float)) {
		std::unique_ptr<float> oBuf(new float[n]);
		float *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<float>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_FLOAT;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_FLOAT, p);
	} else if (tt == typeid(double)) {
		std::unique_ptr<double> oBuf(new double[n]);
		double *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<double>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_DOUBLE;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_DOUBLE, p);
	} else if (tt == typeid(long double)) {
		std::unique_ptr<long double> oBuf(new long double[n]);
		long double *p = oBuf.get();
		if (p == nullptr) {
			return (false);
		}
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = oVec[i];
			if (!vx.empty()) {
				p[i] = boost::any_cast<long double>(vx);
			} else {
				p[i] = Helpers::HDF5_FILL_LDOUBLE;
			}
		} // i
		iRet = ::H5Awrite(id, H5T_NATIVE_LDOUBLE, p);
	}
	return (iRet >= 0);
}
//write_any_vector
bool HDF5Attribute::read_scalar_any(boost::any &v) {
	if (!this->is_valid()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	if (id < 0) {
		return (false);
	}
	hid_t ytype = ::H5Aget_type(id);
	if (ytype < 0) {
		return (false);
	}
	hid_t xtype = ::H5Tget_native_type(ytype, H5T_DIR_ASCEND);
	::H5Tclose(ytype);
	if (xtype < 0) {
		return (false);
	}
	DataType type = typeOther;
	Helpers::my_get_type(xtype, type);
	::H5Tclose(xtype);
	//
	switch (type) {
	case typeSChar: {
		signed char dval = (signed char) 0;
		if (::H5Aread(id, H5T_NATIVE_SCHAR, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeByte: {
		unsigned char dval = (unsigned char) 0;
		if (::H5Aread(id, H5T_NATIVE_UCHAR, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeChar: {
		char dval = (char) 0;
		if (::H5Aread(id, H5T_NATIVE_CHAR, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeInt: {
		int dval = (int) 0;
		if (::H5Aread(id, H5T_NATIVE_INT, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeUInt: {
		unsigned int dval = (unsigned int) 0;
		if (::H5Aread(id, H5T_NATIVE_UINT, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeShort: {
		short dval = (short) 0;
		if (::H5Aread(id, H5T_NATIVE_SHORT, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeUShort: {
		unsigned short dval = (unsigned short) 0;
		if (::H5Aread(id, H5T_NATIVE_USHORT, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeLong: {
		long dval = (long) 0;
		if (::H5Aread(id, H5T_NATIVE_LONG, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeULong: {
		unsigned long dval = (unsigned long) 0;
		if (::H5Aread(id, H5T_NATIVE_ULONG, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeLongLong: {
		long long dval = (long long) 0;
		if (::H5Aread(id, H5T_NATIVE_LLONG, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeULongLong: {
		unsigned long long dval = (unsigned long long) 0;
		if (::H5Aread(id, H5T_NATIVE_ULLONG, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeFloat: {
		float dval = (float) 0;
		if (::H5Aread(id, H5T_NATIVE_FLOAT, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeDouble: {
		double dval = (double) 0;
		if (::H5Aread(id, H5T_NATIVE_DOUBLE, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeLongDouble: {
		long double dval = (long double) 0;
		if (::H5Aread(id, H5T_NATIVE_LDOUBLE, &dval) < 0) {
			return (false);
		}
		v = dval;
	}
		break;
	case typeString: {
		hid_t sid = ::H5Aget_type(id);
		if (sid < 0) {
			return (false);
		}
		size_t nMax = ::H5Tget_size(sid);
		if (nMax < 1) {
			::H5Tclose(sid);
			return (false);
		}
		std::unique_ptr<char> buf(new char[nMax + 1]);
		char *pBuf = buf.get();
		if (pBuf == nullptr) {
			::H5Tclose(sid);
			return (false);
		}
		if (::H5Aread(id, sid, pBuf) < 0) {
			::H5Tclose(sid);
			return (false);
		}
		::H5Tclose(sid);
		pBuf[nMax] = (char)0;
		std::string sval = pBuf;
		v = sval;
		return (true);
	}
		break;
	default:
		return (false);

	} // type
	return (true);
} // read_scalar_any
bool HDF5Attribute::read_vector_any(std::vector<boost::any> &oVec) {
	oVec.clear();
	if (!this->is_valid()) {
		return (false);
	}
	hid_t id = boost::any_cast<hid_t>(this->m_id);
	hid_t ytype = ::H5Aget_type(id);
	if (ytype < 0) {
		return (false);
	}
	hid_t xtype = ::H5Tget_native_type(ytype, H5T_DIR_ASCEND);
	::H5Tclose(ytype);
	if (xtype < 0) {
		return (false);
	}
	DataType type = typeOther;
	Helpers::my_get_type(xtype, type);
	::H5Tclose(xtype);
	hid_t hspace = ::H5Aget_space(id);
	if (hspace < 0) {
		return (false);
	}
	H5S_class_t clx = ::H5Sget_simple_extent_type(hspace);
	if (clx != H5S_SIMPLE) {
		return (false);
	}
	int nRank = ::H5Sget_simple_extent_ndims(hspace);
	if (nRank != 1) {
		::H5Sclose(hspace);
		return (false);
	}
	hsize_t dims[] = { 0 };
	if (::H5Sget_simple_extent_dims(hspace, dims, nullptr) < 0) {
		::H5Sclose(hspace);
		return (false);
	}
	size_t n = (size_t) dims[0];
	::H5Sclose(hspace);
	if (n < 1) {
		return (true);
	}
	//
	switch (type) {
	case typeSChar: {
		std::unique_ptr<signed char> oBuf(new signed char[n]);
		signed char *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_SCHAR, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i
	}
		break;
	case typeByte: {
		std::unique_ptr<unsigned char> oBuf(new unsigned char[n]);
		unsigned char *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_UCHAR, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i
	}
		break;
	case typeChar: {
		std::unique_ptr<char> oBuf(new char[n]);
		char *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_CHAR, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i
	}
		break;
	case typeInt: {
		std::unique_ptr<int> oBuf(new int[n]);
		int *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_INT, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i;
	}
		break;
	case typeUInt: {
		std::unique_ptr<unsigned int> oBuf(new unsigned int[n]);
		unsigned int *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_UINT, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i;
	}
		break;
	case typeShort: {
		std::unique_ptr<short> oBuf(new short[n]);
		short *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_SHORT, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i;
	}
		break;
	case typeUShort: {
		std::unique_ptr<unsigned short> oBuf(new unsigned short[n]);
		unsigned short *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_USHORT, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i;
	}
		break;
	case typeLong: {
		std::unique_ptr<long> oBuf(new long[n]);
		long *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_LONG, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i;
	}
		break;
	case typeULong: {
		std::unique_ptr<unsigned long> oBuf(new unsigned long[n]);
		unsigned long *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_ULONG, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i;
	}
		break;
	case typeLongLong: {
		std::unique_ptr<long long> oBuf(new long long[n]);
		long long *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_LLONG, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i;
	}
		break;
	case typeULongLong: {
		std::unique_ptr<unsigned long long> oBuf(new unsigned long long[n]);
		unsigned long long *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_ULLONG, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i;
	}
		break;
	case typeFloat: {
		std::unique_ptr<float> oBuf(new float[n]);
		float *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_FLOAT, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i
	}
		break;
	case typeDouble: {
		std::unique_ptr<double> oBuf(new double[n]);
		double *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_DOUBLE, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i
	}
		break;
	case typeLongDouble: {
		std::unique_ptr<long double> oBuf(new long double[n]);
		long double *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			return (false);
		}
		if (::H5Aread(id, H5T_NATIVE_LDOUBLE, pBuf) < 0) {
			return (false);
		}
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = pBuf[i];
		} // i
	}
		break;
	case typeString: {
		hid_t sid = ::H5Tcopy(H5T_C_S1);
		if (sid < 0) {
			return (false);
		}
		if (::H5Tset_size(sid, H5T_VARIABLE ) < 0) {
			::H5Tclose(sid);
			return (false);
		}
		typedef char *PChar;
		std::unique_ptr<PChar> oBuf(new PChar[n]);
		PChar *pBuf = oBuf.get();
		if (pBuf == nullptr) {
			::H5Tclose(sid);
			return (false);
		}
		if (::H5Aread(id, sid, pBuf) < 0) {
			::H5Tclose(sid);
			return (false);
		}
		::H5Tclose(sid);
		oVec = std::vector<boost::any>(n);
		for (size_t i = 0; i < n; ++i) {
			std::string s;
			if (pBuf[i] != nullptr) {
				s = pBuf[i];
			}
			oVec[i] = s;
		} // i
	}
		break;
	default:
		return (false);

	} // type
	return (true);
} // read_vector_any
//////////////////////////////////
} // namespace hdf5
/////////////////////////////
} /* namespace statdata */

