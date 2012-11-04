/* 
 * File:   dataset.h
 * Author: boubad
 *
 * Created on 4 novembre 2012, 17:59
 */

#ifndef DATASET_H
#define	DATASET_H
////////////////////////////////
#include "value.h"
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
        DataSet(const std::string &filename);
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
        Variable *add_variable(const std::string &sId,const std::string &sName);
        void get_variables_ids(std::vector<std::string> &names) const;
        bool remove_variable(int nIndex);
        bool remove_variable(const std::string &sId){
            int nIndex = this->get_variable_index(sId);
            if ((nIndex >= 0) && (nIndex < (int)this->m_vars.size())){
                return (this->remove_variable(nIndex));
            }
            return (false);
        }
        //
        int get_indiv_index(const std::string &sId) const;
        const Individu *get_indiv(const std::string &sId) const;
        const Individu *get_indiv(int nIndex) const;
        Individu *add_indiv(const std::string &sId,const std::string &sName);
        void get_indivs_ids(std::vector<std::string> &names) const;
        bool remove_indiv(int nIndex);
        bool remove_indiv(const std::string &sId){
            int nIndex = this->get_indiv_index(sId);
            if ((nIndex >= 0) && (nIndex < (int)this->m_inds.size())){
                return (this->remove_indiv(nIndex));
            }
            return (false);
        }
        //
        std::vector<boost::any> * variable_data(const std::string &sId);
        bool indiv_data(const std::string &sId, std::map<std::string,boost::any> &oMap);
        bool get_value(const std::string &sIndId, const std::string &sVarId,
        boost::any &val);
        bool set_value(const std::string &sIndId, const std::string &sVarId,
        const boost::any &val);
        bool get_value(int iIndIndex, int iVarIndex,boost::any &val);
        bool set_value(int iIndIndex, int iVarIndex, const boost::any &val);
    public:
        void clear(void);
        bool import_csv_file(const std::string &filename);
    protected:
        std::vector<Variable> m_vars;
        std::vector<Individu> m_inds;
        std::map<std::string,std::vector<boost::any> > m_data;
    };// class DataSet
    ///////////////////////////////////////
}// namespace statdata
/////////////////////////////////////
#endif	/* DATASET_H */

