#include "../include/netcdfstatdataset.h"
///////////////////////////////
namespace statdata {
    ///////////////////////////////
    const char *NetCDFStatDataSetFile::ATT_STATDATASET = "IsStatDataSet";
    const char *NetCDFStatDataSetFile::DIM_INDIVS = "indivs";
    const char *NetCDFStatDataSetFile::ATT_INDIVIDS_VAR = "IndivIdsVariable";
    const char *NetCDFStatDataSetFile::ATT_CATEG_VAR = "categ_var";
    const char *NetCDFStatDataSetFile::ATT_ID_VAR = "id_var";
    const char *NetCDFStatDataSetFile::ATT_NAME_VAR = "name_var";
    /////////////////////////////////////////

    std::ostream & NetCDFStatDataSetFile::export_data(std::ostream &os) {
        NetCDFVariables vars;
        if (!this->get_indivs_variables(vars)) {
            return (os);
        }
        size_t startIndex = 0;
        size_t nCount = 0;
        std::map<std::string, Values> oMap;
        for (auto it = vars.begin(); it != vars.end(); ++it) {
            const NetCDFVariable &var = *it;
            const std::string &varname = var.name;
            if (!varname.empty()) {
                Values vals;
                startIndex = 0;
                nCount = 0;
                this->read_indiv_data(varname, vals, startIndex, nCount);
                oMap[varname] = vals;
            }
        } // it
        for (auto it = oMap.begin(); it != oMap.end(); ++it) {
            const std::string &varname = (*it).first;
            if (it != oMap.begin()) {
                os << "\t";
            }
            os << varname;
        } // it
        os << std::endl;
        for (size_t i = 0; i < nCount; ++i) {
            for (auto it = oMap.begin(); it != oMap.end(); ++it) {
                if (it != oMap.begin()) {
                    os << "\t";
                }
                const std::string &varname = (*it).first;
                const Values &vv = oMap[varname];
                const boost::any &v = vv[i];
                if (v.empty()) {
                    os << "N/A";
                } else {
                    Value::to_stream(os, v);
                }
            } // it
            os << std::endl;
        } // i
        return (os);
    } // export_data

    std::wostream & NetCDFStatDataSetFile::export_data(std::wostream &os) {
        NetCDFVariables vars;
        if (!this->get_indivs_variables(vars)) {
            return (os);
        }
        size_t startIndex = 0;
        size_t nCount = 0;
        std::map<std::string, Values> oMap;
        for (auto it = vars.begin(); it != vars.end(); ++it) {
            const NetCDFVariable &var = *it;
            const std::string &varname = var.name;
            if (!varname.empty()) {
                Values vals;
                startIndex = 0;
                nCount = 0;
                this->read_indiv_data(varname, vals, startIndex, nCount);
                oMap[varname] = vals;
            }
        } // it
        for (auto it = oMap.begin(); it != oMap.end(); ++it) {
            const std::string &varname = (*it).first;
            if (it != oMap.begin()) {
                os << L"\t";
            }
            std::wstring ss(varname.length(), L' ');
            std::copy(varname.begin(), varname.end(), ss.begin());
            os << ss;
        } // it
        os << std::endl;
        for (size_t i = 0; i < nCount; ++i) {
            for (auto it = oMap.begin(); it != oMap.end(); ++it) {
                if (it != oMap.begin()) {
                    os << L"\t";
                }
                const std::string &varname = (*it).first;
                const Values &vv = oMap[varname];
                const boost::any &v = vv[i];
                if (v.empty()) {
                    os << L"N/A";
                } else {
                    Value::to_stream(os, v);
                }
            } // it
            os << std::endl;
        } // i
        return (os);
    } // export_data

    bool NetCDFStatDataSetFile::get_indivsids_variable(std::string &varname) {
        varname.clear();
        Values vals;
        std::string attname(ATT_INDIVIDS_VAR);
        if (!this->read_attribute(attname, vals)) {
            return (false);
        }
        if (vals.empty()) {
            return (false);
        }
        const boost::any &v = vals[0];
        if (Value::get_type(v) == statdata::typeString) {
            return (Value::get_value(v,varname));
        }
        return (false);
    } //get_indivsids_variable

    bool NetCDFStatDataSetFile::set_indivsids_variable(const std::string &varname) {
        NetCDFVariable var;
        if (!this->get_indiv_variable(varname, var)) {
            return (false);
        }
        statdata::DataType t = var.type;
        if ((t != typeString) && (t != typeShort) && (t != typeInt)
                && (t != typeByte)) {
            return false;
        }
        Values vals(1);
        std::string attname(ATT_INDIVIDS_VAR);
        vals[0] = varname;
        bool bRet = this->write_attribute(attname, vals);
        if (bRet) {
            vals[0] = (short) 1;
            attname = ATT_ID_VAR;
            bRet = this->write_attribute(varname, attname, vals);
        }
        return (bRet);
    } // set_indivsids_variable

    bool NetCDFStatDataSetFile::set_categ_variable(const std::string &varname) {
        NetCDFVariable var;
        if (!this->get_indiv_variable(varname, var)) {
            return (false);
        }
        Values vals(1);
        std::string attname(ATT_CATEG_VAR);
        vals[0] = (short) 1;
        return (this->write_attribute(var.name, attname, vals));
    } // set_categ_variable

