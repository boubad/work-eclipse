/*
 * hdf5dataset.cpp
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */
#include "../include/hdf5dataset.h"
#include "../include/hdf5file.h"
#include "../include/hdf5group.h"
///////////////////////////////////
#include "helpers.h"
///////////////////////////////////
namespace statdata {
    namespace hdf5 {
        const size_t HDF5Dataset::MAX_STRINGLEN = 512;
        //////////////////////////////////

        HDF5Dataset::HDF5Dataset(HDF5File *pFile, const std::string &name) :
        HDF5Object(pFile, name) {
        }

        HDF5Dataset::HDF5Dataset(HDF5Group *pGroup, const std::string &name) :
        HDF5Object(pGroup, name) {
        }

        HDF5Dataset::HDF5Dataset(const HDF5Dataset &other) :
        HDF5Object(other) {
        }

        HDF5Dataset & HDF5Dataset::operator=(const HDF5Dataset &other) {
            if (this != &other) {
                HDF5Object::operator=(other);
            }
            return (*this);
        }

        HDF5Dataset::~HDF5Dataset() {
            this->close();
        }

        bool HDF5Dataset::close(void) {
            if (!this->is_valid()) {
                return (true);
            }
            hid_t id = boost::any_cast<hid_t > (this->m_id);
            if (id < 0) {
                return (true);
            }
            if (::H5Dclose(id) >= 0) {
                this->m_id = boost::any();
                return (true);
            }
            return false;
        } // close

        bool HDF5Dataset::set_comment(const std::string &s) {
            if (!this->is_valid()) {
                if (!this->open()) {
                    return (false);
                }
            }
            hid_t id = boost::any_cast<hid_t > (this->m_id);
            return (::H5Oset_comment(id, s.c_str()) >= 0);
        } // set_comment

        bool HDF5Dataset::get_comment(std::string &s) {
            s.empty();
            if (!this->is_valid()) {
                if (!this->open()) {
                    return (false);
                }
            }
            hid_t id = boost::any_cast<hid_t > (this->m_id);
            int n = (int) ::H5Oget_comment(id, nullptr, 0);
            if (n < 1) {
                return (false);
            }
            std::unique_ptr<char> oBuf(new char[n + 1]);
            char *pBuf = oBuf.get();
            if (pBuf == nullptr) {
                return (false);
            }
            if (::H5Oget_comment(id, pBuf, n)) {
                return (false);
            }
            pBuf[n] = (char) 0;
            s = pBuf;
            return (true);
        } // get_comments

        bool HDF5Dataset::get_groups_names(std::vector<std::string> &oNames) const {
            oNames.clear();
            return (this->is_valid());
        }

        bool HDF5Dataset::get_datasets_names(std::vector<std::string> &oNames) const {
            oNames.clear();
            return (this->is_valid());
        }

