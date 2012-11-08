/* 
 * File:   netcdfstatdataset.h
 * Author: boubad
 *
 * Created on 8 novembre 2012, 15:42
 */

#ifndef NETCDFSTATDATASET_H
#define	NETCDFSTATDATASET_H

#include "datagroup.h"

///////////////////////////////
#include <cassert>
#include <set>
#include <map>
#include <memory>
//////////////////////////////////
namespace statdata {
    ////////////////////////////////////////////
    class DataSet;
    /////////////////////////////////////////

    class NetCDFStatDataSetFile : public statdata::NetCDFDataGroup {
    public:
        typedef NetCDFStatDataSetFile *PNetCDFStatDataSetFile;
        typedef std::shared_ptr<NetCDFStatDataSetFile> NetCDFStatDataSetFilePtr;
    public:
        NetCDFStatDataSetFile();
        NetCDFStatDataSetFile(const std::string &filename,
                statdata::datafile_open_mode mode =
                statdata::mode_read, bool bClassic = true);
        NetCDFStatDataSetFile(const std::string &filename,
                DataSet &oSet, bool bClassic = true);
        virtual ~NetCDFStatDataSetFile();
    public:
        bool read_from(DataSet &oSet);
        bool read(DataSet &oSet);
        std::ostream & export_data(std::ostream &os);
        std::wostream & export_data(std::wostream &os);
        bool get_indivs_dimension(NetCDFDimension &oDim);
        bool get_indivs_size(size_t &nLen);

        template<class ALLOCV>
        bool get_indivs_variables(std::vector<NetCDFVariable, ALLOCV> &oVars) {
            oVars.clear();
            NetCDFVariables vv;
            if (!this->get_variables(vv)) {
                return (false);
            }
            const size_t n = vv.size();
            if (n < 1) {
                return (true);
            }
            NetCDFDimension oDim;
            if (!this->get_indivs_dimension(oDim)) {
                return (false);
            }
            std::string sdimname = oDim.name;
            const int nDimId = oDim.id;
            std::string attCateg(ATT_CATEG_VAR);
            std::string attId(ATT_ID_VAR);
            std::string attName(ATT_NAME_VAR);
            for (size_t i = 0; i < n; ++i) {
                NetCDFVariable &var = vv[i];
                const std::string &varname = var.name;
                const NetCDFDimensions & dd = var.dims;
                const size_t m = dd.size();
                for (size_t j = 0; j < m; ++j) {
                    if ((dd[j].id == nDimId) && (varname != sdimname)) {
                        var.categ_var = this->has_attribute(varname, attCateg);
                        var.id_var = this->has_attribute(varname, attId);
                        var.name_var = this->has_attribute(varname, attName);
                        oVars.push_back(var);
                    }
                } // j
            } // i
            return (true);
        } // get_indivs_variables

        template<class ALLOCV>
        bool get_indivs_categ_variables(std::vector<NetCDFVariable, ALLOCV> &oVars) {
            oVars.clear();
            NetCDFVariables vv;
            if (!this->get_indivs_variables(vv)) {
                return (false);
            }
            for (auto it = vv.begin(); it != vv.end(); ++it) {
                const NetCDFVariable &v = *it;
                if (v.categ_var) {
                    if ((!v.id_var) && (!v.name_var)) {
                        oVars.push_back(v);
                    }
                }
            } // it
            return (true);
        } //get_indivs_categ_variables

        template<class ALLOCV>
        bool get_indivs_num_variables(std::vector<NetCDFVariable, ALLOCV> &oVars) {
            oVars.clear();
            NetCDFVariables vv;
            if (!this->get_indivs_variables(vv)) {
                return (false);
            }
            for (auto it = vv.begin(); it != vv.end(); ++it) {
                const NetCDFVariable &v = *it;
                if ((!v.categ_var) && (!v.id_var) && (!v.name_var)) {
                    oVars.push_back(v);
                }
            } // it
            return (true);
        } //get_indivs_num_variables
        bool get_indiv_variable(const std::string &varname, NetCDFVariable &oVar);
        bool add_indiv_variable(const std::string &name, const statdata::DataType &dType,
                NetCDFVariable &oVar);
        //

