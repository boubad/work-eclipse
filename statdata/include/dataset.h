/* 
 * File:   dataset.h
 * Author: boubad
 *
 * Created on 4 novembre 2012, 17:59
 */

#ifndef DATASET_H
#define	DATASET_H
////////////////////////////////
#include <value.h>
///////////////////////////////
#include <sstream>
////////////////////////////////
#include "variable.h"
/////////////////////////
#include <vector>
#include <set>
#include <map>
////////////////////////////////////////
namespace statdata {
    //////////////////////////////////

    class DataSet {
    public:
        DataSet();
        explicit
        DataSet(const std::string &filename);
        explicit
        DataSet(size_t nVars, size_t nRows = 0);
        template <class ALLOCV, class ALLOCP>
        DataSet(const std::map<std::string,
        std::vector<boost::any, ALLOCV>,
        std::less<std::string>, ALLOCP> &oMap) {
            this->set_all_data(oMap);
        }
        DataSet(const DataSet &other);
        DataSet & operator=(const DataSet &other);
        virtual ~DataSet();
    public:

        size_t nb_vars(void) const {
            return (this->m_vars.size());
        }

        size_t nb_indivs(void) const {
            return (this->m_inds.size());
        }
        int get_variable_index(const std::string &sId) const;
        const Variable *get_variable(const std::string &sId) const;
        const Variable *get_variable(int nIndex) const;
        Variable *add_variable(const std::string &sId, const std::string &sName);
        void get_variables_ids(std::vector<std::string> &names) const;
        bool remove_variable(int nIndex);

        bool remove_variable(const std::string &sId) {
            int nIndex = this->get_variable_index(sId);
            if ((nIndex >= 0) && (nIndex < (int) this->m_vars.size())) {
                return (this->remove_variable(nIndex));
            }
            return (false);
        }
        //
        int get_indiv_index(const std::string &sId) const;
        const Individu *get_indiv(const std::string &sId) const;
        const Individu *get_indiv(int nIndex) const;
        Individu *add_indiv(const std::string &sId, const std::string &sName);
        void get_indivs_ids(std::vector<std::string> &names) const;
        bool remove_indiv(int nIndex);

        bool remove_indiv(const std::string &sId) {
            int nIndex = this->get_indiv_index(sId);
            if ((nIndex >= 0) && (nIndex < (int) this->m_inds.size())) {
                return (this->remove_indiv(nIndex));
            }
            return (false);
        }
        //
        std::vector<boost::any> * variable_data(const std::string &sId);
        bool indiv_data(const std::string &sId,
                std::map<std::string, boost::any> &oMap);
        bool indiv_data(int indindex,
                std::map<std::string, boost::any> &oMap);
        bool get_value(const std::string &sIndId, const std::string &sVarId,
                boost::any &val);
        bool set_value(const std::string &sIndId, const std::string &sVarId,
                const boost::any &val);
        bool get_value(int iIndIndex, int iVarIndex, boost::any &val);
        bool set_value(int iIndIndex, int iVarIndex, const boost::any &val);
        //
        bool set_ids_variable(const std::string &sId);
        bool set_names_variable(const std::string &sId);
        bool set_weights_variable(const std::string &sId);
        //
        bool get_ids_variable(std::string &sId) const;
        bool get_names_variable(std::string &sId) const;
        bool get_weights_variable(std::string &sId) const;
        //

        template <class ALLOCS>
        bool get_ids(std::vector<std::string, ALLOCS> &data) const {
            data.clear();
            std::string sId;
            if (!this->get_ids_variable(sId)) {
                return (false);
            }
            for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
                if ((*it).first == sId) {
                    auto vv = (*it).second;
                    size_t n = vv.size();
                    if (n > 0) {
                        data.resize(n);
                        for (size_t i = 0; i < n; ++i) {
                            std::string v;
                            if (Value::get_value(vv[i], v)) {
                                data[i] = v;
                            }
                        }// i
                    }// n
                    return (true);
                }// found
            }// it
            return (false);
        }// get_ids

