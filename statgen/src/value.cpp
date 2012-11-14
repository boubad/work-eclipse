/*
 * value.cpp
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */
#include "../include/value.h"
/////////////////////////////
#include <iostream>
#include <sstream>
/////////////////////////////
#include <boost/algorithm/string.hpp>
#include <cctype>
#include <string.h>
////////////////////////////
namespace statdata {
////////////////////////////////////////
bool Value::time2String(const time_t &t, std::string &val) {
	val.clear();
	struct tm m_tm;
	struct tm *px = ::localtime(&t);
	if (px == nullptr) {
		return (false);
	}
	::memcpy(&m_tm, px, sizeof(m_tm));
	std::stringstream os;
	os << (m_tm.tm_year + 1900);
	os << "-";
	int n = m_tm.tm_mon;
	n++;
	if (n < 10) {
		os << "0" << n;
	} else {
		os << n;
	}
	os << "-";
	n = m_tm.tm_mday;
	if (n < 10) {
		os << "0" << n;
	} else {
		os << n;
	}
	os << " ";
	n = m_tm.tm_hour;
	if (n < 10) {
		os << "0" << n;
	} else {
		os << n;
	}
	os < ":";
	n = m_tm.tm_min;
	if (n < 10) {
		os << "0" << n;
	} else {
		os << n;
	}
	os < ":";
	n = m_tm.tm_sec;
	if (n < 10) {
		os << "0" << n;
	} else {
		os << n;
	}
	val = os.str();
	return (true);
} // time2String
bool Value::time2String(const time_t &t, std::wstring &val) {
	val.clear();
	struct tm m_tm;
	struct tm *px = ::localtime(&t);
	if (px == nullptr) {
		return (false);
	}
	::memcpy(&m_tm, px, sizeof(m_tm));
	std::wstringstream os;
	os << (m_tm.tm_year + 1900);
	os << L"-";
	int n = m_tm.tm_mon;
	n++;
	if (n < 10) {
		os << L"0" << n;
	} else {
		os << n;
	}
	os << L"-";
	n = m_tm.tm_mday;
	if (n < 10) {
		os << L"0" << n;
	} else {
		os << n;
	}
	os << L" ";
	n = m_tm.tm_hour;
	if (n < 10) {
		os << L"0" << n;
	} else {
		os << n;
	}
	os < L":";
	n = m_tm.tm_min;
	if (n < 10) {
		os << L"0" << n;
	} else {
		os << n;
	}
	os < L":";
	n = m_tm.tm_sec;
	if (n < 10) {
		os << L"0" << n;
	} else {
		os << n;
	}
	val = os.str();
	return (true);
} // time2String
bool Value::string2time(const std::string &s, time_t &val) {
	val = (time_t) 0;
	std::string ss = boost::trim_copy(s);
	if (ss.empty()) {
		return (false);
	}
	int nYear = 0;
	int nMonth = 0;
	int nDay = 0;
	int nHour = 0;
	int nMinute = 0;
	int nSecond = 0;
	auto it = ss.begin();
	auto itstart = it;
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::string syear(itstart, it);
			std::stringstream in(syear);
			in >> nYear;
			while ((it != ss.end()) && (!std::isdigit(*it))) {
				++it;
			}
			itstart = it;
			break;
		}
		++it;
	} // year
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::string smonth(itstart, it);
			std::stringstream in(smonth);
			in >> nMonth;
			while ((it != ss.end()) && (!std::isdigit(*it))) {
				++it;
			}
			itstart = it;
			break;
		}
		++it;
	} // month
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::string sday(itstart, it);
			std::stringstream in(sday);
			in >> nDay;
			while ((it != ss.end()) && (!std::isdigit(*it))) {
				++it;
			}
			itstart = it;
			break;
		}
		++it;
	} // day
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::string shour(itstart, it);
			std::stringstream in(shour);
			in >> nHour;
			while ((it != ss.end()) && (!std::isdigit(*it))) {
				++it;
			}
			itstart = it;
			break;
		}
		++it;
	} // hour
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::string smin(itstart, it);
			std::stringstream in(smin);
			in >> nMinute;
			while ((it != ss.end()) && (!std::isdigit(*it))) {
				++it;
			}
			itstart = it;
			break;
		}
		++it;
	} // minute
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::string ssec(itstart, it);
			std::stringstream in(ssec);
			in >> nSecond;
			while ((it != ss.end()) && (!std::isdigit(*it))) {
				++it;
			}
			itstart = it;
			break;
		}
		++it;
	} // second
	while (nSecond > 59) {
		nSecond -= 60;
		nMinute++;
	}
	while (nMinute > 59) {
		nMinute -= 60;
		nHour++;
	}
	while (nHour > 23) {
		nHour -= 24;
		nDay++;
	}
	while (nDay > 31) {
		nDay -= 31;
		nMonth++;
	}
	--nMonth;
	nYear -= 1900;
	while (nMonth > 11) {
		nMonth -= 12;
		nYear++;
	}
	struct tm m_tm;
	::memset(&m_tm, 0, sizeof(m_tm));
	m_tm.tm_sec = nSecond;
	m_tm.tm_min = nMinute;
	m_tm.tm_hour = nHour;
	m_tm.tm_mday = nDay;
	m_tm.tm_mon = nMonth;
	m_tm.tm_year = nYear;
	val = ::mktime(&m_tm);
	return (true);
} // string2time
bool Value::string2time(const std::wstring &s, time_t &val) {
	val = (time_t) 0;
	std::wstring ss = boost::trim_copy(s);
	if (ss.empty()) {
		return (false);
	}
	int nYear = 0;
	int nMonth = 0;
	int nDay = 0;
	int nHour = 0;
	int nMinute = 0;
	int nSecond = 0;
	auto it = ss.begin();
	auto itstart = it;
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::wstring syear(itstart, it);
			std::wstringstream in(syear);
			in >> nYear;
			++it;
			itstart = it;
			break;
		}
		++it;
	} // year
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::wstring smonth(itstart, it);
			std::wstringstream in(smonth);
			in >> nMonth;
			++it;
			itstart = it;
			break;
		}
		++it;
	} // month
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::wstring sday(itstart, it);
			std::wstringstream in(sday);
			in >> nDay;
			++it;
			itstart = it;
			break;
		}
		++it;
	} // day
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::wstring shour(itstart, it);
			std::wstringstream in(shour);
			in >> nHour;
			++it;
			itstart = it;
			break;
		}
		++it;
	} // hour
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::wstring smin(itstart, it);
			std::wstringstream in(smin);
			in >> nMinute;
			++it;
			itstart = it;
			break;
		}
		++it;
	} // minute
	while (it != ss.end()) {
		if (!std::isdigit(*it)) {
			std::wstring ssec(itstart, it);
			std::wstringstream in(ssec);
			in >> nSecond;
			++it;
			itstart = it;
			break;
		}
		++it;
	} // second
	while (nSecond > 59) {
		nSecond -= 60;
		nMinute++;
	}
	while (nMinute > 59) {
		nMinute -= 60;
		nHour++;
	}
	while (nHour > 23) {
		nHour -= 24;
		nDay++;
	}
	while (nDay > 31) {
		nDay -= 31;
		nMonth++;
	}
	--nMonth;
	nYear -= 1900;
	while (nMonth > 11) {
		nMonth -= 12;
		nYear++;
	}
	struct tm m_tm;
	::memset(&m_tm, 0, sizeof(m_tm));
	m_tm.tm_sec = nSecond;
	m_tm.tm_min = nMinute;
	m_tm.tm_hour = nHour;
	m_tm.tm_mday = nDay;
	m_tm.tm_mon = nMonth;
	m_tm.tm_year = nYear;
	val = ::mktime(&m_tm);
	return (true);
} // string2time
std::ostream & Value::to_stream(std::ostream &os, const boost::any &val,
		statdata::DataType type) {
	if (val.empty()) {
		os << "N/A";
	} else {
		std::string s;
		if ((type == typeDateTime) || (type == typeDate)
				|| (type == typeTime)) {
			time_t x;
			Value::get_value(val, x);
			time2String(x, s);
		} else {
			Value::get_value(val, s);
		}
		if (!s.empty()) {
			os << s;
		} else {
			os << "N/A";
		}
	}
	return os;
} // to_stream

