/* 
 * File:   datatype.h
 * Author: boubad
 *
 * Created on 5 novembre 2012, 18:36
 */

#ifndef DATATYPE_H
#define	DATATYPE_H
////////////////////////////////
namespace statdata {
    //////////////////////////////////////
    typedef enum {
        typeOther,
        typeBool,
        typeChar,
        typeSChar,
        typeByte,
        typeShort,
        typeUShort,
        typeInt,
        typeUInt,
        typeLong,
        typeULong,
        typeLongLong,
        typeULongLong,
        typeFloat,
        typeDouble,
        typeLongDouble,
        typeString,
        typeTime,
        typeDate,
        typeDateTime,
        typeWString,         
        typeStatIndiv,
        typeStatVariable,
        typeStatCorrel
    } DataType;
    /////////////////////////////////////////
}// namespace statdata
////////////////////////////////////
#endif	/* DATATYPE_H */

