#ifndef __CORRELDESC_H__
#define __CORRELDESC_H__
//////////////////////////////////
#include <cassert>
#include <iostream>
#include <cmath>
#include <valarray>
#include <vector>
#include <set>
#include <memory>
#include <map>
#include <string>
///////////////////////////////////
#include "distributions.h"
///////////////////////////////////////
namespace statdata {
//////////////////////////////////
template<typename FTYPE = double, typename STYPE = std::string>
class CorrelDesc {
public:
	CorrelDesc() :
			m_n(0), m_cor(0) {
	}
	CorrelDesc(size_t n, FTYPE v) :
			m_n(n), m_cor(v) {
	}
	template<typename T, typename Z>
	CorrelDesc(size_t n, const T *p1, const Z *p2) :
			m_n(0), m_cor(0) {
		this->compute(n, p1, p2);
	}
	template<typename T, typename Z, class ALLOCT, class ALLOCZ>
	CorrelDesc(const std::vector<T, ALLOCT> &data1,
			const std::vector<Z, ALLOCZ> &data2) :
			m_n(0), m_cor(0) {
		this->compute(data1, data2);
	}
	template<typename T, typename Z>
	CorrelDesc(const std::valarray<T> &data1, const std::valarray<Z> &data2) :
			m_n(0), m_cor(0) {
		this->compute(data1, data2);
	}
	CorrelDesc(const CorrelDesc<FTYPE, STYPE> &other) :
			m_n(other.m_n), m_cor(other.m_cor), m_name1(other.m_name1), m_name2(
					other.m_name2) {
	}
	CorrelDesc<FTYPE, STYPE> & operator=(
			const CorrelDesc<FTYPE, STYPE> &other) {
		if (this != &other) {
			this->m_n = other.m_n;
			this->m_cor = other.m_cor;
			this->m_name1 = other.m_name1;
			this->m_name2 = other.m_name2;
		}
		return (*this);
	}
	virtual ~CorrelDesc() {
	}
public:
	operator FTYPE(void) const {
		return (this->m_cor);
	}
	bool operator<(const CorrelDesc<FTYPE, STYPE> &other) const {
		if (this->m_name1 < other.m_name1) {
			return (true);
		} else if (this->m_name1 > other.m_name1) {
			return (false);
		}
		return (this->m_name2 < other.m_name2);
	} // operator <
	bool operator==(const CorrelDesc<FTYPE, STYPE> &other) const {
		if (std::abs(this->m_cor) != std::abs(other.m_cor)) {
			return (false);
		}
		return ((this->m_name1 == other.m_name2)
				&& (this->m_name2 == other.m_name2));
	} // operator==
public:
	size_t size(void) const {
		return (this->m_n);
	}
	FTYPE correlation(void) const {
		return (this->m_cor);
	}
	const STYPE & first_name(void) const {
		return (this->m_name1);
	}
	void first_name(const STYPE &s) {
		this->m_name1 = s;
	}
	const STYPE &second_name(void) const {
		return (this->m_name2);
	}
	void second_name(const STYPE &s) {
		this->m_name2 = s;
	}
	std::ostream & writeTo(std::ostream &os) const {
		os << "{" << this->m_name1 << "," << this->m_name2 << ",";
		os << this->m_cor << "," << this->probability() << "}";
		return os;
	} // writeTo
	std::wostream & writeTo(std::wostream &os) const {
		os << L"{" << this->m_name1 << L"," << this->m_name2 << L",";
		os << this->m_cor << L"," << this->probability() << L"}";
		return os;
	} // writeTo
public:
	bool compute(size_t n, const double *pData1, const double *pData2) {
		this->m_n = 0;
		this->m_cor = 0;
		if ((n < 4) || (pData1 == nullptr) || (pData2 == nullptr)) {
			return (false);
		}
		double s1 = 0.0, s2 = 0.0;
		for (size_t i = 0; i < n; ++i) {
			s1 += pData1[i];
			s2 += pData2[i];
		} // i
		const double moy1 = s1 / n;
		const double moy2 = s2 / n;
		s1 = 0;
		s2 = 0;
		double s3 = 0.0;
		for (size_t i = 0; i < n; ++i) {
			double x = pData1[i] - moy1;
			double y = pData2[i] - moy2;
			s1 += x * x;
			s2 += y * y;
			s3 += x * y;
		} // i
		s1 /= n;
		s2 /= n;
		if ((s1 <= 0.0) || (s2 <= 0.0)) {
			return (false);
		}
		s3 /= n;
		this->m_n = n;
		double c = round_corr(s3 / (::sqrt(s1) * ::sqrt(s2)));
		if (c < -1.0) {
			c = -1.0;
		}
		if (c > 1.0) {
			c = 1.0;
		}
		this->m_cor = (FTYPE) c;
		return (true);
	} // compute
	template<typename T>
	bool compute(size_t n, const T *pData1, const T *pData2) {
		if ((n < 4) || (pData1 == nullptr) || (pData2 == nullptr)) {
			return (false);
		}
		std::unique_ptr<double> oTemp1(new double[n]);
		double *p1 = oTemp1.get();
		assert(p1 != nullptr);
		std::unique_ptr<double> oTemp2(new double[n]);
		double *p2 = oTemp2.get();
		assert(p2 != nullptr);
		for (size_t i = 0; i < n; ++i) {
			p1[i] = (double) pData1[i];
			p2[i] = (double) pData2[i];
		} // i
		bool bRet = this->compute(n, p1, p2);
		return (bRet);
	} // compute
	template<typename T, typename Z, class ALLOCT1, class ALLOCT2>
	bool compute(const std::vector<T, ALLOCT1> &data1,
			const std::vector<Z, ALLOCT2> &data2) {
		const size_t n = data1.size();
		if (n < 4) {
			return (false);
		}
		if (data2.size() < n) {
			return (false);
		}
		std::unique_ptr<double> oTemp1(new double[n]);
		double *p1 = oTemp1.get();
		assert(p1 != nullptr);
		std::unique_ptr<double> oTemp2(new double[n]);
		double *p2 = oTemp2.get();
		assert(p2 != nullptr);
		for (size_t i = 0; i < n; ++i) {
			p1[i] = (double) data1[i];
			p2[i] = (double) data2[i];
		} // i
		bool bRet = this->compute(n, p1, p2);
		return (bRet);
	} // compute
	template<typename T, typename Z>
	bool compute(const std::valarray<T> &data1, const std::valarray<Z> &data2) {
		const size_t n = data1.size();
		if (n < 4) {
			return (false);
		}
		if (data2.size() < n) {
			return (false);
		}
		std::unique_ptr<double> oTemp1(new double[n]);
		double *p1 = oTemp1.get();
		assert(p1 != nullptr);
		std::unique_ptr<double> oTemp2(new double[n]);
		double *p2 = oTemp2.get();
		assert(p2 != nullptr);
		for (size_t i = 0; i < n; ++i) {
			p1[i] = (double) data1[i];
			p2[i] = (double) data2[i];
		} // i
		bool bRet = this->compute(n, p1, p2);
		return (bRet);
	} // compute
public:
	FTYPE criteria(void) const {
		const size_t n = this->m_n;
		if (n < 2) {
			return ((FTYPE) 0.0);
		}
		const double r = (double) this->m_cor;
		double r2 = 1.0 - r * r;
		if (r2 <= 0.0) {
			return 0.0;
		}
		double xx = (r < 0.0) ? -r : r;
		double result = (xx * ::sqrt((double) (n - 2))) / ::sqrt(r2);
		return (FTYPE) result;
	} // criteria
	FTYPE probability(void) const {
		const size_t n = this->m_n;
		if (n < 3) {
			return ((FTYPE) 0.0);
		}
		double r = (double) this->m_cor;
		double ra = 1.0 - (r * r);
		if (ra <= 0.0) {
			return ((FTYPE) 0.0);
		}
		double t = this->criteria();
		if (t == 0.0) {
			return ((FTYPE) 0.0);
		}
		double result = statdata::Distributions::prob_student_t(
				(double) (n - 2), t);
		if (result < 0) {
			result = 0.0;
		}
		if (result > 1.0) {
			result = 1.0;
		}
		double z = 1.0 - result;
		if (z < 0) {
			z = 0.0;
		}
		if (z > 1.0) {
			z = 1.0;
		}
		return ((FTYPE) z);
	} // probability
	bool reject_correlation(FTYPE prob = (FTYPE) (0.05)) const {
		assert(prob >= (FTYPE)0.0);
		assert(prob <= (FTYPE)1.0);
		const size_t n = this->m_n;
		if (n < 3) {
			return (false);
		}
		double crit = (double) this->criteria();
		if (crit == 0.0) {
			return (false);
		}
		double tt = statdata::Distributions::quantile_student_t(
				(double) (n - 2), prob / 2.0);
		return (crit < tt);
	} // reject_correlation
	bool correl_confidence_interval(FTYPE &cMin, FTYPE &cMax, FTYPE prob =
			(FTYPE) (0.05)) const {
		assert(prob >= (FTYPE)0.0);
		assert(prob <= (FTYPE)1.0);
		const size_t n = this->m_n;
		if (n < 4) {
			return (false);
		}
		double r = (double) this->m_cor;
		double ra = (r < 0.0) ? -r : r;
		if (ra < VAL_EPSILON) {
			r = 0;
		}
		double r1 = 1.0 - r;
		if (r1 == 0.0) {
			cMin = (FTYPE) r;
			cMax = (FTYPE) r;
			return (true);
		}
		if ((1.0 + r) <= 0.0) {
			cMin = (FTYPE) r;
			cMax = (FTYPE) r;
			return (true);
		} //
		double r2 = (1.0 + r) / r1;
		double z = ::log(r2) / 2.0;
		double zt = statdata::Distributions::quantile_normale(prob / 2.0)
				/ ::sqrt((double) (n - 3));
		double x1 = z - (r / ::sqrt((double) (n - 1)));
		double z1 = x1 - zt;
		double z2 = x1 + zt;
		double c1 = ::tanh(z1);
		double c2 = ::tanh(z2);
		cMin = (FTYPE) round_corr((c1 < c2) ? c1 : c2);
		cMax = (FTYPE) round_corr((c1 > c2) ? c1 : c2);
		return (true);
	} //correl_confidence_interval
public:
	template<class ALLOCCORREL>
	static const CorrelDesc<FTYPE, STYPE> *ana_find(
			const std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL> &oVec,
			const STYPE &name1, const STYPE &name2) {
		const CorrelDesc<FTYPE, STYPE> *pRet = nullptr;
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const CorrelDesc<FTYPE, STYPE> &c = *it;
			if ((c.first_name() == name1) && (c.second_name() == name2)) {
				return (&c);
			} else if ((c.first_name() == name2)
					&& (c.second_name() == name1)) {
				return (&c);
			}
		} // it
		return pRet;
	} // ana_find
	template<class ALLOCCORREL1, class ALLOCCORREL2>
	static void ana_filter(
			const std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL1> &oVec,
			std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL2> &oRes,
			FTYPE prob = (FTYPE) (0.05)) {
		oRes.clear();
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const CorrelDesc<FTYPE, STYPE> &c = *it;
			if (c.first_name() != c.second_name()) {
				FTYPE x = c.probability();
				if (x < prob) {
					CorrelDesc<FTYPE, STYPE> cc(c);
					oRes.push_back(cc);
				} // x
			}
		} // it
		if (oRes.size() > 1) {
			std::sort(oRes.begin(), oRes.end());
		}
	} // filter
	template<class ALLOCCORREL, class ALLOCSTRING>
	static void ana_get_names(
			const std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL> &oVec,
			std::set<STYPE, std::less<STYPE>, ALLOCSTRING> &oSet) {
		oSet.clear();
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const CorrelDesc<FTYPE, STYPE> &c = *it;
			const STYPE &s1 = c.first_name();
			const STYPE &s2 = c.second_name();
			if (oSet.find(s1) == oSet.end()) {
				oSet.insert(s1);
			}
			if (oSet.find(s2) == oSet.end()) {
				oSet.insert(s2);
			}
		} // it
	} //ana_get_names
	template<class ALLOCCORREL1, class ALLOCCORREL2>
	static void ana_get_correls(const STYPE &sname,
			const std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL1> &oVec,
			std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL2> &oFind) {
		oFind.clear();
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const CorrelDesc<FTYPE, STYPE> &c = *it;
			const STYPE &s1 = c.first_name();
			const STYPE &s2 = c.second_name();
			if (s1 != s2) {
				if ((s1 == sname) || (s2 == sname)) {
					oFind.push_back(c);
				}
			} // dif
		} // it
	} //ana_get_names
	template<class ALLOCCORREL, class ALLOCSTRING, class ALLOCVEC>
	static void analyse(
			const std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL> &oVec,
			std::vector<std::set<STYPE, std::less<STYPE>, ALLOCSTRING>, ALLOCVEC> &oRes,
			FTYPE prob = (FTYPE) (0.05)) {
		typedef std::set<STYPE, std::less<STYPE>, ALLOCSTRING> MySetType;
		oRes.clear();
		//
		MySetType oNames;
		MySetType oDones;
		CorrelDesc::ana_get_names(oVec, oNames);
		while (!oNames.empty()) {
			auto it = oNames.begin();
			STYPE sname = *it;
			oNames.erase(it);
			std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL> oFind;
			CorrelDesc<FTYPE, STYPE>::ana_get_correls(sname, oVec, oFind);
			for (auto jt = oFind.begin(); jt != oFind.end(); ++jt) {
				const CorrelDesc<FTYPE, STYPE> &c = *jt;
				STYPE curname;
				if (c.first_name() == sname) {
					curname = c.second_name();
				} else {
					curname = c.first_name();
				}
				if (oDones.find(curname) == oDones.end()) {
					MySetType *pFirst = nullptr;
					MySetType *pSecond = nullptr;
					for (auto kt = oRes.begin(); kt != oRes.end(); ++kt) {
						if ((pFirst != nullptr) && (pSecond != nullptr)) {
							break;
						}
						MySetType & oSet = *kt;
						if (oSet.find(sname) != oSet.end()) {
							pFirst = &oSet;
						}
						if (oSet.find(curname) != oSet.end()) {
							pSecond = &oSet;
						}
					} // kt
					if ((pFirst == nullptr) || (pSecond == nullptr)) {
						bool bReject = c.reject_correlation(prob);
						if ((pFirst == nullptr) && (pSecond == nullptr)) {
							if (bReject) {
								MySetType oSet1;
								oSet1.insert(sname);
								oRes.push_back(oSet1);
								MySetType oSet2;
								oSet2.insert(curname);
								oRes.push_back(oSet2);
								oDones.insert(sname);
								oDones.insert(curname);
								auto zt = oNames.find(curname);
								if (zt != oNames.end()) {
									oNames.erase(zt);
								}
							} else {
								MySetType oSet1;
								oSet1.insert(sname);
								oSet1.insert(curname);
								oRes.push_back(oSet1);
								oDones.insert(sname);
								oDones.insert(curname);
								auto zt = oNames.find(curname);
								if (zt != oNames.end()) {
									oNames.erase(zt);
								}
							}
						} else if ((pFirst != nullptr)
								&& (pSecond == nullptr)) {
							if (bReject) {
								MySetType oSet2;
								oSet2.insert(curname);
								oRes.push_back(oSet2);
								oDones.insert(curname);
								auto zt = oNames.find(curname);
								if (zt != oNames.end()) {
									oNames.erase(zt);
								}
							} else {
								pFirst->insert(curname);
								oDones.insert(curname);
								auto zt = oNames.find(curname);
								if (zt != oNames.end()) {
									oNames.erase(zt);
								}
							}
						} else if ((pFirst == nullptr)
								&& (pSecond != nullptr)) {
							if (bReject) {
								MySetType oSet2;
								oSet2.insert(sname);
								oRes.push_back(oSet2);
								oDones.insert(sname);
								auto zt = oNames.find(sname);
								if (zt != oNames.end()) {
									oNames.erase(zt);
								}
							} else {
								pSecond->insert(sname);
								oDones.insert(sname);
								auto zt = oNames.find(sname);
								if (zt != oNames.end()) {
									oNames.erase(zt);
								}
							}
						}
					} // process ok
				} // process
			} // jt
		} // not empty
	} // analyse
