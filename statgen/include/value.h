/*
 * value.h
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */

#ifndef VALUE_H_
#define VALUE_H_
//////////////////////////
#include "dataport.h"
//////////////////////////////
#include <string>
/////////////////////////
#include <boost/any.hpp>
////////////////////////////
namespace statdata {
///////////////////////////////////////
class Value {
public:
        static statdata::DataType get_type(const boost::any &v);
        //
        static bool get_value(const boost::any &v, bool &val);
	static bool get_value(const boost::any &v, char &val);
	static bool get_value(const boost::any &v, signed char &val);
	static bool get_value(const boost::any &v, unsigned char &val);
	static bool get_value(const boost::any &v, short &val);
	static bool get_value(const boost::any &v, unsigned short &val);
	static bool get_value(const boost::any &v, int &val);
	static bool get_value(const boost::any &v, unsigned int &val);
	static bool get_value(const boost::any &v, long &val);
	static bool get_value(const boost::any &v, unsigned long &val);
	static bool get_value(const boost::any &v, long long &val);
	static bool get_value(const boost::any &v, unsigned long long &val);
	static bool get_value(const boost::any &v, float &val);
	static bool get_value(const boost::any &v, double &val);
	static bool get_value(const boost::any &v, long double &val);
	static bool get_value(const boost::any &v, std::string &val);
	static bool get_value(const boost::any &v, std::wstring &val);
};
// class Value
}// namespace statdata
///////////////////////////////////////////
#endif /* VALUE_H_ */
