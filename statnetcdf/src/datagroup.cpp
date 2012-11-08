#include "../include/datagroup.h"
////////////////////////////////////
#include <cassert>
#include <sstream>
////////////////////////////////
#if defined(_MSC_VER)
#include <netcdf/netcdf.h>
#else
#include <netcdf.h>
#endif
//////////////////////////////
#if defined(VERSION_NETCDF4)
#undef VERSION_NETCDF4
#endif
#ifdef NC_STRING
#define VERSION_NETCDF4  1
#endif
///////////////////////////
namespace statdata {
    //////////////////////////////////////
    const char *NetCDFDataGroup::ATT_MISSING_VALUE = "missing_value";
    const char *NetCDFDataGroup::ATT_C_FORMAT = "C_format";
    const char *NetCDFDataGroup::ATT_LONG_NAME = "long_name";
    const char *NetCDFDataGroup::ATT_VALID_MIN = "valid_min";
    const char *NetCDFDataGroup::ATT_VALID_MAX = "valid_max";
    const char *NetCDFDataGroup::ATT_HISTORY = "history";
    const char *NetCDFDataGroup::ATT_TITLE = "title";
    const char *NetCDFDataGroup::ATT_UNITS = "units";
    const char *NetCDFDataGroup::ATT_VALID_RANGE = "valid_range";
    const char *NetCDFDataGroup::ATT_FILL_VALUE = "_FillValue";
    const char *NetCDFDataGroup::DIM_STRINGAXIS = "axis_s";
    const size_t NetCDFDataGroup::MAX_STRING_LEN = 31;
    ///////////////////////////////////
    namespace internal {
        ////////////////////////////

        static statdata::DataType NCType2DataType(nc_type t) {
            statdata::DataType dRet = statdata::typeOther;
            switch (t) {
                case NC_BYTE:
                    dRet = statdata::typeByte;
                    break;
                case NC_SHORT:
                    dRet = statdata::typeShort;
                    break;
                case NC_INT:
                    dRet = statdata::typeInt;
                    break;
                case NC_FLOAT:
                    dRet = statdata::typeFloat;
                    break;
                case NC_DOUBLE:
                    dRet = statdata::typeDouble;
                    break;
                case NC_CHAR:
                    dRet = statdata::typeString;
                    break;
#ifdef VERSION_NETCDF4
                case NC_STRING:
                    dRet = statdata::typeString;
                    break;
#endif
                default:
                    break;
            } // t
            return dRet;
        } //NCType2Datastatdata::DataType

        static nc_type DataType2NCType(statdata::DataType t) {
            nc_type cRet = NC_DOUBLE;
            switch (t) {
                case statdata::typeByte:
                    cRet = NC_BYTE;
                    break;
                case statdata::typeShort:
                    cRet = NC_SHORT;
                    break;
                case statdata::typeInt:
                    cRet = NC_INT;
                    break;
                case statdata::typeFloat:
                    cRet = NC_FLOAT;
                    break;
                case statdata::typeDouble:
                    cRet = NC_DOUBLE;
                    break;
                case statdata::typeString:
#ifndef VERSION_NETCDF4
                    cRet = NC_CHAR;
#else
                    cRet = NC_STRING;
#endif
                    break;
                default:
                    break;
            } // t
            return cRet;
        } // Datastatdata::DataType2NCType
        ////////////////////////////////
    } // namespace internal
    //////////////////////////////////////

    bool NetCDFDataGroup::get_string_axis(NetCDFDimension &oDim) {
        std::string sname(DIM_STRINGAXIS);
        if (this->get_dimension(sname, oDim)) {
            return (true);
        }
        return (this->create_dimension(oDim, sname, MAX_STRING_LEN + 1));
    } // get_string_axis

    bool NetCDFDataGroup::create_axis(const std::string &sname, const std::string &sunits,
            double vMin, double vMax, double delta) {
        if ((vMin >= vMax) || (delta <= 0.0)) {
            return (false);
        }
        const size_t nCount = (size_t) ((vMax - vMin) / delta);
        if (nCount < 2) {
            return (false);
        }
        Values vv(nCount + 1);
        for (size_t i = 0; i <= nCount; ++i) {
            vv[i] = (double) (vMin + i * delta);
        } // i
        return (this->create_axis(sname, sunits, vv));
    } // create_axis

    bool NetCDFDataGroup::create_axis(const std::string &sname, const std::string &sunits,
            float vMin, float vMax, float delta) {
        if ((vMin >= vMax) || (delta <= 0.0f)) {
            return (false);
        }
        const size_t nCount = (size_t) (((double) vMax - vMin) / delta);
        if (nCount < 2) {
            return (false);
        }
        Values vv(nCount + 1);
        for (size_t i = 0; i <= nCount; ++i) {
            float x = (float) ((double) vMin + i * delta);
            vv[i] = x;
        } // i
        return (this->create_axis(sname, sunits, vv));
    } // create_axis

    bool NetCDFDataGroup::create_axis(const std::string &sname, const std::string &sunits,
            int vMin, int vMax, int delta) {
        if ((vMin >= vMax) || (delta <= (int) 0)) {
            return (false);
        }
        const size_t nCount = (size_t) (((double) vMax - vMin) / delta);
        if (nCount < 2) {
            return (false);
        }
        Values vv(nCount + 1);
        for (size_t i = 0; i <= nCount; ++i) {
            vv[i] = (int) (vMin + i * delta);
        } // i
        return (this->create_axis(sname, sunits, vv));
    } // create_axis

    bool NetCDFDataGroup::create_axis(const std::string &sname, const std::string &sunits,
            short vMin, short vMax, short delta) {
        if ((vMin >= vMax) || (delta <= (short) 0)) {
            return (false);
        }
        const size_t nCount = (size_t) (((double) vMax - vMin) / delta);
        if (nCount < 2) {
            return (false);
        }
        Values vv(nCount + 1);
        for (size_t i = 0; i <= nCount; ++i) {
            vv[i] = (short) (vMin + i * delta);
        } // i
        return (this->create_axis(sname, sunits, vv));
    } // create_axis

    bool NetCDFDataGroup::create_axis(const std::string &sname, const std::string &sunits,
            byte vMin, byte vMax, byte delta) {
        if ((vMin >= vMax) || (delta <= (byte) 0)) {
            return (false);
        }
        const size_t nCount = (size_t) (((double) vMax - vMin) / delta);
        if (nCount < 2) {
            return (false);
        }
        Values vv(nCount + 1);
        for (size_t i = 0; i <= nCount; ++i) {
            vv[i] = (byte) (vMin + i * delta);
        } // i
        return (this->create_axis(sname, sunits, vv));
    } // create_axis

    bool NetCDFDataGroup::get_all_axis(NetCDFDimensions &dims) {
        dims.clear();
        NetCDFDimensions oVec;
        if (!this->get_dimensions(oVec)) {
            return (false);
        }
        const size_t n = oVec.size();
        for (size_t i = 0; i < n; ++i) {
            const NetCDFDimension &oDim = oVec[i];
            NetCDFVariable oVar;
            if (this->get_variable(oDim.name, oVar)) {
                if (oVar.dims.size() == 1) {
                    dims.push_back(oDim);
                }
            }
        } // i
        return (true);
    } // get_all_axis

    bool NetCDFDataGroup::create_axis(const std::string &sname,
            const statdata::DataType &vtype) {
        if (sname.empty()) {
            return (false);
        }
        switch (vtype) {
            case statdata::typeDouble:
            case statdata::typeFloat:
            case statdata::typeShort:
            case statdata::typeInt:
            case statdata::typeByte:
                break;
            default:
                return (false);
        } // vtype
        NetCDFDimension oDim;
        if (!this->get_dimension(sname, oDim)) {
            if (!this->create_dimension(oDim, sname)) {
                return (false);
            }
        } //dim
        NetCDFVariable oVar;
        if (this->get_variable(sname, oVar)) {
            if (oVar.type != vtype) {
                return (false);
            }
            if (oVar.dims.size() != 1) {
                return (false);
            }
        } else {
            oVar.clear();
            oVar.name = sname;
            oVar.type = vtype;
            oVar.dims.push_back(oDim);
            if (!this->create_variable(oVar)) {
                return (false);
            }
        }
        return (true);
    } // create_axis

    bool NetCDFDataGroup::create_axis(const std::string &sname, const std::string &sunits,
            const Values &values) {
        const size_t nCount = values.size();
        if (sname.empty() || (nCount < 2)) {
            return (false);
        }

        auto it = std::find_if(values.begin(), values.end(),
                [](const boost::any & v)->bool {
            return (!v.empty());
        });
        if (it == values.end()) {
            return (false);
        }
        statdata::DataType vtype = Value::get_type(*it);
        switch (vtype) {
            case statdata::typeDouble:
            case statdata::typeFloat:
            case statdata::typeShort:
            case statdata::typeInt:
            case statdata::typeByte:
                break;
            default:
                return (false);
        } // vtype
        for (size_t i = 0; i < nCount; ++i) {
            if (i > 0) {
                double x1 = 0, x2 = 0;
                Value::get_value(values[i - 1], x1);
                Value::get_value(values[i], x2);
                if (x1 >= x2) {
                    return (false);
                }
            }
        } // i
        NetCDFDimension oDim;
        if (!this->get_dimension(sname, oDim)) {
            if (!this->create_dimension(oDim, sname, nCount)) {
                return (false);
            }
        } //dim
        NetCDFVariable oVar;
        if (this->get_variable(sname, oVar)) {
            if (oVar.type != vtype) {
                return (false);
            }
            if (oVar.dims.size() != 1) {
                return (false);
            }
        } else {
            oVar.clear();
            oVar.name = sname;
            oVar.type = vtype;
            if (!sunits.empty()) {
                oVar.units = sunits;
            }
            oVar.dims.push_back(oDim);
            if (!this->create_variable(oVar)) {
                return (false);
            }
        }
        Values range(2);
        range[0] = values[0];
        range[1] = values[nCount - 1];
        std::string attname = get_valid_range_attribute_name();
        if (!this->write_attribute(sname, attname, range)) {
            return (false);
        }
        std::valarray<size_t> index(1), sizes(1);
        index[0] = 0;
        sizes[0] = nCount;
        return (this->write_data(sname, index, sizes, values));
    } // create_axis

