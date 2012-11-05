/*
 * dataport.h
 *
 *  Created on: Sep 5, 2012
 *      Author: boubad
 */

#ifndef DATAPORT_H_
#define DATAPORT_H_
/////////////////////////////////////
#if defined(_MSC_VER)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif
#pragma warning (disable: 4996)
#endif // _MSC_VER
//////////////////////////
#if defined(__GNUG__)
#if (__GNUC__ < 5)
#if !((__GNUC__ == 4) && (__GNUC_MINOR >= 6))
#ifndef nullptr
#define nullptr 0
#endif // nullptr
#endif // 4.6
#endif // 5
#endif // __GNUG__
////////////////
#ifdef __SUNPRO_CC
#ifndef nullptr
#define nullptr 0
#endif
#endif
//////////////////////////////////
#include "datatype.h"
///////////////////////////////////
#endif /* DATAPORT_H_ */
