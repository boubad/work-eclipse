/*
 * hdf5datatype.h
 *
 *  Created on: 15 oct. 2012
 *      Author: boubad
 */

#ifndef HDF5DATATYPE_H_
#define HDF5DATATYPE_H_
//////////////////////////////
#include "hdf5object.h"
//////////////////////////////////////
namespace statdata {
namespace hdf5 {
//////////////////////////////////////
class HDF5File;
class HDF5Group;
////////////////////////////////////////
class HDF5DataType: public HDF5Object {
protected:
	HDF5DataType(HDF5File *pFile, const std::string &name,
			bool bCreateIfNotExists = false, size_t nSize = 0);
	HDF5DataType(HDF5Group *pGroup, const std::string &name,
			bool bCreateIfNotExists = false, size_t nSize = 0);
	HDF5DataType(const HDF5DataType &other);
	HDF5DataType & operator=(const HDF5DataType &other);
public:
	virtual ~HDF5DataType();
public:
	virtual bool close(void);
	virtual bool exists(void) const;
	virtual bool is_valid(void) const;
	virtual bool get_name(std::string &s) const;
	virtual bool get_groups_names(std::vector<std::string> &oNames) const;
	virtual bool get_datasets_names(std::vector<std::string> &oNames) const;
public:
	bool commit(void);
protected:
	bool is_created(void) const {
		return (this->m_created);
	}
	virtual bool register_datatype(void) = 0;
private:
	bool check(bool bCreateIfNotExists, size_t nSize);
protected:
	bool m_created;
};
} /* namespace hdf5 */
} /* namespace statdata */
//////////////////////////
#endif /* HDF5DATATYPE_H_ */
