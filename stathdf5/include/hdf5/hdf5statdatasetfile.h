/*
 * hdf5statdatasetfile.h
 *
 *  Created on: 21 oct. 2012
 *      Author: boubad
 */

#ifndef HDF5STATDATASETFILE_H_
#define HDF5STATDATASETFILE_H_
////////////////////////////////////
#include "hdf5file.h"
#include "hdf5group.h"
#include "hdf5attribute.h"
#include "hdf5dataset.h"
//////////////////////////////////
#include <set>
#include <map>
#include <value.h>
/////////////////////////////////////////////
namespace statdata {
namespace hdf5 {
/////////////////////////////////////////////////
class HDF5DtatDataSetFile: public HDF5File {
public:
	HDF5DtatDataSetFile();
	HDF5DtatDataSetFile(const std::string &filename, bool bReadOnly = true);
	HDF5DtatDataSetFile(const HDF5DtatDataSetFile &other);
	HDF5DtatDataSetFile & operator=(const HDF5DtatDataSetFile &other);
	virtual ~HDF5DtatDataSetFile();
public:
	virtual bool create(const std::string &filename);
	virtual bool open(const std::string &filename, bool bReadOnly = true);
public:
	void get_initialDataGroupName(std::string &s) const;
	bool get_nb_indivs(size_t &nSize) const;
	bool get_ids_varname(std::string &s) const;
	bool set_ids_varname(const std::string &s);
	bool get_names_varname(std::string &s) const;
	bool set_names_varname(const std::string &s);
	bool get_variable_info(const std::string &varname,
			HDF5Object::DataType &type, size_t &nSize, bool &bCateg) const;
	bool set_categ_var(const std::string &varname);
	bool set_num_var(const std::string &varname);
public:
	template <class ALLOCP>
	bool get_ids_map(std::map<size_t,std::string,std::less<size_t>,ALLOCP> &oMap){
		oMap.clear();
		std::string varname;
		if (!this->get_ids_varname(varname)){
			return (false);
		}
		std::vector<std::string> vv;
		if (!this->read(varname,vv)){
			return (false);
		}
		size_t n = vv.size();
		for (size_t i = 0; i < n; ++i){
			std::string s = vv[i];
			oMap[i] = s;
		}// i
		return (true);
	}// get_ids_map
	template <class ALLOCV>
	bool get_notempty_indexes(const std::string &varname,
			std::set<size_t, std::less<size_t>,ALLOCV> &oRes)const {
		oRes.clear();
		std::vector<boost::any> vv;
		if (!this->read_initialData(varname,vv)){
			return (false);
		}
		size_t n = vv.size();
		for (size_t i = 0; i < n; ++i){
			const boost::any &v = vv[i];
			if (!v.empty()){
				oRes.insert(i);
			}
		}// i
		return (true);
	}//get_notempty_indexes
	template <class ALLOCS, class ALLOCV>
	bool get_notempty_indexes(const std::vector<std::string,ALLOCS> &names,
			std::set<size_t,std::less<size_t>,ALLOCV> &oRes){
		typedef std::set<size_t,std::less<size_t>, ALLOCV> MySetType;
		oRes.clear();
		for (auto it = names.begin(); it != names.end(); ++it){
			MySetType oCurSet;
			const std::string &varname = *it;
			if (this->get_notempty_indexes(varname,oCurSet)){
				if (oRes.empty()){
					oRes = oCurSet;
				} else {
					MySetType oDel;
					for (auto jt = oRes.begin(); jt != oRes.end(); ++jt){
						size_t iIndex = *jt;
						if (oCurSet.find(iIndex) == oRes.end()){
							oDel.insert(iIndex);
						}
					}// jt
					while (!oDel.empty()){
						size_t iCur = *(oDel.begin());
						auto jt = oRes.find(iCur);
						if (jt != oRes.end()){
							oRes.erase(jt);
						}
					}// not empty
				}
			}// ok
		}// it
		return (true);
	}// get_notempty_indexes
	template<class ALLOCS>
	bool get_allvariables_names(std::vector<std::string, ALLOCS> &names) const {
		names.clear();
		std::string grpname;
		this->get_initialDataGroupName(grpname);
		HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
		if (!grp.exists()) {
			return (false);
		}
		std::vector<std::string> vv;
		if (!grp.get_datasets_names(vv)) {
			return (false);
		}
		size_t n = vv.size();
		names = std::vector<std::string, ALLOCS>(n);
		for (size_t i = 0; i < n; ++i) {
			names[i] = vv[i];
		}
		return (true);
	} // get_allvariables_names
	template<class ALLOCS>
	bool get_categvariables_names(
			std::vector<std::string, ALLOCS> &names) const {
		names.clear();
		std::string grpname;
		this->get_initialDataGroupName(grpname);
		HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
		if (!grp.exists()) {
			return (false);
		}
		std::vector<std::string> vv;
		if (!grp.get_datasets_names(vv)) {
			return (false);
		}
		size_t n = vv.size();
		std::string attname(ATT_CATEGVAR);
		for (size_t i = 0; i < n; ++i) {
			const std::string &varname = vv[i];
			HDF5Dataset oSet(&grp, varname);
			if (oSet.exists()) {
				HDF5Attribute att(&oSet, attname);
				if (att.exists()) {
					int ival = 0;
					if (att.read_scalar(ival)) {
						if (ival != 0) {
							names.push_back(varname);
						}
					} // read
				} // exist
			} // oSet
		} // i
		return (true);
	} // get_categvariables_names
	template<class ALLOCS>
	bool get_numvariables_names(std::vector<std::string, ALLOCS> &names) const {
		names.clear();
		std::string grpname;
		this->get_initialDataGroupName(grpname);
		HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
		if (!grp.exists()) {
			return (false);
		}
		std::vector<std::string> vv;
		if (!grp.get_datasets_names(vv)) {
			return (false);
		}
		size_t n = vv.size();
		std::string attname(ATT_CATEGVAR);
		for (size_t i = 0; i < n; ++i) {
			const std::string &varname = vv[i];
			HDF5Dataset oSet(&grp, varname);
			if (oSet.exists()) {
				HDF5Attribute att(&oSet, attname);
				if (!att.exists()) {
					names.push_back(varname);
				} else {
					int ival = 0;
					if (att.read_scalar(ival)) {
						if (ival == 0) {
							names.push_back(varname);
						}
					} // read
				} // exists
			} // oSet
		} // i
		return (true);
	} // get_categvariables_names
public:
	bool write_initialData(const std::string &varname,
			std::vector<boost::any> &data);
	bool read_initialData(const std::string &varname,
			std::vector<boost::any> &data) const;
public:
	template<typename T, class ALLOCT>
	bool write(const std::string &varname, const std::vector<T, ALLOCT> &data) {
		size_t n = data.size();
		if (n < 1) {
			return (false);
		}
		std::vector<boost::any> vv(n);
		for (size_t i = 0; i < n; ++i) {
			vv[i] = data[i];
		}
		return (this->write_initialData(varname, vv));
	} // write
	template<typename T>
	bool write(const std::string &varname, const std::valarray<T> &data) {
		size_t n = data.size();
		if (n < 1) {
			return (false);
		}
		std::vector<boost::any> vv(n);
		for (size_t i = 0; i < n; ++i) {
			vv[i] = data[i];
		}
		return (this->write_initialData(varname, vv));
	} // write
	template<typename T, class ALLOCT>
	bool read(const std::string &varname, std::vector<T, ALLOCT> &data) {
		data.clear();
		std::vector<boost::any> vv;
		if (!this->read_initialData(varname, vv)) {
			return (false);
		}
		size_t n = vv.size();
		data = std::vector<T, ALLOCT>(n);
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = vv[i];
			if (!vx.empty()) {
				T val;
				if (Value::get_value(vx, val)) {
					data[i] = val;
				}
			}
		}		// i
		return (true);
	}		// read
	template<typename T>
	bool read(const std::string &varname, std::valarray<T> &data) {
		data.clear();
		std::vector<boost::any> vv;
		if (!this->read_initialData(varname, vv)) {
			return (false);
		}
		size_t n = vv.size();
		data.resize(n);
		for (size_t i = 0; i < n; ++i) {
			const boost::any &vx = vv[i];
			if (!vx.empty()) {
				T val;
				if (Value::get_value(vx, val)) {
					data[i] = val;
				}
			}
		}		// i
		return (true);
	}		// read
protected:
	static const char *GRP_INITIAL_NAME;
	static const char *VAR_IDS_NAME;
	static const char *VAR_NAMES_NAME;
	static const char *ATT_NB_INDIVS;
	static const char *ATT_CATEGVAR;
	static const char *ATT_NUMVAR;
};
//////////////////////////////////////////////////
}// namespace hdf5
} // namespace statdata
////////////////////////////////////////
#endif /* HDF5STATDATASETFILE_H_ */