private:
	static double round_corr(double x) {
		double vx = (x < 0.0) ? -x : x;
		if (vx < VAL_EPSILON) {
			return (0.0);
		}
		int xx = (int) (10000.0 * x + 0.5);
		double xr = (double) xx / 10000.0;
		return ((x < 0) ? (-xr) : xr);
	}
private:
	size_t m_n;
	FTYPE m_cor;
	STYPE m_name1;
	STYPE m_name2;
	//
	static double VAL_EPSILON;
};
template<typename FTYPE, typename STYPE>
double CorrelDesc<FTYPE, STYPE>::VAL_EPSILON = 0.0001;
// class CorrelDesc
/////////////////////////////////////////
template<typename FTYPE = double, typename STYPE = std::string>
class PartialCorrelDesc: public CorrelDesc<FTYPE, STYPE> {
public:
	PartialCorrelDesc() {
	}
	PartialCorrelDesc(size_t n, double v) :
			CorrelDesc<FTYPE, STYPE>(n, v) {
	}
	PartialCorrelDesc(const PartialCorrelDesc<FTYPE, STYPE> &other) :
			CorrelDesc<FTYPE, STYPE>(other), m_refname(other.m_refname) {
	}
	PartialCorrelDesc<FTYPE, STYPE> & operator=(
			const PartialCorrelDesc<FTYPE, STYPE> &other) {
		if (this != &other) {
			CorrelDesc<FTYPE, STYPE>::operator=(other);
			this->m_refname = other.m_refname;
		}
		return (*this);
	}
	virtual ~PartialCorrelDesc() {
	}
public:
	const STYPE & ref_name(void) const {
		return (this->m_refname);
	}
	void ref_name(STYPE &s) {
		this->m_refname = s;
	}
private:
	STYPE m_refname;
};
// class PartialCorrelDesc
///////////////////////////////////////////
}// namespace statdata
///////////////////////////////
template<typename FTYPE, typename STYPE>
std::ostream & operator<<(std::ostream &os,
		const statdata::CorrelDesc<FTYPE, STYPE> &d) {
	return d.writeTo(os);
}
template<typename FTYPE, typename STYPE>
std::wostream & operator<<(std::wostream &os,
		const statdata::CorrelDesc<FTYPE, STYPE> &d) {
	return d.writeTo(os);
}
////////////////////////////////////
namespace statdata {
///////////////////////////////////
template<typename FTYPE = double, typename STYPE = std::string,
		class ALLOCCORREL = my_sub_allocator<CorrelDesc<FTYPE, STYPE>> >
class CorrelDescs {
public:
	typedef std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL> CorrelVectorType;
public:
	CorrelDescs() {
	}
	template<class ALLOC>
	CorrelDescs(const std::vector<CorrelDesc<FTYPE, STYPE>, ALLOC> &descs) {
		for (auto it = descs.begin(); it != descs.end(); ++it) {
			this->m_descs.push_back(*it);
		}	// it
	}
	template<typename T, class ALLOCSTRING, class ALLOCT>
	CorrelDescs(size_t nr, size_t nc,
			const std::vector<STYPE, ALLOCSTRING> &names,
			const std::vector<T, ALLOCT> &data) {
		this->add(nr, nc, names, data);
	}
	template<typename T, class ALLOCSTRING>
	CorrelDescs(size_t nr, size_t nc,
			const std::vector<STYPE, ALLOCSTRING> &names,
			const std::valarray<T> &data) {
		this->add(nr, nc, names, data);
	}
	CorrelDescs(const CorrelDescs<FTYPE, STYPE, ALLOCCORREL> &other) :
			m_descs(other.m_descs) {
	}
	CorrelDescs<FTYPE, STYPE, ALLOCCORREL> & operator=(
			const CorrelDescs<FTYPE, STYPE, ALLOCCORREL> &other) {
		if (this != &other) {
			this->m_descs = other.m_descs;
		}
		return (*this);
	}
	virtual ~CorrelDescs() {
	}
public:
	size_t size(void) const {
		return (this->m_descs.size());
	}
	const CorrelDesc<FTYPE, STYPE> & correl_at(size_t i) const {
		return (this->m_descs[i]);
	}
	CorrelDesc<FTYPE, STYPE> & correl_at(size_t i) {
		return (this->m_descs[i]);
	}
	const typename CorrelVectorType::const_iterator begin(void) const {
		return (this->m_descs.begin());
	}
	const typename CorrelVectorType::const_iterator end(void) const {
		return (this->m_descs.end());
	}
	typename CorrelVectorType::iterator begin(void) {
		return (this->m_descs.begin());
	}
	typename CorrelVectorType::iterator end(void) {
		return (this->m_descs.end());
	}
	void sort(void) {
		std::sort(this->m_descs.begin(), this->m_descs.end());
	}
	void clear(void) {
		this->m_descs.clear();
	}
	const CorrelVectorType &descs(void) const {
		return (this->m_descs);
	}
	CorrelVectorType &descs(void) {
		return (this->m_descs);
	}
	const CorrelDesc<FTYPE, STYPE> *find(const STYPE &name1,
			const STYPE &name2) const {
		return (CorrelDesc<FTYPE, STYPE>::ana_find(this->m_descs, name1, name2));
	}	// find
	template<typename W>
	bool partial_correlation(const STYPE &sname1, const STYPE &sname2,
			const STYPE &srefname, W &vcorr) const {
		bool bRet = false;
		const CorrelDesc<FTYPE, STYPE> *c0 = this->find(sname1, sname2);
		const CorrelDesc<FTYPE, STYPE> *c1 = this->find(sname1, srefname);
		const CorrelDesc<FTYPE, STYPE> *c2 = this->find(sname2, srefname);
		if ((c0 != nullptr) && (c1 != nullptr) && (c2 != nullptr)) {
			double vc1 = (double) c1->correlation();
			double vc2 = (double) c2->correlation();
			double d1 = 1.0 - (vc1 * vc1);
			double d2 = 1.0 - (vc2 * vc2);
			if ((d1 > 0.0) && (d2 > 0.0)) {
				double vc0 = (double) c0->correlation();
				double r = (vc0 - vc1 * vc2) / (::sqrt(d1) * ::sqrt(d2));
				if (r < -1.0) {
					r = -1.0;
				}
				if (r > 1.0) {
					r = 1.0;
				}
				double rx = (r < 0.0) ? -r : r;
				int irx = (int) (10000.0 * rx + 0.5);
				double x = (double) irx / 10000.0;
				vcorr = (W) ((r < 0.0) ? -x : x);
				bRet = true;
			}	// ok && ok
		}	// ok
		return (bRet);
	}	//partial_correlatio
	template<class ALLOCCORREL2>
	void filter(std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL2> &oRes,
			FTYPE prob = (FTYPE) (0.05)) const {
		CorrelDesc<FTYPE, STYPE>::ana_filter(this->m_descs, oRes, prob);
	}	// filter
	template<class ALLOCSTRING>
	void get_names(std::set<STYPE, std::less<STYPE>, ALLOCSTRING> &oSet) const {
		CorrelDesc<FTYPE, STYPE>::ana_get_names(this->m_descs, oSet);
	}	// get_names
	template<class ALLOCCORREL2>
	void get_correls(const STYPE &sname,
			std::vector<CorrelDesc<FTYPE, STYPE>, ALLOCCORREL2> &oFind) const {
		CorrelDesc<FTYPE, STYPE>::ana_get_correls(sname, this->m_descs, oFind);
	}	// get_correls
	template<class ALLOCSTRING, class ALLOCVEC>
	void analyse(
			std::vector<std::set<STYPE, std::less<STYPE>, ALLOCSTRING>, ALLOCVEC> &oRes,
			FTYPE prob = (FTYPE) (0.05)) const {
		CorrelDesc<FTYPE, STYPE>::analyse(this->m_descs, oRes, prob);
	}	// analyse
	void add(const CorrelDesc<FTYPE, STYPE> &c) {
		CorrelDesc<FTYPE, STYPE> cc(c);
		this->m_descs.push_back(cc);
	}	// add
	template<typename T, typename Z>
	bool add(const STYPE & name1, const STYPE &name2, size_t n, const T *pData1,
			const Z *pData2) {
		bool bRet = false;
		if (this->find(name1, name2) == nullptr) {
			CorrelDesc<FTYPE, STYPE> c;
			if (c.compute(n, pData1, pData2)) {
				c.first_name(name1);
				c.second_name(name2);
				this->m_descs.push_back(c);
				bRet = true;
			}	// ok
		}
		return (bRet);
	}	// add
	template<typename T, typename Z, class ALLOCT, class ALLOCZ>
	bool add(const STYPE &name1, const STYPE &name2,
			const std::vector<T, ALLOCT> &data1,
			const std::vector<Z, ALLOCZ> &data2) {
		bool bRet = false;
		if (this->find(name1, name2) == nullptr) {
			CorrelDesc<FTYPE, STYPE> c;
			if (c.compute(data1, data2)) {
				c.first_name(name1);
				c.second_name(name2);
				this->m_descs.push_back(c);
				bRet = true;
			}	// ok
		}
		return (bRet);
	}	// add
	template<typename T, typename Z>
	bool add(const STYPE &name1, const STYPE &name2,
			const std::valarray<T> &data1, const std::valarray<Z> &data2) {
		bool bRet = false;
		if (this->find(name1, name2) == nullptr) {
			CorrelDesc<FTYPE, STYPE> c;
			if (c.compute(data1, data2)) {
				c.first_name(name1);
				c.second_name(name2);
				this->m_descs.push_back(c);
				bRet = true;
			}	// ok
		}
		return (bRet);
	}	// add
	template<typename T, class ALLOCSTRING, class ALLOCT>
	bool add(size_t nr, size_t nc, const std::vector<STYPE, ALLOCSTRING> &names,
			const std::vector<T, ALLOCT> &data) {
		bool bRet = false;
		if ((nr > 3) && (nc > 0) && (names.size() >= nc)
				&& (data.size() >= (size_t) (nr * nc))) {
			std::unique_ptr<double> oTemp1(new double[nr]);
			double *p1 = oTemp1.get();
			assert(p1 != nullptr);
			std::unique_ptr<double> oTemp2(new double[nr]);
			double *p2 = oTemp2.get();
			assert(p2 != nullptr);
			for (size_t icol1 = 0; icol1 < nc; ++icol1) {
				const std::string &name1 = names[icol1];
				for (size_t j = 0; j < nr; ++j) {
					p1[j] = (double) data[j * nc + icol1];
				}	// j
				for (size_t icol2 = 0; icol2 < icol1; ++icol2) {
					for (size_t j = 0; j < nr; ++j) {
						p2[j] = (double) data[j * nc + icol2];
					}	// j
					const std::string &name2 = names[icol2];
					if (!this->add(name1, name2, nr, p1, p2)) {
						return (false);
					}
				}	// icol2
			}	// icol1
		}	// compute
		return (bRet);
	}	// add
	template<typename T, class ALLOCSTRING>
	bool add(size_t nr, size_t nc, const std::vector<STYPE, ALLOCSTRING> &names,
			const std::valarray<T> &data) {
		bool bRet = false;
		if ((nr > 3) && (nc > 0) && (names.size() >= nc)
				&& (data.size() >= (size_t) (nr * nc))) {
			std::unique_ptr<double> oTemp1(new double[nr]);
			double *p1 = oTemp1.get();
			assert(p1 != nullptr);
			std::unique_ptr<double> oTemp2(new double[nr]);
			double *p2 = oTemp2.get();
			assert(p2 != nullptr);
			for (size_t icol1 = 0; icol1 < nc; ++icol1) {
				const std::string &name1 = names[icol1];
				for (size_t j = 0; j < nr; ++j) {
					p1[j] = (double) data[j * nc + icol1];
				}	// j
				for (size_t icol2 = 0; icol2 < icol1; ++icol2) {
					for (size_t j = 0; j < nr; ++j) {
						p2[j] = (double) data[j * nc + icol2];
					}	// j
					const std::string &name2 = names[icol2];
					if (!this->add(name1, name2, nr, p1, p2)) {
						return (false);
					}
				}	// icol2
			}	// icol1
		}	// compute
		return (bRet);
	}	// add
	std::ostream & writeTo(std::ostream &os) const {
		for (auto it = this->m_descs.begin(); it != this->m_descs.end(); ++it) {
			os << *it << std::endl;
		}	// it
		return os;
	}	// wtiteTo
	std::wostream & writeTo(std::wostream &os) const {
		for (auto it = this->m_descs.begin(); it != this->m_descs.end(); ++it) {
			os << *it << std::endl;
		}	// it
		return os;
	}	// wtiteTo
	template<class ALLOCSTRING>
	void get_rejected_vector(std::vector<STYPE, ALLOCSTRING> &oVec, FTYPE prob =
			(FTYPE) (0.05)) const {
		//
		typedef std::set<STYPE> MySet;
		//
		oVec.clear();
		MySet oNames;
		this->get_names(oNames);
		for (auto it = oNames.begin(); it != oNames.end(); ++it) {
			STYPE sname = *it;
			CorrelVectorType oFind;
			this->get_correls(sname, oFind);
			bool bOk = true;
			for (auto jt = oFind.begin(); jt != oFind.end(); ++jt) {
				const CorrelDesc<FTYPE, STYPE> &c = *jt;
				if (c.first_name() != c.second_name()) {
					if (!c.reject_correlation(prob)) {
						bOk = false;
						break;
					}
				}
			}	// jt
			if (bOk) {
				oVec.push_back(sname);
			}
		}	// it
	}	// get_rejected_vector
	template<class ALLOCSTRING, class ALLOCSETPAIR>
	void get_rejected_map(
			std::map<STYPE, std::set<STYPE, std::less<STYPE>, ALLOCSTRING>,
					ALLOCSETPAIR> &oMap, FTYPE prob = (FTYPE) (0.05)) const {
		typedef std::set<STYPE, std::less<STYPE>, ALLOCSTRING> MySet;
		//
		oMap.clear();
		MySet oNames;
		this->get_names(oNames);
		for (auto it = oNames.begin(); it != oNames.end(); ++it) {
			STYPE sname = *it;
			CorrelVectorType oFind;
			this->get_correls(sname, oFind);
			MySet oCur;
			for (auto jt = oFind.begin(); jt != oFind.end(); ++jt) {
				const CorrelDesc<FTYPE, STYPE> &c = *jt;
				if (c.reject_correlation(prob)) {
					STYPE scur;
					if (c.first_name() == sname) {
						scur = c.second_name();
					} else {
						scur = c.first_name();
					}
					oCur.insert(scur);
				}	// ok
			}	// jt
			oMap[sname] = oCur;
		}	// it
	}	// get_rejected_map
	template<class ALLOCSTRING, class ALLOCSETPAIR>
	void get_correlated_map(
			std::map<STYPE, std::set<STYPE, std::less<STYPE>, ALLOCSTRING>,
					ALLOCSETPAIR> &oMap, FTYPE prob = (FTYPE) (0.05)) {
		typedef std::set<STYPE, std::less<STYPE>, ALLOCSTRING> MySet;
		//
		oMap.clear();
		MySet oNames;
		this->get_names(oNames);
		for (auto it = oNames.begin(); it != oNames.end(); ++it) {
			STYPE sname = *it;
			CorrelVectorType oFind;
			this->get_correls(sname, oFind);
			MySet oCur;
			for (auto jt = oFind.begin(); jt != oFind.end(); ++jt) {
				const CorrelDesc<FTYPE, STYPE> &c = *jt;
				if (c.probability() < prob) {
					STYPE scur;
					if (c.first_name() == sname) {
						scur = c.second_name();
					} else {
						scur = c.first_name();
					}
					oCur.insert(scur);
				}	// ok
			}	// jt
			oMap[sname] = oCur;
		}	// it
	}	//  get_correlated_map
private:
	CorrelVectorType m_descs;
};
// class CorrelDescs<ALLOCCORREL>
}// namespace statdata
//////////////////////////////////////////
template<typename FTYPE, typename STYPE, class ALLOC>
std::ostream & operator<<(std::ostream &os,
		const statdata::CorrelDescs<FTYPE, STYPE, ALLOC> &d) {
	return d.writeTo(os);
}
template<typename FTYPE, typename STYPE, class ALLOC>
std::wostream & operator<<(std::wostream &os,
		const statdata::CorrelDescs<FTYPE, STYPE, ALLOC> &d) {
	return d.writeTo(os);
}
////////////////////////////////////
#endif // __CORRELDESC_H__