        template<typename T, typename ALLOCT>
        bool write_indiv_data(const std::string &varname,
                const std::vector<T, ALLOCT> &values, size_t nStartIndex = 0,
                size_t nCount = 0) {
            const size_t len = values.size();
            Values vv(len);
            for (size_t i = 0; i < len; ++i) {
                vv[i] = values[i];
            } // i
            return (this->write_indiv_data(varname, vv, nStartIndex, nCount));
        } // write_indiv_data
        bool write_indiv_data(const std::string &varname, const Values &values,
                size_t nStartIndex = 0, size_t nCount = 0);
        //

        template<typename T, typename ALLOCT>
        bool read_indiv_data(const std::string &varname,
                std::vector<T, ALLOCT> &values, size_t & nStartIndex,
                size_t & nCount) {
            values.clear();
            Values vv;
            if (!this->read_indiv_data(varname, vv, nStartIndex, nCount)) {
                return (false);
            }
            const size_t n = vv.size();
            values = std::vector<T, ALLOCT > (n);
            for (size_t i = 0; i < n; ++i) {
                T x;
                const boost::any &v = vv[i];
                if (!v.empty()) {
                    Value::get_value(v, x);
                } // not empty
                values[i] = x;
            } // i
            return (true);
        } // read_indiv_data
        bool read_indiv_data(const std::string &varname, Values &values,
                size_t & nStartIndex, size_t & nCount);
        //

        template<class ALLOCZ>
        bool get_variable_not_missing_indexes(const std::string &varname,
        std::set<size_t, std::less<size_t>, ALLOCZ> &oSet) {
            oSet.clear();
            size_t nStart = 0;
            size_t nCount = 0;
            Values vals;
            if (!this->read_indiv_data(varname, vals, nStart, nCount)) {
                return (false);
            }
            const size_t n = vals.size();
            for (size_t i = 0; i < n; ++i) {
                const boost::any &v = vals[i];
                if (!v.empty()) {
                    oSet.insert(i);
                }
            } // i
            return (true);
        } //get_variable_not_missing_indexes

        template<class ALLOCS, class ALLOCZ>
        bool get_variables_not_missing_indexes(
        const std::vector<std::string, ALLOCS> &varnames,
        std::set<size_t, std::less<size_t>, ALLOCZ> &oSet) {
            oSet.clear();
            const size_t n = varnames.size();
            for (size_t i = 0; i < n; ++i) {
                const std::string &varname = varnames[i];
                std::set<size_t> xSet;
                if (!this->get_variable_not_missing_indexes(varname, xSet)) {
                    return (false);
                }
                if (oSet.empty()) {
                    oSet = xSet;
                } else {
                    std::set<size_t> oDel;
                    for (auto it = oSet.begin(); it != oSet.end();
                            ++it) {
                        size_t ind = *it;
                        if (xSet.find(ind) == xSet.end()) {
                            oDel.insert(ind);
                        }
                    } // it
                    for (auto it = oDel.begin(); it != oDel.end();
                            ++it) {
                        size_t ind = *it;
                        auto jt = oSet.find(ind);
                        if (jt != oSet.end()) {
                            oSet.erase(jt);
                        }
                    } // it
                }
            } // i
            return (true);
        } //get_variables_not_missing_indexes

        template<class ALLOCP>
        bool get_variable_valuemap(const std::string &varname,
        std::map<size_t, boost::any, std::less<size_t>, ALLOCP> &oMap) {
            oMap.clear();
            Values vals;
            size_t nStart = 0;
            size_t nCount = 0;
            if (!this->read_indiv_data(varname, vals, nStart, nCount)) {
                return (false);
            }
            for (size_t i = 0; i < nCount; ++i) {
                const boost::any &v = vals[i];
                if (!v.empty()) {
                    oMap[i] = v;
                }
            } // i
            return (true);
        } // get_variable_valuemap