        template <class ALLOCS>
        bool get_names(std::vector<std::string, ALLOCS> &data) const {
            data.clear();
            std::string sId;
            if (!this->get_names_variable(sId)) {
                if (!this->get_ids_variable(sId)) {
                    return (false);
                }
            }
            for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
                if ((*it).first == sId) {
                    auto vv = (*it).second;
                    size_t n = vv.size();
                    if (n > 0) {
                        data.resize(n);
                        for (size_t i = 0; i < n; ++i) {
                            std::string v;
                            if (Value::get_value(vv[i], v)) {
                                data[i] = v;
                            }
                        }// i
                    }// n
                    return (true);
                }// found
            }// it
            return (false);
        }// get_names

        template <typename T, class ALLOCT>
        bool get_weights(std::vector<T, ALLOCT> &data) const {
            data.clear();
            std::string sId;
            if (!this->get_ids_variable(sId)) {
                return (false);
            }
            for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
                if ((*it).first == sId) {
                    auto vv = (*it).second;
                    size_t n = vv.size();
                    if (n > 0) {
                        data.resize(n);
                        for (size_t i = 0; i < n; ++i) {
                            T v;
                            if (Value::get_value(vv[i], v)) {
                                data[i] = v;
                            }
                        }// i
                    }// n
                    return (true);
                }// found
            }// it
            return (false);
        }// get_weights

        template <typename T, class ALLOCT>
        bool get_data(const std::string &sId, std::vector<T, ALLOCT> &data) const {
            data.clear();
            std::string sid = boost::to_lower_copy(boost::trim_copy(sId));
            if (sid.empty()) {
                return (false);
            }
            for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
                if ((*it).first == sid) {
                    auto vv = (*it).second;
                    size_t n = vv.size();
                    if (n > 0) {
                        data.resize(n);
                        for (size_t i = 0; i < n; ++i) {
                            T v;
                            if (Value::get_value(vv[i], v)) {
                                data[i] = v;
                            }
                        }// i
                    }// n
                    return (true);
                }// found
            }// it
            return (false);
        }// get_data

        template <typename T, class ALLOCT>
        bool set_data(const std::string &sId, const std::vector<T, ALLOCT> &data) {
            std::string sid = boost::to_lower_copy(boost::trim_copy(sId));
            if (sid.empty()) {
                return (false);
            }
            size_t nx = data.size();
            for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
                if ((*it).first == sid) {
                    std::vector<boost::any> & vv = (*it).second;
                    size_t n = vv.size();
                    if (nx < n) {
                        n = nx;
                    }
                    if (n > 0) {
                        data.resize(n);
                        for (size_t i = 0; i < n; ++i) {
                            vv[i] = boost::any(data[i]);
                        }// i
                    }// n
                    return (true);
                }// found
            }// it
            return (false);
        }// set_data

        template <class ALLOCV, class ALLOCP>
        void set_all_data(const std::map<std::string,
        std::vector<boost::any, ALLOCV>,
        std::less<std::string>, ALLOCP> &oMap) {
            this->clear();
            int nInds = 0;
            for (auto it = oMap.begin(); it != oMap.end(); ++it) {
                std::string s = (*it).first;
                std::string varid = boost::to_lower_copy(boost::trim_copy(s));
                if (!varid.empty()) {
                    const std::vector<boost::any, ALLOCV> vv = (*it).second;
                    statdata::DataType rtype = statdata::typeOther;
                    size_t n = vv.size();
                    for (size_t i = 0; i < n; ++i) {
                        const boost::any &v = vv[i];
                        if (!v.empty()) {
                            rtype = Value::get_type(v);
                            break;
                        }
                    }// i
                    if (rtype != statdata::typeOther) {
                        if ((size_t)nInds < n) {
                            nInds = n;
                        }
                        Variable var(varid, rtype);
                        var.name(varid);
                        var.index((int) this->m_vars.size());
                        if (rtype == statdata::typeString) {
                            var.set_categ_var(true);
                        }
                        this->m_vars.push_back(var);
                        this->m_data[varid] = vv;
                    }
                }// not empty
            }// it
            for (int i = 0; i < nInds; ++i) {
                std::stringstream os;
                os << "ind" << (i + 1);
                std::string sid = os.str();
                Individu ind(sid);
                ind.index((int) this->m_inds.size());
                this->m_inds.push_back(ind);
            }// i
            for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it) {
                std::vector<boost::any> & vv = (*it).second;
                if (vv.size() < (size_t)nInds) {
                    vv.resize(nInds);
                }
            }// it
        }// set_all_data
        void initialize(size_t nVars, size_t nRows);
        bool change_variable_type(const std::string &sId, statdata::DataType rtype);
        statdata::DataType get_variable_type(const std::string &sId);
    public:

        boost::any operator()(int indindex, int varindex) {
            boost::any v;
            if (this->get_value(indindex, varindex, v)) {
                return (v);
            } else {
                return (this->m_emptyval);
            }
        }
    public:
        void clear(void);
        bool import_csv_file(const std::string &filename);
    protected:
        std::vector<Variable> m_vars;
        std::vector<Individu> m_inds;
        std::map<std::string, std::vector<boost::any> > m_data;
        boost::any m_emptyval;
    }; // class DataSet
    ///////////////////////////////////////
}// namespace statdata
/////////////////////////////////////
#endif	/* DATASET_H */

