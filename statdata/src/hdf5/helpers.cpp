/*
 * helpers.cpp
 *
 *  Created on: 16 oct. 2012
 *      Author: boubad
 */
#include "helpers.h"
///////////////////////////
#define FILL_SCHAR	((signed char)-127)
#define FILL_CHAR	((char)0)
#define FILL_SHORT	((short)-32767)
#define FILL_INT	(-2147483647L)
#define FILL_FLOAT	(9.9692099683868690e+36f) /* near 15 * 2^119 */
#define FILL_DOUBLE	(9.9692099683868690e+36)
////////////////////////////////////
namespace statdata {
    namespace hdf5 {
        /////////////////////////////////////
        const char Helpers::HDF5_FILL_CHAR = FILL_CHAR;
        const signed char Helpers::HDF5_FILL_SCHAR = FILL_SCHAR;
        const unsigned char Helpers::HDF5_FILL_UCHAR = ((unsigned char) 0);
        const short Helpers::HDF5_FILL_SHORT = FILL_SHORT;
        const unsigned short Helpers::HDF5_FILL_USHORT = (unsigned short) FILL_SHORT;
        const int Helpers::HDF5_FILL_INT = FILL_INT;
        const unsigned int Helpers::HDF5_FILL_UINT = (unsigned int) FILL_INT;
        const long Helpers::HDF5_FILL_LONG = (long) FILL_INT;
        const unsigned long Helpers::HDF5_FILL_ULONG = (unsigned long) FILL_INT;
        const long long Helpers::HDF5_FILL_LLONG = (long long) FILL_INT;
        const unsigned long long Helpers::HDF5_FILL_ULLONG =
                (unsigned long long) FILL_INT;
        const float Helpers::HDF5_FILL_FLOAT = (float) FILL_FLOAT;
        const double Helpers::HDF5_FILL_DOUBLE = (double) FILL_DOUBLE;
        const long double Helpers::HDF5_FILL_LDOUBLE = (long double) FILL_DOUBLE;
        //////////////////////////////////////////////

        bool Helpers::my_get_type(hid_t xtype,
                statdata::DataType &type) {
            H5T_class_t clt = ::H5Tget_class(xtype);
            bool bRet = false;
            if (clt == H5T_FLOAT) {
                if (::H5Tequal(xtype, H5T_NATIVE_LDOUBLE) > 0) {
                    type = statdata::typeLongDouble;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_DOUBLE) > 0) {
                    type = statdata::typeDouble;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_FLOAT) > 0) {
                    type = statdata::typeFloat;
                    bRet = true;
                } else {
                    type = statdata::typeDouble;
                    bRet = true;
                }
            } else if (clt == H5T_INTEGER) {
                if (::H5Tequal(xtype, H5T_NATIVE_ULLONG) > 0) {
                    type = statdata::typeULongLong;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_LLONG) > 0) {
                    type = statdata::typeLongLong;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_ULONG) > 0) {
                    type = statdata::typeULong;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_LONG) > 0) {
                    type = statdata::typeLong;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_UINT) > 0) {
                    type = statdata::typeUInt;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_INT) > 0) {
                    type = statdata::typeInt;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_USHORT) > 0) {
                    type = statdata::typeUShort;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_SHORT) > 0) {
                    type = statdata::typeShort;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_UCHAR) > 0) {
                    type = statdata::typeByte;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_SCHAR) > 0) {
                    type = statdata::typeSChar;
                    bRet = true;
                } else if (::H5Tequal(xtype, H5T_NATIVE_CHAR) > 0) {
                    type = statdata::typeChar;
                    bRet = true;
                } else {
                    type = statdata::typeInt;
                }
            } else if (clt == H5T_STRING) {
                type = statdata::typeString;
            }
            return bRet;
        } // my_get_type
        ///////////////////////////////////////////

        bool Helpers::my_conv_type(const boost::any &v, hid_t &rtype) {
            bool bRet = false;
            if (!v.empty()) {
                const std::type_info &tt = v.type();
                if (tt == typeid (std::string)) {
                    rtype = H5T_C_S1;
                    bRet = true;
                } else if (tt == typeid (signed char)) {
                    rtype = H5T_NATIVE_SCHAR;
                    bRet = true;
                } else if (tt == typeid (unsigned char)) {
                    rtype = H5T_NATIVE_UCHAR;
                    bRet = true;
                } else if (tt == typeid (char)) {
                    rtype = H5T_NATIVE_CHAR;
                    bRet = true;
                } else if (tt == typeid (short)) {
                    rtype = H5T_NATIVE_SHORT;
                    bRet = true;
                } else if (tt == typeid (unsigned short)) {
                    rtype = H5T_NATIVE_USHORT;
                    bRet = true;
                } else if (tt == typeid (int)) {
                    rtype = H5T_NATIVE_INT;
                    bRet = true;
                } else if (tt == typeid (unsigned int)) {
                    rtype = H5T_NATIVE_UINT;
                    bRet = true;
                } else if (tt == typeid (long)) {
                    rtype = H5T_NATIVE_LONG;
                    bRet = true;
                } else if (tt == typeid (unsigned long)) {
                    rtype = H5T_NATIVE_ULONG;
                    bRet = true;
                } else if (tt == typeid (long long)) {
                    rtype = H5T_NATIVE_LLONG;
                    bRet = true;
                } else if (tt == typeid (unsigned long long)) {
                    rtype = H5T_NATIVE_ULLONG;
                    bRet = true;
                } else if (tt == typeid (float)) {
                    rtype = H5T_NATIVE_FLOAT;
                    bRet = true;
                } else if (tt == typeid (double)) {
                    rtype = H5T_NATIVE_DOUBLE;
                    bRet = true;
                } else if (tt == typeid (long double)) {
                    rtype = H5T_NATIVE_LDOUBLE;
                    bRet = true;
                } else if (tt == typeid (bool)) {
                    rtype = H5T_NATIVE_UCHAR;
                    bRet = true;
                }
            } // not empty
            return (bRet);
        } //my_conv_type
        ////////////////////////////////////////////////
    } /* namespace hdf5 */
}/* namespace statdata */

