/*
 * quantiles.cpp
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */
#include "../include/univarstatdesc.h"
///////////////////////////////////
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/p_square_quantile.hpp>
///////////////////////////////////
namespace statdata {
using namespace boost::accumulators;
///////////////////////////////////////////
void UnivarStatDesc::compute_quants(const size_t n, const double *pData) {
	//
	typedef accumulator_set<double, stats<tag::p_square_quantile> > accumulator_t;
	//
	accumulator_t aq05(quantile_probability = 0.05);
	accumulator_t aq10(quantile_probability = 0.10);
	accumulator_t aq25(quantile_probability = 0.25);
	accumulator_t aq75(quantile_probability = 0.75);
	accumulator_t aq90(quantile_probability = 0.90);
	accumulator_t aq95(quantile_probability = 0.95);
	//
	for (size_t i = 0; i < n; ++i) {
		double x = pData[i];
		aq05(x);
		aq10(x);
		aq25(x);
		aq75(x);
		aq90(x);
		aq95(x);
	}
	this->m_quant05 = p_square_quantile(aq05);
	this->m_quant10 = p_square_quantile(aq10);
	this->m_quant25 = p_square_quantile(aq25);
	this->m_quant75 = p_square_quantile(aq75);
	this->m_quant90 = p_square_quantile(aq90);
	this->m_quant95 = p_square_quantile(aq95);
} // compute_quants
///////////////////////////////////////
} // namespace statdata
