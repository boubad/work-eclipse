// dataset.cpp
//////////////////////////
#include "../include/dataset.h"
#include "../include/csvreader.h"
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
    std::vector<boost::any> * DataSet::variable_data(const std::string &sId){
         std::string sid = boost::trim_copy(sId);
        if (sid.empty()) {
            return (nullptr);
        }
        std::string sx = boost::to_lower_copy(sid);
        auto it = this->m_data.find(sx);
        if (it != this->m_data.end()){
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
        std::map<std::string,std::vector<boost::any> > oMap;
        if (!CSVReader::read_csv_file(filename,oMap)){
            return (false);
        }
        this->clear();
        size_t nr = 0;
        for (auto it = oMap.begin(); it != oMap.end(); ++it){
            std::string sId = (*it).first;
            std::string sid = boost::trim_copy(boost::to_lower_copy(sId));
            if (sid.empty()){
                continue;
            }
            const std::vector<boost::any> vv = (*it).second;
            size_t n = vv.size();
            if (nr == 0){
                nr = n;
            }
            statdata::DataType rtype = statdata::typeOther;
            for (size_t i = 0; i < n; ++i){
                const boost::any &vx = vv[i];
                if (!vx.empty()){
                    rtype = Value::get_type(vx);
                    break;
                }
            }// i
            if (rtype == statdata::typeOther){
                continue;
            }
            Variable var(sid,rtype);
            var.name(sId);
            if ((rtype == statdata::typeBool) || (rtype == statdata::typeString)){
                var.set_categ_var(true);
            }
            var.index((int)this->m_vars.size());
            this->m_vars.push_back(var);
            this->m_data[sid] = vv;
        }// it
        this->m_inds.resize(nr);
        for (size_t i = 0; i < nr; ++i){
            std::string sid;
            std::string sname;
            {
                std::stringstream os;
                os << "I" << (i+1);
                sid = os.str();
            }
            {
                std::stringstream os;
                os << "INDIVIDU" << (i+1);
                sname = os.str();
            }
            Individu &ind = this->m_inds[i];
            ind.id(sid);
            ind.name(sname);
            ind.index((int)i);
        }// i
        return (true);
    }// import_csv_file
    /////////////////////////////////////
}// namespace statdata
