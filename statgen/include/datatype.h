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

    typedef enum {
        mode_read, mode_write, mode_replace, mode_notopen
    } datafile_open_mode;

    typedef enum {
        flag_missing_value = 0x01,
        flag_valid_min = 0x02,
        flag_valid_max = 0x04,
        flag_units = 0x08,
        flag_long_name = 0x10,
        flag_c_format = 0x20,
        flag_title = 0x40,
        flag_all = 0xFF
    } datafile_attributes_flag;
    //////////////////////////////
}// namespace statdata
////////////////////////////////////
#endif	/* DATATYPE_H */

