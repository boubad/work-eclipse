/* 
 * File:   datadefs.h
 * Author: boubad
 *
 * Created on 8 novembre 2012, 13:19
 */

#ifndef DATADEFS_H
#define	DATADEFS_H
//////////////////////////////////////
#include <value.h>
#include <iostream>
#include <memory>
#include <algorithm>
/////////////////////////////////////
namespace statdata {
    typedef char *PChar;
    typedef const char *PConstChar;
    ////////////////////////////////////////
    typedef std::unique_ptr<char> CharPtr;
    typedef std::unique_ptr<PChar> PCharPtr;
    typedef std::unique_ptr<byte> BytePtr;
    typedef std::unique_ptr<short> ShortPtr;
    typedef std::unique_ptr<int> IntPtr;
    typedef std::unique_ptr<float> FloatPtr;
    typedef std::unique_ptr<double> DoublePtr;
    //////////////////////////////////////

    class NetCDFDataInfo {
    public:

        NetCDFDataInfo() :
        id(-1000) {
        }

        NetCDFDataInfo(const std::string &sname, int nId = -1000) :
        id(nId), name(sname) {
        }

        NetCDFDataInfo(const NetCDFDataInfo &other) :
        id(other.id), name(other.name) {
        }

        NetCDFDataInfo & operator=(const NetCDFDataInfo &other) {
            if (this != &other) {
                this->id = other.id;
                this->name = other.name;
            }
            return (*this);
        }

        virtual ~NetCDFDataInfo() {
        }
    public:

        bool operator==(const NetCDFDataInfo &other) const {
            return (this->name == other.name);
        }

        bool operator<(const NetCDFDataInfo &other) const {
            return (this->name < other.name);
        }

        virtual void clean(void) {
            this->id = -1000;
            this->name.clear();
        }

        virtual bool is_valid(void) const {
            return (!this->name.empty());
        }

        void clear(void) {
            this->clean();
        }

        virtual std::ostream & writeTo(std::ostream &os) const {
            os << "ID: " << this->id << ",Name: " << this->name;
            return (os);
        }

        virtual std::wostream & writeTo(std::wostream &os) const {
            os << L"ID: " << this->id << L",Name: ";
            if (!this->name.empty()) {
                std::wstring ss(this->name.length(), L' ');
                std::copy(this->name.begin(), this->name.end(), ss.begin());
                os << ss;
            }
            return (os);
        }

        static void valueToString(const statdata::DataType &t, std::string &s) {
            s = "UNKNOWN";
            switch (t) {
                case statdata::typeString:
                    s = "STRING";
                    break;
                case statdata::typeByte:
                    s = "BYTE";
                    break;
                case statdata::typeShort:
                    s = "SHORT";
                    break;
                case statdata::typeInt:
                    s = "INT";
                    break;
                case statdata::typeFloat:
                    s = "FLOAT";
                    break;
                case statdata::typeDouble:
                    s = "DOUBLE";
                    break;
                default:
                    break;
            } // t
        }

        static void valueToString(const statdata::DataType &t, std::wstring &s) {
            s = L"UNKNOWN";
            switch (t) {
                case statdata::typeString:
                    s = L"STRING";
                    break;
                case statdata::typeByte:
                    s = L"BYTE";
                    break;
                case statdata::typeShort:
                    s = L"SHORT";
                    break;
                case statdata::typeInt:
                    s = L"INT";
                    break;
                case statdata::typeFloat:
                    s = L"FLOAT";
                    break;
                case statdata::typeDouble:
                    s = L"DOUBLE";
                    break;
                default:
                    break;
            } // t
        }
    public:
        int id;
        std::string name;
    };
} // namespace statdata
////////////////////////////////////

inline std::ostream & operator<<(std::ostream &os,
        const statdata::NetCDFDataInfo &v) {
    return v.writeTo(os);
}

inline std::wostream & operator<<(std::wostream &os,
        const statdata::NetCDFDataInfo &v) {
    return v.writeTo(os);
}
////////////////////////////////////
namespace statdata {
    ///////////////////////////////////

    class NetCDFDimension : public NetCDFDataInfo {
    public:

        NetCDFDimension() :
        len(0), is_unlimited(false) {
        }

        NetCDFDimension(const std::string &sname, int nId = -1000, size_t nLen = 0,
                bool bUnlimited = false) :
        NetCDFDataInfo(sname, nId), len(nLen), is_unlimited(bUnlimited) {
        }

        NetCDFDimension(const NetCDFDimension &other) :
        NetCDFDataInfo(other), len(other.len), is_unlimited(other.is_unlimited) {
        }

        NetCDFDimension & operator=(const NetCDFDimension &other) {
            if (this != &other) {
                NetCDFDataInfo::operator=(other);
                this->len = other.len;
                this->is_unlimited = other.is_unlimited;
            }
            return (*this);
        }

        virtual ~NetCDFDimension() {
        }
    public:

