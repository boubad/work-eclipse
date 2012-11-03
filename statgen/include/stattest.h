/*
 * stattest.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef STATTEST_H_
#define STATTEST_H_
/////////////////////////////
#include <cassert>
#include <cmath>
#include <string>
#include <valarray>
#include <memory>
////////////////////////////////////
namespace statdata {
////////////////////////////////////////
class StatTest {
public:
	typedef enum {
		h_unknown, h_accept, h_reject
	} StatTestResultType;
protected:
	StatTest(size_t nx = 0) :
			m_n(nx) {
	}
	StatTest(const StatTest &other) : m_n(other.m_n) {
	}
	StatTest & operator=(const StatTest &other) {
		if (this != &other) {
			this->m_n = other.m_n;
		}
		return (*this);
	}
public:
	virtual ~StatTest() {
	}
public:
	size_t size(void) const {
		return (this->m_n);
	}
	void size(size_t n) {
		assert(n > 0);
		this->m_n = n;
	}
public:
	virtual bool compute_double(size_t n, const double *pData) = 0;
	virtual bool test(void) const = 0;
	virtual StatTestResultType result(void) const {
		return (this->test() ? h_accept : h_reject);
	} // result
public:
	virtual void clear(void) {
		this->m_n = 0;
	}
public:
	void result_string(std::string &s) const {
		StatTestResultType r = this->result();
		switch (r) {
		case h_accept:
			s = "ACCEPT";
			break;
		case h_reject:
			s = "REJECT";
			break;
		default:
			s = "UNKNOWN";
			break;
		} // r
	} // result_string
	void result_string(std::wstring &s) const {
		StatTestResultType r = this->result();
		switch (r) {
		case h_accept:
			s = L"ACCEPT";
			break;
		case h_reject:
			s = L"REJECT";
			break;
		default:
			s = L"UNKNOWN";
			break;
		} // r
	} // result_string
	template<typename T>
	bool compute(size_t n, const T *pData) {
		assert(n > 1);
		std::unique_ptr<double>  oTemp(new double[n]);
		double *p = oTemp.get();
		assert(p != nullptr);
		for (size_t i = 0; i < n; ++i) {
			p[i] = (double) pData[i];
		} // i
		return (this->compute_double(n, p));
	} // compute
	template<typename T>
	bool compute(std::valarray<T> &odata) {
		const size_t n = odata.size();
		assert(n > 1);
		std::unique_ptr<double>  oTemp(new double[n]);
		double *p = oTemp.get();
		assert(p != nullptr);
		for (size_t i = 0; i < n; ++i) {
			p[i] = (double) odata[i];
		} // i
		return (this->compute_double(n, p));
	} // compute
	template<typename T, class ALLOCT>
	bool compute(const std::vector<T, ALLOCT> &odata) {
		const size_t n = odata.size();
		assert(n > 1);
		std::unique_ptr<double>  oTemp(new double[n]);
		double *p = oTemp.get();
		assert(p != nullptr);
		for (size_t i = 0; i < n; ++i) {
			p[i] = (double) odata[i];
		} // i
		return (this->compute_double(n, p));
	} // test_mediane
protected:
	size_t m_n;
};
// class StatTest
///////////////////////////////////////////////////
class NormalityBySkewnessKurtosisTest: public StatTest {
public:
	NormalityBySkewnessKurtosisTest(size_t nx = 0) :
			StatTest(nx), m_skew(0), m_kurt(0), m_skewdev(0), m_kurtdev(0) {
	}
	template<typename T>
	NormalityBySkewnessKurtosisTest(size_t n, const T *pData) :
			m_skew(0), m_kurt(0), m_skewdev(0), m_kurtdev(0) {
		this->compute(n, pData);
	}
	template<typename T>
	NormalityBySkewnessKurtosisTest(const std::valarray<T> &odata) :
			m_skew(0), m_kurt(0), m_skewdev(0), m_kurtdev(0) {
		this->compute(odata);
	}
	template<typename T, class ALLOCT>
	NormalityBySkewnessKurtosisTest(const std::vector<T, ALLOCT> &odata) :
			m_skew(0), m_kurt(0), m_skewdev(0), m_kurtdev(0) {
		this->compute(odata);
	}
	NormalityBySkewnessKurtosisTest(
			const NormalityBySkewnessKurtosisTest &other) :
			StatTest(other), m_skew(other.m_skew), m_kurt(other.m_kurt), m_skewdev(
					other.m_skewdev), m_kurtdev(other.m_kurtdev) {
	}
	NormalityBySkewnessKurtosisTest & operator=(
			const NormalityBySkewnessKurtosisTest &other) {
		if (this != &other) {
			StatTest::operator=(other);
			this->m_skew = other.m_skew;
			this->m_kurt = other.m_kurt;
			this->m_skewdev = other.m_skewdev;
			this->m_kurtdev = other.m_kurtdev;
		}
		return (*this);
	}
	virtual ~NormalityBySkewnessKurtosisTest() {
	}
public:
	double skewness(void) const {
		return (this->m_skew);
	}
	double kurtosis(void) const {
		return (this->m_kurt);
	}
	double skewness_deviation(void) const {
		return (this->m_skewdev);
	}
	double kurtosis_deviation(void) const {
		return (this->m_kurtdev);
	}
public:
	virtual void clear(void) {
		StatTest::clear();
		this->m_skew = 0;
		this->m_kurt = 0;
		this->m_skewdev = 0;
		this->m_kurtdev = 0;
	}
	virtual bool compute_double(size_t n, const double *pData) {
		assert(n > 1);
		assert(pData != nullptr);
		this->clear();
		if (n < 4) {
			return (false);
		}
		double s = 0.0;
		double dn = (double) n;
		for (size_t i = 0; i < n; ++i) {
			s += pData[i];
		}	// i
		const double xbarre = s / dn;
		double s2 = 0.0;
		double s3 = 0.0;
		double s4 = 0.0;
		for (size_t i = 0; i < n; ++i) {
			double dx = pData[i] - xbarre;
			double dx2 = dx * dx;
			s2 += dx2;
			double dx3 = dx2 * dx;
			s3 += dx3;
			s4 += dx3 * dx;
		}	// i
		s2 /= dn;
		s3 /= dn;
		s4 /= dn;
		double ec = std::sqrt(s2);
		if (s2 <= 0.0) {
			return (false);
		}
		this->m_n = n;
		this->m_skew = s3 / (s2 * ec);
		this->m_kurt = (s4 / (s2 * s2)) - 3.0;
		double v1 = (6.0 * (dn - 2.0)) / ((dn + 1.0) * (dn + 3.0));
		this->m_skewdev = std::sqrt(v1);
		v1 = 24.0 * dn * (dn - 2.0) * (dn - 3.0);
		double v2 = (dn + 1.0) * (dn + 1.0) * (dn + 3.0) * (dn + 5.0);
		this->m_kurtdev = std::sqrt(v1 / v2);
		return (true);
	}	// compute_double
	virtual StatTestResultType result(void) const {
		StatTestResultType r = h_unknown;
		double v1 = std::abs(this->m_skew);
		double t1 = 1.5 * this->m_skewdev;
		double v2 = std::abs(this->m_kurt + (6.0 / (this->m_n + 1)));
		double t2 = 1.5 * this->m_kurtdev;
		if ((v1 < t1) && (v2 < t2)) {
			r = h_accept;
		} else {
			t1 = 2.0 * this->m_skewdev;
			t2 = 2.0 * this->m_kurtdev;
			if ((v1 >= t1) || (v2 >= t2)) {
				r = h_reject;
			}
		}
		return r;
	}	// result
	virtual bool test(void) const {
		double v1 = std::abs(this->m_skew);
		double t1 = 1.5 * this->m_skewdev;
		double v2 = std::abs(this->m_kurt + (6.0 / (this->m_n + 1)));
		double t2 = 1.5 * this->m_kurtdev;
		return ((v1 < t1) && (v2 < t2));
	}	// test
private:
	double m_skew;
	double m_kurt;
	double m_skewdev;
	double m_kurtdev;
};
// class NormalityBySkewnessKurtosisTest
/////////////////////////////////////////////////
class StochasticIndependanceSquaredDiff: public StatTest {
public:
	StochasticIndependanceSquaredDiff(size_t nx = 0) :
			m_crit(0), m_critmin(0) {
	}
	template<typename T>
	StochasticIndependanceSquaredDiff(size_t n, const T *pData) :
			m_crit(0), m_critmin(0) {
		this->compute(n, pData);
	}
	template<typename T>
	StochasticIndependanceSquaredDiff(const std::valarray<T> &odata) :
			m_crit(0), m_critmin(0) {
		this->compute(odata);
	}
	template<typename T, class ALLOCT>
	StochasticIndependanceSquaredDiff(const std::vector<T, ALLOCT> &odata) :
			m_crit(0), m_critmin(0) {
		this->compute(odata);
	}
	StochasticIndependanceSquaredDiff(
			const StochasticIndependanceSquaredDiff &other) :
			StatTest(other), m_crit(other.m_crit), m_critmin(other.m_critmin) {
	}
	StochasticIndependanceSquaredDiff & operator=(
			const StochasticIndependanceSquaredDiff &other) {
		if (this != &other) {
			StatTest::operator=(other);
			this->m_crit = other.m_crit;
			this->m_critmin = other.m_critmin;
		}
		return (*this);
	}
	virtual ~StochasticIndependanceSquaredDiff() {
	}
public:
	double criteria(void) const {
		return (this->m_crit);
	}
	double min_criteria(void) const {
		return (this->m_critmin);
	}
public:
	virtual void clear(void) {
		StatTest::clear();
		this->m_crit = 0;
		this->m_critmin = 0;
	}
	virtual bool compute_double(size_t n, const double *pData) {
		assert(n > 1);
		assert(pData != nullptr);
		this->clear();
		if (n < 4) {
			return (false);
		}
		this->m_critmin = StochasticIndependanceSquaredDiff::find_mincrit(n);
		double s1 = 0.0;
		for (size_t i = 0; i < n; ++i) {
			double xi = pData[i];
			s1 += xi;
		}	// i
		const double moy = s1 / n;
		double s2 = 0.0;
		s1 = 0.0;
		for (size_t i = 0; i < n; ++i) {
			double xi = pData[i];
			double xx = xi - moy;
			s2 += xx * xx;
			if (i > 0) {
				double xm1 = pData[i - 1];
				xx = xi - xm1;
				s1 += xx * xx;
			}	// i
		}	// i
		double dd = (double) (n - 1);
		s2 /= dd;
		if (s2 <= 0.0) {
			return (false);
		}
		s1 /= 2.0 * dd;
		this->m_crit = s1 / s2;
		this->m_n = n;
		return (this->m_crit >= this->m_critmin);
	}	// compute_double
	virtual bool test(void) const {
		return (this->m_crit >= this->m_critmin);
	}	// test
private:
	double m_crit;
	double m_critmin;
private:
	static double find_mincrit(size_t n);
	static double TAB_VALS[];
};
// class StochasticIndependanceSquaredDiff
///////////////////////////////////////////////
template<class ALLOCBOOL = std::allocator<bool> >
class StochasticIndependanceSeriesTest : public StatTest {
public:
	typedef std::vector<bool,ALLOCBOOL> SignVectorType;
protected:
	StochasticIndependanceSeriesTest(size_t nx = 0): StatTest(nx),
	m_nu(0),m_tau(0),m_nucrit(0),m_taucrit(0) {
	}
	template <typename T>
	StochasticIndependanceSeriesTest(size_t nx,const T *pData): StatTest(nx),
	m_nu(0),m_tau(0),m_nucrit(0),m_taucrit(0) {
		this->compute(nx,pData);
	}
	template <typename T>
	StochasticIndependanceSeriesTest(std::valarray<T> &odata): StatTest(odata.size()),
			m_nu(0),m_tau(0),m_nucrit(0),m_taucrit(0){
				this->compute(odata);
		}
		template <typename T,class ALLOCT>
		StochasticIndependanceSeriesTest(std::vector<T,ALLOCT> &odata): StatTest(odata.size()),
			m_nu(0),m_tau(0),m_nucrit(0),m_taucrit(0){
				this->compute(odata);
		}
		StochasticIndependanceSeriesTest(const StochasticIndependanceSeriesTest<ALLOCBOOL> &other):
			StatTest(other),
			m_nu(other.m_nu),m_tau(other.m_tau),
			m_nucrit(other.m_nucrit),m_taucrit(other.m_taucrit),
			m_signs(other.m_signs){}
		StochasticIndependanceSeriesTest<ALLOCBOOL> & operator=(const StochasticIndependanceSeriesTest<ALLOCBOOL> &other){
			if (this != &other){
				StatTest::operator=(other);
				this->m_nu = other.m_nu;
				this->m_tau = other.m_tau;
				this->m_nucrit = other.m_nucrit;
				this->m_taucrit = other.m_taucrit;
				this->m_signs = other.m_signs;
			}
			return (*this);
		}
	public:
		virtual ~StochasticIndependanceSeriesTest(){}
	public:
		virtual void clear(void){
			StatTest::clear();
			this->m_nu = 0;
			this->m_tau = 0;
			this->m_nucrit = 0;
			this->m_taucrit = 0;
			this->m_signs.clear();
		}
	public:
		virtual bool test(void) const {
			return ((this->m_nu > this->m_nucrit) && (this->m_tau < this->m_taucrit));
		}// test
		virtual StatTestResultType result(void) const  {
			StatTestResultType oRet = h_unknown;
			if ((this->m_nu <= this->m_nucrit) || (this->m_tau >= this->m_taucrit)){
				oRet = h_reject;
			} else if ((this->m_nu > this->m_nucrit) && (this->m_tau < this->m_taucrit)){
				oRet = h_accept;
			}
			return (oRet);
		}// result
	public:
		size_t series_count(void) const {
			return (this->m_nu);
		}
		size_t max_series_length(void) const {
			return (this->m_tau);
		}
		size_t series_count_criteria(void) const {
			return (this->m_nucrit);
		}
		size_t max_series_length_criteria(void) const {
			return (this->m_taucrit);
		}
		const SignVectorType & signs(void) const {
			return (this->m_signs);
		}
	protected:
		size_t	m_nu;
		size_t  m_tau;
		size_t  m_nucrit;
		size_t  m_taucrit;
		SignVectorType m_signs;
	}; // class StochasticIndependanceSeriesTest<ALLOCBOOL>
//////////////////////////////////////////////
template<class ALLOCBOOL = std::allocator<bool> >
class StochasticIndependanceMedianTest : public StochasticIndependanceSeriesTest<ALLOCBOOL> {
public:
	typedef typename StochasticIndependanceSeriesTest<ALLOCBOOL>::SignVectorType SignVectorType;
public:
	StochasticIndependanceMedianTest(size_t nx = 0): StochasticIndependanceSeriesTest<ALLOCBOOL>(nx)
	{
	}
	template <typename T>
	StochasticIndependanceMedianTest(size_t nx,const T *pData)
	{
		this->compute(nx,pData);
	}
	template <typename T>
	StochasticIndependanceMedianTest(std::valarray<T> &odata)
	{
		this->compute(odata);
	}
	template <typename T,class ALLOCT>
	StochasticIndependanceMedianTest(std::vector<T,ALLOCT> &odata)
	{
		this->compute(odata);
	}
	StochasticIndependanceMedianTest(const StochasticIndependanceMedianTest<ALLOCBOOL> &other):
	StochasticIndependanceSeriesTest<ALLOCBOOL>(other) {}
	StochasticIndependanceMedianTest<ALLOCBOOL> & operator=(const StochasticIndependanceMedianTest<ALLOCBOOL> &other) {
		if (this != &other) {
			StochasticIndependanceSeriesTest<ALLOCBOOL>::operator=(other);
		}
		return (*this);
	}
	virtual ~StochasticIndependanceMedianTest() {}
public:
	virtual bool compute_double(size_t n, const double *pData) {
		assert(n > 1);
		assert(pData != nullptr);
		//
		std::unique_ptr<double>  oTemp(new double[n]);
		double *p = oTemp.get();
		assert(p != nullptr);
		for (size_t i = 0; i < n; ++i) {
			p[i] = (double)pData[i];
		}	// i
		double *p2 = (double *)(p + n);
		std::sort(p,p2);
		double vmed = 0.0;
		if ((n % 2) == 0) {
			vmed = (p[(n/2)] + p[(n/2) + 1])/2.0;
		} else {
			vmed = p[(n+1)/2];
		}
		size_t nu = 0;
		size_t tau = 0;
		SignVectorType &vec = this->m_signs;
		vec.clear();
		size_t tau_cur = 0;
		bool nu_cur = false;
		for (size_t i = 0; i < n; ++i) {
			double x = pData[i];
			if (x != vmed) {
				bool scur = (x < vmed) ? false : true;
				vec.push_back(scur);
				if (vec.size() == 1) {
					nu_cur = scur;
					tau_cur = 1;
					nu = 1;
				} else {
					if (scur == nu_cur) {
						++tau_cur;
					} else {
						++nu;
						nu_cur = scur;
						tau_cur = 1;
					}
				}
				if (tau_cur > tau) {
					tau = tau_cur;
				}
			}	// ok
		}	// i
		this->m_n = n;
		this->m_nu = nu;
		this->m_tau = tau;
		this->m_nucrit = (size_t)(0.5*(double)n - 1.0 - 1.96 * ::sqrt((double)n - 1.0));
		this->m_taucrit = (size_t)(3.3*::log10((double)n) + 1.0);
		return ((this->m_nu > this->m_nucrit) && (this->m_tau < this->m_taucrit));
	}	// compute
}; // class StochasticIndependanceMedianTest<ALLOCBOOL>
////////////////////////////////////////////
template<class ALLOCBOOL = std::allocator<bool> >
class StochasticIndependanceAscDescTest : public StochasticIndependanceSeriesTest<ALLOCBOOL> {
	public:
			typedef typename StochasticIndependanceSeriesTest<ALLOCBOOL>::SignVectorType SignVectorType;
	public:
		StochasticIndependanceAscDescTest(size_t nx = 0): StochasticIndependanceSeriesTest<ALLOCBOOL>(nx)
		{
		}
		template <typename T>
		StochasticIndependanceAscDescTest(size_t nx,const T *pData)
		{
			this->compute(nx,pData);
		}
		template <typename T>
		StochasticIndependanceAscDescTest(std::valarray<T> &odata)
		{
			this->compute(odata);
		}
		template <typename T,class ALLOCT>
		StochasticIndependanceAscDescTest(std::vector<T,ALLOCT> &odata)
		{
			this->compute(odata);
		}
		StochasticIndependanceAscDescTest(const StochasticIndependanceAscDescTest<ALLOCBOOL> &other):
			StochasticIndependanceSeriesTest<ALLOCBOOL>(other){}
		StochasticIndependanceAscDescTest<ALLOCBOOL> & operator=(const StochasticIndependanceAscDescTest<ALLOCBOOL> &other){
			if (this != &other){
				StochasticIndependanceSeriesTest<ALLOCBOOL>::operator=(other);
			}
			return (*this);
		}
		virtual ~StochasticIndependanceAscDescTest(){}
	public:
		virtual bool compute_double(size_t n, const double *pData){
			assert(n > 1);
			assert(pData != nullptr);
			//
			size_t nu = 0;
			size_t tau = 0;
			SignVectorType &vec = this->m_signs;
			vec.clear();
			size_t tau_cur = 0;
			bool nu_cur = false;
			bool bFirst = true;
			for (size_t i = 1; i < n; ++i){
				double x1 = pData[i - 1];
				double x2 = pData[i];
				if (x1 == x2){
					if (!bFirst){
						continue;
					}
				} else {
					bFirst = true;
				}
				bool scur = (x1 < x2) ? false : true;
				vec.push_back(scur);
				if (vec.size() == 1){
					nu_cur = scur;
					tau_cur = 1;
					nu = 1;
				} else {
					if (scur == nu_cur){
						++tau_cur;
					} else {
						++nu;
						nu_cur = scur;
						tau_cur = 1;
					}
				}
				if (tau_cur > tau){
					tau = tau_cur;
				}
			}// i
			this->m_n = n;
			this->m_nu = nu;
			this->m_tau = tau;
			this->m_nucrit = (size_t)(((2.0 * n -1.0)/3.0) - 1.96 * ::sqrt((16.0 * n - 29.0)/90.0));
			if (n <= 26){
				this->m_taucrit = 5;
			} else if ((n > 26) && (n <= 153)) {
				this->m_taucrit = 6;
			} else if (n > 153){
				this->m_taucrit = 7;
			}
			return ((this->m_nu > this->m_nucrit) && (this->m_tau < this->m_taucrit));
		}// compute
	}; // class StochasticIndependanceMedianTest<ALLOCBOOL>
	////////////////////////////////////////////
}// namespace statdata
/////////////////////////////////
#endif /* STATTEST_H_ */