        template<class ALLOCS, class ALLOCP1, class ALLOCP2>
        bool get_variables_dictionary(
        const std::vector<std::string, ALLOCS> &varnames,
        std::map<std::string,
        std::map<size_t, boost::any, std::less<size_t>, ALLOCP1>,
        std::less<std::string>, ALLOCP2>&oDict) {
            oDict.clear();
            std::set<size_t> oSet;
            if (!this->get_variables_not_missing_indexes(varnames, oSet)) {
                return (false);
            }
            const size_t n = varnames.size();
            for (size_t i = 0; i < n; ++i) {
                std::map<size_t, boost::any> xMap;
                const std::string &varname = varnames[i];
                if (this->get_variable_valuemap(varname, xMap)) {
                    std::map<size_t, Value, std::less<size_t>, ALLOCP1> oMap;
                    for (auto it = oSet.begin(); it != oSet.end(); ++it) {
                        size_t curindex = *it;
                        auto jt = xMap.find(curindex);
                        if (jt != xMap.end()) {
                            const Value &v = xMap[curindex];
                            oMap[curindex] = v;
                        }
                    } // it
                    if (!oMap.empty()) {
                        oDict[varname] = oMap;
                    }
                } // ok
            } // i
            return (true);
        } // get_variables_dictionary

        template<class ALLOCS, class ALLOCSIZET, class ALLOCVALUE, class ALLOCP2>
        bool get_variables_data(const std::vector<std::string, ALLOCS> &varnames,
        std::vector<size_t, ALLOCSIZET> &oIndexes,
        std::map<std::string, std::vector<boost::any, ALLOCVALUE>,
        std::less<std::string>, ALLOCP2>&oData) {
            oIndexes.clear();
            oData.clear();
            std::map<std::string, std::map<size_t, boost::any> > oDict;
            if (!this->get_variables_dictionary(varnames, oDict)) {
                return (false);
            }
            if (oDict.empty()) {
                return (true);
            }
            const std::map<size_t, boost::any> &oFirstMap = (*(oDict.begin())).second;
            for (auto it = oFirstMap.begin(); it != oFirstMap.end(); ++it) {
                size_t nIndex = (*it).first;
                oIndexes.push_back(nIndex);
            } // it
            const size_t n = oIndexes.size();
            for (auto it = oDict.begin(); it != oDict.end(); ++it) {
                const std::string &varname = (*it).first;
                const std::map<size_t, boost::any> &oMap = (*it).second;
                std::vector<boost::any, ALLOCVALUE> oVec(n);
                for (size_t i = 0; i < n; ++i) {
                    size_t nIndex = oIndexes[i];
                    auto jt = oMap.find(nIndex);
                    if (jt != oMap.end()) {
                        oVec[i] = (*jt).second;
                    }
                } // i
                oData[varname] = oVec;
            } // it
            return (true);
        } // get_variables_data

        template<typename T, class ALLOCS, class ALLOCSIZET, class ALLOCT>
        bool get_variables_data(const std::vector<std::string, ALLOCS> &varnames,
        std::vector<size_t, ALLOCSIZET> &indexes,
        std::vector<T, ALLOCT> &oData) {
            indexes.clear();
            oData.clear();
            std::map<std::string, Values> oMap;
            if (!this->get_variables_data(varnames, indexes, oMap)) {
                return (false);
            }
            const size_t nr = indexes.size();
            const size_t nc = varnames.size();
            oData = std::vector<T, ALLOCT > (nr * nc);
            for (size_t icol = 0; icol < nc; ++icol) {
                const std::string &varname = varnames[icol];
                auto it = oMap.find(varname);
                assert(it != oMap.end());
                const Values &oVec = (*it).second;
                assert(oVec.size() >= nr);
                for (size_t j = 0; j < nr; ++j) {
                    const boost::any &v = oVec[j];
                    if (!v.empty()) {
                        T x;
                        if (Value::get_value(v, x)) {
                            oData[j * nc + icol] = x;
                        } // x
                    } // v
                } // j
            } // icol
            return (true);
        } // get_variables_data
        bool set_indivsids_variable(const std::string &varname);
        bool set_categ_variable(const std::string &varname);
        bool set_indivsnames_variable(const std::string &varname);
        bool get_indivsids_variable(std::string &varname);
        bool get_indivsnames_variable(std::string &varname);
        bool set_indivsweights_variable(const std::string &varname);
        bool get_indivsweights_variable(std::string &varname);
        