    bool NetCDFStatDataSetFile::set_indivsnames_variable(const std::string &varname) {
        NetCDFVariable var;
        if (!this->get_indiv_variable(varname, var)) {
            return (false);
        }
        statdata::DataType t = var.type;
        if ((t != typeString) && (t != typeShort) && (t != typeInt)
                && (t != typeByte)) {
            return false;
        }
        Values vals(1);
        std::string attname(ATT_NAME_VAR);
        vals[0] = (short) 1;
        return (this->write_attribute(varname, attname, vals));
    } //set_indivsnames_variable

    bool NetCDFStatDataSetFile::read_indiv_data(const std::string &varname,
            Values &values, size_t &nStartIndex, size_t &nCount) {
        values.clear();
        std::valarray<size_t> index(1);
        std::valarray<size_t> count(1);
        index[0] = nStartIndex;
        count[0] = nCount;
        if (!this->read_data(varname, index, count, values)) {
            return (false);
        }
        nStartIndex = index[0];
        nCount = count[0];
        return (true);
    } // read_indiv_data

    bool NetCDFStatDataSetFile::write_indiv_data(const std::string &varname,
            const Values &values, size_t nStartIndex /*= 0 */,
            size_t nCount /*= 0 */) {
        size_t len = values.size();
        if (len < 1) {
            return (false);
        }
        statdata::DataType xtype = statdata::typeOther;
        for (size_t i = 0; i < len; ++i) {
            const boost::any &v = values[i];
            if (!v.empty()) {
                xtype = Value::get_type(v);
                break;
            }
        } // i
        if (xtype == statdata::typeOther) {
            return (false);
        }
        NetCDFVariable oVar;
        if (!this->add_indiv_variable(varname, xtype, oVar)) {
            return (false);
        }
        std::valarray<size_t> index(1);
        std::valarray<size_t> count(1);
        index[0] = nStartIndex;
        if ((nCount < 1) || (nCount > len)) {
            count[0] = len;
        } else {
            count[0] = nCount;
        }
        return (this->write_data(varname, index, count, values));
    } // write_indiv_data

    bool NetCDFStatDataSetFile::add_indiv_variable(const std::string &varname,
            const statdata::DataType &dType, NetCDFVariable &oVar) {
        NetCDFVariables vv;
        if (!this->get_indivs_variables(vv)) {
            return (false);
        }
        const size_t n = vv.size();
        for (size_t i = 0; i < n; ++i) {
            if (vv[i].name == varname) {
                oVar = vv[i];
                return (true);
            }
        } // i
        NetCDFDimension oDim;
        if (!this->get_indivs_dimension(oDim)) {
            return (false);
        }
        NetCDFVariable var;
        var.name = varname;
        var.type = dType;
        var.dims.push_back(oDim);
        if (this->create_variable(var)) {
            if (var.type == typeString) {
                if (!this->set_categ_variable(var.name)) {
                    return (false);
                }
                var.categ_var = true;
            }
            oVar = var;
            return (true);
        }
        return (false);
    } // add_indiv_variable

    bool NetCDFStatDataSetFile::get_indiv_variable(const std::string &varname,
            NetCDFVariable &oVar) {
        NetCDFVariables vv;
        if (!this->get_indivs_variables(vv)) {
            return (false);
        }
        const size_t n = vv.size();
        for (size_t i = 0; i < n; ++i) {
            if (vv[i].name == varname) {
                oVar = vv[i];
                return (true);
            }
        } // i
        return (false);
    } // get_indiv_variable

    bool NetCDFStatDataSetFile::get_indivs_dimension(NetCDFDimension &oDim) {
        NetCDFDimensions dims;
        if (this->get_unlimited_dimensions(dims)) {
            if (!dims.empty()) {
                oDim = dims[0];
                return (true);
            }
        }
        return (false);
    } // get_indivs_dimension

    bool NetCDFStatDataSetFile::get_indivs_size(size_t &nLen) {
        nLen = 0;
        NetCDFDimension oDim;
        if (this->get_indivs_dimension(oDim)) {
            nLen = oDim.len;
            return (true);
        }
        return (false);
    } // get_indivs_size

    bool NetCDFStatDataSetFile::open(const std::string &filename,
            statdata::datafile_open_mode mode /*= mode_read */, bool bClassic /* = false */) {
        if (!NetCDFDataGroup::open(filename, mode, bClassic)) {
            return false;
        }
        if (mode != statdata::mode_read) {
            NetCDFDimensions dims;
            this->get_unlimited_dimensions(dims);
            if (dims.empty()) {
                NetCDFDimension oRowDim;
                std::string sname(DIM_INDIVS);
                if (!this->create_dimension(oRowDim, sname)) {
                    return (false);
                }
            } // dims
            Values vals;
            std::string attname(ATT_STATDATASET);
            if (!this->read_attribute(attname, vals)) {
                vals = Values(1);
                vals[0] = (short) 1;
                if (!this->write_attribute(attname, vals)) {
                    return (false);
                }
            }
        } // not read
        return true;
    } // open
    /////////////////////////////////

    NetCDFStatDataSetFile::NetCDFStatDataSetFile() {
    }

    NetCDFStatDataSetFile::NetCDFStatDataSetFile(const std::string &filename,
            datafile_open_mode mode/*= mode_read */, bool bClassic /*= true */) :
    NetCDFDataGroup(nullptr) {
        this->open(filename, mode, bClassic);
    }

    NetCDFStatDataSetFile::~NetCDFStatDataSetFile() {
    }
    ////////////////////////////////////////
} /* namespace statdata */