        virtual bool is_valid(void) const {
            if (!NetCDFDataInfo::is_valid()) {
                return (false);
            }
            if (!(this->is_unlimited)) {
                return ((this->len > 0) && (this->id >= 0));
            } else {
                return (this->id >= 0);
            }
        }

        virtual void clean(void) {
            NetCDFDataInfo::clean();
            this->len = 0;
            this->is_unlimited = false;
        }

        virtual std::ostream & writeTo(std::ostream &os) const {
            os << "{ ";
            NetCDFDataInfo::writeTo(os);
            os << ",len: " << len;
            if (is_unlimited) {
                os << ",UNLIMITED";
            }
            os << " }" << std::endl;
            return (os);
        }

        virtual std::wostream & writeTo(std::wostream &os) const {
            os << L"{ ";
            NetCDFDataInfo::writeTo(os);
            os << L",len: " << len;
            if (is_unlimited) {
                os << L",UNLIMITED";
            }
            os << L" }" << std::endl;
            return (os);
        }
    public:
        size_t len;
        bool is_unlimited;
    };
    // class NetCDFDimension
    typedef std::vector<NetCDFDimension> NetCDFDimensions;
    //////////////////////////////////////

    class NetCDFAttribute : public NetCDFDataInfo {
    public:

        NetCDFAttribute() :
        var_id(-1000), type(statdata::typeOther), len(0) {
        }

        NetCDFAttribute(const std::string &sname, statdata::DataType sType,
                int nId = -1000,
                int nVarId = -1000, size_t nLen = 1) :
        NetCDFDataInfo(sname, nId), var_id(nVarId), type(sType), len(nLen) {
        }

        NetCDFAttribute(const std::string &sname, statdata::DataType sType,
                const Values &vals) :
        NetCDFDataInfo(sname), var_id(-1000), type(sType), len(vals.size()), values(
        vals) {
        }

        NetCDFAttribute(const NetCDFAttribute &other) :
        NetCDFDataInfo(other), var_id(other.var_id), type(other.type), len(
        other.len), values(other.values) {
        }

        NetCDFAttribute & operator=(const NetCDFAttribute &other) {
            if (this != &other) {
                NetCDFDataInfo::operator=(other);
                this->var_id = other.var_id;
                this->type = other.type;
                this->len = other.len;
                this->values = other.values;
            }
            return (*this);
        }

        virtual ~NetCDFAttribute() {
        }
    public:

        virtual void clean(void) {
            NetCDFDataInfo::clean();
            this->var_id = -1000;
            this->type = statdata::typeOther;
            this->len = 0;
            this->values.clear();
        }

        virtual std::ostream & writeTo(std::ostream &os) const {
            os << "{ ";
            NetCDFDataInfo::writeTo(os);
            os << ",\tNetCDFVariable ID: " << var_id;
            std::string s;
            NetCDFDataInfo::valueToString(type, s);
            os << ",Type: " << s;
            os << ",Len: " << 1;
            os << ",Values [";
            size_t n = values.size();
            for (size_t i = 0; i < n; ++i) {
                if (i != 0) {
                    os << ",";
                }
                Value::to_stream(os, values[i]);
            }
            os << "]";
            os << " }" << std::endl;
            return (os);
        }

        virtual std::wostream & writeTo(std::wostream &os) const {
            os << L"{ ";
            NetCDFDataInfo::writeTo(os);
            os << L",\tNetCDFVariable ID: " << var_id;
            std::wstring s;
            NetCDFDataInfo::valueToString(type, s);
            os << L",Type: " << s;
            os << L",Len: " << 1;
            os << L",Values [";
            size_t n = values.size();
            for (size_t i = 0; i < n; ++i) {
                if (i != 0) {
                    os << L",";
                }
                Value::to_stream(os, values[i]);
            }
            os << L"]";
            os << L" }" << std::endl;
            return (os);
        }
    public:
        int var_id;
        statdata::DataType type;
        size_t len;
        Values values;
    };
    // class NetCDFAttribute
    typedef std::vector<NetCDFAttribute> NetCDFAttributes;
    ////////////////////////////////////

    class NetCDFVariable : public NetCDFDataInfo {
    public:

        NetCDFVariable() :
        categ_var(false), id_var(false), name_var(false), type(statdata::typeOther) {
        }

        NetCDFVariable(const std::string &sname, const statdata::DataType &stype) :
        NetCDFDataInfo(sname), categ_var(false), id_var(false), name_var(false), type(
        stype) {
        }

        NetCDFVariable(const NetCDFVariable &other) :
        NetCDFDataInfo(other), categ_var(other.categ_var), id_var(other.id_var), name_var(
        other.name_var), type(other.type), long_name(
        other.long_name), title(other.title), c_format(
        other.c_format), units(other.units), valid_min(
        other.valid_min), valid_max(other.valid_max), missing_value(
        other.missing_value), dims(other.dims), attrs(other.attrs) {
        }

