// dataset.cpp
//////////////////////////
#include "../include/dataset.h"
#include <csvreader.h>
//////////////////////////////////
#include <algorithm>
#include <sstream>
/////////////////////////////
#include <boost/algorithm/string.hpp>
//////////////////////////////////
namespace statdata {
    //////////////////////////////////

    DataSet::DataSet() {
    }

    DataSet::DataSet(const std::string &filename) {
        this->import_csv_file(filename);
    }

    DataSet::DataSet(size_t nVars, size_t nRows) {
        this->initialize(nVars, nRows);
    }// DataSet

    DataSet::DataSet(const DataSet &other) : m_vars(other.m_vars),
    m_inds(other.m_inds), m_data(other.m_data) {

    }

    DataSet & DataSet::operator=(const DataSet &other) {
        if (this != &other) {
            this->m_vars = other.m_vars;
            this->m_inds = other.m_inds;
            this->m_data = other.m_data;
        }
        return (*this);
    }

    DataSet::~DataSet() {

    }

    void DataSet::clear(void) {
        this->m_data.clear();
        this->m_inds.clear();
        this->m_vars.clear();
    }// clear

    void DataSet::initialize(size_t nVars, size_t nRows) {
        this->clear();
        //
        {
            std::string sIdVar("ids");
            Variable var1(sIdVar, statdata::typeString);
            var1.index(0);
            var1.set_categ_var(true);
            var1.set_id_var(true);
            this->m_vars.push_back(var1);
            std::vector<boost::any> vv;
            if (nRows > 0) {
                vv.resize(nRows);
                for (int i = 0; i < (int)nRows; ++i) {
                    std::stringstream os;
                    os << "ind" << (i + 1);
                    std::string sid = os.str();
                    vv[i] = sid;
                    Individu ind(sid);
                    ind.index(i);
                    this->m_inds.push_back(ind);
                }// i
            }// Rows
            std::string sx = var1.id();
            this->m_data[sx] = vv;
        }
        //
        {
            std::string sNameVar("names");
            Variable var2(sNameVar, statdata::typeString);
            var2.index(1);
            var2.set_categ_var(true);
            var2.set_name_var(true);
            this->m_vars.push_back(var2);
            std::vector<boost::any> vv;
            if (nRows > 0) {
                vv.resize(nRows);
                for (int i = 0; i < (int)nRows; ++i) {
                    std::stringstream os;
                    os << "individu" << (i + 1);
                    std::string sid = os.str();
                    vv[i] = sid;
                }// i
            }// Rows
            std::string sx = var2.id();
            this->m_data[sx] = vv;
        }
        //
        {
            std::string sWVar("weights");
            Variable var3(sWVar, statdata::typeDouble);
            var3.index(2);
            var3.set_num_var(true);
            var3.set_weight_var(true);
            this->m_vars.push_back(var3);
            std::vector<boost::any> vv;
            if (nRows > 0) {
                vv.resize(nRows);
                double dval = 1.0;
                for (int i = 0; i < (int)nRows; ++i) {
                    vv[i] = dval;
                }// i
            }// Rows
            std::string sx = var3.id();
            this->m_data[sx] = vv;
        }
        //
        for (size_t i = 0; i < nVars; ++i) {
            std::stringstream os;
            os << "var" << (i + 1);
            std::string sid = os.str();
            Variable varx(sid, statdata::typeDouble);
            varx.index((int) this->m_vars.size());
            varx.set_num_var(true);
            this->m_vars.push_back(varx);
            std::vector<boost::any> vv;
            if (nRows > 0) {
                vv.resize(nRows);
            }// Rows
            std::string sx = varx.id();
            this->m_data[sx] = vv;
        }// i
    }// initialize

