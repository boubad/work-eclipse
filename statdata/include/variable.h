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
/////////////////////////////
#include <boost/algorithm/string.hpp>
////////////////////////////////////////
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>
///////////////////////////////////////////
#define STAT_SERIALZE_VERSION   1
/////////////////////////////////////////
namespace statdata {
    /////////////////////////////////////

    class Variable {
    private:
        friend class boost::serialization::access;

        template<class Archive>
        void save(Archive & ar, const unsigned int /*version */) const {
            // note, version is always the latest when saving
            ar & this->m_index;
            ar & this->m_isactive;
            ar & this->m_iscateg;
            ar & this->m_isid;
            ar & this->m_isname;
            ar & this->m_isweight;
            int t = (int) this->m_type;
            ar & t;
            ar & this->m_id;
            ar & this->m_name;
        }

        template<class Archive>
        void load(Archive & ar, const unsigned int version) {
            ar & this->m_index;
            ar & this->m_isactive;
            ar & this->m_iscateg;
            ar & this->m_isid;
            ar & this->m_isname;
            ar & this->m_isweight;
            int t;
            ar & t;
            this->m_type = (statdata::DataType)t;
            ar & this->m_id;
            ar & this->m_name;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()
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
            return (this->m_id < other.m_id);
        }

        bool operator==(const Variable &other) const {
            return (this->m_id == other.m_id);
        }

        bool operator!=(const Variable &other) const {
            return (this->m_id != other.m_id);
        }
    public:

        statdata::DataType get_type(void) const {
            return (this->m_type);
        }

        void set_type(statdata::DataType t) {
            this->m_type = t;
        }

        int index(void) const {
            return (this->m_index);
        }

        void index(int n) {
            this->m_index = n;
        }

        bool is_active_var(void) const {
            return (this->m_isactive);
        }

        void set_active_var(bool b = true) {
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

        void set_weight_var(bool b = true) {
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
            this->m_id = boost::to_lower_copy(boost::trim_copy(s));
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

  //  BOOST_CLASS_VERSION(statdata::Variable, STAT_SERIALZE_VERSION )

    ////////////////////////////////////////

    class Individu {
    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version */) {
            ar & this->m_index;
            ar & this->m_isactive;
            ar & this-> m_issup;
            ar & this-> m_id;
            ar & this-> m_name;
        }
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
            return (this->m_id < other.m_id);
        }

        bool operator==(const Individu &other) const {
            return (this->m_id == other.m_id);
        }

        bool operator!=(const Individu &other) const {
            return (this->m_id != other.m_id);
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

        void set_active_ind(bool b = true) {
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
            this->m_id = boost::to_lower_copy(boost::trim_copy(s));
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
  //  BOOST_CLASS_VERSION(Individu,STAT_SERIALZE_VERSION )
    typedef Individu *PIndividu;
    /////////////////////////////////////////
}// namespace statdata
////////////////////////////////////
#endif	/* VARIABLE_H */

