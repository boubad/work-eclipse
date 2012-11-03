/*
 * hdf5file.h
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */

#ifndef HDF5FILE_H_
#define HDF5FILE_H_
///////////////////////////////
#include "hdf5object.h"
///////////////////////////////////
namespace statdata {
namespace hdf5 {
/////////////////////////////
class HDF5File: public  HDF5Object {
public:
	HDF5File();
	HDF5File(const std::string &filename, bool bReadOnly = true);
	HDF5File(const HDF5File &other);
	HDF5File & operator=(const HDF5File &other);
	virtual ~HDF5File();
public:
	virtual bool exists(void) const;
	virtual bool has_attribute(const std::string &name) const;
	virtual bool close(void);
	virtual bool get_name(std::string &s) const;
	virtual bool get_attributes_names(std::vector<std::string> &oNames) const;
	virtual bool create(const std::string &filename);
	virtual bool open(const std::string &filename, bool bReadOnly = true);
	virtual bool open(bool bReadOnly = true);
};
} // namespace hdf5
///////////////////////////////
} /* namespace statdata */
#endif /* HDF5FILE_H_ */
