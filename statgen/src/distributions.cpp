/*
 * distributions.cpp
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */
#include "../include/distributions.h"
/////////////////////////////////////
#include <cassert>
//////////////////////////////////////
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/fisher_f.hpp>
////////////////////////////////////
namespace statdata {
// Bring everything into global namespace for ease of use:
using namespace boost::math;
/////////////////////////////////////////
double Distributions::quantile_f(double df1, double df2, double prob) {
	assert(df1 > 0.0);
	assert(df2 > 0.0);
	assert(prob >= 0.0);
	assert(prob <= 1.0);
	fisher_f dist(df1, df2);
	double r = quantile(complement(dist, prob));
	return (r);
} // quantile_f
double Distributions::prob_f(double df1, double df2, double t) {
	assert(df1 > 0.0);
	assert(df2 > 0.0);
	fisher_f dist(df1, df2);
	double r = cdf(dist, t);
	return (r);
} //prob_f
////////////////////////////////////////
double Distributions::quantile_chi_squared(double df, double prob) {
	assert(df > 0.0);
	assert(prob >= 0.0);
	assert(prob <= 1.0);
	chi_squared dist(df);
	double r = quantile(complement(dist, prob));
	return (r);
}	// quantile_chi_squared
double Distributions::prob_chi_squared(double df, double t) {
	assert(df > 0.0);
	assert(t >= 0.0);
	chi_squared dist(df);
	double r = cdf(dist, t);
	return (r);
}	//prob_chi_squared
//////////////////////////////////////
double Distributions::quantile_student_t(double df, double prob) {
	assert(df > 0.0);
	assert(prob >= 0.0);
	assert(prob <= 1.0);
	students_t dist(df);
	double r = quantile(complement(dist, prob));
	return (r);
}	//student_t_prob
double Distributions::prob_student_t(double df, double t) {
	assert(df > 0.0);
	students_t dist(df);
	double r = cdf(dist, t);
	return (r);
}	//prob_student_t
double Distributions::quantile_normale(double prob) {
	assert(prob >= 0.0);
	assert(prob <= 1.0);
	normal dist;
	double r = quantile(complement(dist, prob));
	return (r);
}	// quantile_normale
double Distributions::prob_normale(double t) {
	normal dist;
	double r = cdf(dist, t);
	return (r);
}	// prob_normale
///////////////////////////////////////
}	// namespace statdata
/////////////////////////////////////