    bool DataSet::set_ids_variable(const std::string &sId) {
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (false);
        }
        std::string sx = boost::to_lower_copy(sid);
        bool bRet = false;
        for (auto it = this->m_vars.begin(); it != this->m_vars.end(); ++it) {
            Variable &v = *it;
            std::string s = boost::trim_copy(v.id());
            std::string ss = boost::to_lower_copy(s);
            if (ss == sx) {
                v.set_categ_var(true);
                v.set_id_var(true);
                bRet = true;
            } else {
                v.set_id_var(false);
            }
        }// it
        return (bRet);
    }// set_ids_variable

    bool DataSet::set_names_variable(const std::string &sId) {
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (false);
        }
        std::string sx = boost::to_lower_copy(sid);
        bool bRet = false;
        for (auto it = this->m_vars.begin(); it != this->m_vars.end(); ++it) {
            Variable &v = *it;
            std::string s = boost::trim_copy(v.id());
            std::string ss = boost::to_lower_copy(s);
            if (ss == sx) {
                v.set_categ_var(true);
                v.set_name_var(true);
                bRet = true;
            } else {
                v.set_name_var(false);
            }
        }// it
        return (bRet);
    }// set_names_variable

    bool DataSet::set_weights_variable(const std::string &sId) {
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (false);
        }
        std::string sx = boost::to_lower_copy(sid);
        bool bRet = false;
        for (auto it = this->m_vars.begin(); it != this->m_vars.end(); ++it) {
            Variable &v = *it;
            std::string s = boost::trim_copy(v.id());
            std::string ss = boost::to_lower_copy(s);
            if (ss == sx) {
                v.set_num_var(true);
                v.set_weight_var(true);
                bRet = true;
            } else {
                v.set_weight_var(false);
            }
        }// it
        return (bRet);
    }// set_weights_variable

    bool DataSet::get_ids_variable(std::string &sId) const {
        sId.clear();
        for (auto it = this->m_vars.begin(); it != this->m_vars.end(); ++it) {
            const Variable &v = *it;
            if (v.is_id_var()) {
                std::string s = boost::trim_copy(v.id());
                sId = boost::to_lower_copy(s);
                return (true);
            }
        }// it
        return (false);
    }// get_ids_variable

    bool DataSet::get_names_variable(std::string &sId) const {
        sId.clear();
        for (auto it = this->m_vars.begin(); it != this->m_vars.end(); ++it) {
            const Variable &v = *it;
            if (v.is_name_var()) {
                std::string s = boost::trim_copy(v.id());
                sId = boost::to_lower_copy(s);
                return (true);
            }
        }// it
        return (false);
    }// get_names_variable

    bool DataSet::get_weights_variable(std::string &sId) const {
        sId.clear();
        for (auto it = this->m_vars.begin(); it != this->m_vars.end(); ++it) {
            const Variable &v = *it;
            if (v.is_weight_var()) {
                std::string s = boost::trim_copy(v.id());
                sId = boost::to_lower_copy(s);
                return (true);
            }
        }// it
        return (false);
    }// get_weights_variable

    bool DataSet::indiv_data(const std::string &sId, std::map<std::string,
            boost::any> &oMap) {
        oMap.clear();
        int indindex = this->get_indiv_index(sId);
        if ((indindex < 0) || (indindex >= (int) this->m_inds.size())) {
            return (false);
        }
        for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
            std::string varid = (*it).first;
            const std::vector<boost::any> &vv = (*it).second;
            if (indindex < (int) vv.size()) {
                boost::any v = vv[indindex];
                oMap[varid] = v;
            }
        }// it
        return (true);
    }// indiv_data

    bool DataSet::indiv_data(int indindex, std::map<std::string,
            boost::any> &oMap) {
        oMap.clear();
        if ((indindex < 0) || (indindex >= (int) this->m_inds.size())) {
            return (false);
        }
        for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
            std::string varid = (*it).first;
            const std::vector<boost::any> &vv = (*it).second;
            if (indindex < (int) vv.size()) {
                boost::any v = vv[indindex];
                oMap[varid] = v;
            }
        }// it
        return (true);
    }// indiv_data

    bool DataSet::get_value(const std::string &sIndId, const std::string &sVarId,
            boost::any &val) {
        val = boost::any();
        int indindex = this->get_indiv_index(sIndId);
        if ((indindex < 0) || (indindex >= (int) this->m_inds.size())) {
            return (false);
        }
        std::string sid = boost::trim_copy(sVarId);
        if (sid.empty()) {
            return (false);
        }
        std::string sx = boost::to_lower_copy(sid);
        if (this->m_data.find(sx) == this->m_data.end()) {
            return (false);
        }
        auto vv = this->m_data[sx];
        if (indindex < (int) vv.size()) {
            val = vv[indindex];
            return (true);
        }
        return (false);
    }// get_value

    bool DataSet::set_value(const std::string &sIndId, const std::string &sVarId,
            const boost::any &val) {
        int indindex = this->get_indiv_index(sIndId);
        if ((indindex < 0) || (indindex >= (int) this->m_inds.size())) {
            return (false);
        }
        std::string sid = boost::trim_copy(sVarId);
        if (sid.empty()) {
            return (false);
        }
        std::string sx = boost::to_lower_copy(sid);
        if (this->m_data.find(sx) == this->m_data.end()) {
            return (false);
        }
        std::vector<boost::any> &vv = this->m_data[sx];
        if (indindex < (int) vv.size()) {
            vv[indindex] = val;
            return (true);
        }
        return (false);
    }// set_value

    bool DataSet::get_value(int indindex, int varindex, boost::any &val) {
        if ((indindex < 0) || (indindex >= (int) this->m_inds.size())) {
            return (false);
        }
        if ((varindex < 0) || (varindex >= (int) this->m_vars.size())) {
            return (false);
        }
        std::string sId = (this->m_vars[varindex]).id();
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (false);
        }
        std::string sx = boost::to_lower_copy(sid);
        if (this->m_data.find(sx) == this->m_data.end()) {
            return (false);
        }
        std::vector<boost::any> &vv = this->m_data[sx];
        if (indindex < (int) vv.size()) {
            val = vv[indindex];
            return (true);
        }
        return (false);
    }// get_value

    bool DataSet::set_value(int indindex, int varindex, const boost::any &val) {
        if ((indindex < 0) || (indindex >= (int) this->m_inds.size())) {
            return (false);
        }
        if ((varindex < 0) || (varindex >= (int) this->m_vars.size())) {
            return (false);
        }
        std::string sId = (this->m_vars[varindex]).id();
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (false);
        }
        std::string sx = boost::to_lower_copy(sid);
        if (this->m_data.find(sx) == this->m_data.end()) {
            return (false);
        }
        std::vector<boost::any> &vv = this->m_data[sx];
        if (indindex < (int) vv.size()) {
            vv[indindex] = val;
            return (true);
        }
        return (false);
    }// set_value

    void DataSet::get_variables_ids(std::vector<std::string> &names) const {
        names.clear();
        const std::vector<Variable> &vars = this->m_vars;
        for (auto it = vars.begin(); it != vars.end(); ++it) {
            const Variable &v = (*it);
            std::string varname = v.id();
            names.push_back(varname);
        }// it
    }// get_variables_ids

    int DataSet::get_variable_index(const std::string &sId) const {
        int nRet = -1;
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (nRet);
        }
        std::string sx = boost::to_lower_copy(sid);
        std::vector<Variable> &vars = const_cast<std::vector<Variable> &> (this->m_vars);
        size_t n = vars.size();
        for (size_t i = 0; i < n; ++i) {
            Variable &v = vars[i];
            std::string sy = boost::trim_copy(v.id());
            std::string syy = boost::to_lower_copy(sy);
            if (syy == sx) {
                nRet = (int) i;
                if (v.index() != nRet) {
                    v.index(nRet);
                }
                return (nRet);
            }
        }
        return (nRet);
    }// get_variable_index

    const Variable *DataSet::get_variable(int nIndex) const {
        if ((nIndex >= 0) && (nIndex < (int) this->m_vars.size())) {
            return &((this->m_vars[nIndex]));
        }
        return (nullptr);
    }// get_variable

    std::vector<boost::any> * DataSet::variable_data(const std::string &sId) {
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (nullptr);
        }
        std::string sx = boost::to_lower_copy(sid);
        auto it = this->m_data.find(sx);
        if (it != this->m_data.end()) {
            return (&(this->m_data[sx]));
        }
        return (nullptr);
    }// variable_data

    const Variable *DataSet::get_variable(const std::string &sId) const {
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (nullptr);
        }
        std::string sx = boost::to_lower_copy(sid);
        const std::vector<Variable> &vars = this->m_vars;

        auto it = std::find_if(vars.begin(), vars.end(),
                [&](const Variable & v)->bool {
            return (v.id() == sx);
        });
        if (it != vars.end()) {
            return (&(*it));
        }
        return (nullptr);
    }// get_variable

    Variable *DataSet::add_variable(const std::string &sId, const std::string &sName) {
        Variable *pRet = nullptr;
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (nullptr);
        }
        std::string sx = boost::to_lower_copy(sid);
        std::vector<Variable> &vars = this->m_vars;

        auto it = std::find_if(vars.begin(), vars.end(),
                [&](Variable & v)->bool {
            return (v.id() == sx);
        });
        if (it != vars.end()) {
            pRet = &(*it);
            pRet->name(sName);
            return pRet;
        }
        Variable v;
        v.index((int) vars.size());
        v.id(sx);
        v.name(sName);
        this->m_vars.push_back(v);
        size_t n = this->m_inds.size();
        std::vector<boost::any> vv;
        if (n > 0) {
            vv.resize(n);
        }
        this->m_data[sx] = vv;
        return const_cast<Variable *> (this->get_variable(sx));
    }// add_variable

    bool DataSet::remove_variable(int nIndex) {
        std::vector<Variable> &vars = this->m_vars;
        if ((nIndex >= 0) && (nIndex < (int) vars.size())) {
            std::string sId = (vars[nIndex]).id();
            for (int i = nIndex + 1; i < (int) vars.size(); ++i) {
                vars[i - 1] = vars[i];
            }// i
            vars.pop_back();
            std::string sid = boost::trim_copy(sId);
            std::string sx = boost::to_lower_copy(sid);
            auto it = this->m_data.find(sx);
            if (it != this->m_data.end()) {
                this->m_data.erase(it);
            }
            size_t n = vars.size();
            for (size_t i = 0; i < n; ++i) {
                (vars[i]).index((int) i);
            }// i
            return (true);
        }

        return (false);
    }// remove_variable

    int DataSet::get_indiv_index(const std::string &sId) const {
        int nRet = -1;
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (nRet);
        }
        std::string sx = boost::to_lower_copy(sid);
        std::vector<Individu> &inds = const_cast<std::vector<Individu> &> (this->m_inds);
        size_t n = inds.size();
        for (size_t i = 0; i < n; ++i) {
            Individu &v = inds[i];
            std::string sy = boost::trim_copy(v.id());
            std::string syy = boost::to_lower_copy(sy);
            if (syy == sx) {
                nRet = (int) i;
                if (v.index() != nRet) {
                    v.index(nRet);
                }
                return (nRet);
            }
        }
        return (nRet);
    }

    const Individu *DataSet::get_indiv(const std::string &sId) const {
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (nullptr);
        }
        std::string sx = boost::to_lower_copy(sid);
        const std::vector<Individu> &inds = this->m_inds;

        auto it = std::find_if(inds.begin(), inds.end(),
                [&](const Individu & v)->bool {
            return (v.id() == sx);
        });
        if (it != inds.end()) {
            return (&(*it));
        }
        return (nullptr);
    }

    const Individu *DataSet::get_indiv(int nIndex) const {
        if ((nIndex >= 0) && (nIndex < (int) this->m_inds.size())) {
            return &((this->m_inds[nIndex]));
        }
        return (nullptr);
    }

    Individu *DataSet::add_indiv(const std::string &sId, const std::string &sName) {
        Individu *pRet = nullptr;
        std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (nullptr);
        }
        std::string sx = boost::to_lower_copy(sid);
        std::vector<Individu> &inds = this->m_inds;

        auto it = std::find_if(inds.begin(), inds.end(),
                [&](Individu & v)->bool {
            return (v.id() == sx);
        });
        if (it != inds.end()) {
            pRet = &(*it);
            pRet->name(sName);
            return pRet;
        }
        Individu v;
        v.index((int) inds.size());
        v.id(sx);
        v.name(sName);
        this->m_inds.push_back(v);
        boost::any val;
        for (auto jt = this->m_data.begin(); jt != this->m_data.end(); ++jt) {
            ((*jt).second).push_back(val);
        }
        return (const_cast<Individu *> (this->get_indiv(sx)));
    }// add_indiv

    void DataSet::get_indivs_ids(std::vector<std::string> &names) const {
        names.clear();
        const std::vector<Individu> &inds = this->m_inds;
        for (auto it = inds.begin(); it != inds.end(); ++it) {
            const Individu &v = (*it);
            std::string varname = v.id();
            names.push_back(varname);
        }// it
    }

    bool DataSet::remove_indiv(int nIndex) {
        std::vector<Individu> &inds = this->m_inds;
        if ((nIndex >= 0) && (nIndex < (int) inds.size())) {
            std::string sId = (inds[nIndex]).id();
            for (int i = nIndex + 1; i < (int) inds.size(); ++i) {
                inds[i - 1] = inds[i];
            }// i
            inds.pop_back();
            size_t pos = (size_t) nIndex;
            for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
                std::vector<boost::any> &vv = (*it).second;
                size_t nx = vv.size();
                for (size_t i = (size_t) (pos + 1); i < nx; ++i) {
                    vv[i - 1] = vv[i];
                }
                vv.pop_back();
            }// it
            return (true);
        }
        return (false);
    }

    bool DataSet::import_csv_file(const std::string & filename) {
        std::map<std::string, std::vector<boost::any> > oMap;
        if (!CSVReader::read_csv_file(filename, oMap)) {
            return (false);
        }
        this->set_all_data(oMap);
        return (true);
    }// import_csv_file
    /////////////////////////////////////
}// namespace statdata
