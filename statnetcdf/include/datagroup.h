/* 
 * File:   datagroup.h
 * Author: boubad
 *
 * Created on 8 novembre 2012, 13:34
 */

#ifndef DATAGROUP_H
#define	DATAGROUP_H
//////////////////////////////
#include "../include/datadefs.h"
/////////////////////////////////
#include <boost/utility.hpp>
///////////////////////////////
#include <memory>
#include <valarray>
////////////////////////////////
namespace statdata {
    ///////////////////////////

    class NetCDFDataGroup : private boost::noncopyable {
    public:
        typedef NetCDFDataGroup *PNetCDFDataGroup;
        typedef std::shared_ptr<NetCDFDataGroup> NetCDFDataGroupPtr;
        typedef std::vector<NetCDFDataGroupPtr> NetCDFDataGroups;
    public:
        NetCDFDataGroup(NetCDFDataGroup *pParent = nullptr);
        NetCDFDataGroup(const std::string &filename,
                datafile_open_mode mode = statdata::mode_read,
                bool bClassic = true);
        virtual ~NetCDFDataGroup();
    public:
        static void version(std::string &s);

        bool is_classic(void) const {
            return (this->m_bclassic);
        }
        bool is_netcdf4(void);

        bool is_modified(void) const {
            return (this->m_modified);
        }

        bool is_open(void) const {
            return (this->m_mode != mode_notopen);
        }
        bool has_error(void) const;

        const std::string &name(void) const {
            return (this->m_name);
        }

        const std::string &fullname(void) const {
            return (this->m_fullname);
        }

        NetCDFDataGroup *root(void) {
            NetCDFDataGroup *pRet = this;
            while (pRet->m_parent != nullptr) {
                pRet = pRet->m_parent;
            }
            return (pRet);
        }

        NetCDFDataGroup *parent(void) const {
            return (this->m_parent);
        }
        bool get_chidren(NetCDFDataGroups &children);
        bool create_child(const std::string &sname, NetCDFDataGroup &oChild);
        bool get_child(const std::string &sname, NetCDFDataGroup &oChild);

        int get_last_status(void) const {
            return (this->m_laststatus);
        }

        const std::string & get_last_errorstring(void) const {
            return (this->m_lasterror);
        }
        bool write_history(const std::string &app, const std::string &user);
        //
        //
        bool get_attributes(const std::string &varnames, NetCDFAttributes &attrs);

        bool get_attributes(NetCDFAttributes &attrs) {
            std::string s;
            return this->get_attributes(s, attrs);
        }
        bool read_attribute(const std::string &varname, const std::string &attname,
                Values &vals);

        bool read_attribute(const std::string &attname, Values &vals) {
            std::string s;
            return (this->read_attribute(s, attname, vals));
        }
        bool write_attribute(const std::string &varname, const std::string &attname,
                const Values &vals);

        bool write_attribute(const std::string &attname, const Values &vals) {
            std::string s;
            return (this->write_attribute(s, attname, vals));
        }
        bool remove_attribute(const std::string &varname,
                const std::string &attname);
        bool has_attribute(const std::string &varname, const std::string &attname);

        bool has_attribute(const std::string &attname) {
            std::string s;
            return (this->has_attribute(s, attname));
        }
        //
        bool get_all_axis(NetCDFDimensions &dims);
        bool get_dimension(const std::string &name, NetCDFDimension &oInfo);
        bool create_dimension(NetCDFDimension &oInfo, const std::string &sname,
                size_t nLen = 0);
        bool get_dimensions(NetCDFDimensions &oVec);
        bool get_unlimited_dimensions(NetCDFDimensions &oVec);
        bool rename_dimension(const std::string &sOldName,
                const std::string &sNewName);
        //
        bool create_axis(const std::string &sname, const std::string &sunits,
                double vMin, double vMax, double delta);
        bool create_axis(const std::string &sname, const std::string &sunits,
                float vMin, float vMax, float delta);
        bool create_axis(const std::string &sname, const std::string &sunits,
                int vMin, int vMax, int delta);
        bool create_axis(const std::string &sname, const std::string &sunits,
                short vMin, short vMax, short delta);
        bool create_axis(const std::string &sname, const std::string &sunits,
                byte vMin, byte vMax, byte delta);
        //
        bool create_axis(const std::string &sname, const statdata::DataType &vtype);

        bool create_double_axis(const std::string &sname) {
            statdata::DataType vtype = statdata::typeDouble;
            return (this->create_axis(sname, vtype));
        }

        bool create_float_axis(const std::string &sname) {
            statdata::DataType vtype = statdata::typeFloat;
            return (this->create_axis(sname, vtype));
        }

        bool create_int_axis(const std::string &sname) {
            statdata::DataType vtype = statdata::typeInt;
            return (this->create_axis(sname, vtype));
        }

        bool create_short_axis(const std::string &sname) {
            statdata::DataType vtype = statdata::typeShort;
            return (this->create_axis(sname, vtype));
        }

        bool create_byte_axis(const std::string &sname) {
            statdata::DataType vtype = statdata::typeByte;
            return (this->create_axis(sname, vtype));
        }
        bool create_axis(const std::string &sname, const std::string &sunits,
                const Values &values);

        bool create_axis(const std::string &sname, const std::string &sunits,
                const std::vector<byte> &values) {
            const size_t n = values.size();
            Values vv(n);
            for (size_t i = 0; i < n; ++i) {
                vv[i] = values[i];
            }
            return (this->create_axis(sname, sunits, vv));
        }

