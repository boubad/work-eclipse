/*
 * value.cpp
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */
#include "../include/value.h"
////////////////////////////
namespace statdata {
////////////////////////////////////////
bool Value::get_value(const boost::any &v, char &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (char) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (char) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (char) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (char) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (char) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (char) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (char) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (char) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (char) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (char) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (char) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (char) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (char) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			val = (char) (*(ss.begin()));
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, signed char &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (signed char) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (signed char) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (signed char) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (signed char) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (signed char) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (signed char) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (signed char) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (signed char) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (signed char) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (signed char) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (signed char) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (signed char) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (signed char) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			val = (signed char) (*(ss.begin()));
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, unsigned char &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (unsigned char) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (unsigned char) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (unsigned char) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (unsigned char) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (unsigned char) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (unsigned char) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (unsigned char) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (unsigned char) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (unsigned char) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (unsigned char) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (unsigned char) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (unsigned char) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (unsigned char) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			val = (unsigned char) (*(ss.begin()));
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, short &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (short) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (short) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (short) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (short) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (short) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (short) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (short) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (short) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (short) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (short) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (short) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (short) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (short) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, unsigned short &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (unsigned short) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (unsigned short) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (unsigned short) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (unsigned short) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (unsigned short) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (unsigned short) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (unsigned short) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (unsigned short) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (unsigned short) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (unsigned short) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (unsigned short) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (unsigned short) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (unsigned short) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, int &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (int) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (int) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (int) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (int) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (int) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (int) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (int) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (int) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (int) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (int) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (int) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (int) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (int) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (int) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, unsigned int &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (unsigned int) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (unsigned int) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (unsigned int) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (unsigned int) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (unsigned int) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (unsigned int) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (unsigned int) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (unsigned int) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (int) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (int) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (int) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (unsigned int) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (unsigned int) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (unsigned int) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, long &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (long) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (long) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (long) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (long) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (long) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (long) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (long) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (long) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (long) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (long) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (long) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (long) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (long) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (long) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, unsigned long &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (unsigned long) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (unsigned long) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (unsigned long) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (unsigned long) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (unsigned long) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (unsigned long) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (unsigned long) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (unsigned long) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (unsigned long) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (unsigned long) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (unsigned long) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (unsigned long) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (unsigned long) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (unsigned long) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, long long &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (long long) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (long long) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (long long) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (long long) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (long long) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (long long) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (long long) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (long long) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (long long) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (long long) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (long long) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (long long) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (long long) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (long long) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, unsigned long long &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (unsigned long long) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (unsigned long long) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (unsigned long long) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (unsigned long long) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (unsigned long long) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (unsigned long long) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (unsigned long long) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (unsigned long long) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (unsigned long long) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (unsigned long long) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (unsigned long long) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (unsigned long long) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (unsigned long long) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (unsigned long long) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, float &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (float) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (float) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (float) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (float) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (float) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (float) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (float) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (float) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (float) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (float) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (float) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (float) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (float) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (float) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, double &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (double) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (double) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (double) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (double) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (double) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (double) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (double) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (double) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (double) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (double) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (double) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (double) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (double) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (double) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, long double &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		val = (long double) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		val = (long double) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		val = (long double) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt ==  typeid(short)) {
		val = (long double) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		val = (long double) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt ==  typeid(int)) {
		val = (long double) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		val = (long double) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt ==  typeid(long)) {
		val = (long double) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		val = (long double) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		val = (long double) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		val = (long double) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt ==  typeid(float)) {
		val = (long double) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt ==  typeid(double)) {
		val = (long double) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		val = (long double) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, std::string &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		char xval = boost::any_cast<char>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		signed char xval = boost::any_cast<signed char>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		unsigned char xval = boost::any_cast<unsigned char>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(short)) {
		short xval = boost::any_cast<short>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		unsigned short xval = boost::any_cast<unsigned short>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(int)) {
		int xval = boost::any_cast<int>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		unsigned int xval = boost::any_cast<unsigned int>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(long)) {
		long xval = boost::any_cast<long>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		unsigned long xval = boost::any_cast<unsigned long>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		long long xval = boost::any_cast<long long>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;

	} else if (tt ==  typeid(unsigned long long)) {
		unsigned long long xval = boost::any_cast<unsigned long long>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(float)) {
		float xval = boost::any_cast<float>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(double)) {
		double xval = boost::any_cast<double>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		long double xval = boost::any_cast<long double>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(std::string)) {
		val = boost::any_cast<std::string>(v);
		bRet = true;
	}
	return (bRet);
} // get_value
bool Value::get_value(const boost::any &v, std::wstring &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt ==  typeid(char)) {
		char xval = boost::any_cast<char>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(signed char)) {
		signed char xval = boost::any_cast<signed char>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned char)) {
		unsigned char xval = boost::any_cast<unsigned char>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(short)) {
		short xval = boost::any_cast<short>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned short)) {
		unsigned short xval = boost::any_cast<unsigned short>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(int)) {
		int xval = boost::any_cast<int>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned int)) {
		unsigned int xval = boost::any_cast<unsigned int>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(long)) {
		long xval = boost::any_cast<long>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned long)) {
		unsigned long xval = boost::any_cast<unsigned long>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(long long)) {
		long long xval = boost::any_cast<long long>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(unsigned long long)) {
		unsigned long long xval = boost::any_cast<unsigned long long>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(float)) {
		float xval = boost::any_cast<float>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(double)) {
		double xval = boost::any_cast<double>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(long double)) {
		long double xval = boost::any_cast<long double>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt ==  typeid(std::wstring)) {
		val = boost::any_cast<std::wstring>(v);
		bRet = true;
	}else if (tt ==  typeid(std::string)) {
		std::string xval = boost::any_cast<std::string>(v);
		if (xval.empty()){
			val = std::wstring();
		} else {
			std::wstring s(xval.length(),L' ');
			std::copy(xval.begin(),xval.end(),s.begin());
			val = s;
		}
		bRet = true;
	}
	return (bRet);
} // get_value
////////////////////////////////
} // namespace statdata