        NetCDFVariable & operator=(const NetCDFVariable &other) {
            if (this != &other) {
                NetCDFDataInfo::operator=(other);
                this->type = other.type;
                this->long_name = other.long_name;
                this->title = other.title;
                this->c_format = other.c_format;
                this->units = other.units;
                this->valid_min = other.valid_min;
                this->valid_max = other.valid_max;
                this->missing_value = other.missing_value;
                this->dims = other.dims;
                this->attrs = other.attrs;
                this->categ_var = other.categ_var;
                this->id_var = other.id_var;
                this->name_var = other.name_var;
            }
            return (*this);
        }

        virtual ~NetCDFVariable() {
        }

        virtual std::ostream & writeTo(std::ostream &os) const {
            os << "{ ";
            NetCDFDataInfo::writeTo(os);
            std::string s;
            NetCDFDataInfo::valueToString(this->type, s);
            os << ",Type: " << s;
            if (!this->long_name.empty()) {
                os << ",Long_name: " << this->long_name;
            }
            if (!this->title.empty()) {
                os << ",Title: " << this->title;
            }
            if (this->categ_var) {
                os << ",CATEG_VAR ";
            }
            if (this->id_var) {
                os << ",ID_VAR ";
            }
            if (this->name_var) {
                os << ",NAME_VAR ";
            }
            if (!this->c_format.empty()) {
                os << ",C_format: " << this->c_format;
            }
            if (!this->units.empty()) {
                os << ",Units: " << this->units;
            }
            if (!this->valid_min.empty()) {
                os << ",Valid_min: ";
                Value::to_stream(os, this->valid_min);
            }
            if (!this->valid_max.empty()) {
                os << ",Valid_max: ";
                Value::to_stream(os, this->valid_max);
            }
            if (!this->missing_value.empty()) {
                os << ",Missing_value: ";
                Value::to_stream(os, this->missing_value);
            }
            os << ",NetCDFDimensions:";
            size_t n = this->dims.size();
            for (size_t i = 0; i < n; ++i) {
                os << "\t" << this->dims[i];
            }
            os << ",\tNetCDFAttributes:";
            n = this->attrs.size();
            for (size_t i = 0; i < n; ++i) {
                os << "\t" << this->attrs[i];
            }
            os << " }" << std::endl;
            return (os);
        }

        virtual std::wostream & writeTo(std::wostream &os) const {
            os << L"{ ";
            NetCDFDataInfo::writeTo(os);
            std::wstring s;
            NetCDFDataInfo::valueToString(this->type, s);
            os << L",Type: " << s;
            if (!this->long_name.empty()) {
                std::wstring ss(this->long_name.length(), L' ');
                std::copy(this->long_name.begin(), this->long_name.end(), ss.begin());
                os << L",Long_name: " << ss;
            }
            if (!this->title.empty()) {
                std::wstring ss(this->title.length(), L' ');
                std::copy(this->title.begin(), this->title.end(), ss.begin());
                os << L",Title: " << ss;
            }
            if (this->categ_var) {
                os << L",CATEG_VAR ";
            }
            if (this->id_var) {
                os << L",ID_VAR ";
            }
            if (this->name_var) {
                os << L",NAME_VAR ";
            }
            if (!this->c_format.empty()) {
                std::wstring ss(this->c_format.length(), L' ');
                std::copy(this->c_format.begin(), this->c_format.end(), ss.begin());
                os << L",C_format: " << ss;
            }
            if (!this->units.empty()) {
                std::wstring ss(this->units.length(), L' ');
                std::copy(this->units.begin(), this->units.end(), ss.begin());
                os << L",Units: " << ss;
            }
            if (!this->valid_min.empty()) {
                os << L",Valid_min: ";
                Value::to_stream(os, this->valid_min);
            }
            if (!this->valid_max.empty()) {
                os << L",Valid_max: ";
                Value::to_stream(os, this->valid_max);
            }
            if (!this->missing_value.empty()) {
                os << L",Missing_value: ";
                Value::to_stream(os, this->missing_value);
            }
            os << L",NetCDFDimensions:";
            size_t n = this->dims.size();
            for (size_t i = 0; i < n; ++i) {
                os << L"\t" << this->dims[i];
            }
            os << L",\tNetCDFAttributes:";
            n = this->attrs.size();
            for (size_t i = 0; i < n; ++i) {
                os << L"\t" << this->attrs[i];
            }
            os << L" }" << std::endl;
            return (os);
        }
    public:
        bool categ_var;
        bool id_var;
        bool name_var;
        statdata::DataType type;
        std::string long_name;
        std::string title;
        std::string c_format;
        std::string units;
        boost::any valid_min;
        boost::any valid_max;
        boost::any missing_value;
        NetCDFDimensions dims;
        NetCDFAttributes attrs;
    };
    // class NetCDFVariable
    typedef std::vector<NetCDFVariable> NetCDFVariables;
    ///////////////////////////////////
}// namespace statdata
/////////////////////////////
#endif	/* DATADEFS_H */
