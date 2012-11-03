/*
 * stattest.cpp
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */
#include "../include/stattest.h"
/////////////////////////////////
namespace statdata {
////////////////////////////////////
double StochasticIndependanceSquaredDiff::TAB_VALS[] = { 0.390, 0.410, 0.445,
		0.468, 0.491, 0.512, 0.531, 0.548, 0.564, 0.578, 0.591, 0.603, 0.614,
		0.624, 0.633, 0, 642, 0.650 };
double StochasticIndependanceSquaredDiff::find_mincrit(size_t n) {
	if (n < 4) {
		return (0.0);
	} else if (n > 20) {
		double ua = -1.64485;
		double ux = 0.5 * (1.0 + (ua * ua));
		double x = ua / ::sqrt((double) n + ux);
		return (1.0 + x);
	} else {
		size_t nx = (size_t) (n - 4);
		return TAB_VALS[nx];
	}
} // find_mincrit
/////////////////////////////////////
}	// namespace statdata
////////////////////////////////////