std::wostream & Value::to_stream(std::wostream &os, const boost::any &val,
		statdata::DataType type) {
	if (val.empty()) {
		os << L"N/A";
	} else {
		std::wstring s;
		if ((type == typeDateTime) || (type == typeDate)
				|| (type == typeTime)) {
			time_t x;
			Value::get_value(val, x);
			time2String(x, s);
		} else {
			Value::get_value(val, s);
		}
		if (!s.empty()) {
			os << s;
		} else {
			os << L"N/A";
		}
	}
	return os;
} // to_stream

statdata::DataType Value::get_type(const boost::any &v) {
	statdata::DataType rtype = statdata::typeOther;
	if (v.empty()) {
		return (rtype);
	}
	const std::type_info &tt = v.type();
	if (tt == typeid(std::string)) {
		rtype = typeString;
	} else if (tt == typeid(bool)) {
		rtype = typeByte;
	} else if (tt == typeid(signed char)) {
		rtype = typeSChar;
	} else if (tt == typeid(char)) {
		rtype = typeChar;
	} else if (tt == typeid(unsigned char)) {
		rtype = typeByte;
	} else if (tt == typeid(short)) {
		rtype = typeShort;
	} else if (tt == typeid(unsigned short)) {
		rtype = typeUShort;
	} else if (tt == typeid(int)) {
		rtype = typeInt;
	} else if (tt == typeid(unsigned int)) {
		rtype = typeUInt;
	} else if (tt == typeid(long)) {
		rtype = typeLong;
	} else if (tt == typeid(unsigned long)) {
		rtype = typeULong;
	} else if (tt == typeid(long long)) {
		rtype = typeLongLong;
	} else if (tt == typeid(unsigned long long)) {
		rtype = typeULongLong;
	} else if (tt == typeid(float)) {
		rtype = typeFloat;
	} else if (tt == typeid(double)) {
		rtype = typeDouble;
	} else if (tt == typeid(long double)) {
		rtype = typeLongDouble;
	}
	return (rtype);
} // get_type
////////////////////////////////////////

