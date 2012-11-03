/*
 * kurtosis.cpp
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */
#include "../include/univarstatdesc.h"
///////////////////////////////////
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/kurtosis.hpp>
///////////////////////////////////
namespace statdata {
using namespace boost::accumulators;
///////////////////////////////////////////
void UnivarStatDesc::compute_kurtosis(const size_t n, const double *pData) {
	accumulator_set<double, features<tag::kurtosis> > acc7;
	//
	for (size_t i = 0; i < n; ++i) {
		double x = pData[i];
		acc7(x);
	}
	if (n > 3) {
		this->m_flat = boost::accumulators::kurtosis(acc7);
	} // nx > 3
} // compute_var
///////////////////////////////////////
} // namespace statdata







