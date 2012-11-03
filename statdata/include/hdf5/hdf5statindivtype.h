/*
 * hdf5statindivtype.h
 *
 *  Created on: 17 oct. 2012
 *      Author: boubad
 */

#ifndef HDF5STATINDIVTYPE_H_
#define HDF5STATINDIVTYPE_H_
////////////////////////////////////
#include "../statindiv.h"
#include "hdf5datatype.h"
//////////////////////////////////
namespace statdata {
namespace hdf5 {
////////////////////////////////////////
class HDF5StatIndivDataType: public HDF5DataType {
public:
	HDF5StatIndivDataType(HDF5File *pFile);
	HDF5StatIndivDataType(HDF5Group *pGroup);
	HDF5StatIndivDataType(const HDF5StatIndivDataType &other);
	HDF5StatIndivDataType & operator=(const HDF5StatIndivDataType &other);
	virtual ~HDF5StatIndivDataType();
protected:
	virtual bool register_datatype(void);
};// class HDF5StatIndivDataType
//////////////////////////////////////////
}// namespace hdf5
} // namespace statdata
////////////////////////////////////
#endif /* HDF5STATINDIVTYPE_H_ */
