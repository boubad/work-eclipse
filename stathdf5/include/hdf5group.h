/*
 * hdf5group.h
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */

#ifndef HDF5GROUP_H_
#define HDF5GROUP_H_
//////////////////////////////////////
#include "hdf5object.h"
//////////////////////////////////////
namespace statdata {
namespace hdf5 {
class HDF5File;
//////////////////////////////////////
class HDF5Group: public HDF5Object {
protected:
	HDF5Group(HDF5Object *pParent = nullptr);
public:
	HDF5Group(HDF5File *pFile,
			const std::string &name, bool bCreateIfNotExists = false);
	HDF5Group(HDF5Group *pGroup,
			const std::string &name, bool bCreateIfNotExists = false);
	HDF5Group(const HDF5Group &other);
	HDF5Group & operator=(const HDF5Group &other);
	virtual ~HDF5Group();
public:
	virtual bool close(void);
	virtual bool exists(void) const;
public:
	bool set_comment(const std::string &s);
	bool get_comment(std::string &s);
private:
	void check_exists(bool bCreateIfNotExists);
};
} /* namespace hdf5 */
} /* namespace statdata */

#endif /* HDF5GROUP_H_ */
