/*
 * hdf5statdatasetfile.cpp
 *
 *  Created on: 21 oct. 2012
 *      Author: boubad
 */
#include "../../include/hdf5/hdf5statdatasetfile.h"
////////////////////////////////
namespace statdata {
namespace hdf5 {
////////////////////////////////////////////////
const char *HDF5DtatDataSetFile::GRP_INITIAL_NAME = "g_initial";
const char *HDF5DtatDataSetFile::VAR_IDS_NAME = "var_ids";
const char *HDF5DtatDataSetFile::VAR_NAMES_NAME = "var_names";
const char *HDF5DtatDataSetFile::ATT_NB_INDIVS = "nb_indivs";
const char *HDF5DtatDataSetFile::ATT_CATEGVAR = "categ_var";
const char *HDF5DtatDataSetFile::ATT_NUMVAR = "num_var";
//////////////////////////////////////////////
HDF5DtatDataSetFile::HDF5DtatDataSetFile() {

}
HDF5DtatDataSetFile::HDF5DtatDataSetFile(const std::string &filename,
		bool bReadOnly) :
		HDF5File(filename, bReadOnly) {

}
HDF5DtatDataSetFile::HDF5DtatDataSetFile(const HDF5DtatDataSetFile &other) :
		HDF5File(other) {

}
HDF5DtatDataSetFile & HDF5DtatDataSetFile::operator=(
		const HDF5DtatDataSetFile &other) {
	if (this != &other) {
		HDF5File::operator=(other);
	}
	return (*this);
}
HDF5DtatDataSetFile::~HDF5DtatDataSetFile() {

}
void HDF5DtatDataSetFile::get_initialDataGroupName(std::string &s) const {
	s = GRP_INITIAL_NAME;
} //get_initialDataGroupName
bool HDF5DtatDataSetFile::create(const std::string &filename) {
	if (!HDF5File::create(filename)) {
		return (false);
	}
	std::string name;
	this->get_initialDataGroupName(name);
	{
		HDF5Group grp(this, name, true);
		size_t nSize = 0;
		std::string att_name(ATT_NB_INDIVS);
		HDF5Attribute att(&grp, att_name);
		if (!att.write_scalar(nSize)) {
			return (false);
		}
	}
	return (true);
} // create
bool HDF5DtatDataSetFile::open(const std::string &filename, bool bReadOnly) {
	if (!HDF5File::open(filename, bReadOnly)) {
		return (false);
	}
	std::string name;
	this->get_initialDataGroupName(name);
	if (bReadOnly) {
		HDF5Group grp(this, name, false);
		return (grp.exists());
	} else {
		HDF5Group grp(this, name, true);
		size_t nSize = 0;
		std::string att_name(ATT_NB_INDIVS);
		HDF5Attribute att(&grp, att_name);
		if (!att.write_scalar(nSize)) {
			return (false);
		}
	}
	return (true);
} // open
bool HDF5DtatDataSetFile::get_variable_info(const std::string &varname,
		HDF5Object::DataType &type, size_t &nSize, bool &bCateg) const {
	type = HDF5Object::typeOther;
	nSize = 0;
	bCateg = false;
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	std::valarray<size_t> dims;
	HDF5Dataset oSet(&grp, varname);
	if (!oSet.get_info(type, dims)) {
		return (false);
	}
	if (dims.size() < 1) {
		return (false);
	}
	nSize = dims[0];
	std::string att_name(ATT_CATEGVAR);
	HDF5Attribute att(&oSet, att_name);
	int ival = 0;
	if (att.read_scalar(ival)) {
		bCateg = (ival != 0);
	}
	return (true);
} //get_variable_info
bool HDF5DtatDataSetFile::get_nb_indivs(size_t &nSize) const {
	nSize = 0;
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	std::string att_name(ATT_NB_INDIVS);
	HDF5Attribute att(&grp, att_name);
	return (att.read_scalar(nSize));
} //get_nb_indivs
bool HDF5DtatDataSetFile::get_ids_varname(std::string &s) const {
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	std::string att_name(VAR_IDS_NAME);
	HDF5Attribute att(&grp, att_name);
	return (att.read_scalar(s));
}
bool HDF5DtatDataSetFile::set_ids_varname(const std::string &s) {
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	HDF5Dataset oSet(&grp, s);
	if (!oSet.exists()) {
		return (false);
	}
	std::string att_name(VAR_IDS_NAME);
	HDF5Attribute att(&grp, att_name);
	if (!att.write_scalar(s)) {
		return (false);
	}
	int ival = 1;
	std::string att2_name(ATT_CATEGVAR);
	HDF5Attribute att2(&oSet, att2_name);
	return (att2.write_scalar(ival));
}
bool HDF5DtatDataSetFile::get_names_varname(std::string &s) const {
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	std::string att_name(VAR_NAMES_NAME);
	HDF5Attribute att(&grp, att_name);
	return (att.read_scalar(s));
}
bool HDF5DtatDataSetFile::set_names_varname(const std::string &s) {
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	HDF5Dataset oSet(&grp, s);
	if (!oSet.exists()) {
		return (false);
	}
	std::string att_name(VAR_NAMES_NAME);
	HDF5Attribute att(&grp, att_name);
	if (!att.write_scalar(s)) {
		return (false);
	}
	int ival = 1;
	std::string att2_name(ATT_CATEGVAR);
	HDF5Attribute att2(&oSet, att2_name);
	return (att2.write_scalar(ival));
}
bool HDF5DtatDataSetFile::set_categ_var(const std::string &varname) {
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	HDF5Dataset oSet(&grp, varname);
	if (!oSet.exists()) {
		return (false);
	}
	{
		std::string att2_name(ATT_CATEGVAR);
		HDF5Attribute att2(&oSet, att2_name);
		int ival = 1;
		if (!att2.write_scalar(ival)) {
			return (false);
		}
	}
	{
		std::string att2_name(ATT_NUMVAR);
		HDF5Attribute att2(&oSet, att2_name);
		if (att2.exists()) {
			int ival = 0;
			if (!att2.write_scalar(ival)) {
				return (false);
			}
		}
	}
	return (true);
} //set_categ_var
bool HDF5DtatDataSetFile::set_num_var(const std::string &varname) {
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	HDF5Dataset oSet(&grp, varname);
	if (!oSet.exists()) {
		return (false);
	}
	std::valarray<size_t> dims;
	HDF5Object::DataType type = HDF5Object::typeOther;
	if (!oSet.get_info(type, dims)) {
		return (false);
	}
	if (dims.size() < 1) {
		return (false);
	}
	if (type == HDF5Object::typeString){
		return (false);
	}
	{
		std::string att2_name(ATT_NUMVAR);
		HDF5Attribute att2(&oSet, att2_name);
		int ival = 1;
		if (!att2.write_scalar(ival)) {
			return (false);
		}
	}
	{
		std::string att2_name(ATT_CATEGVAR);
		HDF5Attribute att2(&oSet, att2_name);
		if (att2.exists()) {
			int ival = 0;
			if (!att2.write_scalar(ival)) {
				return (false);
			}
		}
	}
	return (true);
} //set_num_var
bool HDF5DtatDataSetFile::write_initialData(const std::string &varname,
		std::vector<boost::any> &data) {
	size_t nSize = data.size();
	if (nSize < 1) {
		return (false);
	}
	boost::any v;
	for (size_t i = 0; i < nSize; ++i) {
		const boost::any &vx = data[i];
		if (!vx.empty()) {
			v = vx;
			break;
		}
	} // i
	if (v.empty()) {
		return (false);
	}
	size_t nOldSize = 0;
	if (!this->get_nb_indivs(nOldSize)) {
		return (false);
	}
	bool bAdjust = false;
	std::vector<boost::any> xdata;
	if (nSize >= nOldSize) {
		xdata = data;
		if (nSize > nOldSize) {
			bAdjust = true;
		}
	} else {
		xdata = std::vector<boost::any>(nOldSize);
		for (size_t i = 0; i < nSize; ++i) {
			xdata[i] = data[i];
		} // i
	}
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	{
		HDF5Dataset oSet(&grp, varname);
		std::valarray<size_t> oCount(1);
		oCount[0] = xdata.size();
		if (!oSet.write_any(data, oCount)) {
			return (false);
		}
		if (v.type() == typeid(std::string)) {
			std::string attname(ATT_CATEGVAR);
			int ival = 1;
			HDF5Attribute att(&oSet, attname);
			if (!att.write_scalar(ival)) {
				return (false);
			}
		}
	}
	if (!bAdjust) {
		return (true);
	}
	std::vector<std::string> allvarnames;
	if (!grp.get_datasets_names(allvarnames)) {
		return (false);
	}
	for (auto it = allvarnames.begin(); it != allvarnames.end(); ++it) {
		const std::string &scur = *it;
		if (scur != varname) {
			HDF5Dataset oSet(&grp, scur);
			HDF5Object::DataType type;
			std::valarray<size_t> dims;
			if (!oSet.get_info(type, dims)) {
				return (false);
			}
			if (dims.size() < 1) {
				return (false);
			}
			if (dims[0] < nSize) {
				size_t nx = (size_t) (nSize - dims[0]);
				std::vector<boost::any> vv(nx);
				std::valarray<size_t> xStart(1);
				xStart[0] = dims[0];
				std::valarray<size_t> xCount(1);
				xCount[0] = nx;
				if (!oSet.write_any(vv, xCount, xStart)) {
					return (false);
				}
			} // reajust
		} // check
	} // it
	std::string att_name(ATT_NB_INDIVS);
	HDF5Attribute att(&grp, att_name);
	return (att.write_scalar(nSize));
} // write_initialData
bool HDF5DtatDataSetFile::read_initialData(const std::string &varname,
		std::vector<boost::any> &data) const {
	std::string grpname;
	this->get_initialDataGroupName(grpname);
	HDF5Group grp(const_cast<HDF5DtatDataSetFile *>(this), grpname);
	if (!grp.exists()) {
		return (false);
	}
	HDF5Dataset oSet(&grp, varname);
	std::valarray<size_t> dims;
	if (!oSet.read_any(data, dims)) {
		return (false);
	}
	return (true);
} // read_initialData
////////////////////////////////////////////////
} // namespace hdf5
} // namespace statdata