        template<class ALLOCP1>
        bool get_indivsids(
        std::map<size_t, boost::any, std::less<size_t>, ALLOCP1> &oMap) {
            oMap.clear();
            std::string varname;
            if (!this->get_indivsids_variable(varname)) {
                return (false);
            }
            return (this->get_variable_valuemap(varname, oMap));
        } // get_indivsids

        template<class ALLOCP1>
        bool set_indiv_data(const boost::any &vId,
        const std::map<std::string, boost::any, std::less<std::string>, ALLOCP1> &oMap) {
            if (vId.empty() || oMap.empty()) {
                return (false);
            }
            std::map<size_t, boost::any> vMap;
            if (!this->get_indivsids(vMap)) {
                return (false);
            }
            std::string sId;
            Value::get_value(vId, sId);
            if (sId.empty()) {
                return (false);
            }
            size_t pos = 0;
            bool bFound = false;
            for (auto it = vMap.begin(); it != vMap.end(); ++it) {
                const boost::any &vx = (*it).second;
                if (!vx.empty()) {
                    std::string sx;
                    if (Value::get_value(vx, sx)) {
                        if (sx == sId) {
                            pos = (*it).first;
                            bFound = true;
                            break;
                        }
                    }// ok
                }
            } // it
            if (!bFound) {
                if (!this->get_indivs_size(pos)) {
                    return (false);
                }
            }
            size_t count = 1;
            for (auto it = oMap.begin(); it != oMap.end(); ++it) {
                const std::string &varname = (*it).first;
                const boost::any &v = (*it).second;
                Values vv(1);
                vv[0] = v;
                if (!this->write_indiv_data(varname, vv, pos, count)) {
                    return (false);
                }
            } // it
            return (true);
        } // set_indiv_data

        template<class ALLOCP1>
        bool get_indiv_data(const boost::any &vId,
        std::map<std::string, boost::any, std::less<std::string>, ALLOCP1> &oMap) {
            oMap.clear();
            if (vId.empty()) {
                return (false);
            }
            std::string varname;
            if (!this->get_indivsids_variable(varname)) {
                return (false);
            }
            std::map<size_t, boost::any> vMap;
            if (!this->get_indivsids(vMap)) {
                return (false);
            }
            std::string sId;
            Value::get_value(vId, sId);
            if (sId.empty()) {
                return (false);
            }
            size_t pos = 0;
            bool bFound = false;
            for (auto it = vMap.begin(); it != vMap.end(); ++it) {
                const boost::any &vx = (*it).second;
                if (!vx.empty()) {
                    std::string sx;
                    if (Value::get_value(vx, sx)) {
                        if (sx == sId) {
                            pos = (*it).first;
                            bFound = true;
                            break;
                        }
                    }// ok
                }
            } // it
            if (!bFound) {
                return false;
            }
            NetCDFVariables vars;
            if (!this->get_indivs_variables(vars)) {
                return (false);
            }
            const size_t m = vars.size();
            std::valarray<size_t> index(1);
            for (size_t i = 0; i < m; ++i) {
                const std::string &vname = vars[i].name;
                if (vname != varname) {
                    index[0] = pos;
                    boost::any vz;
                    if (this->read_one_data(vname, index, vz)) {
                        if (!vz.empty()) {
                            oMap[vname] = vz;
                        }
                    }
                } // ok
            } // i
            return (true);
        } // get_indiv_data
    public:
        virtual bool open(const std::string &filename,
                statdata::datafile_open_mode mode =
                statdata::mode_read, bool bClassic = true);
    protected:
        static const char *ATT_STATDATASET;
        static const char *DIM_INDIVS;
        static const char *ATT_INDIVIDS_VAR;
        static const char *ATT_CATEG_VAR;
        static const char *ATT_ID_VAR;
        static const char *ATT_NAME_VAR;
         static const char *ATT_WEIGHT_VAR;
    };

} /* namespace statdata */


#endif	/* NETCDFSTATDATASET_H */

