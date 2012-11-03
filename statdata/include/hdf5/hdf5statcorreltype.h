/*
 * hdf5statcorreltype.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef HDF5STATCORRELTYPE_H_
#define HDF5STATCORRELTYPE_H_
////////////////////////////////////
#include "../statcorrel.h"
#include "hdf5datatype.h"
//////////////////////////////////
namespace statdata {
namespace hdf5 {
////////////////////////////////////////
class HDF5StatCorrelDataType: public HDF5DataType {
public:
	HDF5StatCorrelDataType(HDF5File *pFile);
	HDF5StatCorrelDataType(HDF5Group *pGroup);
	HDF5StatCorrelDataType(const HDF5StatCorrelDataType &other);
	HDF5StatCorrelDataType & operator=(const HDF5StatCorrelDataType &other);
	virtual ~HDF5StatCorrelDataType();
protected:
	virtual bool register_datatype(void);
};
// class HDF5StatCorrelDataType
//////////////////////////////////////////
}// namespace hdf5
} // namespace statdata
////////////////////////////////////

#endif /* HDF5STATCORRELTYPE_H_ */