    bool NetCDFDataGroup::check_variable_data(const std::string &varname, Values &vals) {
        if (varname.empty()) {
            return (false);
        }
        const size_t n = vals.size();
        if (n < 1) {
            return (true);
        }
        const int nId = this->id();
        int status = -1;
        int nVarId = -1;
        const char *px = varname.c_str();
        if ((status = ::nc_inq_varid(nId, px, &nVarId)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        nc_type xtype;
        if ((status = ::nc_inq_vartype(nId, nVarId, &xtype)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        if ((xtype == NC_BYTE) || (xtype == NC_CHAR)) {
            return (true);
        }
#ifdef VERSION_NETCDF4
        if (xtype == NC_STRING) {
            for (size_t i = 0; i < n; ++i) {
                std::string s;
                if (Value::get_value(vals[i], s)) {
                    if (s.empty()) {
                        vals[i] = boost::any();
                    }
                }
            } // i
            return (true);
        }
#endif
        switch (xtype) {
            case NC_DOUBLE:
            {
                double vFill, vmin, vmax, fMissing;
                double dd[2];
                bool bMin = false, bMax = false, bMissing = false;
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_FILL_VALUE, &vFill))
                        != NC_NOERR) {
                    vFill = NC_FILL_DOUBLE;
                }
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_VALID_RANGE, dd))
                        == NC_NOERR) {
                    vmin = dd[0];
                    vmax = dd[1];
                    if (vmin < vmax) {
                        bMin = true;
                        bMax = true;
                    }
                }
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_MISSING_VALUE,
                        &fMissing)) == NC_NOERR) {
                    bMissing = true;
                }
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_VALID_MIN, &vmin))
                        == NC_NOERR) {
                    bMin = true;
                }
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_VALID_MAX, &vmax))
                        == NC_NOERR) {
                    bMax = true;
                }
                for (size_t i = 0; i < n; ++i) {
                    double v;
                    if (Value::get_value(vals[i], v)) {
                        if (v == vFill) {
                            vals[i] = boost::any();
                        } else if (bMissing && (v == fMissing)) {
                            vals[i] = boost::any();
                        } else if (bMin && (v < vmin)) {
                            vals[i] = boost::any();
                        } else if (bMax && (v > vmax)) {
                            vals[i] = boost::any();
                        }
                    } // v
                } // i
            }
                break;
            case NC_FLOAT:
            {
                float vFill, vmin, vmax, fMissing;
                float dd[2];
                bool bMin = false, bMax = false, bMissing = false;
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_FILL_VALUE, &vFill))
                        != NC_NOERR) {
                    vFill = NC_FILL_FLOAT;
                }
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_VALID_RANGE, dd))
                        == NC_NOERR) {
                    vmin = dd[0];
                    vmax = dd[1];
                    if (vmin < vmax) {
                        bMin = true;
                        bMax = true;
                    }
                }
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_MISSING_VALUE,
                        &fMissing)) == NC_NOERR) {
                    bMissing = true;
                }
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_VALID_MIN, &vmin))
                        == NC_NOERR) {
                    bMin = true;
                }
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_VALID_MAX, &vmax))
                        == NC_NOERR) {
                    bMax = true;
                }
                for (size_t i = 0; i < n; ++i) {
                    float v;
                    if (Value::get_value(vals[i], v)) {
                        if (v == vFill) {
                            vals[i] = boost::any();
                        } else if (bMissing && (v == fMissing)) {
                            vals[i] = boost::any();
                        } else if (bMin && (v < vmin)) {
                            vals[i] = boost::any();
                        } else if (bMax && (v > vmax)) {
                            vals[i] = boost::any();
                        }
                    } // v
                } // i
            }
                break;
            case NC_INT:
            {
                int vFill, vmin, vmax, fMissing;
                int dd[2];
                bool bMin = false, bMax = false, bMissing = false;
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_FILL_VALUE, &vFill))
                        != NC_NOERR) {
                    vFill = NC_FILL_INT;
                }
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_VALID_RANGE, dd))
                        == NC_NOERR) {
                    vmin = dd[0];
                    vmax = dd[1];
                    if (vmin < vmax) {
                        bMin = true;
                        bMax = true;
                    }
                }
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_MISSING_VALUE,
                        &fMissing)) == NC_NOERR) {
                    bMissing = true;
                }
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_VALID_MIN, &vmin))
                        == NC_NOERR) {
                    bMin = true;
                }
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_VALID_MAX, &vmax))
                        == NC_NOERR) {
                    bMax = true;
                }
                for (size_t i = 0; i < n; ++i) {
                    int v;
                    if (Value::get_value(vals[i], v)) {
                        if (v == vFill) {
                            vals[i] = boost::any();
                        } else if (bMissing && (v == fMissing)) {
                            vals[i] = boost::any();
                        } else if (bMin && (v < vmin)) {
                            vals[i] = boost::any();
                        } else if (bMax && (v > vmax)) {
                            vals[i] = boost::any();
                        }
                    } // v
                } // i
            }
                break;
            case NC_SHORT:
            {
                short vFill, vmin, vmax, fMissing;
                short dd[2];
                bool bMin = false, bMax = false, bMissing = false;
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_FILL_VALUE, &vFill))
                        != NC_NOERR) {
                    vFill = NC_FILL_SHORT;
                }
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_VALID_RANGE, dd))
                        == NC_NOERR) {
                    vmin = dd[0];
                    vmax = dd[1];
                    if (vmin < vmax) {
                        bMin = true;
                        bMax = true;
                    }
                }
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_MISSING_VALUE,
                        &fMissing)) == NC_NOERR) {
                    bMissing = true;
                }
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_VALID_MIN, &vmin))
                        == NC_NOERR) {
                    bMin = true;
                }
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_VALID_MAX, &vmax))
                        == NC_NOERR) {
                    bMax = true;
                }
                for (size_t i = 0; i < n; ++i) {
                    short v;
                    if (Value::get_value(vals[i], v)) {
                        if (v == vFill) {
                            vals[i] = boost::any();
                        } else if (bMissing && (v == fMissing)) {
                            vals[i] = boost::any();
                        } else if (bMin && (v < vmin)) {
                            vals[i] = boost::any();
                        } else if (bMax && (v > vmax)) {
                            vals[i] = boost::any();
                        }
                    } // v
                } // i
            }
                break;
            default:
                break;
        } // type
        return (true);
    } // check_variable_data

    bool NetCDFDataGroup::check_variable_data(const std::string &varname,
            boost::any &val) {
        if (varname.empty()) {
            return (false);
        }
        if (val.empty()) {
            return (true);
        }
        const int nId = this->id();
        int status = -1;
        int nVarId = -1;
        const char *px = varname.c_str();
        if ((status = ::nc_inq_varid(nId, px, &nVarId)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        nc_type xtype;
        if ((status = ::nc_inq_vartype(nId, nVarId, &xtype)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        if ((xtype == NC_BYTE) || (xtype == NC_CHAR)) {
            return (true);
        }
#ifdef VERSION_NETCDF4
        if (xtype == NC_STRING) {
            std::string s;
            if (Value::get_value(val, s)) {
                if (s.empty()) {
                    val = boost::any();
                }
            }
            return (true);
        }
#endif
        switch (xtype) {
            case NC_DOUBLE:
            {
                double vFill, vmin, vmax, fMissing;
                double dd[2];
                bool bMin = false, bMax = false, bMissing = false;
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_FILL_VALUE, &vFill))
                        != NC_NOERR) {
                    vFill = NC_FILL_DOUBLE;
                }
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_VALID_RANGE, dd))
                        == NC_NOERR) {
                    vmin = dd[0];
                    vmax = dd[1];
                }
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_MISSING_VALUE,
                        &fMissing)) == NC_NOERR) {
                    bMissing = true;
                }
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_VALID_MIN, &vmin))
                        == NC_NOERR) {
                    bMin = true;
                }
                if ((status = ::nc_get_att_double(nId, nVarId, ATT_VALID_MAX, &vmax))
                        == NC_NOERR) {
                    bMax = true;
                }
                double v;
                if (Value::get_value(val, v)) {
                    if (v == vFill) {
                        val = boost::any();
                    } else if (bMissing && (v == fMissing)) {
                        val = boost::any();
                    } else if (bMin && (v < vmin)) {
                        val = boost::any();
                    } else if (bMax && (v > vmax)) {
                        val = boost::any();
                    }
                } // v
            }
                break;
            case NC_FLOAT:
            {
                float vFill, vmin, vmax, fMissing;
                float dd[2];
                bool bMin = false, bMax = false, bMissing = false;
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_FILL_VALUE, &vFill))
                        != NC_NOERR) {
                    vFill = NC_FILL_FLOAT;
                }
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_VALID_RANGE, dd))
                        == NC_NOERR) {
                    vmin = dd[0];
                    vmax = dd[1];
                    if (vmin < vmax) {
                        bMin = true;
                        bMax = true;
                    }
                }
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_MISSING_VALUE,
                        &fMissing)) == NC_NOERR) {
                    bMissing = true;
                }
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_VALID_MIN, &vmin))
                        == NC_NOERR) {
                    bMin = true;
                }
                if ((status = ::nc_get_att_float(nId, nVarId, ATT_VALID_MAX, &vmax))
                        == NC_NOERR) {
                    bMax = true;
                }
                float v;
                if (Value::get_value(val, v)) {
                    if (v == vFill) {
                        val = boost::any();
                    } else if (bMissing && (v == fMissing)) {
                        val = boost::any();
                    } else if (bMin && (v < vmin)) {
                        val = boost::any();
                    } else if (bMax && (v > vmax)) {
                        val = boost::any();
                    }
                } // v
            }
                break;
            case NC_INT:
            {
                int vFill, vmin, vmax, fMissing;
                int dd[2];
                bool bMin = false, bMax = false, bMissing = false;
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_FILL_VALUE, &vFill))
                        != NC_NOERR) {
                    vFill = NC_FILL_INT;
                }
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_VALID_RANGE, dd))
                        == NC_NOERR) {
                    vmin = dd[0];
                    vmax = dd[1];
                    if (vmin < vmax) {
                        bMin = true;
                        bMax = true;
                    }
                }
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_MISSING_VALUE,
                        &fMissing)) == NC_NOERR) {
                    bMissing = true;
                }
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_VALID_MIN, &vmin))
                        == NC_NOERR) {
                    bMin = true;
                }
                if ((status = ::nc_get_att_int(nId, nVarId, ATT_VALID_MAX, &vmax))
                        == NC_NOERR) {
                    bMax = true;
                }
                int v;
                if (Value::get_value(val, v)) {
                    if (v == vFill) {
                        val = boost::any();
                    } else if (bMissing && (v == fMissing)) {
                        val = boost::any();
                    } else if (bMin && (v < vmin)) {
                        val = boost::any();
                    } else if (bMax && (v > vmax)) {
                        val = boost::any();
                    }
                } // v
            }
                break;
            case NC_SHORT:
            {
                short vFill, vmin, vmax, fMissing;
                short dd[2];
                bool bMin = false, bMax = false, bMissing = false;
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_FILL_VALUE, &vFill))
                        != NC_NOERR) {
                    vFill = NC_FILL_SHORT;
                }
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_VALID_RANGE, dd))
                        == NC_NOERR) {
                    vmin = dd[0];
                    vmax = dd[1];
                    if (vmin < vmax) {
                        bMin = true;
                        bMax = true;
                    }
                }
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_MISSING_VALUE,
                        &fMissing)) == NC_NOERR) {
                    bMissing = true;
                }
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_VALID_MIN, &vmin))
                        == NC_NOERR) {
                    bMin = true;
                }
                if ((status = ::nc_get_att_short(nId, nVarId, ATT_VALID_MAX, &vmax))
                        == NC_NOERR) {
                    bMax = true;
                }
                short v;
                if (Value::get_value(val, v)) {
                    if (v == vFill) {
                        val = boost::any();
                    } else if (bMissing && (v == fMissing)) {
                        val = boost::any();
                    } else if (bMin && (v < vmin)) {
                        val = boost::any();
                    } else if (bMax && (v > vmax)) {
                        val = boost::any();
                    }
                } // v
            }
                break;
            default:
                break;
        } // type
        return (true);
    }
    // check_variable_data

    bool NetCDFDataGroup::get_variable_fill(const std::string &varname,
            boost::any &val) {
#ifdef VERSION_NETCDF4
        val = boost::any();
        int nVarId = -1;
        const int nId = this->id();
        int status = ::nc_inq_varid(nId, varname.c_str(), &nVarId);
        if (status != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        nc_type xtype;
        if ((status = ::nc_inq_vartype(nId, nVarId, &xtype) != NC_NOERR)) {
            this->handle_error(status);
            return (false);
        }
        int no_fill = 0;
        switch (xtype) {
            case NC_CHAR:
            {
                char x;
                if ((status = ::nc_inq_var_fill(nId, nVarId, &no_fill, &x)) != NC_NOERR) {
                    x = NC_FILL_CHAR;
                }
                val = x;
            }
                break;
            case NC_BYTE:
            {
                byte x;
                if ((status = ::nc_inq_var_fill(nId, nVarId, &no_fill, &x)) != NC_NOERR) {
                    x = NC_FILL_BYTE;
                }
                val = x;
            }
                break;
            case NC_SHORT:
            {
                short x;
                if ((status = ::nc_inq_var_fill(nId, nVarId, &no_fill, &x)) != NC_NOERR) {
                    x = NC_FILL_SHORT;
                }
                val = x;
            }
                break;
            case NC_INT:
            {
                int x;
                if ((status = ::nc_inq_var_fill(nId, nVarId, &no_fill, &x)) != NC_NOERR) {
                    x = NC_FILL_INT;
                }
                val = x;
            }
                break;
            case NC_FLOAT:
            {
                float x;
                if ((status = ::nc_inq_var_fill(nId, nVarId, &no_fill, &x)) != NC_NOERR) {
                    x = NC_FILL_FLOAT;
                }
                val = x;
            }
                break;
            case NC_DOUBLE:
            {
                double x;
                if ((status = ::nc_inq_var_fill(nId, nVarId, &no_fill, &x)) != NC_NOERR) {
                    x = NC_FILL_DOUBLE;
                }
                val = x;
            }
                break;
            case NC_STRING:
            {
                char *x = nullptr;
                std::string ss;
                if ((status = ::nc_inq_var_fill(nId, nVarId, &no_fill, &x)) == NC_NOERR) {
                    if (x != nullptr) {
                        ss = x;
                        ::nc_free_string(1, &x);
                    }
                }
                val = ss;
            }
                break;
            default:
                return (false);
        } // xtype
        return (true);
#else
        return (false);
#endif
    } // get_variable_fill

    std::ostream & NetCDFDataGroup::writeTo(std::ostream &os) {
        if (this->parent() != nullptr) {
            os << "{ GROUP: ";
        } else {
            os << "{ NETCDF: ";
        }
        os << this->fullname() << std::endl;
        NetCDFAttributes attrs;
        this->get_attributes(attrs);
        os << std::endl << "NetCDFAttributes";
        for (size_t i = 0; i < attrs.size(); ++i) {
            std::cout << std::endl << attrs[i];
        } // i
        NetCDFDimensions dims;
        this->get_dimensions(dims);
        os << std::endl << "NetCDFDimensions:";
        for (size_t i = 0; i < dims.size(); ++i) {
            std::cout << std::endl << dims[i];
        } // i
        NetCDFVariables vars;
        this->get_variables(vars);
        os << std::endl << "NetCDFVariables";
        for (size_t i = 0; i < vars.size(); ++i) {
            NetCDFVariable &var = vars[i];
            os << std::endl << "\t" << var;
            std::string varname = var.name;
            std::valarray<size_t> count;
            statdata::DataType xtype;
            this->get_variable_shape(varname, xtype, count);
            size_t nDims = count.size();
            std::valarray<size_t> index(nDims);
            for (size_t i = 0; i < nDims; ++i) {
                index[i] = 0;
            } // i
            Values values;
            this->read_data(varname, index, count, values);
            os << std::endl << varname << " : [";
            for (size_t i = 0; i < values.size(); ++i) {
                if (i != 0) {
                    os << ",";
                }
                if ((i != 0) && ((i % 10) == 0)) {
                    os << std::endl << "\t";
                }
                Value::to_stream(os, values[i]);
            } // i
            os << " ]" << std::endl;
            NetCDFDataGroup::NetCDFDataGroups groupes;
            this->get_chidren(groupes);
            for (size_t i = 0; i < groupes.size(); ++i) {
                NetCDFDataGroup::NetCDFDataGroupPtr g = groupes[i];
                if (g.get() != nullptr) {
                    g->writeTo(os);
                }
            } // i
        }
        if (this->parent() != nullptr) {
            os << std::endl << "}/* GROUP " << this->fullname() << " */"
                    << std::endl;
        } else {
            os << std::endl << "}/* NETCDF " << this->fullname() << " */"
                    << std::endl;
        }
        return (os);
    } // writeTo

    std::wostream & NetCDFDataGroup::writeTo(std::wostream &os) {
        if (this->parent() != nullptr) {
            os << L"{ GROUP: ";
        } else {
            os << L"{ NETCDF: ";
        }
        std::string s = this->fullname();
        if (!s.empty()) {
            std::wstring ss(s.length(), L' ');
            std::copy(s.begin(), s.end(), ss.begin());
            os << ss;
        }
        os << std::endl;
        NetCDFAttributes attrs;
        this->get_attributes(attrs);
        os << std::endl << L"NetCDFAttributes";
        for (size_t i = 0; i < attrs.size(); ++i) {
            std::cout << std::endl << attrs[i];
        } // i
        NetCDFDimensions dims;
        this->get_dimensions(dims);
        os << std::endl << L"NetCDFDimensions:";
        for (size_t i = 0; i < dims.size(); ++i) {
            std::cout << std::endl << dims[i];
        } // i
        NetCDFVariables vars;
        this->get_variables(vars);
        os << std::endl << L"NetCDFVariables";
        for (size_t i = 0; i < vars.size(); ++i) {
            NetCDFVariable &var = vars[i];
            os << std::endl << L"\t" << var;
            std::string varname = var.name;
            std::valarray<size_t> count;
            statdata::DataType xtype;
            this->get_variable_shape(varname, xtype, count);
            size_t nDims = count.size();
            std::valarray<size_t> index(nDims);
            for (size_t i = 0; i < nDims; ++i) {
                index[i] = 0;
            } // i
            Values values;
            this->read_data(varname, index, count, values);
            os << std::endl;
            s = varname;
            if (!s.empty()) {
                std::wstring ss(s.length(), L' ');
                std::copy(s.begin(), s.end(), ss.begin());
                os << ss;
            }
            os << L" : [";
            for (size_t i = 0; i < values.size(); ++i) {
                if (i != 0) {
                    os << L",";
                }
                if ((i != 0) && ((i % 10) == 0)) {
                    os << std::endl << L"\t";
                }
                Value::to_stream(os, values[i]);
            } // i
            os << L" ]" << std::endl;
            NetCDFDataGroup::NetCDFDataGroups groupes;
            this->get_chidren(groupes);
            for (size_t i = 0; i < groupes.size(); ++i) {
                NetCDFDataGroup::NetCDFDataGroupPtr g = groupes[i];
                if (g.get() != nullptr) {
                    g->writeTo(os);
                }
            } // i
        }
        std::wstring wws;
        s = this->fullname();
        if (!s.empty()) {
            std::wstring ss(s.length(), L' ');
            std::copy(s.begin(), s.end(), ss.begin());
            wws = ss;

        }
        if (this->parent() != nullptr) {
            os << std::endl << L"}/* GROUP " << wws << L" */"
                    << std::endl;
        } else {
            os << std::endl << L"}/* NETCDF " << wws << " */"
                    << std::endl;
        }
        return (os);
    } // writeTo
    //////////////////////////////////

    bool NetCDFDataGroup::get_variable_shape(const std::string &varname,
            statdata::DataType &type,
            std::valarray<size_t> &sizes) {
        type = statdata::typeOther;
        const int nId = this->id();
        int nVarId = -1000;
        int status = ::nc_inq_varid(nId, varname.c_str(), &nVarId);
        if (status != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        nc_type xtype;
        int nbAttrs = 0;
        int nDims = 0;
        int dimids[NC_MAX_VAR_DIMS];
        char buf[NC_MAX_NAME + 1];
        if ((status = ::nc_inq_var(nId, nVarId, buf, &xtype, &nDims, dimids,
                &nbAttrs)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        type = statdata::internal::NCType2DataType(xtype);
        sizes.resize(nDims);
        for (int i = 0; i < nDims; ++i) {
            size_t len = 0;
            int xid = dimids[i];
            if ((status = ::nc_inq_dim(nId, xid, buf, &len)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            sizes[i] = len;
        } // i
        return (true);
    } // get_variable_shape

    bool NetCDFDataGroup::is_netcdf4(void) {
        NetCDFDataGroup *p = this->root();
        if (p != nullptr) {
            int format = 0;
            if (::nc_inq_format(p->id(), &format) == NC_NOERR) {
                return (format == NC_FORMAT_NETCDF4);
            } // ok
        } // p
        return (false);
    } // is_netcdf4

    void NetCDFDataGroup::version(std::string &s) {
        s = ::nc_inq_libvers();
    } // version

    bool NetCDFDataGroup::get_attributes(const std::string &varname,
            NetCDFAttributes &attrs) {
        attrs.clear();
        int status = -1;
        const int nId = this->id();
        const char *px = varname.c_str();
        int nVarId = -1;
        int nbAttrs = 0;
        if (varname.empty()) {
            nVarId = NC_GLOBAL;
            if ((status = ::nc_inq_natts(nId, &nbAttrs)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        } else {
            if ((status = ::nc_inq_varid(nId, px, &nVarId)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            if ((status = ::nc_inq_varnatts(nId, nVarId, &nbAttrs)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        }
        char buf[MAX_NC_NAME + 1];
        for (int xid = 0; xid < nbAttrs; ++xid) {
            nc_type xxtype;
            size_t len = 0;
            if ((status = ::nc_inq_attname(nId, nVarId, xid, buf)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            std::string attname = (char *) (&buf[0]);
            if ((status = ::nc_inq_att(nId, nVarId, buf, &xxtype, &len)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            NetCDFAttribute oInfo;
            oInfo.id = xid;
            oInfo.var_id = nVarId;
            oInfo.name = attname;
            oInfo.len = len;
            oInfo.type = statdata::internal::NCType2DataType(xxtype);
            this->read_attribute(varname, attname, oInfo.values);
            attrs.push_back(oInfo);
        } // xid
        return (true);
    } // get_attributes

    bool NetCDFDataGroup::read_attribute(const std::string &varname,
            const std::string &attname, Values &vals) {
        vals.clear();
        int status = -1;
        const int nId = this->id();
        int nVarId = -1;
        if (varname.empty()) {
            nVarId = NC_GLOBAL;
        } else {
            if ((status = ::nc_inq_varid(nId, varname.c_str(), &nVarId)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        }
        nc_type type;
        size_t len = 0;
        const char *px = attname.c_str();
        if ((status = ::nc_inq_att(nId, nVarId, px, &type, &len)) != NC_NOERR) {
            return (false);
        }
        if (len < 1) {
            return (true);
        }
        if (type != NC_CHAR) {
            vals = Values(len);
        }
        switch (type) {
#ifdef VERSION_NETCDF4
            case NC_STRING:
            {
                PCharPtr oBuf(new PChar[len]);
                PChar *indata = oBuf.get();
                for (size_t i = 0; i < len; ++i) {
                    indata[i] = nullptr;
                } // i
                status = ::nc_get_att_string(nId, nVarId, px, indata);
                for (size_t i = 0; i < len; ++i) {
                    PChar pp = indata[i];
                    std::string s;
                    if (pp != nullptr) {
                        s = pp;
                    }
                    vals[i] = s;
                }
                status = ::nc_free_string(len, (char **) indata);
            }
                break;
#endif // NC_STRING
            case NC_DOUBLE:
            {
                DoublePtr oBuf(new double[len]);
                double *p = oBuf.get();
                if ((status = ::nc_get_att_double(nId, nVarId, px, p)) != NC_NOERR) {
                    return (false);
                }
                for (size_t i = 0; i < len; ++i) {
                    vals[i] = p[i];
                } // i
            }
                break;
            case NC_FLOAT:
            {
                FloatPtr oBuf(new float[len]);
                float *p = oBuf.get();
                if ((status = ::nc_get_att_float(nId, nVarId, px, p)) != NC_NOERR) {
                    return (false);
                }
                for (size_t i = 0; i < len; ++i) {
                    vals[i] = p[i];
                } // i
            }
                break;
            case NC_INT:
            {
                IntPtr oBuf(new int[len]);
                int *p = oBuf.get();
                if ((status = ::nc_get_att_int(nId, nVarId, px, p)) != NC_NOERR) {
                    return (false);
                }
                for (size_t i = 0; i < len; ++i) {
                    vals[i] = p[i];
                } // i
            }
                break;
            case NC_SHORT:
            {
                ShortPtr oBuf(new short[len]);
                short *p = oBuf.get();
                if ((status = ::nc_get_att_short(nId, nVarId, px, p)) != NC_NOERR) {
                    return (false);
                }
                for (size_t i = 0; i < len; ++i) {
                    vals[i] = p[i];
                } // i
            }
                break;
            case NC_BYTE:
            {
                BytePtr oBuf(new byte[len]);
                byte *p = oBuf.get();
                if ((status = ::nc_get_att_uchar(nId, nVarId, px, p)) != NC_NOERR) {
                    return (false);
                }
                for (size_t i = 0; i < len; ++i) {
                    vals[i] = p[i];
                } // i
            }
                break;
            case NC_CHAR:
            {
                CharPtr oBuf(new char[len + 1]);
                char *p = oBuf.get();
                p[len] = (char) 0;
                if ((status = ::nc_get_att_text(nId, nVarId, px, p)) != NC_NOERR) {
                    return (false);
                }
                std::string s = p;
                vals.clear();
                vals.push_back(boost::any(s));
            }
                break;
            default:
                return (false);
        } // type
        return (true);
    } // read_attributes

    bool NetCDFDataGroup::remove_attribute(const std::string &varname,
            const std::string &attname) {
        if (attname.empty()) {
            return (false);
        }
        if (!this->set_define_mode()) {
            return (false);
        }
        int status = -1;
        const int nId = this->id();
        int nVarId = -1;
        if (varname.empty()) {
            nVarId = NC_GLOBAL;
        } else {
            if ((status = ::nc_inq_varid(nId, varname.c_str(), &nVarId)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        }
        size_t len = 0;
        nc_type xtype;
        const char *px = attname.c_str();
        if ((status = ::nc_inq_att(nId, nVarId, px, &xtype, &len)) == NC_NOERR) {
            if ((status = ::nc_del_att(nId, nVarId, px)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            this->set_modified();
        }
        return (true);
    } // remove_attribute

    bool NetCDFDataGroup::has_attribute(const std::string &varname,
            const std::string &attname) {
        if (attname.empty()) {
            return false;
        }
        int status = -1;
        const int nId = this->id();
        int nVarId = -1;
        if (varname.empty()) {
            nVarId = NC_GLOBAL;
        } else {
            if ((status = ::nc_inq_varid(nId, varname.c_str(), &nVarId)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        }
        nc_type type;
        size_t len = 0;
        const char *px = attname.c_str();
        if ((status = ::nc_inq_att(nId, nVarId, px, &type, &len)) != NC_NOERR) {
            return (false);
        }
        return (true);
    } // has_attribute

    bool NetCDFDataGroup::write_attribute(const std::string &varname,
            const std::string &attname, const Values &vals) {
        size_t len = vals.size();
        if (len < 1) {
            return (false);
        }
        if (!this->set_define_mode()) {
            return (false);
        }

        auto it = std::find_if(vals.begin(), vals.end(), [](const boost::any & v)->bool {
            return (!v.empty());
        });
        if (it == vals.end()) {
            return (false);
        }
        statdata::DataType vtype = Value::get_type(*it);
        if (vtype == statdata::typeOther) {
            return (false);
        }
        nc_type xtype = statdata::internal::DataType2NCType(vtype);
        if ((vtype == statdata::typeString) && (this->is_classic())) {
            xtype = NC_CHAR;
        }
        int status = -1;
        const int nId = this->id();
        int nVarId = -1;
        if (varname.empty()) {
            nVarId = NC_GLOBAL;
        } else {
            if ((status = ::nc_inq_varid(nId, varname.c_str(), &nVarId)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        }
        const char *px = attname.c_str();
        switch (xtype) {
            case NC_DOUBLE:
            {
                DoublePtr oBuf(new double[len]);
                double *p = oBuf.get();
                for (size_t i = 0; i < len; ++i) {
                    double x = NC_FILL_DOUBLE;
                    const boost::any &v = vals[i];
                    if (!v.empty()) {
                        Value::get_value(v, x);
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_att_double(nId, nVarId, px, xtype, len, p);
            }
                break;
            case NC_FLOAT:
            {
                FloatPtr oBuf(new float[len]);
                float *p = oBuf.get();
                for (size_t i = 0; i < len; ++i) {
                    float x = NC_FILL_FLOAT;
                    const boost::any &v = vals[i];
                    if (!v.empty()) {
                        Value::get_value(v, x);
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_att_float(nId, nVarId, px, xtype, len, p);
            }
                break;
            case NC_INT:
            {
                IntPtr oBuf(new int[len]);
                int *p = oBuf.get();
                for (size_t i = 0; i < len; ++i) {
                    int x = NC_FILL_INT;
                    const boost::any &v = vals[i];
                    if (!v.empty()) {
                        Value::get_value(v, x);
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_att_int(nId, nVarId, px, xtype, len, p);
            }
                break;
            case NC_SHORT:
            {
                ShortPtr oBuf(new short[len]);
                short *p = oBuf.get();
                for (size_t i = 0; i < len; ++i) {
                    short x = NC_FILL_SHORT;
                    const boost::any &v = vals[i];
                    if (!v.empty()) {
                        Value::get_value(v, x);
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_att_short(nId, nVarId, px, xtype, len, p);
            }
                break;
            case NC_BYTE:
            {
                BytePtr oBuf(new byte[len]);
                byte *p = oBuf.get();
                for (size_t i = 0; i < len; ++i) {
                    byte x = NC_FILL_BYTE;
                    const boost::any &v = vals[i];
                    if (!v.empty()) {
                        Value::get_value(v, x);
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_att_uchar(nId, nVarId, px, xtype, len, p);
            }
                break;
            case NC_CHAR:
            {
                std::stringstream os;
                for (size_t i = 0; i < len; ++i) {
                    std::string s;
                    if (i > 0) {
                        os << std::endl;
                    }
                    const boost::any &v = vals[i];
                    if (!v.empty()) {
                        Value::get_value(v, s);
                    }
                    if (!s.empty()) {
                        os << s;
                    }
                } // i
                std::string sTotal = os.str();
                size_t xlen = sTotal.length();
                const char *pz = sTotal.c_str();
                status = ::nc_put_att_text(nId, nVarId, px, xlen, pz);
            }
                break;
#ifdef VERSION_NETCDF4
            case NC_STRING:
            {
                std::vector<std::string> oVec(len);
                PCharPtr oBuf(new PChar[len]);
                PChar *indata = oBuf.get();
                for (size_t i = 0; i < len; ++i) {
                    std::string s;
                    const boost::any &v = vals[i];
                    if (!v.empty()) {
                        Value::get_value(v, s);
                    }
                    oVec[i] = s;
                    indata[i] = (PChar) (oVec[i]).c_str();
                } // i
                status = ::nc_put_att_string(nId, nVarId, attname.c_str(), len,
                        (const char **) indata);
            }
                break;
#endif
            default:
                return (false);
        } // type
        if (status != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        this->set_modified();
        return (true);
    } // write_attribute
    /////////////////////////////////

    bool NetCDFDataGroup::read_one_data(const std::string &varname,
            std::valarray<size_t> &index, boost::any &val) {
        NetCDFVariable info;
        if (!this->get_variable(varname, info)) {
            return false;
        }
        const NetCDFDimensions & dims = info.dims;
        const size_t nDims = dims.size();
        if ((index.size() < nDims)) {
            return (false);
        }
        size_t start[MAX_NC_DIMS];
        size_t count[MAX_NC_DIMS];
        size_t nTotal = 1;
        for (size_t i = 0; i < nDims; ++i) {
            const size_t nx = dims[i].len;
            if (nx < 1) {
                return (false);
            }
            size_t nStart = index[i];
            if (nStart > nx) {
                nStart = 0;
            }
            start[i] = nStart;
            if (i == 0) {
                count[0] = 1;
            } else {
                count[i] = nx;
                nTotal = (size_t) (nTotal * nx);
            }
        } // i
        index.resize(nDims);
        for (size_t i = 0; i < nDims; ++i) {
            index[i] = start[i];
        } // i
        if (nTotal < 1) {
            return (false);
        }
        int status = -1;
        nc_type type = statdata::internal::DataType2NCType(info.type);
        const int nId = this->id();
        const int nVarId = info.id;
        if (!this->set_data_mode()) {
            return (false);
        }
        val = boost::any();
        switch (type) {
#ifdef VERSION_NETCDF4
            case NC_STRING:
            {
                char *oBuf[1];
                oBuf[0] = nullptr;
                if ((status = ::nc_get_vara_string(nId, nVarId, start, count,
                        (char **) oBuf)) != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                char *px = oBuf[0];
                if (px != nullptr) {
                    std::string s = px;
                    if (s.length() > 0) {
                        val = s;
                    }
                    status = ::nc_free_string(1, (char **) oBuf);
                } // px
            }
                break;
#endif // NC_STRING
            case NC_CHAR:
            {
                if (nTotal < 256) {
                    char oBuf[256];
                    ::memset(oBuf, 0, sizeof (oBuf));
                    if ((status = ::nc_get_vara_text(nId, nVarId, start, count, oBuf))
                            != NC_NOERR) {
                        return (false);
                    }
                    std::string s = &(oBuf[0]);
                    if (s.length() > 0) {
                        val = s;
                    }
                } else {
                    CharPtr oBuf(new char[nTotal + 1]);
                    char *p = oBuf.get();
                    ::memset(p, 0, (nTotal + 1) * sizeof (char));
                    if ((status = ::nc_get_vara_text(nId, nVarId, start, count, p))
                            != NC_NOERR) {
                        return (false);
                    }
                    std::string s = p;
                    if (s.length() > 0) {
                        val = s;
                    }
                }
            }
                break;
            case NC_DOUBLE:
            {
                double oBuf;
                double *p = &oBuf;
                if ((status = ::nc_get_vara_double(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                Values vv;
                vv.push_back(boost::any(oBuf));
                this->check_variable_data(varname, vv);
                const boost::any &vx = vv[0];
                if (!vx.empty()) {
                    val = vx;
                }
            }
                break;
            case NC_FLOAT:
            {
                float oBuf;
                float *p = &oBuf;
                if ((status = ::nc_get_vara_float(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                Values vv;
                vv.push_back(boost::any(oBuf));
                this->check_variable_data(varname, vv);
                const boost::any &vx = vv[0];
                if (!vx.empty()) {
                    val = vx;
                }
            }
                break;
            case NC_INT:
            {
                int oBuf;
                int *p = &oBuf;
                if ((status = ::nc_get_vara_int(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                Values vv;
                vv.push_back(boost::any(oBuf));
                this->check_variable_data(varname, vv);
                const boost::any &vx = vv[0];
                if (!vx.empty()) {
                    val = vx;
                }
            }
                break;
            case NC_SHORT:
            {
                short oBuf;
                short *p = &oBuf;
                if ((status = ::nc_get_vara_short(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                Values vv;
                vv.push_back(boost::any(oBuf));
                this->check_variable_data(varname, vv);
                const boost::any &vx = vv[0];
                if (!vx.empty()) {
                    val = vx;
                }
            }
                break;
            case NC_BYTE:
            {
                byte oBuf;
                byte *p = &oBuf;
                if ((status = ::nc_get_vara_uchar(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                Values vv;
                vv.push_back(boost::any(oBuf));
                this->check_variable_data(varname, vv);
                const boost::any &vx = vv[0];
                if (!vx.empty()) {
                    val = vx;
                }
            }
                break;
            default:
                return (false);
        } // type
        return (true);
    } // read_one_data

    bool NetCDFDataGroup::read_data(const std::string &varname,
            std::valarray<size_t> &index, std::valarray<size_t> &sizes,
            Values &values) {
        NetCDFVariable info;
        values.clear();
        if (!this->get_variable(varname, info)) {
            return false;
        }
        const NetCDFDimensions & dims = info.dims;
        const size_t nDims = dims.size();
        const size_t nIndexSize = index.size();
        const size_t nSizesSize = sizes.size();
        size_t start[MAX_NC_DIMS];
        size_t count[MAX_NC_DIMS];
        size_t nTotal = 1;
        for (size_t i = 0; i < nDims; ++i) {
            size_t nx = dims[i].len;
            if (nx < 1) {
                return (false);
            }
            size_t nStart = 0;
            if (i < nIndexSize) {
                nStart = index[i];
            }
            if (nStart > nx) {
                nStart = 0;
            }
            start[i] = nStart;
            size_t len = nx;
            if (i < nSizesSize) {
                len = sizes[i];
            }
            if ((len < 1) || (len > nx)) {
                len = nx;
            }
            if ((size_t) (nStart + len) > nx) {
                len = (size_t) (nx - nStart);
            }
            count[i] = len;
            nTotal = (size_t) (nTotal * len);
        } // i
        index.resize(nDims);
        sizes.resize(nDims);
        for (size_t i = 0; i < nDims; ++i) {
            index[i] = start[i];
            sizes[i] = count[i];
        } // i
        if (nTotal < 1) {
            return (false);
        }
        if (!this->set_data_mode()) {
            return (false);
        }
        int status = -1;
        const int nId = this->id();
        const int nVarId = info.id;
        nc_type type;
        if ((status == ::nc_inq_vartype(nId, nVarId, &type)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        if (type == NC_CHAR) {
            CharPtr oBuf(new char[nTotal]);
            char *p = oBuf.get();
            if ((status = ::nc_get_vara_text(nId, nVarId, start, count, p))
                    != NC_NOERR) {
                return (false);
            }
            size_t delta = dims[nDims - 1].len;
            size_t np = (size_t) (nTotal / delta);
            values = Values(np);
            for (size_t i = 0; i < np; ++i) {
                char *ps = (char *) (p + i * delta);
                std::string ss = ps;
                values[i] = ss;
            } // i
            return (true);
        } // char
        values = Values(nTotal);
        switch (type) {
#ifdef VERSION_NETCDF4
            case NC_STRING:
            {
                PCharPtr oBuf(new PChar[nTotal]);
                PChar *indata = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    indata[i] = nullptr;
                } // i
                if ((status = ::nc_get_vara_string(nId, nVarId, start, count, indata))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                for (size_t i = 0; i < nTotal; ++i) {
                    PChar pp = indata[i];
                    std::string s;
                    if (pp != nullptr) {
                        s = pp;
                    }
                    values[i] = s;
                }
                //oBuf.reset();
                status = ::nc_free_string(nTotal, (char **) indata);
                this->check_variable_data(varname, values);
            }
                break;
#endif // NC_STRING
            case NC_DOUBLE:
            {
                DoublePtr oBuf(new double[nTotal]);
                double *p = oBuf.get();
                if ((status = ::nc_get_vara_double(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                for (size_t i = 0; i < nTotal; ++i) {
                    values[i] = p[i];
                }
                this->check_variable_data(varname, values);
            }
                break;
            case NC_FLOAT:
            {
                FloatPtr oBufx(new float[nTotal]);
                float *pw = oBufx.get();
                if ((status = ::nc_get_vara_float(nId, nVarId, start, count, pw))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                for (size_t i = 0; i < nTotal; ++i) {
                    values[i] = pw[i];
                }
                this->check_variable_data(varname, values);
            }
                break;
            case NC_INT:
            {
                IntPtr oBuf(new int[nTotal]);
                int *p = oBuf.get();
                if ((status = ::nc_get_vara_int(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                for (size_t i = 0; i < nTotal; ++i) {
                    values[i] = p[i];
                }
                this->check_variable_data(varname, values);
            }
                break;
            case NC_SHORT:
            {
                ShortPtr oBuf(new short[nTotal]);
                short *p = oBuf.get();
                if ((status = ::nc_get_vara_short(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                for (size_t i = 0; i < nTotal; ++i) {
                    values[i] = p[i];
                }
                this->check_variable_data(varname, values);
            }
                break;
            case NC_BYTE:
            {
                BytePtr oBuf(new byte[nTotal]);
                byte *p = oBuf.get();
                if ((status = ::nc_get_vara_uchar(nId, nVarId, start, count, p))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                for (size_t i = 0; i < nTotal; ++i) {
                    values[i] = p[i];
                }
            }
                break;
            default:
                return (false);
        } // type
        return (true);
    } // read_data
    ///////////////////////////////////////

    bool NetCDFDataGroup::write_data(const std::string &varname,
            const std::valarray<size_t> &index, const std::valarray<size_t> &sizes,
            const Values &values) {
        const size_t nCount = values.size();
        if (nCount < 1) {
            return (false);
        }

        auto it = std::find_if(values.begin(), values.end(),
                [](const boost::any & v)->bool {
            return (!v.empty());
        });
        if (it == values.end()) {
            return (false);
        }
        statdata::DataType vtype = Value::get_type(*it);
        if (vtype == statdata::typeOther) {
            return (false);
        }
        NetCDFVariable info;
        if (!this->get_variable(varname, info)) {
            return false;
        }
        const NetCDFDimensions &dims = info.dims;
        const size_t nDims = dims.size();
        size_t nIndexSize = index.size();
        size_t nSizesSize = sizes.size();
        size_t start[MAX_NC_DIMS];
        size_t count[MAX_NC_DIMS];
        size_t nTotal = 1;
        for (size_t i = 0; i < nDims; ++i) {
            const NetCDFDimension &d = info.dims[i];
            size_t xlen = d.len;
            size_t len = d.len;
            if ((len < 1) && (!d.is_unlimited)) {
                return (false);
            }
            size_t iStart = 0;
            if (i < nIndexSize) {
                iStart = index[i];
            }
            if (!d.is_unlimited) {
                if (iStart >= xlen) {
                    iStart = 0;
                }
            }// not unlimited
            if (i < nSizesSize) {
                len = sizes[i];
            } else {
                len = xlen;
            }
            if (!d.is_unlimited) {
                if ((size_t) (iStart + len) > xlen) {
                    int nx = (int) (xlen - iStart);
                    if (nx >= 0) {
                        len = nx;
                    }
                }
            }
            if (len < 1) {
                return (false);
            }
            start[i] = iStart;
            count[i] = len;
            nTotal = (size_t) (nTotal * len);
        } // i
        if (nTotal < 1) {
            return (false);
        }
        if (!this->set_data_mode()) {
            return (false);
        }
        int status = -1;
        const int nId = this->id();
        const int nVarId = info.id;
        nc_type type;
        if ((status = ::nc_inq_vartype(nId, nVarId, &type)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        if (type == NC_CHAR) {
            CharPtr oBuf(new char[nTotal]);
            char *p = oBuf.get();
            ::memset(p, 0, nTotal * sizeof (char));
            size_t delta = count[nDims - 1];
            size_t nMax = (size_t) (delta - 1);
            const size_t nCount = values.size();
            size_t lim = 0;
            for (size_t i = 0; (i < nCount) && (lim < nTotal); ++i) {
                std::string s;
                Value::get_value(values[i], s);
                if (s.length() > nMax) {
                    s = s.substr(0, nMax);
                }
                char *ps = (char *) (p + lim);
                ::strcpy(ps, s.c_str());
                lim = (size_t) (lim + delta);
            } // i
            status = ::nc_put_vara_text(nId, nVarId, start, count, p);
            if (status != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            return (true);
        }
        boost::any fVal;
        get_variable_fill(varname, fVal);
        switch (type) {
#ifdef VERSION_NETCDF4
            case NC_STRING:
            {
                std::vector<std::string> oVec(nTotal);
                PCharPtr oBuf(new PChar[nTotal]);
                PChar *indata = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    std::string s;
                    boost::any v = values[i];
                    if (!v.empty()) {
                        Value::get_value(v, s);
                    }
                    if ((!fVal.empty()) && s.empty()) {
                        Value::get_value(fVal, s);
                    }
                    oVec[i] = s;
                    indata[i] = (PChar) (oVec[i]).c_str();
                } // i
                status = ::nc_put_vara_string(nId, nVarId, start, count,
                        (const char **) indata);
            }
                break;
#endif // NC_STRING
            case NC_DOUBLE:
            {
                DoublePtr oBuf(new double[nTotal]);
                double *p = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    double x;
                    const boost::any &vy = values[i];
                    if (!vy.empty()) {
                        Value::get_value(vy, x);
                    } else if (!fVal.empty()) {
                        Value::get_value(fVal, x);
                    } else {
                        x = std::numeric_limits<double>::quiet_NaN();
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_vara_double(nId, nVarId, start, count, p);
            }
                break;
            case NC_FLOAT:
            {
                FloatPtr oBuf(new float[nTotal]);
                float *p = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    float x;
                    const boost::any &vy = values[i];
                    if (!vy.empty()) {
                        Value::get_value(vy, x);
                    } else if (!fVal.empty()) {
                        Value::get_value(fVal, x);
                    } else {
                        x = std::numeric_limits<float>::quiet_NaN();
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_vara_float(nId, nVarId, start, count, p);
            }
                break;
            case NC_INT:
            {
                IntPtr oBuf(new int[nTotal]);
                int *p = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    int x;
                    const boost::any &vy = values[i];
                    if (!vy.empty()) {
                        Value::get_value(vy, x);
                    } else if (!fVal.empty()) {
                        Value::get_value(fVal, x);
                    } else {
                        x = std::numeric_limits<int>::min();
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_vara_int(nId, nVarId, start, count, p);
            }
                break;
            case NC_SHORT:
            {
                ShortPtr oBuf(new short[nTotal]);
                short *p = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    short x;
                    const boost::any &vy = values[i];
                    if (!vy.empty()) {
                        Value::get_value(vy, x);
                    } else if (!fVal.empty()) {
                        Value::get_value(fVal, x);
                    } else {
                        x = std::numeric_limits<short>::min();
                    }
                    p[i] = x;
                } // i
                status = ::nc_put_vara_short(nId, nVarId, start, count, p);
            }
                break;
            case NC_BYTE:
            {
                BytePtr oBuf(new byte[nTotal]);
                byte *p = oBuf.get();
                for (size_t i = 0; i < nTotal; ++i) {
                    byte x = 0;
                    Value::get_value(values[i], x);
                    p[i] = x;
                } // i
                status = ::nc_put_vara_uchar(nId, nVarId, start, count, p);
            }
                break;
            default:
                return false;
        } // type
        if (status != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        this->set_modified();
        return (true);
    } // write_data
    //////////////////////////////////////

    bool NetCDFDataGroup::rename_variable(const std::string &oldname,
            const std::string &newname) {
        const int nId = this->id();
        int nVarId = 0;
        int status = ::nc_inq_varid(nId, oldname.c_str(), &nVarId);
        if (status != NC_NOERR) {
            return (false);
        }
        if (!this->set_define_mode()) {
            return (false);
        }
        status = ::nc_rename_var(nId, nVarId, newname.c_str());
        if (status == NC_NOERR) {
            this->set_modified();
            return (true);
        }
        return (false);
    } // rename_variable

    bool NetCDFDataGroup::set_metadata(const std::string &varname, int nFlags,
            NetCDFVariable &oInfo) {
        int nVarId = -1;
        const int nId = this->id();
        int status = -1;
        if ((status = ::nc_inq_varid(nId, varname.c_str(), &nVarId)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        nc_type vartype;
        if ((status = ::nc_inq_vartype(nId, nVarId, &vartype)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        if (!this->set_define_mode()) {
            return false;
        }
        nc_type xtype;
        size_t len = 0;
        if ((nFlags & (int) flag_c_format) != 0) {
            if ((vartype == NC_FLOAT) || (vartype == NC_DOUBLE)) {
                const std::string & s = oInfo.c_format;
                status = ::nc_inq_att(nId, nVarId, ATT_C_FORMAT, &xtype, &len);
                if ((status == NC_NOERR) && s.empty()) {
                    if ((status = ::nc_del_att(nId, nVarId, ATT_C_FORMAT))
                            != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                    this->set_modified();
                } else if ((status != NC_NOERR) && (!s.empty())) {
                    size_t xlen = s.length();
                    const char *px = s.c_str();
                    if ((status = ::nc_put_att_text(nId, nVarId, ATT_C_FORMAT, xlen,
                            px)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                    this->set_modified();
                }
            } // type
        } //c_format
        if ((nFlags & (int) flag_long_name) != 0) {
            std::string & s = oInfo.long_name;
            status = ::nc_inq_att(nId, nVarId, ATT_LONG_NAME, &xtype, &len);
            if ((status == NC_NOERR) && s.empty()) {
                if ((status = ::nc_del_att(nId, nVarId, ATT_LONG_NAME)) != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                this->set_modified();
            } else if ((status != NC_NOERR) && (!s.empty())) {
                size_t xlen = s.length();
                const char *px = s.c_str();
                if ((status = ::nc_put_att_text(nId, nVarId, ATT_LONG_NAME, xlen,
                        px)) != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                this->set_modified();
            }
        } // long_name
        if ((nFlags & (int) flag_title) != 0) {
            std::string & s = oInfo.title;
            status = ::nc_inq_att(nId, nVarId, ATT_TITLE, &xtype, &len);
            if ((status == NC_NOERR) && s.empty()) {
                if ((status = ::nc_del_att(nId, nVarId, ATT_TITLE)) != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                this->set_modified();
            } else if ((status != NC_NOERR) && (!s.empty())) {
                size_t xlen = s.length();
                const char *px = s.c_str();
                if ((status = ::nc_put_att_text(nId, nVarId, ATT_TITLE, xlen, px))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                this->set_modified();
            }
        } // title
        if ((nFlags & (int) flag_units) != 0) {
            std::string & s = oInfo.units;
            status = ::nc_inq_att(nId, nVarId, ATT_UNITS, &xtype, &len);
            if ((status == NC_NOERR) && s.empty()) {
                if ((status = ::nc_del_att(nId, nVarId, ATT_UNITS)) != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                this->set_modified();
            } else if ((status != NC_NOERR) && (!s.empty())) {
                size_t xlen = s.length();
                const char *px = s.c_str();
                if ((status = ::nc_put_att_text(nId, nVarId, ATT_UNITS, xlen, px))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                this->set_modified();
            }
        } // units
        if ((nFlags & (int) flag_missing_value) != 0) {
            const boost::any &v = oInfo.missing_value;
            status = ::nc_inq_att(nId, nVarId, ATT_MISSING_VALUE, &xtype, &len);
            if ((status == NC_NOERR) && v.empty()) {
                if ((status = ::nc_del_att(nId, nVarId, ATT_MISSING_VALUE))
                        != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                this->set_modified();
            } else if ((status != NC_NOERR) && (!v.empty())) {
                switch (vartype) {
                    case NC_BYTE:
                    {
                        byte val;
                        if (Value::get_value(v, val)) {
                            if ((status = ::nc_put_att_uchar(nId, nVarId,
                                    ATT_MISSING_VALUE, vartype, 1, &val)) != NC_NOERR) {
                                this->handle_error(status);
                                return (false);
                            }
                            this->m_modified = true;
                        } else {
                            return (false);
                        }
                    }
                        break;
                    case NC_SHORT:
                    {
                        short val;
                        if (Value::get_value(v, val)) {
                            if ((status = ::nc_put_att_short(nId, nVarId,
                                    ATT_MISSING_VALUE, vartype, 1, &val)) != NC_NOERR) {
                                this->handle_error(status);
                                return (false);
                            }
                            this->m_modified = true;
                        } else {
                            return (false);
                        }
                    }
                        break;
                    case NC_INT:
                    {
                        int val;
                        if (Value::get_value(v, val)) {
                            if ((status = ::nc_put_att_int(nId, nVarId,
                                    ATT_MISSING_VALUE, vartype, 1, &val)) != NC_NOERR) {
                                this->handle_error(status);
                                return (false);
                            }
                            this->m_modified = true;
                        } else {
                            return (false);
                        }
                    }
                        break;
                    case NC_FLOAT:
                    {
                        float val;
                        if (Value::get_value(v, val)) {
                            if ((status = ::nc_put_att_float(nId, nVarId,
                                    ATT_MISSING_VALUE, vartype, 1, &val)) != NC_NOERR) {
                                this->handle_error(status);
                                return (false);
                            }
                            this->m_modified = true;
                        } else {
                            return (false);
                        }
                    }
                        break;
                    case NC_DOUBLE:
                    {
                        double val;
                        if (Value::get_value(v, val)) {
                            if ((status = ::nc_put_att_double(nId, nVarId,
                                    ATT_MISSING_VALUE, vartype, 1, &val)) != NC_NOERR) {
                                this->handle_error(status);
                                return (false);
                            }
                            this->m_modified = true;
                        } else {
                            return (false);
                        }
                    }
                        break;
                    default:
                        break;
                } // vartyoe
            }
        } // missing_value
        return (true);
    } // set_metadata

    bool NetCDFDataGroup::create_variable(NetCDFVariable &oInfo) {
        NetCDFVariable info;
        std::string varname = oInfo.name;
        if (this->get_variable(varname, info)) {
            oInfo = info;
            return (true);
        }
        if (!this->set_define_mode()) {
            return (false);
        }
        const int nId = this->id();
        nc_type type = statdata::internal::DataType2NCType(oInfo.type);
        if ((oInfo.type == statdata::typeString) && this->is_classic()) {
            type = NC_CHAR;
        } // statdata::typeString
        NetCDFDimensions &dims = oInfo.dims;
        int nDims = (int) dims.size();
        if ((nDims < 1) || (nDims > NC_MAX_VAR_DIMS)) {
            return (false);
        }
        if (type == NC_CHAR) {
            if ((size_t) (nDims + 1) > (size_t) NC_MAX_VAR_DIMS) {
                return (false);
            }
            std::string dimname(DIM_STRINGAXIS);
            NetCDFDimension oCharDim;
            if (!this->get_string_axis(oCharDim)) {
                return false;
            }
            dims.push_back(oCharDim);
            nDims = (int) dims.size();
        }
        int dimids[NC_MAX_VAR_DIMS];
        for (int i = 0; i < nDims; ++i) {
            NetCDFDimension &f = dims[i];
            if (f.is_valid()) {
                dimids[i] = f.id;
            } else {
                std::string dimname = f.name;
                size_t len = f.len;
                if (!this->create_dimension(f, dimname, len)) {
                    return (false);
                }
                dimids[i] = f.id;
            }
        } // i
        int status = -1;
        int nVarId = -1;
        if ((status = ::nc_def_var(nId, varname.c_str(), type, nDims, dimids,
                &nVarId)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
#ifdef VERSION_NETCDF4
        if (type == NC_BYTE) {
            int no_fill = 1;
            if ((status = ::nc_def_var_fill(nId, nVarId, no_fill, NULL)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        }
#endif // 4
        if (!oInfo.long_name.empty()) {
            const char *px = oInfo.long_name.c_str();
            if ((status = ::nc_put_att_text(nId, nVarId, ATT_LONG_NAME,
                    ::strlen(px), px)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        }
        if (!oInfo.title.empty()) {
            const char *px = oInfo.title.c_str();
            if ((status = ::nc_put_att_text(nId, nVarId, ATT_TITLE, ::strlen(px),
                    px)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
        }
        switch (type) {
            case NC_DOUBLE:
            {
                if (!oInfo.missing_value.empty()) {
                    double val = std::numeric_limits<double>::quiet_NaN();
                    Value::get_value(oInfo.missing_value, val);
                    if ((status = ::nc_put_att_double(nId, nVarId, ATT_MISSING_VALUE,
                            type, 1, &val)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                }
                if (!oInfo.valid_min.empty()) {
                    double val;
                    if (Value::get_value(oInfo.valid_min, val)) {
                        if ((status = ::nc_put_att_double(nId, nVarId, ATT_VALID_MIN,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.valid_max.empty()) {
                    double val;
                    if (Value::get_value(oInfo.valid_max, val)) {
                        if ((status = ::nc_put_att_double(nId, nVarId, ATT_VALID_MAX,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.units.empty()) {
                    const char *px = oInfo.units.c_str();
                    if ((status = ::nc_put_att_text(nId, nVarId, ATT_UNITS,
                            ::strlen(px), px)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                } // units
                if (!oInfo.c_format.empty()) {
                    const char *px = oInfo.c_format.c_str();
                    if ((status = ::nc_put_att_text(nId, nVarId, ATT_C_FORMAT,
                            ::strlen(px), px)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                } // units

            }
                break;
            case NC_FLOAT:
            {
                if (!oInfo.missing_value.empty()) {
                    float val = std::numeric_limits<float>::quiet_NaN();
                    Value::get_value(oInfo.missing_value, val);
                    if ((status = ::nc_put_att_float(nId, nVarId, ATT_MISSING_VALUE,
                            type, 1, &val)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                }
                if (!oInfo.valid_min.empty()) {
                    float val;
                    if (Value::get_value(oInfo.valid_min, val)) {
                        if ((status = ::nc_put_att_float(nId, nVarId, ATT_VALID_MIN,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.valid_max.empty()) {
                    float val;
                    if (Value::get_value(oInfo.valid_max, val)) {
                        if ((status = ::nc_put_att_float(nId, nVarId, ATT_VALID_MAX,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.units.empty()) {
                    const char *px = oInfo.units.c_str();
                    if ((status = ::nc_put_att_text(nId, nVarId, ATT_UNITS,
                            ::strlen(px), px)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                } // units
                if (!oInfo.c_format.empty()) {
                    const char *px = oInfo.c_format.c_str();
                    if ((status = ::nc_put_att_text(nId, nVarId, ATT_C_FORMAT,
                            ::strlen(px), px)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                } // units

            }
                break;
            case NC_INT:
            {
                if (!oInfo.missing_value.empty()) {
                    int val = std::numeric_limits<int>::min();
                    if (Value::get_value(oInfo.missing_value, val)) {
                        if ((status = ::nc_put_att_int(nId, nVarId, ATT_MISSING_VALUE,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    }
                }
                if (!oInfo.valid_min.empty()) {
                    int val;
                    if (Value::get_value(oInfo.valid_min, val)) {
                        if ((status = ::nc_put_att_int(nId, nVarId, ATT_VALID_MIN, type,
                                1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.valid_max.empty()) {
                    int val;
                    if (Value::get_value(oInfo.valid_max, val)) {
                        if ((status = ::nc_put_att_int(nId, nVarId, ATT_VALID_MAX, type,
                                1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.units.empty()) {
                    const char *px = oInfo.units.c_str();
                    if ((status = ::nc_put_att_text(nId, nVarId, ATT_UNITS,
                            ::strlen(px), px)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                } // units

            }
                break;
            case NC_SHORT:
            {
                if (!oInfo.missing_value.empty()) {
                    short val = std::numeric_limits<short>::min();
                    if (Value::get_value(oInfo.missing_value, val)) {
                        if ((status = ::nc_put_att_short(nId, nVarId, ATT_MISSING_VALUE,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    }
                }
                if (!oInfo.valid_min.empty()) {
                    short val;
                    if (Value::get_value(oInfo.valid_min, val)) {
                        if ((status = ::nc_put_att_short(nId, nVarId, ATT_VALID_MIN,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.valid_max.empty()) {
                    short val;
                    if (Value::get_value(oInfo.valid_max, val)) {
                        if ((status = ::nc_put_att_short(nId, nVarId, ATT_VALID_MAX,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.units.empty()) {
                    const char *px = oInfo.units.c_str();
                    if ((status = ::nc_put_att_text(nId, nVarId, ATT_UNITS,
                            ::strlen(px), px)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                } // units

            }
                break;
            case NC_BYTE:
            {
                if (!oInfo.missing_value.empty()) {
                    byte val = std::numeric_limits<byte>::min();
                    if (Value::get_value(oInfo.missing_value, val)) {
                        if ((status = ::nc_put_att_uchar(nId, nVarId, ATT_MISSING_VALUE,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    }
                }
                if (!oInfo.valid_min.empty()) {
                    byte val;
                    if (Value::get_value(oInfo.valid_min, val)) {
                        if ((status = ::nc_put_att_uchar(nId, nVarId, ATT_VALID_MIN,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.valid_max.empty()) {
                    byte val;
                    if (Value::get_value(oInfo.valid_max, val)) {
                        if ((status = ::nc_put_att_uchar(nId, nVarId, ATT_VALID_MAX,
                                type, 1, &val)) != NC_NOERR) {
                            this->handle_error(status);
                            return (false);
                        }
                    } // val
                } // valid_min
                if (!oInfo.units.empty()) {
                    const char *px = oInfo.units.c_str();
                    if ((status = ::nc_put_att_text(nId, nVarId, ATT_UNITS,
                            ::strlen(px), px)) != NC_NOERR) {
                        this->handle_error(status);
                        return (false);
                    }
                } // units
            }
                break;
            default:
                break;
        } // type
        this->set_modified();
        return (this->get_variable(varname, oInfo));
    } // create_variable

    bool NetCDFDataGroup::get_variables(NetCDFVariables &vars) {
        vars.clear();
        const int nId = this->id();
        int nVars = 0;
        int status = ::nc_inq_nvars(nId, &nVars);
        if (status != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        char buf[NC_MAX_NAME + 1];
        for (int nVarId = 0; nVarId < nVars; ++nVarId) {
            if ((status = ::nc_inq_varname(nId, nVarId, buf)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            std::string varname = buf;
            NetCDFVariable v;
            if (!this->get_variable(varname, v)) {
                return (false);
            }
            vars.push_back(v);
        } // nVarId
        return (true);
    } // get_variables

    bool NetCDFDataGroup::get_variable(const std::string &varname, NetCDFVariable &oInfo) {
        const int nId = this->id();
        int status = -1000;
        int nVarId = -1;
        oInfo.clean();
        if ((status = ::nc_inq_varid(nId, varname.c_str(), &nVarId)) != NC_NOERR) {
            return (false);
        }
        nc_type xtype;
        int nbAttrs = 0;
        int nDims = 0;
        int dimids[NC_MAX_VAR_DIMS];
        char buf[NC_MAX_NAME + 1];
        if ((status = ::nc_inq_var(nId, nVarId, buf, &xtype, &nDims, dimids,
                &nbAttrs)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        oInfo.id = nVarId;
        oInfo.name = buf;
        oInfo.type = statdata::internal::NCType2DataType(xtype);
        NetCDFDimensions &oVec = oInfo.dims;
        NetCDFDimensions oUnlimiteds;
        this->get_unlimited_dimensions(oUnlimiteds);
        const size_t m = oUnlimiteds.size();
        for (int i = 0; i < nDims; ++i) {
            size_t len = 0;
            int xid = dimids[i];
            if ((status = ::nc_inq_dim(nId, xid, buf, &len)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            NetCDFDimension info;
            info.id = xid;
            info.len = len;
            info.name = buf;
            for (size_t j = 0; j < m; ++j) {
                if (oUnlimiteds[j].id == xid) {
                    info.is_unlimited = true;
                    break;
                }
            }
            oVec.push_back(info);
        } // i
        NetCDFAttributes &vv = oInfo.attrs;
        for (int i = 0; i < nbAttrs; ++i) {
            nc_type xxtype;
            size_t len = 0;
            if ((status = ::nc_inq_attname(nId, nVarId, i, buf)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            if ((status = ::nc_inq_att(nId, nVarId, buf, &xxtype, &len)) != NC_NOERR) {
                this->handle_error(status);
                return (false);
            }
            NetCDFAttribute f;
            std::string attname = buf;
            if (this->read_attribute(varname, attname, f.values)) {
                f.id = i;
                f.name = attname;
                f.len = len;
                f.type = statdata::internal::NCType2DataType(xxtype);
                f.var_id = nVarId;
                vv.push_back(f);
            }
        } // i
        return (true);
    } // get_variable

    bool NetCDFDataGroup::rename_dimension(const std::string &sOldName,
            const std::string &sNewName) {
        const int nId = this->id();
        int nDimId = 0;
        int status = ::nc_inq_dimid(nId, sOldName.c_str(), &nDimId);
        if (status != NC_NOERR) {
            return (false);
        }
        if (!this->set_define_mode()) {
            return (false);
        }
        status = ::nc_rename_dim(nId, nDimId, sNewName.c_str());
        if (status == NC_NOERR) {
            this->set_modified();
            return (true);
        }
        return (false);
    } // rename_dimension

    bool NetCDFDataGroup::create_dimension(NetCDFDimension &oInfo, const std::string &sname,
            size_t nLen /*= 0 */) {
        if (this->get_dimension(sname, oInfo)) {
            return (true);
        }
        if (!this->set_define_mode()) {
            return (false);
        }
        oInfo.clean();
        const int nId = this->id();
        int xstatus = -1;
        int xid = -1;
        if (nLen < 1) {
            nLen = NC_UNLIMITED;
        }
        if ((xstatus = ::nc_def_dim(nId, sname.c_str(), nLen, &xid)) != NC_NOERR) {
            this->handle_error(xstatus);
            return (false);
        }
        oInfo.id = xid;
        if (nLen == NC_UNLIMITED) {
            oInfo.len = 0;
            oInfo.is_unlimited = true;
        } else {
            oInfo.len = nLen;
        }
        oInfo.name = sname;
        this->set_modified();
        return (true);
    } // create_dimension

    bool NetCDFDataGroup::get_dimension(const std::string &xname, NetCDFDimension &oInfo) {
        oInfo.clean();
        const int nId = this->id();
        int xstatus = -1;
        int xid = -1;
        if ((xstatus = ::nc_inq_dimid(nId, xname.c_str(), &xid)) == NC_NOERR) {
            size_t len = 0;
            if ((xstatus = ::nc_inq_dimlen(nId, xid, &len)) != NC_NOERR) {
                this->handle_error(xstatus);
                return (false);
            }
            oInfo.id = xid;
            oInfo.len = len;
            oInfo.name = xname;
            NetCDFDimensions oUnlimiteds;
            this->get_unlimited_dimensions(oUnlimiteds);
            const size_t m = oUnlimiteds.size();
            for (size_t i = 0; i < m; ++i) {
                if (oUnlimiteds[i].id == xid) {
                    oInfo.is_unlimited = true;
                    break;
                }
            }
            return (true);
        }
        return (false);
    } // get_dimension

    bool NetCDFDataGroup::get_dimensions(NetCDFDimensions &oVec) {
        oVec.clear();
        const int nId = this->id();
        int xstatus = 0;
        int xid = -1;
        NetCDFDimensions oUnlimiteds;
        this->get_unlimited_dimensions(oUnlimiteds);
        int nDims = 0;
        int dimids[NC_MAX_DIMS];
#ifdef VERSION_NETCDF4
        if ((xstatus = ::nc_inq_dimids(nId, &nDims, dimids, 1)) != NC_NOERR) {
            this->handle_error(xstatus);
            return (false);
        }
#else
        if ((xstatus = ::nc_inq_ndims(nId, &nDims)) != NC_NOERR) {
            this->handle_error(xstatus);
            return (false);
        }
        for (int i = 0; i < nDims; ++i) {
            dimids[i] = i;
        }
#endif // VERSION_NETCDF4
        char buf[MAX_NC_NAME + 1];
        size_t len = 0;
        ::memset(buf, 0, sizeof (buf));
        size_t m = oUnlimiteds.size();
        for (int i = 0; i < nDims; ++i) {
            xid = dimids[i];
            if ((xstatus = ::nc_inq_dim(nId, xid, buf, &len)) != NC_NOERR) {
                this->handle_error(xstatus);
                return (false);
            }
            NetCDFDimension oInfo;
            oInfo.id = xid;
            oInfo.len = len;
            oInfo.name = buf;
            for (size_t j = 0; j < m; ++j) {
                if (xid == oUnlimiteds[j].id) {
                    oInfo.is_unlimited = true;
                    break;
                }
            } // j
            oVec.push_back(oInfo);
        } // i
        return (true);
    } // get_dimensions
#ifdef VERSION_NETCDF4

    bool NetCDFDataGroup::get_unlimited_dimensions(NetCDFDimensions &oVec) {
        oVec.clear();
        const int nId = this->id();
        int xstatus = 0;
        if (this->is_classic()) {
            int nUnlimited = -1;
            if ((xstatus = ::nc_inq_unlimdim(nId, &nUnlimited)) != NC_NOERR) {
                nUnlimited = -1000;
            }
            if (nUnlimited >= 0) {
                char buf[MAX_NC_NAME + 1];
                size_t len = 0;
                ::memset(buf, 0, sizeof (buf));
                if ((xstatus = ::nc_inq_dim(nId, nUnlimited, buf, &len)) != NC_NOERR) {
                    this->handle_error(xstatus);
                    return (false);
                }
                NetCDFDimension oInfo;
                oInfo.id = nUnlimited;
                oInfo.len = len;
                oInfo.name = buf;
                oInfo.is_unlimited = true;
                oVec.push_back(oInfo);
            }
            return (true);
        } else {
            int nbDims = 0;
            int dimids[NC_MAX_DIMS];
            if ((xstatus = ::nc_inq_unlimdims(nId, &nbDims, dimids)) != NC_NOERR) {
                this->handle_error(xstatus);
                return (false);
            }
            char buf[MAX_NC_NAME + 1];
            for (int i = 0; i < nbDims; ++i) {
                size_t len = 0;
                int xid = dimids[i];
                if ((xstatus = ::nc_inq_dim(nId, xid, buf, &len)) != NC_NOERR) {
                    this->handle_error(xstatus);
                    return (false);
                }
                NetCDFDimension oInfo;
                oInfo.id = xid;
                oInfo.len = len;
                oInfo.name = buf;
                oInfo.is_unlimited = true;
                oVec.push_back(oInfo);
            } // i
            return (true);
        }
    } // get_unlimited_dimensions
#else

    bool NetCDFDataGroup::get_unlimited_dimensions(NetCDFDimensions &oVec) {
        oVec.clear();
        const int nId = this->id();
        int xstatus = 0;
        int nUnlimited = -1;
        if ((xstatus = ::nc_inq_unlimdim(nId, &nUnlimited)) != NC_NOERR) {
            nUnlimited = -1000;
        }
        if (nUnlimited >= 0) {
            char buf[MAX_NC_NAME + 1];
            size_t len = 0;
            ::memset(buf, 0, sizeof (buf));
            if ((xstatus = ::nc_inq_dim(nId, nUnlimited, buf, &len)) != NC_NOERR) {
                this->handle_error(xstatus);
                return (false);
            }
            NetCDFDimension oInfo;
            oInfo.id = nUnlimited;
            oInfo.len = len;
            oInfo.name = buf;
            oInfo.is_unlimited = true;
            oVec.push_back(oInfo);
        }
        return (true);
    } //get_unlimited_dimensions
#endif

    bool NetCDFDataGroup::create_child(const std::string &sname, NetCDFDataGroup &oChild) {
#ifndef VERSION_NETCDF4
        return (false);
#else
        if (this->get_child(sname, oChild)) {
            return (true);
        }
        oChild.m_bdefinemode = false;
        oChild.m_mode = mode_notopen;
        oChild.m_modified = false;
        oChild.m_history = false;
        oChild.m_id = -1000;
        oChild.m_laststatus = -1000;
        oChild.m_parent = this;
        oChild.m_name.clear();
        oChild.m_lasterror.clear();
        oChild.m_fullname.clear();
        return (oChild.open_groupe(sname));
#endif         
    } // create_child

    bool NetCDFDataGroup::get_child(const std::string &sname, NetCDFDataGroup &oChild) {
#ifndef VERSION_NETCDF4
        return (false);
#else
        oChild.m_bdefinemode = false;
        oChild.m_mode = mode_notopen;
        oChild.m_modified = false;
        oChild.m_history = false;
        oChild.m_id = -1000;
        oChild.m_laststatus = -1000;
        oChild.m_parent = this;
        oChild.m_name.clear();
        oChild.m_lasterror.clear();
        oChild.m_fullname.clear();
        int status = ::nc_inq_grp_ncid(this->m_id, sname.c_str(), &oChild.m_id);
        if (status != NC_NOERR) {
            return false;
        }
        const int nId = oChild.m_id;
        size_t len = 0;
        if ((status = ::nc_inq_grpname_full(nId, &len, NULL)) != NC_NOERR) {
            this->handle_error(status);
            return false;
        }
        CharPtr oBuf(new char[len + 1]);
        char *p = oBuf.get();
        if ((status = ::nc_inq_grpname_full(nId, &len, p)) != NC_NOERR) {
            this->handle_error(status);
            return false;
        }
        oChild.m_fullname = p;
        char buf[NC_MAX_NAME + 1];
        if ((status = ::nc_inq_grpname(nId, buf)) != NC_NOERR) {
            this->handle_error(status);
            return false;
        }
        oChild.m_name = buf;
        oChild.m_mode = this->m_mode;
        oChild.m_laststatus = NC_NOERR;
        return (true);
#endif         
    } // get_child

    bool NetCDFDataGroup::get_chidren(NetCDFDataGroups &children) {
        children.clear();
#ifdef VERSION_NETCDF4
        int n = 0;
        const int nId = this->id();
        int status = ::nc_inq_grps(nId, &n, NULL);
        if (status != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        if (n < 1) {
            return (true);
        }
        IntPtr oBuf(new int[n]);
        int *p = oBuf.get();
        if ((status = ::nc_inq_grps(nId, &n, p)) != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        for (int i = 0; i < n; ++i) {
            int xid = p[i];
            char buf[NC_MAX_NAME + 1];
            char *px = &buf[0];
            ::memset(buf, 0, sizeof (buf));
            if ((status = ::nc_inq_grpname(xid, px)) == NC_NOERR) {
                std::string filename = buf;
                NetCDFDataGroupPtr o(new NetCDFDataGroup(this));
                NetCDFDataGroup *pp = o.get();
                if (pp->open(filename, this->m_mode)) {
                    children.push_back(o);
                }

            }
        } // i
#endif // VERSION_NETCDF4
        return (true);
    } // get_children

    bool NetCDFDataGroup::close(void) {
        if (this->m_mode == mode_notopen) {
            return (true);
        }
        if (this->m_mode != mode_read) {
            if (this->is_modified()) {
                if (!this->is_history_written()) {
                    std::string s;
                    this->write_history(s, s);
                }
            }
        }
        if (this->m_parent != nullptr) {
            this->m_bdefinemode = false;
            this->m_mode = mode_notopen;
            this->m_modified = false;
            this->m_history = false;
            this->m_id = -1000;
            this->m_laststatus = -1000;
            this->m_lasterror.clear();
            return (true);
        }
        int status = ::nc_close(this->m_id);
        if (status != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        this->m_bdefinemode = false;
        this->m_mode = mode_notopen;
        this->m_modified = false;
        this->m_history = false;
        this->m_id = -1000;
        this->m_laststatus = -1000;
        this->m_lasterror.clear();
        return (true);
    } // close

    bool NetCDFDataGroup::open_groupe(const std::string &fname) {
#ifdef VERSION_NETCDF4
        if (this->m_parent == nullptr) {
            return (false);
        }
        if (this->m_parent->m_mode == mode_notopen) {
            return (false);
        }
        this->m_mode = this->m_parent->m_mode;
        const int nParentId = this->m_parent->id();
        const char *px = fname.c_str();
        int status = ::nc_inq_grp_ncid(nParentId, px, &(this->m_id));
        if (status != NC_NOERR) {
            if (this->m_parent->m_mode == mode_read) {
                return (false);
            } else {
                status = ::nc_def_grp(nParentId, px, &(this->m_id));
                if (status != NC_NOERR) {
                    this->handle_error(status);
                    return false;
                }
                this->set_modified();
            }
        }
        size_t len = 0;
        const int nId = this->m_id;
        if ((status = ::nc_inq_grpname_full(nId, &len, NULL)) != NC_NOERR) {
            this->handle_error(status);
            return false;
        }
        CharPtr oBuf(new char[len + 1]);
        char *p = oBuf.get();
        if ((status = ::nc_inq_grpname_full(nId, &len, p)) != NC_NOERR) {
            this->handle_error(status);
            return false;
        }
        this->m_fullname = p;
        char buf[NC_MAX_NAME + 1];
        if ((status = ::nc_inq_grpname(nId, buf)) != NC_NOERR) {
            this->handle_error(status);
            return false;
        }
        this->m_name = buf;
        this->m_mode = this->m_parent->m_mode;
        this->m_bdefinemode = this->m_parent->m_bdefinemode;
        this->m_lasterror.clear();
        this->m_laststatus = NC_NOERR;
        return (true);
#else
        return (false);
#endif
    } // open_groupe

    bool NetCDFDataGroup::open_file(const std::string &filename,
            statdata::datafile_open_mode mode,
            bool bClassic) {
        this->m_bclassic = bClassic;
        int status = -1;
        const char *px = filename.c_str();
        if (mode == mode_read) {
            status = ::nc_open(px, NC_NOWRITE, &(this->m_id));
        } else {
            status = ::nc_open(px, NC_WRITE | NC_SHARE, &(this->m_id));
            if (status != NC_NOERR) {
                if (mode != mode_replace) {
                    return (false);
                }
#ifndef VERSION_NETCDF4
                status = ::nc_create(px,
                        NC_CLOBBER | NC_64BIT_OFFSET | NC_SHARE, &(this->m_id));
#else
                if (!bClassic) {
                    status = ::nc_create(px, NC_CLOBBER | NC_NETCDF4 | NC_SHARE,
                            &(this->m_id));
                } else {
                    status = ::nc_create(px,
                            NC_CLOBBER | NC_64BIT_OFFSET | NC_SHARE, &(this->m_id));
                }
#endif                
                if (status != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                status = ::nc_close(this->m_id);
                if (status != NC_NOERR) {
                    this->handle_error(status);
                    return (false);
                }
                status = ::nc_open(px, NC_WRITE | NC_SHARE, &(this->m_id));
            }
        }
        if (mode != mode_read) {
            int oldmode = 0;
            int curmode = NC_FILL;
            status = ::nc_set_fill(this->m_id, curmode, &oldmode);
        }
        if (status != NC_NOERR) {
            this->handle_error(status);
            return (false);
        }
        this->m_name = filename;
        this->m_fullname = filename;
        this->m_mode = mode;
        this->m_modified = false;
        this->m_history = false;
        if (mode != mode_read) {
            this->m_bdefinemode = true;
        }
        if (!this->m_bclassic) {
            std::string ver;
            NetCDFDataGroup::version(ver);
            size_t n = ver.length();
            if (n > 0) {
                if (ver.at(0) == '3') {
                    this->m_bclassic = true;
                }
            }
        }
        if (!this->m_bclassic) {
            int format = 0;
            if (::nc_inq_format(this->m_id, &format) == NC_NOERR) {
                switch (format) {
                    case NC_FORMAT_CLASSIC:
                    case NC_FORMAT_64BIT:
                    case NC_FORMAT_NETCDF4_CLASSIC:
                        this->m_bclassic = true;
                        break;
                    default:
                        break;
                } // format
            } // ok
        }
        return (true);
    } // open_file

    bool NetCDFDataGroup::open(const std::string &filename,
            statdata::datafile_open_mode mode,
            bool bClassic) {
        if (this->m_parent != nullptr) {
            return (this->open_groupe(filename));
        } else {
            return (this->open_file(filename, mode, bClassic));
        }
    } // open

    NetCDFDataGroup::NetCDFDataGroup(NetCDFDataGroup * pParent) :
    m_bclassic(true), m_bdefinemode(false), m_mode(statdata::mode_notopen), m_modified(
    false), m_history(false), m_id(-1000), m_laststatus(-1000), m_parent(
    pParent) {
    } // NetCDFDataGroup

    NetCDFDataGroup::NetCDFDataGroup(const std::string &filename,
            datafile_open_mode mode /*= mode_read*/, bool bClassic /*= true */) :
    m_bclassic(true), m_bdefinemode(false), m_mode(statdata::mode_notopen), m_modified(
    false), m_history(false), m_id(-1000), m_laststatus(-1000),
    m_parent(nullptr) {
        this->open(filename, mode, bClassic);
    }// NetCDFDataGroup

    NetCDFDataGroup::~NetCDFDataGroup() {
        this->close();
    }

    bool NetCDFDataGroup::has_error(void) const {
        return (this->m_laststatus == NC_NOERR);
    } // has_error

    void NetCDFDataGroup::handle_error(int status) {
        this->m_lasterror.clear();
        this->m_laststatus = NC_NOERR;
        if (status != NC_NOERR) {
            this->m_laststatus = status;
            this->m_lasterror = ::nc_strerror(status);
        }
    } // handle_error

    bool NetCDFDataGroup::set_define_mode(void) {
        if ((this->m_mode == statdata::mode_notopen) ||
                (this->m_mode == statdata::mode_read)) {
            return (false);
        }
        int status = ::nc_redef(this->m_id);
        if (status != NC_NOERR) {
            if (status != NC_EINDEFINE) {
                this->handle_error(status);
                return (false);
            }
        }
        this->m_bdefinemode = true;
        return (true);
    } // set_define_mode

    bool NetCDFDataGroup::set_data_mode(void) {
        if (this->m_mode == statdata::mode_notopen) {
            return false;
        }
        int status = ::nc_enddef(this->m_id);
        if (status != NC_NOERR) {
            if (status != NC_ENOTINDEFINE) {
                this->handle_error(status);
                return (false);
            }
        }
        this->m_bdefinemode = false;
        return (true);
    } // set_data_mode

    bool NetCDFDataGroup::write_history(const std::string &app, const std::string &user) {
        if (this->m_history) {
            return (true);
        }
        nc_type type;
        std::string sOld;
        size_t nLen = 0;
        const int nId = this->id();
        int status = ::nc_inq_att(nId, NC_GLOBAL, ATT_HISTORY, &type, &nLen);
        if ((status == NC_NOERR) && (nLen > 0)) {
            CharPtr buf(new char[nLen + 1]);
            char *pBuf = buf.get();
            assert(pBuf != nullptr);
            ::memset(pBuf, 0, (nLen + 1) * sizeof (char));
            if ((status = ::nc_get_att_text(nId, NC_GLOBAL, ATT_HISTORY, pBuf))
                    != NC_NOERR) {
                return (false);
            }
            std::string ss = pBuf;
            sOld = ss;
        }
        time_t rawtime;
        ::time(&rawtime);
        std::string s;
#if defined(_MSC_VER)
        struct tm timeinfo;
        ::localtime_s(&timeinfo, &rawtime);
        char tbuf[256];
        ::asctime_s(tbuf, sizeof (tbuf), &timeinfo);
        s = tbuf;
#else
        struct tm *timeinfo;
        timeinfo = ::localtime(&rawtime);
        s = ::asctime(timeinfo);
#endif
        if (!user.empty()) {
            s += "\t";
            s += user;
        }
        if (!app.empty()) {
            s += "\t";
            s += app;
        }
        if (!sOld.empty()) {
            sOld += "\r\n";
        }
        sOld += s;
        const char *pRes = sOld.c_str();
        nLen = ::strlen(pRes);
        CharPtr oBuf(new char[nLen + 1]);
        char *p = oBuf.get();
        assert(p != nullptr);
#if defined(_MSC_VER)
        //::strcpy_s(p, nLen, pRes);
        ::strcpy(p, pRes);
#else
        ::strcpy(p, pRes);
#endif
        p[nLen] = (char) 0;
        if (!this->set_define_mode()) {
            return (false);
        }
        if ((status = ::nc_put_att_text(nId, NC_GLOBAL, ATT_HISTORY, ::strlen(p), p))
                != NC_NOERR) {
            return (false);
        }
        this->m_history = true;
        return (true);
    } // write_history
    ////////////////////////////////
} // namespace statdata

