/*
 * skewness.cpp
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */
#include "../include/univarstatdesc.h"
///////////////////////////////////
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/skewness.hpp>
#include <boost/accumulators/statistics/kurtosis.hpp>
///////////////////////////////////
namespace statdata {
using namespace boost::accumulators;
///////////////////////////////////////////
void UnivarStatDesc::compute_var(const size_t n, const double *pData) {
	accumulator_set<double, features<tag::variance> > acc5;
	accumulator_set<double, features<tag::skewness> > acc6;
	accumulator_set<double, features<tag::kurtosis> > acc7;
	//
	for (size_t i = 0; i < n; ++i) {
		double x = pData[i];
		acc5(x);
		acc6(x);
		//acc7(x);
	}
	if (n > 1) {
		this->m_var = boost::accumulators::variance(acc5);
		this->m_std = ::sqrt(this->m_var / (double) (n - 1));
		if (n > 2) {
			this->m_skew = boost::accumulators::skewness(acc6);
			if (n > 3) {
				this->m_flat = boost::accumulators::kurtosis(acc7);
			} // nx > 3
		} // nx > 2
	} // nx > 1
} // compute_var
///////////////////////////////////////
} // namespace statdata




