/*
 * univarstatdesc.cpp
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */
#include "../include/univarstatdesc.h"
///////////////////////////////////
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/median.hpp>
////////////////////////////////////
#include "../include/distributions.h"
///////////////////////////////////
namespace statdata {
using namespace boost::accumulators;
///////////////////////////////////////////
bool UnivarStatDesc::mean_confidence_interval(double &vMin, double &vMax,double prob /*= 0.05 */){
	if ((prob < 0.0) || (prob > 1.0)){
		return (false);
	}
	double vMean = this->m_mean;
	double vec = this->m_std;
	size_t n = this->m_n;
	if ((n < 1) || (vec == 0.0)){
		vMin = vMean;
		vMax = vMean;
		return (true);
	}
	double t = statdata::Distributions::quantile_student_t((double)(n -1),prob / 2.0);
	double w = t * vec / sqrt(double((double)n));
	vMin = vMean - w;
	vMax = vMean + w;
	return (true);
}//mean_confidence_interval
void UnivarStatDesc::clear(void) {
	this->m_n = 0;
	this->m_min = 0;
	this->m_max = 0;
	this->m_mean = 0;
	this->m_median = 0;
	this->m_var = 0;
	this->m_std = 0;
	this->m_skew = 0;
	this->m_flat = 0;
	this->m_quant05 = 0;
	this->m_quant10 = 0;
	this->m_quant25 = 0;
	this->m_quant75 = 0;
	this->m_quant90 = 0;
	this->m_quant95 = 0;
} // clear
bool UnivarStatDesc::compute(const size_t n, const double *pData) {
	//
	if ((n < 1) || (pData == nullptr)) {
		return (false);
	}
	accumulator_set<double, features<tag::count> > acc0;
	accumulator_set<double, features<tag::min> > acc1;
	accumulator_set<double, features<tag::max> > acc2;
	accumulator_set<double, features<tag::mean> > acc3;
	accumulator_set<double, features<tag::median> > acc4;
	//
	for (size_t i = 0; i < n; ++i) {
		double x = pData[i];
		acc0(x);
		acc1(x);
		acc2(x);
		acc3(x);
		acc4(x);
	}
	const size_t nx = boost::accumulators::count(acc0);
	this->m_n = nx;
	this->m_min = boost::accumulators::min(acc1);
	this->m_max = boost::accumulators::max(acc2);
	this->m_mean = boost::accumulators::mean(acc3);
	this->m_median = boost::accumulators::median(acc4);
	this->compute_var(n, pData);
	this->compute_kurtosis(n,pData);
	this->compute_quants(n,pData);
	return (true);
} // compute
///////////////////////////////////////
} // namespace statdata




