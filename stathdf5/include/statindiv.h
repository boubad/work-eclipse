/*
 * statindiv.h
 *
 *  Created on: 17 oct. 2012
 *      Author: boubad
 */

#ifndef STATINDIV_H_
#define STATINDIV_H_
/////////////////////////////////
#include <string>
#include <string.h>
/////////////////////////////////////
namespace statdata {
//////////////////////////
struct statindiv_t {
	statindiv_t(): index((unsigned int)-1),active(1),categ(0),supplement(0),
			weight(1.0){
		id[0] = (char)0;
		name[0] = (char)0;
	}
	//
	unsigned int index;
	int active;
	int categ;
	int supplement;
	double weight;
	char id[16];
	char name[32];
};
/////////////////////////////////////////////
class StatIndiv {
public:
	StatIndiv() {
		::memset(&(this->m_data), 0, sizeof(this->m_data));
		this->m_data.active = 1;
		this->m_data.weight = 1.0;
	}
	StatIndiv(const StatIndiv &other) {
		::memcpy(&(this->m_data), &(other.m_data), sizeof(this->m_data));
	}
	StatIndiv(const statindiv_t &other) {
		::memcpy(&(this->m_data), &(other), sizeof(this->m_data));
	}
	StatIndiv & operator=(const StatIndiv &other) {
		if (this != &other) {
			::memcpy(&(this->m_data), &(other.m_data), sizeof(this->m_data));
		}
		return (*this);
	}
	StatIndiv & operator=(const statindiv_t &other) {
		if (&(this->m_data) != &other) {
			::memcpy(&(this->m_data), &other, sizeof(this->m_data));
		}
		return (*this);
	}
	virtual ~StatIndiv() {
	}
	bool operator==(const StatIndiv &other) const {
		if (this->m_data.index == other.m_data.index) {
			return (true);
		}
		std::string s1 = this->id();
		std::string s2 = other.id();
		if (s1 == s2) {
			return (true);
		}
		s1 = this->name();
		s2 = other.name();
		return (s1 == s2);
	}
	bool operator<(const StatIndiv &other) const {
		if (this->m_data.index < other.m_data.index) {
			return (true);
		}
		std::string s1 = this->id();
		std::string s2 = other.id();
		if (s1 < s2) {
			return (true);
		}
		s1 = this->name();
		s2 = other.name();
		return (s1 < s2);
	}
public:
	static const char *type_name(void) {
		return "StatIndiv";
	}
	static const size_t id_max_len(void) {
		return ((size_t) 15);
	}
	static const size_t name_max_len(void) {
		return ((size_t) 31);
	}
public:
	const statindiv_t *data(void) const {
		return &(this->m_data);
	}
	size_t index(void) const {
		return ((size_t) this->m_data.index);
	}
	void index(size_t n) {
		this->m_data.index = n;
	}
	bool is_active(void) const {
		return (this->m_data.active != 0);
	}
	void set_active(bool b = true) {
		this->m_data.active = (b) ? (int) 1 : 0;
	}
	bool is_categ(void) const {
		return (this->m_data.categ != 0);
	}
	void set_categ(bool b = true) {
		this->m_data.categ = (b) ? (int) 1 : 0;
	}
	bool is_sup(void) const {
		return (this->m_data.supplement != 0);
	}
	void set_sup(bool b = true) {
		this->m_data.supplement = (b) ? (int) 1 : 0;
	}
	double weight(void) const {
		return (this->m_data.weight);
	}
	void weight(double f) {
		if (f >= 0.0) {
			this->m_data.weight = f;
		}
	}
	const char *id(void) const {
		return (&(this->m_data.id[0]));
	}
	void id(const std::string &s) {
		std::string ss = (s.length() > 15) ? s.substr(0, 15) : s;
		size_t n = ss.length();
		::strcpy(&this->m_data.id[0], ss.c_str());
		this->m_data.id[n] = (char) 0;
	}
	const char *name(void) const {
		return (&(this->m_data.name[0]));
	}
	void name(const std::string &s) {
		std::string ss = (s.length() > 31) ? s.substr(0, 31) : s;
		size_t n = ss.length();
		::strcpy(&this->m_data.name[0], ss.c_str());
		this->m_data.name[n] = (char) 0;
	}
private:
	statindiv_t m_data;
};
// class StatIndiv
////////////////////////////////////////////
} // namespace statdata

/////////////////////////////////////
#endif /* STATINDIV_H_ */
