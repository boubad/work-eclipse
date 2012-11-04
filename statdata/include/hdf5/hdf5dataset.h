/*
 * hdf5dataset.h
 *
 *  Created on: 14 oct. 2012
 *      Author: boubad
 */

#ifndef HDF5DATASET_H_
#define HDF5DATASET_H_
////////////////////////////
#include "hdf5object.h"
////////////////////////////
#include "../statindiv.h"
#include "../statcorrel.h"
/////////////////////////////////
namespace statdata {
namespace hdf5 {
////////////////////////
class HDF5File;
class HDF5Group;
/////////////////////////////////
class HDF5Dataset: public HDF5Object {
public:
	HDF5Dataset(HDF5File *pFile, const std::string &name);
	HDF5Dataset(HDF5Group *pGroup, const std::string &name);
	HDF5Dataset(const HDF5Dataset &other);
	HDF5Dataset & operator=(const HDF5Dataset &other);
	virtual ~HDF5Dataset();
public:
	virtual bool exists(void) const;
	virtual bool close(void);
	virtual bool get_groups_names(std::vector<std::string> &oNames) const;
	virtual bool get_datasets_names(std::vector<std::string> &oNames) const;
public:
	bool set_comment(const std::string &s);
	bool get_comment(std::string &s);
	bool get_info(statdata::DataType &type, std::valarray<size_t> &dims);
public:
	bool write_any(const std::vector<boost::any> &data,
			const std::valarray<size_t> &oCount);
	bool read_any(std::vector<boost::any> &data, std::valarray<size_t> &dims);
	//
public:
	template<typename T, class ALLOCT>
	bool read(std::vector<T, ALLOCT> &data, std::valarray<size_t> &dims) {
		data.clear();
		std::vector<boost::any> vv;
		if (!this->read_any(vv, dims)) {
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
	bool read(std::valarray<T> &data, std::valarray<size_t> &dims) {
		data.clear();
		std::vector<boost::any> vv;
		if (!this->read_any(vv, dims)) {
			return (false);
		}
		size_t n = vv.size();
		data = std::vector<T>(n);
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
	template<typename T, class ALLOCT>
	bool read_vector(std::vector<T, ALLOCT> &data) {
		std::valarray<size_t> dims;
		if (!this->read(data, dims)) {
			return (false);
		}
		return (true);
	}		// read_vector
	template<typename T>
	bool read_vector(std::valarray<T> &data) {
		std::valarray<size_t> dims;
		if (!this->read(data, dims)) {
			return (false);
		}
		return (true);
	}		// read_vector
	template<typename T, class ALLOCT>
	bool read_array(std::vector<T, ALLOCT> &data, size_t &nr, size_t &nc) {
		std::valarray<size_t> dims;
		if (!this->read(data, dims)) {
			return (false);
		}
		if (dims.size() < 2) {
			return (false);
		}
		nr = dims[0];
		nc = dims[1];
		return (true);
	}		// read_array
	template<typename T>
	bool read_array(std::valarray<T> &data, size_t &nr, size_t &nc) {
		std::valarray<size_t> dims;
		if (!this->read(data, dims)) {
			return (false);
		}
		if (dims.size() < 2) {
			return (false);
		}
		nr = dims[0];
		nc = dims[1];
		return (true);
	}		// read_array
public:
	template<typename T, class ALLOCT>
	bool write(const std::vector<T, ALLOCT> &data,
			const std::valarray<size_t> &oCount) {
		size_t n = data.size();
		std::vector<boost::any> oVec(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = boost::any(data[i]);
		}
		return (this->write_any(oVec, oCount));
	} // write
	template<typename T>
	bool write(const std::valarray<T> &data,
			const std::valarray<size_t> &oCount) {
		size_t n = data.size();
		std::vector<boost::any> oVec(n);
		for (size_t i = 0; i < n; ++i) {
			oVec[i] = boost::any(data[i]);
		}
		return (this->write_any(oVec, oCount));
	} // write
	template<typename T, class ALLOCT>
	bool write_vector(const std::vector<T, ALLOCT> &data) {
		std::valarray<size_t> oCount(1);
		oCount[0] = data.size();
		return (this->write(data, oCount));
	} // write_vector
	template<typename T>
	bool write_vector(const std::valarray<T> &data) {
		std::valarray<size_t> oCount(1);
		oCount[0] = data.size();
		return (this->write(data, oCount));
	} // write_vector
	template<typename T, class ALLOCT>
	bool write_array(const std::vector<T, ALLOCT> &data, size_t nr, size_t nv) {
		assert(nr > 0);
		assert(nv > 0);
		std::valarray<size_t> oCount(2);
		oCount[0] = nr;
		oCount[1] = nv;
		return (this->write(data, oCount));
	} // write_array
	template<typename T>
	bool write_array(const std::valarray<T> &data, size_t nr, size_t nv) {
		assert(nr > 0);
		assert(nv > 0);
		std::valarray<size_t> oCount(2);
		oCount[0] = nr;
		oCount[1] = nv;
		return (this->write(data, oCount));
	} // write_array
public:
	bool write(const std::vector<StatIndiv> &oIndivs);
	bool read(std::vector<StatIndiv> &oIndivs);
	bool write(const std::vector<StatCorrel> &data,
			const std::valarray<size_t> &oCount);
	bool read(std::vector<StatCorrel> &data, std::valarray<size_t> &dims);
public:
	template<class ALLOCV>
	bool read_correlations(std::vector<StatCorrel, ALLOCV> &data,
			std::valarray<size_t> &dims) {
		data.clear();
		std::vector<StatCorrel> vv;
		if (!this->read(vv, dims)) {
			return (false);
		}
		size_t n = vv.size();
		data = std::vector<StatCorrel, ALLOCV>(n);
		for (size_t i = 0; i < n; ++i) {
			data[i] = vv[i];
		}
		return (true);
	} // read_correlations
	template<class ALLOCV>
	bool write_correlations(const std::vector<StatCorrel, ALLOCV> &data,
			const std::valarray<size_t> &oCount) {
		size_t n = data.size();
		std::vector<StatCorrel> vv(n);
		for (size_t i = 0; i < n; ++i) {
			vv[i] = data[i];
		}
		return (this->write(vv, oCount));
	} // write_correlations
	template<class ALLOCV>
	bool write_indivs(const std::vector<StatIndiv, ALLOCV> &data) {
		size_t n = data.size();
		std::vector<StatIndiv> vv(n);
		for (size_t i = 0; i < n; ++i) {
			vv[i] = data[i];
		}
		return (this->write(vv));
	} // write_indivs
	template<class ALLOCV>
	bool read_indivs(std::vector<StatIndiv, ALLOCV> &data) {
		data.clear();
		std::vector<StatIndiv> vv;
		if (!this->read(vv)) {
			return (false);
		}
		size_t n = vv.size();
		data = std::vector<StatIndiv, ALLOCV>(n);
		for (size_t i = 0; i < n; ++i) {
			data[i] = vv[i];
		}
		return (true);
	} // read_indivs
protected:
	bool create(statdata::DataType xtype, size_t nRank, const size_t *dims);
	bool open(void);
private:
	static const size_t MAX_STRINGLEN;
};
} /* namespace hdf5 */
} /* namespace statdata */

#endif /* HDF5DATASET_H_ */