bool Value::get_value(const boost::any &v, bool &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt == typeid(bool)) {
		val = boost::any_cast<bool>(v);
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (boost::any_cast<char>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (boost::any_cast<signed char>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (boost::any_cast<unsigned char>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (boost::any_cast<short>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (boost::any_cast<unsigned>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (boost::any_cast<int>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (boost::any_cast<unsigned>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (boost::any_cast<long>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (boost::any_cast<unsigned long>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (boost::any_cast<long long>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (boost::any_cast<unsigned long long>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (boost::any_cast<float>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (boost::any_cast<double>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (boost::any_cast<long double>(v) != 0) ? true : false;
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			char c = (char) (*(ss.begin()));
			val = ((c == 't') || (c == 'T') || (c == 'v') || (c == 'V')
					|| (c == 'o') || (c == 'O')) ? true : false;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			wchar_t c = (wchar_t) (*(ss.begin()));
			val = ((c == L't') || (c == L'T') || (c == L'v') || (c == L'V')
					|| (c == L'o') || (c == L'O')) ? true : false;
			bRet = true;
		}
	}
	return (bRet);
} // get_value

bool Value::get_value(const boost::any &v, char &val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (char) 1 : (char) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (char) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (char) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (char) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (char) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (char) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (char) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (char) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (char) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (char) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (char) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (char) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (char) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (char) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			val = (char) (*(ss.begin()));
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::string sx(ss.length(), ' ');
			std::copy(ss.begin(), ss.end(), sx.begin());
			val = (char) (*(sx.begin()));
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (signed char) 1 : (signed char) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (signed char) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (signed char) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (signed char) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (signed char) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (signed char) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (signed char) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (signed char) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (signed char) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (signed char) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (signed char) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (signed char) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (signed char) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (signed char) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			val = (signed char) (*(ss.begin()));
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::string sx(ss.length(), ' ');
			std::copy(ss.begin(), ss.end(), sx.begin());
			val = (signed char) (*(sx.begin()));
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (unsigned char) 1 : (unsigned char) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (unsigned char) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (unsigned char) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (unsigned char) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (unsigned char) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (unsigned char) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (unsigned char) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (unsigned char) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (unsigned char) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (unsigned char) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (unsigned char) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (unsigned char) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (unsigned char) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (unsigned char) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			val = (unsigned char) (*(ss.begin()));
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::string sx(ss.length(), ' ');
			std::copy(ss.begin(), ss.end(), sx.begin());
			val = (unsigned char) (*(sx.begin()));
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (short) 1 : (short) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (short) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (short) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (short) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (short) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (short) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (short) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (short) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (short) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (short) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (short) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (short) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (short) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (short) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (unsigned short) 1 : (unsigned short) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (unsigned short) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (unsigned short) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (unsigned short) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (unsigned short) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (unsigned short) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (unsigned short) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (unsigned short) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (unsigned short) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (unsigned short) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (unsigned short) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (unsigned short) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (unsigned short) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (unsigned short) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (int) 1 : (int) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (int) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (int) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (int) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (int) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (int) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (int) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (int) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (int) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (int) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (int) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (int) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (int) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (int) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (int) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (unsigned int) 1 : (unsigned int) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (unsigned int) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (unsigned int) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (unsigned int) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (unsigned int) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (unsigned int) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (unsigned int) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (unsigned int) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (unsigned int) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (unsigned int) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (unsigned int) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (unsigned int) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (unsigned int) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (unsigned int) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (unsigned int) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (long) 1 : (long) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (long) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (long) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (long) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (long) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (long) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (long) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (long) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (long) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (long) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (long) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (long) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (long) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (long) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (long) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (unsigned long) 1 : (unsigned long) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (unsigned long) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (unsigned long) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (unsigned long) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (unsigned long) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (unsigned long) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (unsigned long) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (unsigned long) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (unsigned long) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (unsigned long) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (unsigned long) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (unsigned long) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (unsigned long) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (unsigned long) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (unsigned long) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (long long) 1 : (long long) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (long long) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (long long) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (long long) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (long long) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (long long) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (long long) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (long long) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (long long) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (long long) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (long long) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (long long) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (long long) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (long long) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (long long) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (unsigned long long) 1 : (unsigned long long) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (unsigned long long) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (unsigned long long) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (unsigned long long) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (unsigned long long) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (unsigned long long) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (unsigned long long) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (unsigned long long) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (unsigned long long) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (unsigned long long) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (unsigned long long) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (unsigned long long) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (unsigned long long) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (unsigned long long) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (unsigned long long) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (float) 1 : (float) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (float) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (float) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (float) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (float) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (float) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (float) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (float) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (float) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (float) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (float) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (float) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (float) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (float) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (float) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (double) 1 : (double) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (double) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (double) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (double) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (double) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (double) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (double) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (double) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (double) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (double) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (double) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (double) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (double) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (double) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (double) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
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
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(v);
		val = (b) ? (long double) 1 : (long double) 0;
		bRet = true;
	} else if (tt == typeid(char)) {
		val = (long double) boost::any_cast<char>(v);
		bRet = true;
	} else if (tt == typeid(signed char)) {
		val = (long double) boost::any_cast<signed char>(v);
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		val = (long double) boost::any_cast<unsigned char>(v);
		bRet = true;
	} else if (tt == typeid(short)) {
		val = (long double) boost::any_cast<short>(v);
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		val = (long double) boost::any_cast<unsigned short>(v);
		bRet = true;
	} else if (tt == typeid(int)) {
		val = (long double) boost::any_cast<int>(v);
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		val = (long double) boost::any_cast<unsigned int>(v);
		bRet = true;
	} else if (tt == typeid(long)) {
		val = (long double) boost::any_cast<long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		val = (long double) boost::any_cast<unsigned long>(v);
		bRet = true;
	} else if (tt == typeid(long long)) {
		val = (long double) boost::any_cast<long long>(v);
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		val = (long double) boost::any_cast<unsigned long long>(v);
		bRet = true;
	} else if (tt == typeid(float)) {
		val = (long double) boost::any_cast<float>(v);
		bRet = true;
	} else if (tt == typeid(double)) {
		val = (long double) boost::any_cast<double>(v);
		bRet = true;
	} else if (tt == typeid(long double)) {
		val = (long double) boost::any_cast<long double>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string ss = boost::any_cast<std::string>(v);
		if (!ss.empty()) {
			std::stringstream in(ss);
			in >> val;
			bRet = true;
		}
	} else if (tt == typeid(std::wstring)) {
		std::wstring ss = boost::any_cast<std::wstring>(v);
		if (!ss.empty()) {
			std::wstringstream in(ss);
			in >> val;
			bRet = true;
		}
	}
	return (bRet);
} // get_value

bool Value::get_value(const boost::any &v, std::string & val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(b);
		val = (b) ? std::string("true") : std::string("false");
		bRet = true;
	} else if (tt == typeid(char)) {
		char xval = boost::any_cast<char>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(signed char)) {
		signed char xval = boost::any_cast<signed char>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		unsigned char xval = boost::any_cast<unsigned char>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(short)) {
		short xval = boost::any_cast<short>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		unsigned short xval = boost::any_cast<unsigned short>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(int)) {
		int xval = boost::any_cast<int>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		unsigned int xval = boost::any_cast<unsigned int>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(long)) {
		long xval = boost::any_cast<long>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		unsigned long xval = boost::any_cast<unsigned long>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(long long)) {
		long long xval = boost::any_cast<long long>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;

	} else if (tt == typeid(unsigned long long)) {
		unsigned long long xval = boost::any_cast<unsigned long long>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(float)) {
		float xval = boost::any_cast<float>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(double)) {
		double xval = boost::any_cast<double>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(long double)) {
		long double xval = boost::any_cast<long double>(v);
		std::stringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(std::string)) {
		val = boost::any_cast<std::string>(v);
		bRet = true;
	} else if (tt == typeid(std::wstring)) {
		std::wstring sx = boost::any_cast<std::wstring>(v);
		val.clear();
		if (!sx.empty()) {
			std::string sy(sx.length(), ' ');
			std::copy(sx.begin(), sx.end(), sy.begin());
			val = sy;
			bRet = true;
		}
	}
	return (bRet);
} // get_value

bool Value::get_value(const boost::any &v, std::wstring & val) {
	bool bRet = false;
	if (v.empty()) {
		return (bRet);
	}
	const std::type_info &tt = v.type();
	if (tt == typeid(bool)) {
		bool b = boost::any_cast<bool>(b);
		val = (b) ? std::wstring(L"true") : std::wstring(L"false");
		bRet = true;
	} else if (tt == typeid(char)) {
		char xval = boost::any_cast<char>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(signed char)) {
		signed char xval = boost::any_cast<signed char>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned char)) {
		unsigned char xval = boost::any_cast<unsigned char>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(short)) {
		short xval = boost::any_cast<short>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned short)) {
		unsigned short xval = boost::any_cast<unsigned short>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(int)) {
		int xval = boost::any_cast<int>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned int)) {
		unsigned int xval = boost::any_cast<unsigned int>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(long)) {
		long xval = boost::any_cast<long>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned long)) {
		unsigned long xval = boost::any_cast<unsigned long>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(long long)) {
		long long xval = boost::any_cast<long long>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(unsigned long long)) {
		unsigned long long xval = boost::any_cast<unsigned long long>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(float)) {
		float xval = boost::any_cast<float>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(double)) {
		double xval = boost::any_cast<double>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(long double)) {
		long double xval = boost::any_cast<long double>(v);
		std::wstringstream os;
		os << xval;
		val = os.str();
		bRet = true;
	} else if (tt == typeid(std::wstring)) {
		val = boost::any_cast<std::wstring>(v);
		bRet = true;
	} else if (tt == typeid(std::string)) {
		std::string xval = boost::any_cast<std::string>(v);
		if (xval.empty()) {
			val = std::wstring();
		} else {
			std::wstring s(xval.length(), L' ');
			std::copy(xval.begin(), xval.end(), s.begin());
			val = s;
		}
		bRet = true;
	}
	return (bRet);
} // get_value
////////////////////////////////
} // namespace statdata
