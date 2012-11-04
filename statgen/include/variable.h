/* 
 * File:   variable.h
 * Author: boubad
 *
 * Created on 4 novembre 2012, 15:42
 */

#ifndef VARIABLE_H
#define	VARIABLE_H
/////////////////////////////////////
#include "dataport.h"
////////////////////////////////////
#include <string>
#include <memory>
/////////////////////////////////////////
#include <vector>
#include <map>
//////////////////////////////////////
namespace statdata {
    /////////////////////////////////////

    class Variable {
    public:

        Variable() : m_index(-1), m_isactive(true), m_iscateg(false), m_isid(false), m_isname(false),
        m_isweight(false), m_type(statdata::typeOther) {
        }

        Variable(const std::string &sId, statdata::DataType t) :
        m_index(-1), m_isactive(true),
        m_iscateg(false), m_isid(false), m_isname(false),
        m_isweight(false), m_type(t), m_id(sId), m_name(sId) {
        }

        Variable(const Variable &other) : m_index(other.m_index),
        m_isactive(other.m_isactive),
        m_iscateg(other.m_iscateg), m_isid(other.m_isid), m_isname(false),
        m_isweight(other.m_isweight), m_type(other.m_type),
        m_id(other.m_id), m_name(other.m_name) {
        }

        Variable & operator=(const Variable &other) {
            if (this != &other) {
                this->m_index = other.m_index;
                this->m_isactive = other.m_isactive;
                this->m_iscateg = other.m_iscateg;
                this->m_isid = other.m_isid;
                this->m_isname = other.m_isname;
                this->m_isweight = other.m_isweight;
                this->m_type = other.m_type;
                this->m_id = other.m_id;
                this->m_name = other.m_name;
            }
            return (*this);
        }

        virtual ~Variable() {
        }

        bool operator<(const Variable &other) const {
            return (this->m_index < other.m_index);
        }

        bool operator==(const Variable &other) const {
            return (this->m_index == other.m_index);
        }

        bool operator!=(const Variable &other) const {
            return (this->m_index != other.m_index);
        }
    public:

        int index(void) const {
            return (this->m_index);
        }

        void index(int n) {
            this->m_index = n;
        }

        bool is_active_var(void) const {
            return (this->m_isactive);
        }

        bool set_active_var(bool b = true) {
            this->m_isactive = b;
        }

        bool is_categ_var(void) const {
            return (this->m_iscateg);
        }

        void set_categ_var(bool b = true) {
            this->m_iscateg = b;
            if (!this->m_iscateg) {
                this->m_isid = false;
                this->m_isname = false;
            }
            if (this->m_iscateg) {
                this->m_isweight = false;
            }
        }

        bool is_num_var(void) const {
            return (!this->m_iscateg);
        }

        void set_num_var(bool b = true) {
            this->m_iscateg = !b;
            if (!this->m_iscateg) {
                this->m_isid = false;
                this->m_isname = false;
            }
            if (this->m_iscateg) {
                this->m_isweight = false;
            }
        }

        bool is_id_var(void) const {
            return (this->m_isid);
        }

        void set_id_var(bool b = true) {
            this->m_isid = b;
            if (this->m_isname) {
                this->m_iscateg = true;
                this->m_isweight = false;
            }
        }

        bool is_name_var(void) const {
            return (this->m_isname);
        }

        void set_name_var(bool b = true) {
            this->m_isname = b;
            if (this->m_isname) {
                this->m_iscateg = true;
                this->m_isweight = false;
            }
        }

        bool is_weight_var(void) const {
            return (this->m_isweight);
        }

        bool set_weight_var(bool b = true) {
            this->m_isweight = b;
            if (this->m_isweight) {
                this->m_iscateg = false;
                this->m_isid = false;
                this->m_isname = false;
            }
        }

        const std::string &id(void) const {
            return (this->m_id);
        }

        void id(const std::string &s) {
            this->m_id = s;
        }

        const std::string &name(void) const {
            return (this->m_name);
        }

        void name(const std::string &s) {
            this->m_name = s;
        }
    protected:
        int m_index;
        bool m_isactive;
        bool m_iscateg;
        bool m_isid;
        bool m_isname;
        bool m_isweight;
        statdata::DataType m_type;
        std::string m_id;
        std::string m_name;
    }; // class Variable
    typedef Variable *PVariable;
    typedef std::shared_ptr<Variable> VariablePtr;
    ////////////////////////////////////////

    class Individu {
    public:

        Individu() : m_index(-1), m_isactive(true), m_issup(false) {
        }

        Individu(const std::string &sId) :
        m_index(-1), m_isactive(true),
        m_issup(false), m_id(sId), m_name(sId) {
        }

        Individu(const Individu &other) : m_index(other.m_index),
        m_isactive(other.m_isactive),
        m_issup(other.m_issup), m_id(other.m_id), m_name(other.m_name) {
        }

        Individu & operator=(const Individu &other) {
            if (this != &other) {
                this->m_index = other.m_index;
                this->m_isactive = other.m_isactive;
                this->m_issup = other.m_issup;
                this->m_id = other.m_id;
                this->m_name = other.m_name;
            }
            return (*this);
        }

        virtual ~Individu() {
        }

        bool operator<(const Individu &other) const {
            return (this->m_index < other.m_index);
        }

        bool operator==(const Individu &other) const {
            return (this->m_index == other.m_index);
        }

        bool operator!=(const Individu &other) const {
            return (this->m_index != other.m_index);
        }
    public:

        int index(void) const {
            return (this->m_index);
        }

        void index(int n) {
            this->m_index = n;
        }

        bool is_active_ind(void) const {
            return (this->m_isactive);
        }

        bool set_active_ind(bool b = true) {
            this->m_isactive = b;
        }

        bool is_sup_ind(void) const {
            return (this->m_issup);
        }

        void set_sup_ind(bool b = true) {
            this->m_issup = b;
        }

        const std::string &id(void) const {
            return (this->m_id);
        }

        void id(const std::string &s) {
            this->m_id = s;
        }

        const std::string &name(void) const {
            return (this->m_name);
        }

        void name(const std::string &s) {
            this->m_name = s;
        }
    protected:
        int m_index;
        bool m_isactive;
        bool m_issup;
        std::string m_id;
        std::string m_name;
    }; // class Individu
    typedef Individu *PIndividu;
    typedef std::shared_ptr<Individu> IndividuPtr;
    /////////////////////////////////////////
}// namespace statdata
////////////////////////////////////
#endif	/* VARIABLE_H */

