/*
 * hdf5object.h
 *
 *  Created on: Oct 12, 2012
 *      Author: boubad
 */

#ifndef HDF5OBJECT_H_
#define HDF5OBJECT_H_
//////////////////////////////
#include <dataport.h>
/////////////////////////////////////////
#include <cassert>
#include <ctime>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <valarray>
#include <list>
#include <set>
#include <map>
#include <memory>
////////////////////////////
#include <boost/any.hpp>
////////////////////////////
namespace statdata {
namespace hdf5 {
/////////////////////////////////
class HDF5Attribute;
class HDF5Group;
class HDF5Dataset;
class HDF5DataType;
///////////////////////////
class HDF5Object {
	friend class HDF5Attribute;
	friend class HDF5Group;
	friend class HDF5Dataset;
	friend class HDF5DataType;
public:
	typedef enum {
		typeOther,
		typeChar,
		typeSChar,
		typeByte,
		typeShort,
		typeUShort,
		typeInt,
		typeUInt,
		typeLong,
		typeULong,
		typeLongLong,
		typeULongLong,
		typeFloat,
		typeDouble,
		typeLongDouble,
		typeString,
		typeStatIndiv,
		typeStatCorrel
	} DataType;
public:
	virtual ~HDF5Object();
protected:
	HDF5Object(HDF5Object *pParent = nullptr);
	HDF5Object(HDF5Object *pParent, const std::string &name);
	HDF5Object(const HDF5Object &other);
	HDF5Object & operator=(const HDF5Object &other);
public:
	const boost::any & id(void) const {
		return (this->m_id);
	}
	HDF5Object *parent(void) const {
		return (this->m_parent);
	}
public:
	virtual bool close(void) = 0;
	virtual bool exists(void) const = 0;
public:
	virtual bool is_valid(void) const;
	virtual bool has_attribute(const std::string &name) const;
	virtual bool get_name(std::string &s) const;
	virtual bool get_groups_names(std::vector<std::string> &oNames) const;
	virtual bool get_datasets_names(std::vector<std::string> &oNames) const;
	virtual bool get_attributes_names(std::vector<std::string> &oNames) const;
protected:
	HDF5Object *m_parent;
	boost::any m_id;
	std::string m_name;
	//
	static const size_t NAME_BUFFER_SIZE;
};
} // namespace hdf5
}// namespace statdata
///////////////////////////////////////////

#endif /* HDF5OBJECT_H_ */
