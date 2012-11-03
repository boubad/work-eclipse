/*
 * helpers.h
 *
 *  Created on: 16 oct. 2012
 *      Author: boubad
 */

#ifndef HELPERS_H_
#define HELPERS_H_
//////////////////////////////////
#include <boost/any.hpp>
#include "../../include/hdf5/hdf5object.h"
//////////////////////////////////////
#ifdef _MSC_VER
#include <hdf5/hdf5.h>
#else
#include <hdf5.h>
#endif
///////////////////////////////////
namespace statdata {
namespace hdf5 {
//////////////////////////////////////////////
class Helpers {
public:
	static bool my_conv_type(const boost::any &v, hid_t &rtype);
	static bool my_get_type(hid_t inType, statdata::hdf5::HDF5Object::DataType &rtype);
public:
	static const char HDF5_FILL_CHAR;
	static const signed char HDF5_FILL_SCHAR;
	static const unsigned char HDF5_FILL_UCHAR;
	static const short HDF5_FILL_SHORT;
	static const unsigned short HDF5_FILL_USHORT;
	static const int HDF5_FILL_INT;
	static const unsigned int HDF5_FILL_UINT;
	static const long HDF5_FILL_LONG;
	static const unsigned long HDF5_FILL_ULONG;
	static const long long HDF5_FILL_LLONG;
	static const unsigned long long HDF5_FILL_ULLONG;
	static const float HDF5_FILL_FLOAT;
	static const double HDF5_FILL_DOUBLE;
	static const long double HDF5_FILL_LDOUBLE;
}; // class Helpers
/////////////////////////////////////////////
}/* namespace hdf5 */
}/* namespace statdata */

////////////////////////////////////
#endif /* HELPERS_H_ */
