/*
 * distributions.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef DISTRIBUTIONS_H_
#define DISTRIBUTIONS_H_
///////////////////////////////////
namespace statdata {
/////////////////////////////////////
class Distributions {
public:
	static double quantile_f(double df1, double df2, double prob);
	static double prob_f(double df1, double df2, double t);
	static double quantile_chi_squared(double df, double prob);
	static double prob_chi_squared(double df, double t);
	static double quantile_student_t(double df, double prob);
	static double prob_student_t(double df, double t);
	static double quantile_normale(double prob);
	static double prob_normale(double t);
};
// class Distributions
///////////////////////////////////////////
}// namespace statdata
///////////////////////////////////////
#endif /* DISTRIBUTIONS_H_ */
