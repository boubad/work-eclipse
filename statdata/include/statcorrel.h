/*
 * statcorrel.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef STATCORREL_H_
#define STATCORREL_H_
/////////////////////////////////////
#include <string.h>
////////////////////////////////////
namespace statdata {
/////////////////////////////////////////
struct statcorrel_t {
	statcorrel_t() :
			count(0), corr(0.0), prob(1.0), min_corr(0.0), max_corr(0.0) {
	}
	unsigned int count;
	double corr;
	double prob;
	double min_corr;
	double max_corr;
};
///////////////////////////////////////////
class StatCorrel {
public:
	StatCorrel() {
	}
	StatCorrel(const statcorrel_t &other) {
		::memcpy(&(this->m_data), &other, sizeof(this->m_data));
	}
	StatCorrel(const StatCorrel &other) {
		::memcpy(&(this->m_data), &(other.m_data), sizeof(this->m_data));
	}
	StatCorrel & operator=(const StatCorrel &other) {
		if (this != &other) {
			::memcpy(&(this->m_data), &(other.m_data), sizeof(this->m_data));
		}
		return (*this);
	}
	StatCorrel & operator=(const statcorrel_t &other) {
		if (&(this->m_data) != &other) {
			::memcpy(&(this->m_data), &other, sizeof(this->m_data));
		}
		return (*this);
	}
	virtual ~StatCorrel() {
	}
	bool operator==(const StatCorrel &other) const {
		return (this->m_data.corr == other.m_data.corr);
	} // operator==
	bool operator<(const StatCorrel &other) const {
		return (this->m_data.corr < other.m_data.corr);
	} // operator==
	bool operator>(const StatCorrel &other) const {
		return (this->m_data.corr > other.m_data.corr);
	} // operator==
public:
	static const char *type_name(void) {
		return "StatCorrel";
	}
	const statcorrel_t *data(void) const {
		return (&(this->m_data));
	}
public:
	size_t count(void) const {
		return ((size_t) this->m_data.count);
	}
	void count(size_t n) {
		this->m_data.count = (size_t) n;
	}
	double correlation(void) const {
		return (this->m_data.corr);
	}
	void correlation(double f) {
		double ff = f;
		if (ff < -1.0) {
			ff = -1.0;
		}
		if (ff > 1.0) {
			ff = 1.0;
		}
		this->m_data.corr = ff;
	}
	double probability(void) const {
		return (this->m_data.prob);
	}
	void probability(double f) {
		double ff = f;
		if (ff <= 0.0) {
			ff = 0.0;
		}
		if (ff >= 1.0) {
			ff = 1.0;
		}
		this->m_data.prob = ff;
	}
	double min_correlation(void) const {
		return (this->m_data.min_corr);
	}
	void min_correlation(double f) {
		double ff = f;
		if (ff < -1.0) {
			ff = -1.0;
		}
		if (ff > 1.0) {
			ff = 1.0;
		}
		this->m_data.min_corr = ff;
	}
	double max_correlation(void) const {
		return (this->m_data.max_corr);
	}
	void max_correlation(double f) {
		double ff = f;
		if (ff < -1.0) {
			ff = -1.0;
		}
		if (ff > 1.0) {
			ff = 1.0;
		}
		this->m_data.max_corr = ff;
	}
private:
	statcorrel_t m_data;
};
/* class StatCorrel */
////////////////////////////////////////////
}// namespace statdata
//////////////////////////////////////
#endif /* STATCORREL_H_ */