        bool create_axis(const std::string &sname, const std::string &sunits,
                const std::vector<short> &values) {
            const size_t n = values.size();
            Values vv(n);
            for (size_t i = 0; i < n; ++i) {
                vv[i] = values[i];
            }
            return (this->create_axis(sname, sunits, vv));
        }

        bool create_axis(const std::string &sname, const std::string &sunits,
                const std::vector<int> &values) {
            const size_t n = values.size();
            Values vv(n);
            for (size_t i = 0; i < n; ++i) {
                vv[i] = values[i];
            }
            return (this->create_axis(sname, sunits, vv));
        }

        bool create_axis(const std::string &sname, const std::string &sunits,
                const std::vector<double> &values) {
            const size_t n = values.size();
            Values vv(n);
            for (size_t i = 0; i < n; ++i) {
                vv[i] = values[i];
            }
            return (this->create_axis(sname, sunits, vv));
        }

        bool create_axis(const std::string &sname, const std::string &sunits,
                const std::vector<float> &values) {
            const size_t n = values.size();
            Values vv(n);
            for (size_t i = 0; i < n; ++i) {
                vv[i] = values[i];
            }
            return (this->create_axis(sname, sunits, vv));
        }
        //
        bool read_data(const std::string &varname, std::valarray<size_t> &index,
                std::valarray<size_t> &sizes, Values &values);
        bool read_one_data(const std::string &varname, std::valarray<size_t> &index,
                boost::any &val);
        bool write_data(const std::string &varname,
                const std::valarray<size_t> &index,
                const std::valarray<size_t> &sizes, const Values &values);
        bool rename_variable(const std::string &oldname,
                const std::string &newname);
        bool set_metadata(const std::string &varname, int nFlags, NetCDFVariable &oInfo);
        bool create_variable(NetCDFVariable &oInfo);
        bool get_variable(const std::string &varname, NetCDFVariable &oInfo);
        bool get_variable_shape(const std::string &varname, statdata::DataType &type,
                std::valarray<size_t> &sizes);
        bool get_variables(NetCDFVariables &vars);
        bool get_variable_fill(const std::string &varname, boost::any &val);
        bool check_variable_data(const std::string &varname, Values &vals);
        bool check_variable_data(const std::string &varname, boost::any &val);
        //
        std::ostream & writeTo(std::ostream &os);
        std::wostream & writeTo(std::wostream &os);
    public:
        virtual bool close(void);
        virtual bool open(const std::string &filename,
                datafile_open_mode mode = statdata::mode_read, bool bClassic = true);
    protected:
        bool set_define_mode(void);
        bool set_data_mode(void);
    protected:

        int id(void) const {
            return (this->m_id);
        }

        void id(int n) {
            this->m_id = n;
        }

        void set_modified(void) {
            if ((this->m_mode != mode_notopen) && (this->m_mode != mode_read)) {
                this->m_modified = true;
                if (this->m_parent != nullptr) {
                    this->m_parent->set_modified();
                }
            }
        }

        bool is_history_written(void) const {
            return (this->m_history);
        }

        void set_history_written(bool b = true) {
            this->m_history = b;
        }

        void set_last_status(int n) {
            this->m_laststatus = n;
        }

        void set_last_errorstring(const std::string &s) {
            this->m_lasterror = s;
        }
        void handle_error(int status);
        bool open_groupe(const std::string &fname);
        bool open_file(const std::string &filename,
                datafile_open_mode mode = statdata::mode_read, bool bClassic = true);
        bool get_string_axis(NetCDFDimension &oDim);
    private:
        bool m_bclassic;
        bool m_bdefinemode;
        statdata::datafile_open_mode m_mode;
        bool m_modified;
        bool m_history;
        int m_id;
        int m_laststatus;
        NetCDFDataGroup *m_parent;
        std::string m_name;
        std::string m_lasterror;
        std::string m_fullname;
    public:

        static const char *get_c_format_attribute_name(void) {
            return ATT_C_FORMAT;
        }

        static const char *get_missing_value_attribute_name(void) {
            return ATT_MISSING_VALUE;
        }

        static const char *get_long_name_attribute_name(void) {
            return ATT_LONG_NAME;
        }

        static const char *get_valid_min_attribute_name(void) {
            return ATT_VALID_MIN;
        }

        static const char *get_valid_max_attribute_name(void) {
            return ATT_VALID_MAX;
        }

        static const char *get_history_attribute_name(void) {
            return ATT_HISTORY;
        }

        static const char *get_title_attribute_name(void) {
            return ATT_TITLE;
        }

        static const char *get_units_attribute_name(void) {
            return ATT_C_FORMAT;
        }

        static const char *get_valid_range_attribute_name(void) {
            return ATT_VALID_RANGE;
        }

        static const char *get_fill_value_attribute_name(void) {
            return ATT_FILL_VALUE;
        }
    protected:
        static const char *ATT_C_FORMAT;
        static const char *ATT_MISSING_VALUE;
        static const char *ATT_LONG_NAME;
        static const char *ATT_VALID_MIN;
        static const char *ATT_VALID_MAX;
        static const char *ATT_HISTORY;
        static const char *ATT_TITLE;
        static const char *ATT_UNITS;
        static const char *ATT_VALID_RANGE;
        static const char *ATT_FILL_VALUE;
        static const char *DIM_STRINGAXIS;
        static const size_t MAX_STRING_LEN;
    };
    // class NetCDFDataGroup
    ////////////////////
}// namespace statdata
/////////////////////////////

inline std::ostream & operator<<(std::ostream &os, statdata::NetCDFDataGroup &d) {
    return d.writeTo(os);
}
///////////////////////////////


#endif	/* DATAGROUP_H */