        bool HDF5Dataset::get_info(statdata::DataType &type,
                std::valarray<size_t> &dims) {
            if (!this->is_valid()) {
                if (!this->open()) {
                    return (false);
                }
            }
            hid_t id = boost::any_cast<hid_t > (this->m_id);
            //
            type = typeOther;
            hid_t ytype = ::H5Dget_type(id);
            if (ytype < 0) {
                return (false);
            }
            hid_t xtype = ::H5Tget_native_type(ytype, H5T_DIR_ASCEND);
            ::H5Tclose(ytype);
            if (xtype < 0) {
                return (false);
            }
            Helpers::my_get_type(xtype, type);
            ::H5Tclose(xtype);
            if (type == typeOther) {
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
            int nRank = ::H5Sget_simple_extent_ndims(hspace);
            if (nRank < 0) {
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
            dims.resize(nRank);
            for (int i = 0; i < nRank; ++i) {
                dims[i] = odims1[i];
            }
            return (true);
        } // get_info
        ///////////////////////////////

        bool HDF5Dataset::open(void) {
            if (this->is_valid()) {
                return true;
            }
            if (this->m_parent == nullptr) {
                return (false);
            }
            if (!this->m_parent->is_valid()) {
                return (false);
            }
            if (this->m_name.empty()) {
                return (false);
            }
            hid_t pid = boost::any_cast<hid_t > (this->m_parent->m_id);
            hid_t id = ::H5Dopen2(pid, this->m_name.c_str(), H5P_DEFAULT);
            return (id >= 0);
        } // open

        bool HDF5Dataset::exists(void) const {
            if (this->is_valid()) {
                return true;
            }
            if (this->m_parent == nullptr) {
                return (false);
            }
            if (!this->m_parent->is_valid()) {
                return (false);
            }
            if (this->m_name.empty()) {
                return (false);
            }
            ::H5Eset_auto2(H5E_DEFAULT, NULL, NULL);
            hid_t pid = boost::any_cast<hid_t > (this->m_parent->m_id);
            const char *pName = this->m_name.c_str();
            hid_t temp = ::H5Dopen2(pid, pName, H5P_DEFAULT);
            bool bRet = (temp >= 0);
            if (temp >= 0) {
                ::H5Dclose(temp);
            }
            return bRet;
        } // exists

        bool HDF5Dataset::create(statdata::DataType xtype, size_t nRank,
                const size_t *dims) {
            if ((nRank < 1) || (nRank > H5S_MAX_RANK) || (dims == nullptr)) {
                return (false);
            }
            if (this->is_valid()) {
                return true;
            }
            if (this->m_parent == nullptr) {
                return (false);
            }
            if (!this->m_parent->is_valid()) {
                return (false);
            }
            if (this->m_name.empty()) {
                return (false);
            }
            hid_t proplist = ::H5Pcreate(H5P_DATASET_CREATE);
            if (proplist < 0) {
                return false;
            }
            herr_t iRet = -1;
            hid_t etype = H5T_NATIVE_FLOAT;
            hid_t stringid = -1;
            switch (xtype) {
                case typeSChar:
                {
                    signed char fillchar = Helpers::HDF5_FILL_SCHAR;
                    etype = H5T_NATIVE_SCHAR;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fillchar);
                }
                    break;
                case typeChar:
                {
                    char fillchar = Helpers::HDF5_FILL_CHAR;
                    etype = H5T_NATIVE_CHAR;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fillchar);
                }
                    break;
                case typeByte:
                    // fall through
                case typeBool:
                {
                    unsigned char fillchar = Helpers::HDF5_FILL_UCHAR;
                    etype = H5T_NATIVE_UCHAR;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fillchar);
                }
                    break;
                case typeShort:
                {
                    short fill = Helpers::HDF5_FILL_SHORT;
                    etype = H5T_NATIVE_SHORT;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeUShort:
                {
                    etype = H5T_NATIVE_USHORT;
                    unsigned short fill = Helpers::HDF5_FILL_SHORT;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeInt:
                {
                    etype = H5T_NATIVE_INT;
                    int fillint = Helpers::HDF5_FILL_INT;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fillint);
                }
                    break;
                case typeUInt:
                {
                    etype = H5T_NATIVE_UINT;
                    size_t fillsizet = Helpers::HDF5_FILL_UINT;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fillsizet);
                }
                    break;
                case typeLong:
                {
                    etype = H5T_NATIVE_LONG;
                    long fill = Helpers::HDF5_FILL_LONG;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeULong:
                {
                    etype = H5T_NATIVE_ULONG;
                    unsigned long fill = Helpers::HDF5_FILL_ULONG;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeLongLong:
                {
                    etype = H5T_NATIVE_LLONG;
                    unsigned long fill = Helpers::HDF5_FILL_LLONG;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeULongLong:
                {
                    etype = H5T_NATIVE_ULLONG;
                    unsigned long long fill = Helpers::HDF5_FILL_ULLONG;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeFloat:
                {
                    etype = H5T_NATIVE_FLOAT;
                    float fill = Helpers::HDF5_FILL_FLOAT;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeDouble:
                {
                    etype = H5T_NATIVE_DOUBLE;
                    double fill = Helpers::HDF5_FILL_DOUBLE;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeLongDouble:
                {
                    etype = H5T_NATIVE_LDOUBLE;
                    long double fill = Helpers::HDF5_FILL_LDOUBLE;
                    iRet = ::H5Pset_fill_value(proplist, etype, &fill);
                }
                    break;
                case typeString:
                    stringid = ::H5Tcopy(H5T_C_S1);
                    if (stringid < 0) {
                        return (false);
                    }
                    if (::H5Tset_size(stringid, H5T_VARIABLE) < 0) {
                        ::H5Tclose(stringid);
                        return (false);
                    }
                    etype = stringid;
                    iRet = 0;
                    break;
                default:
                    ::H5Pclose(proplist);
                    return (false);
            } // xtype
            if (iRet < 0) {
                ::H5Pclose(proplist);
                if (stringid >= 0) {
                    ::H5Tclose(stringid);
                }
                return (false);
            }
            hsize_t pStart[H5S_MAX_RANK];
            hsize_t pEnd[H5S_MAX_RANK];
            hsize_t chunks[H5S_MAX_RANK];
            for (size_t i = 0; i < nRank; ++i) {
                hsize_t size = (hsize_t) dims[i];
                if (size < 1) {
                    size = 1;
                }
                pStart[i] = size;
                pEnd[i] = H5S_UNLIMITED;
            } // i
            int nx = 1;
            for (int i = (int) (nRank - 1); i >= 0; --i) {
                chunks[i] = (hsize_t) nx;
                nx = (int) (nx * pStart[i]);
            } // i
            if (::H5Pset_chunk(proplist, nRank, chunks) < 0) {
                if (stringid >= 0) {
                    ::H5Tclose(stringid);
                }
                ::H5Pclose(proplist);
                return (false);
            }
            hid_t hspace = ::H5Screate(H5S_SIMPLE);
            if (hspace < 0) {
                if (stringid >= 0) {
                    ::H5Tclose(stringid);
                }
                ::H5Pclose(proplist);
                return (false);
            }
            if (::H5Sset_extent_simple(hspace, nRank, pStart, pEnd) < 0) {
                ::H5Pclose(proplist);
                ::H5Sclose(hspace);
                if (stringid >= 0) {
                    ::H5Tclose(stringid);
                }
                return (false);
            }
            const char *pName = this->m_name.c_str();
            hid_t plink = ::H5Pcreate(H5P_LINK_CREATE);
            hid_t p2 = ::H5Pcreate(H5P_DATASET_ACCESS);
            hid_t pid = boost::any_cast<hid_t > (this->m_parent->m_id);
            hid_t id = -1;
            if ((plink >= 0) && (p2 >= 0)) {
                id = ::H5Dcreate2(pid, pName, etype, hspace, plink, proplist, p2);
            }
            if (stringid >= 0) {
                ::H5Tclose(stringid);
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
            return (id >= 0);
        } // create
        //////////////////////////////

        bool HDF5Dataset::write_any(const std::vector<boost::any> &data,
                const std::valarray<size_t> &oCount) {
            size_t n = data.size();
            if (n < 1) {
                return (false);
            }
            size_t nDims = oCount.size();
            if ((nDims < 1) || (nDims > H5S_MAX_RANK)) {
                return (false);
            }
            size_t xDims[H5S_MAX_RANK];
            size_t nTotal = 1;
            for (size_t i = 0; i < nDims; ++i) {
                size_t c = oCount[i];
                if (c < 1) {
                    return (false);
                }
                xDims[i] = c;
                nTotal = (size_t) (nTotal * c);
            }
            if (nTotal > n) {
                return (false);
            }
            DataType rtype = typeOther;
            boost::any v;
            for (size_t i = 0; i < n; ++i) {
                const boost::any &vx = data[i];
                if (!vx.empty()) {
                    v = vx;
                    break;
                }
            } // i
            if (v.empty()) {
                return (false);
            }
            rtype = Value::get_type(v);
            if (rtype == typeOther) {
                return (false);
            }
            const std::type_info &tt = v.type();
            if (!this->is_valid()) {
                if (!this->open()) {
                    if (!this->create(rtype, nDims, xDims)) {
                        return (false);
                    }
                }
            }
            if (!this->is_valid()) {
                return (false);
            }
            hid_t id = boost::any_cast<hid_t > (this->m_id);
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
            if ((nRank < 0) || (nRank > nDims)) {
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
            herr_t iRet = -1;
            if (tt == typeid (char)) {
                std::unique_ptr<char> oBuf(new char[nTotal]);
                char *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<char>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_CHAR;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_CHAR, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (unsigned char)) {
                std::unique_ptr<unsigned char> oBuf(new unsigned char[nTotal]);
                unsigned char *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<unsigned char>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_UCHAR;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_UCHAR, hspace_mem, hspace, htransf,
                        pBuf);
            }  else if (tt == typeid (bool)) {
                std::unique_ptr<unsigned char> oBuf(new unsigned char[nTotal]);
                unsigned char *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        unsigned char c;
                        Value::get_value(vx,c);
                        pBuf[i] = c;
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_UCHAR;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_UCHAR, hspace_mem, hspace, htransf,
                        pBuf);
            }
            else if (tt == typeid (signed char)) {
                std::unique_ptr<signed char> oBuf(new signed char[nTotal]);
                signed char *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<signed char>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_SCHAR;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_SCHAR, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (short)) {
                std::unique_ptr<short> oBuf(new short[nTotal]);
                short *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<short>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_SHORT;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_SHORT, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (unsigned short)) {
                std::unique_ptr<unsigned short> oBuf(new unsigned short[nTotal]);
                unsigned short *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<unsigned short>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_USHORT;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_USHORT, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (int)) {
                std::unique_ptr<int> oBuf(new int[nTotal]);
                int *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<int>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_INT;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_INT, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (unsigned int)) {
                std::unique_ptr<unsigned int> oBuf(new unsigned int[nTotal]);
                unsigned int *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<unsigned int>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_UINT;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_UINT, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (long)) {
                std::unique_ptr<long> oBuf(new long[nTotal]);
                long *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<long>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_LONG;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_LONG, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (unsigned long)) {
                std::unique_ptr<unsigned long> oBuf(new unsigned long[nTotal]);
                unsigned long *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<unsigned long>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_ULONG;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_ULONG, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (long long)) {
                std::unique_ptr<long long> oBuf(new long long[nTotal]);
                long long *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<long long>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_LLONG;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_LLONG, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (unsigned long long)) {
                std::unique_ptr<unsigned long long> oBuf(
                        new unsigned long long[nTotal]);
                unsigned long long *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<unsigned long long>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_ULLONG;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_ULLONG, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (float)) {
                std::unique_ptr<float> oBuf(new float[nTotal]);
                float *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<float>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_FLOAT;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_FLOAT, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (double)) {
                std::unique_ptr<double> oBuf(new double[nTotal]);
                double *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<double>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_DOUBLE;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_DOUBLE, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (long double)) {
                std::unique_ptr<long double> oBuf(new long double[nTotal]);
                long double *pBuf = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    const boost::any &vx = data[i];
                    if (!vx.empty()) {
                        pBuf[i] = boost::any_cast<long double>(vx);
                    } else {
                        pBuf[i] = Helpers::HDF5_FILL_LDOUBLE;
                    }
                } // i
                iRet = ::H5Dwrite(id, H5T_NATIVE_LDOUBLE, hspace_mem, hspace, htransf,
                        pBuf);
            } else if (tt == typeid (std::string)) {
                hid_t xtype = ::H5Tcopy(H5T_C_S1);
                if (xtype < 0) {
                    ::H5Pclose(htransf);
                    ::H5Sclose(hspace);
                    ::H5Sclose(hspace_mem);
                    return (false);
                }
                if (::H5Tset_size(xtype, H5T_VARIABLE) < 0) {
                    ::H5Tclose(xtype);
                    ::H5Pclose(htransf);
                    ::H5Sclose(hspace);
                    ::H5Sclose(hspace_mem);
                    return (false);
                }
                typedef char *PChar;
                std::unique_ptr<PChar> oBuf(new PChar[nTotal]);
                PChar *pBuf = oBuf.get();
                std::vector<std::string> oTemp(n);
                for (size_t i = 0; i < n; ++i) {
                    const boost::any &vx = data[i];
                    pBuf[i] = nullptr;
                    if (!vx.empty()) {
                        std::string s = boost::any_cast<std::string > (vx);
                        if (!s.empty()) {
                            oTemp[i] = s;
                            pBuf[i] = (PChar) s.c_str();
                        }
                    }
                } // i
                iRet = ::H5Dwrite(id, xtype, hspace_mem, hspace, htransf, pBuf);
                ::H5Tclose(xtype);
            }
            ::H5Sclose(hspace);
            ::H5Sclose(hspace_mem);
            ::H5Pclose(htransf);
            return (iRet >= 0);
        } // write_any

        bool HDF5Dataset::read_any(std::vector<boost::any> &data,
                std::valarray<size_t> &dims) {
            data.clear();
            if (!this->is_valid()) {
                if (!this->open()) {
                    return (false);
                }
            }
            hid_t id = boost::any_cast<hid_t > (this->m_id);
            //
            DataType type = typeOther;
            hid_t ytype = ::H5Dget_type(id);
            if (ytype < 0) {
                return (false);
            }
            hid_t xtype = ::H5Tget_native_type(ytype, H5T_DIR_ASCEND);
            ::H5Tclose(ytype);
            if (xtype < 0) {
                return (false);
            }
            Helpers::my_get_type(xtype, type);
            ::H5Tclose(xtype);
            hid_t hspace = ::H5Dget_space(id);
            if (hspace < 0) {
                return (false);
            }
            H5S_class_t cl = ::H5Sget_simple_extent_type(hspace);
            if (cl != H5S_SIMPLE) {
                ::H5Sclose(hspace);
                return (false);
            }
            int nRank = ::H5Sget_simple_extent_ndims(hspace);
            if (nRank < 0) {
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
            dims.resize(nRank);
            size_t nTotal = 1;
            for (int i = 0; i < nRank; ++i) {
                size_t c = (size_t) odims1[i];
                nTotal = (size_t) (nTotal * c);
                dims[i] = c;
            } //i
            switch (type) {
                case typeSChar:
                {
                    std::unique_ptr<signed char> oBuf(new signed char[nTotal]);
                    signed char *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_SCHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        data[i] = pBuf[i];
                    } // i
                }
                    break;
                case typeByte:
                {
                    std::unique_ptr<unsigned char> oBuf(new unsigned char[nTotal]);
                    unsigned char *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_UCHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        data[i] = pBuf[i];
                    } // i
                }
                    break;
                case typeChar:
                {
                    std::unique_ptr<char> oBuf(new char[nTotal]);
                    char *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_CHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        data[i] = pBuf[i];
                    } // i
                }
                    break;
                case typeFloat:
                {
                    std::unique_ptr<float> oBuf(new float[nTotal]);
                    float *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        float xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_FLOAT) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeDouble:
                {
                    std::unique_ptr<double> oBuf(new double[nTotal]);
                    double *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                            pBuf) < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        double xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_DOUBLE) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeLongDouble:
                {
                    std::unique_ptr<long double> oBuf(new long double[nTotal]);
                    long double *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_LDOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                            pBuf) < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        long double xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_LDOUBLE) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeShort:
                {
                    std::unique_ptr<short> oBuf(new short[nTotal]);
                    short *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        float xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_SHORT) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeUShort:
                {
                    std::unique_ptr<unsigned short> oBuf(new unsigned short[nTotal]);
                    unsigned short *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_USHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                            pBuf) < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        short xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_USHORT) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeInt:
                {
                    std::unique_ptr<int> oBuf(new int[nTotal]);
                    int *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        int xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_INT) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeUInt:
                {
                    std::unique_ptr<unsigned int> oBuf(new unsigned int[nTotal]);
                    unsigned int *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        unsigned int xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_UINT) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeLong:
                {
                    std::unique_ptr<long> oBuf(new long[nTotal]);
                    long *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_LONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        long xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_LONG) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeULong:
                {
                    std::unique_ptr<unsigned long> oBuf(new unsigned long[nTotal]);
                    unsigned long *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_ULONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        unsigned long xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_ULONG) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeLongLong:
                {
                    std::unique_ptr<long long> oBuf(new long long[nTotal]);
                    long long *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_LLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf)
                            < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        long long xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_LLONG) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeULongLong:
                {
                    std::unique_ptr<unsigned long long> oBuf(
                            new unsigned long long[nTotal]);
                    unsigned long long *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        return (false);
                    }
                    if (::H5Dread(id, H5T_NATIVE_ULLONG, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                            pBuf) < 0) {
                        return (false);
                    }
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        unsigned long long xv = pBuf[i];
                        if (xv != Helpers::HDF5_FILL_ULLONG) {
                            data[i] = xv;
                        }
                    } // i
                }
                    break;
                case typeString:
                {
                    hid_t sid = ::H5Dget_type(id);
                    if (sid < 0) {
                        return (false);
                    }
                    typedef char *PChar;
                    std::unique_ptr<PChar> oBuf(new PChar[nTotal]);
                    PChar *pBuf = oBuf.get();
                    if (pBuf == nullptr) {
                        ::H5Tclose(sid);
                        return (false);
                    }
                    if (::H5Dread(id, sid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pBuf) < 0) {
                        return (false);
                    }
                    ::H5Tclose(sid);
                    data = std::vector<boost::any > (nTotal);
                    for (size_t i = 0; i < nTotal; ++i) {
                        std::string s;
                        if (pBuf[i] != nullptr) {
                            s = pBuf[i];
                        }
                        if (!s.empty()) {
                            data[i] = s;
                        }
                    } // i
                }
                    break;
                default:
                    return (false);
            } // type
            return (true);
        } // read_any
    } // namespace hdf5 */
    //////////////////////////
} /* namespace datastore */

