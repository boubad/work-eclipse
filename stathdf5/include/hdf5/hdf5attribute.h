/*
 * hdf5attribute.h
 *
 *  Created on: 13 oct. 2012
 *      Author: boubad
 */

#ifndef HDF5ATTRIBUTE_H_
#define HDF5ATTRIBUTE_H_
//////////////////////////////
#include "hdf5object.h"
#include "value.h"
////////////////////////////////
namespace statdata {
namespace hdf5 {
////////////////////////////
class HDF5Attribute: public statdata::hdf5::HDF5Object {
protected:
	HDF5Attribute();
public:
	HDF5Attribute(statdata::hdf5::HDF5Group *pParent, const std::string &name);
	HDF5Attribute(statdata::hdf5::HDF5Dataset *pParent, const std::string &name);
	HDF5Attribute(const HDF5Attribute &other);
	HDF5Attribute & operator=(const HDF5Attribute &other);
	virtual ~HDF5Attribute();
public:
	virtual bool exists(void) const;
	virtual bool close(void);
	virtual bool has_attribute(const std::string &name) const;
	virtual bool get_groups_names(std::vector<std::string> &oNames) const;
	virtual bool get_datasets_names(std::vector<std::string> &oNames) const;
	virtual bool get_attributes_names(std::vector<std::string> &oNames) const;
public:
	bool get_info(statdata::hdf5::HDF5Object::DataType &type, size_t &nSize,
			bool &bScalar) const;
	//
	bool write_scalar_any(const boost::any &v);
	bool write_any_vector(const std::vector<boost::any> &oVec);
	template<typename T>
	bool write_scalar(const T &v) {
		boost::any vv(v);
		return (this->write_scalar_any(vv));
	}
	template<typename T, class ALLOCT>
	bool write_vector(const std::vector<T, ALLOCT> &data) {
		size_t n = data.size();
		std::vector<boost::any> vv(n);
		for (size_t i = 0; i < n; ++i) {
			vv[i] = boost::any(data[i]);
		}		// i
		return (this->write_any_vector(vv));
	}
	template<typename T>
	bool write_vector(const std::valarray<T> &data) {
		size_t n = data.size();
		std::vector<boost::any> vv(n);
		for (size_t i = 0; i < n; ++i) {
			vv[i] = boost::any(data[i]);
		}
		return (this->write_any_vector(vv));
	}
	//
	bool read_scalar_any(boost::any &v);
	bool read_vector_any(std::vector<boost::any> &oVec);
	//
	template<typename T>
	bool read_scalar(T &val) {
		boost::any vv;
		if (!this->read_scalar_any(vv)) {
			return (false);
		}
		return (Value::get_value(vv, val));
	}
	template<typename T, class ALLOCT>
	bool read_vector(std::vector<T, ALLOCT> &oVec) {
		std::vector<boost::any> vv;
		if (!this->read_vector_any(vv)) {
			return (false);
		}
		size_t n = vv.size();
		oVec = std::vector<T, ALLOCT>(n);
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = vv[i];
			if (!vx.empty()) {
				T val;
				if (Value::get_value(vx, val)) {
					oVec[i] = val;
				}
			}
		}		// i
		return (true);
	}
	template<typename T>
	bool read_vector(std::valarray<T> &oVec) {
		std::vector<boost::any> vv;
		if (!this->read_vector_any(vv)) {
			return (false);
		}
		size_t n = vv.size();
		oVec.resize(n);
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = vv[i];
			if (!vx.empty()) {
				T val;
				if (Value::get_value(vx, val)) {
					oVec[i] = val;
				}
			}
		}		// i
		return (true);
	}
protected:
	bool open(void);
};
} /* namespace hdf5 */
//////////////////////////////////////////////////
} /* namespace statdata */

#endif /* HDF5ATTRIBUTE_H_ */
