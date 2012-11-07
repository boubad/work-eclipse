/*
 * eigen.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef EIGEN_H_
#define EIGEN_H_
//////////////////////////
#include "../../statgen/include/dataport.h"
////////////////////////////////////
#include <cstdlib>
/////////////////////////////////////////
namespace statdata {
//////////////////////////////////////////////
extern bool compute_eigen_impl(const size_t n, const double *pData,
		double *pVals, double *pVecs, size_t *pNbFacts = nullptr);
//////////////////////////////////////////////
}// namespace statdata
///////////////////////////////////////////

#endif /* EIGEN_H_ */
