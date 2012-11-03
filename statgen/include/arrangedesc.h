/*
 * arrangedesc.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef ARRANGEDESC_H_
#define ARRANGEDESC_H_
/////////////////////////////////////////
#include <vector>
#include <set>
#include <list>
///////////////////////////////////////////
namespace statdata {
//////////////////////////////////////////////
template<typename T = double, typename Z = int,
		class ALLOCZ = std::allocator<Z>, class ALLOCVECTORZ = std::allocator<
				std::list<Z, ALLOCZ> > >
class ArrangeResult {
public:
	typedef T CriteriaType;
	typedef Z IndexType;
	typedef std::list<Z, ALLOCZ> IndexListType;
	typedef std::vector<std::list<Z, ALLOCZ>, ALLOCVECTORZ> IndexListVectorType;
	typedef ArrangeResult<T, Z, ALLOCZ, ALLOCVECTORZ> ArrangeResultType;
public:
	ArrangeResult() :
			m_crit(0) {
	}
	ArrangeResult(const ArrangeResultType &other) {
		this->m_crit = other.m_crit;
		this->m_vec = other.m_vec;
	}
	ArrangeResultType & operator=(const ArrangeResultType &other) {
		if (this != &other) {
			this->m_crit = other.m_crit;
			this->m_vec = other.m_vec;
		}
		return (*this);
	}
	virtual ~ArrangeResult() {
	}
public:
	CriteriaType criteria(void) const {
		return (this->m_crit);
	} // criteria
	const IndexListVectorType & indexes(void) const {
		return (this->m_vec);
	} // indexes
	CriteriaType get_criteria(bool &bEmpty) {
		bEmpty = this->m_vec.empty();
		return (this->m_crit);
	} // get_criteria
	void add_result(const ArrangeResult &oRes) {
		CriteriaType c = oRes.criteria();
		for (auto it = oRes.m_vec.begin(); it != oRes.m_vec.end(); ++it) {
			this->add(c, *it);
		} // it
	} // add_result
	template<typename TX, typename ZX, class ALLOCZX, class ALLOCVECX>
	void get_result(TX &crit,
			std::vector<std::list<ZX, ALLOCZX>, ALLOCVECX> &oVec) {
		crit = (TX) this->m_crit;
		oVec.clear();
		for (auto it = this->m_vec.begin(); it != this->m_vec.end(); ++it) {
			std::list<ZX, ALLOCZX> xList;
			const IndexListType & vv = *it;
			for (auto jt = vv.begin(); jt != vv.end(); ++jt) {
				xList.push_back((ZX) *jt);
			} // jt
			oVec.push_back(xList);
		} // it
	} // set_result
	virtual bool add(CriteriaType crit, const IndexListType &oList) {
		bool bRet = false;
		size_t n = oList.size();
		if (n > 0) {
			IndexListType oIndex(oList);
			IndexListVectorType &vec = this->m_vec;
			if (vec.empty()) {
				vec.push_back(oIndex);
				this->m_crit = crit;
				bRet = true;
			} else if (crit < this->m_crit) {
				IndexListType oIndex(oList);
				vec.clear();
				vec.push_back(oIndex);
				this->m_crit = crit;
				bRet = true;
			} else if (crit == this->m_crit) {
				if (vec[0].size() == n) {
					auto vend = vec.end();
					bool bFound = false;
					for (auto it = vec.begin(); (!bFound) && (it != vend);
							++it) {
						bool xfound = true;
						IndexListType &xList = *it;
						auto xend = xList.end();
						auto jt = xList.begin();
						auto kt = oIndex.begin();
						while (jt != xend) {
							if (*jt++ != *kt++) {
								xfound = false;
								break;
							}
						} // while
						bFound = bFound || xfound;
					} // it
					if (!bFound) {
						vec.push_back(oIndex);
						bRet = true;
					}
				} // size
			}
		} // n
		return (bRet);
	} // add
protected:
	CriteriaType m_crit;
	IndexListVectorType m_vec;
};
// class ArrangeResult<T,Z,ALLOCZ,ALLOCVECTORZ>
//////////////////////////////////////////
template<typename T = double, typename Z = int, class ALLOCZ = std::allocator<Z> >
class ArrangeStruct {
public:
	typedef T CriteriaType;
	typedef Z IndexType;
	typedef std::list<Z, ALLOCZ> IndexListType;
	typedef std::set<Z, std::less<Z>, ALLOCZ> IndexSetType;
	typedef std::vector<Z, ALLOCZ> IndexVectorType;
	typedef ArrangeStruct<T, Z, ALLOCZ> ArrangeStructType;
public:
	ArrangeStruct() :
			m_crit(0) {
	}
	ArrangeStruct(size_t n, IndexType iStart) :
			m_crit(0) {
		if ((n > 0) && ((size_t) iStart < n)) {
			this->m_index.push_back(iStart);
			IndexSetType &oSet = this->m_set;
			for (size_t i = 0; i < n; ++i) {
				if (iStart != (IndexType) i) {
					oSet.insert((IndexType) i);
				}
			}	// i
		}
	}	// ArrangeStruct
	ArrangeStruct(const ArrangeStructType &other) :
			m_crit(other.m_crit), m_index(other.m_index), m_set(other.m_set) {
	}	// ArrangeStruct
	ArrangeStructType & operator=(const ArrangeStructType &other) {
		if (this != &other) {
			this->m_crit = other.m_crit;
			this->m_index = other.m_index;
			this->m_set = other.m_set;
		}
		return (*this);
	}	// operator=
	virtual ~ArrangeStruct() {
	}
public:
	template<class DISTFUNC>
	size_t next_best_moves(const DISTFUNC &fdist, const IndexSetType &oSet,
			const IndexType nStart, IndexVectorType &oRes,
			CriteriaType &crit) const {
		oRes.clear();
		crit = 0;
		auto vend = oSet.end();
		for (auto it = oSet.begin(); it != vend; ++it) {
			IndexType nCur = *it;
			CriteriaType c = fdist(nStart, nCur);
			if (oRes.empty()) {
				oRes.push_back(nCur);
				crit = c;
			} else if (c < crit) {
				oRes.clear();
				oRes.push_back(nCur);
				crit = c;
			} else if (c == crit) {
				oRes.push_back(nCur);
			}
		}	// c}// it
		return (oRes.size());
	}	// next_best_moves
	template<class DISTFUNC>
	size_t next_best_moves(const DISTFUNC &fdist, IndexVectorType &oRes,
			CriteriaType &crit) const {
		const IndexListType &index = this->m_index;
		if (!index.empty()) {
			IndexType nStart = index.back();
			return (this->next_best_moves(fdist, this->m_set, nStart, oRes,
					crit));
		} else {
			return (0);
		}
	}	// next_best_moves
	template<class DISTFUNC, class ALLOCVECTORZ>
	void arrange_impl(const DISTFUNC &fdist,
			std::vector<IndexListType, ALLOCVECTORZ> &oList,
			CriteriaType &crit) {
		IndexSetType &oSet = this->m_set;
		IndexListType &indexes = this->m_index;
		while (!oSet.empty()) {
			IndexVectorType oRes;
			CriteriaType ccur = 0;
			size_t np = this->next_best_moves(fdist, oRes, ccur);
			while ((np == 1) && (!oSet.empty())) {
				IndexType nCur = oRes[0];
				oSet.erase(nCur);
				indexes.push_back(nCur);
				np = this->next_best_moves(fdist, oRes, ccur);
			}	// one move
			if ((np < 1) || oSet.empty()) {
				break;
			}
			bool hasmin = false;
			bool bfirst = true;
			IndexType nbest = 0;
			CriteriaType cbest = 0;
			auto vend1 = oRes.end();
			for (auto it = oRes.begin(); it != vend1; ++it) {
				IndexType nCur = *it;
				IndexSetType xset(oSet);
				xset.erase(nCur);
				IndexVectorType xres;
				CriteriaType xcrit = 0;
				size_t nx = this->next_best_moves(fdist, xset, nCur, xres,
						xcrit);
				if (nx > 0) {
					if (bfirst) {
						bfirst = false;
						nbest = nCur;
						cbest = xcrit;
					} else if (xcrit < cbest) {
						hasmin = true;
						nbest = nCur;
						cbest = xcrit;
					} else if (xcrit > cbest) {
						hasmin = true;
					}
				}	// nx
			}	// it
			if (hasmin) {
				oSet.erase(nbest);
				indexes.push_back(nbest);
			} else {
				for (auto it = oRes.begin(); it != vend1; ++it) {
					IndexType nCur = *it;
					ArrangeStructType oWork(*this);
					oWork.m_set.erase(nCur);
					oWork.m_index.push_back(nCur);
					oWork.arrange_impl(fdist, oList, crit);
				}	// it
				return;
			}
		}	// oSet
		CriteriaType xxcrit = 0;
		size_t nz = indexes.size();
		auto iend = indexes.end();
		for (auto it = indexes.begin(); it != iend; ++it) {
			auto jt = it;
			++jt;
			if (jt != iend) {
				xxcrit = (CriteriaType) (xxcrit + fdist(*it, *jt));
			}
		}
		if (oList.empty()) {
			crit = xxcrit;
			IndexListType oInd(indexes);
			oList.push_back(oInd);
		} else {
			if (xxcrit < crit) {
				oList.clear();
				crit = xxcrit;
				IndexListType oInd(indexes);
				oList.push_back(oInd);
			} else if (xxcrit == crit) {
				bool bFound = false;
				auto xend = oList.end();
				auto iend = indexes.end();
				for (auto it = oList.begin(); (!bFound) && (it != xend); ++it) {
					const IndexListType &vx = *it;
					if (vx.size() == nz) {
						bool xfound = true;
						auto jt = indexes.begin();
						auto kt = vx.begin();
						while (xfound && (jt != iend)) {
							if (*jt != *kt) {
								xfound = false;
								break;
							}
						}
						bFound = bFound || xfound;
					}	// size
				}	// it
				if (!bFound) {
					IndexListType oInd(indexes);
					oList.push_back(oInd);
				}
			}
		}
	}	// arrange_impl
	template<class DISTFUNC, class ALLOCVECTORZ>
	void arrange_impl(DISTFUNC &fdist,
			ArrangeResult<T, Z, ALLOCZ, ALLOCVECTORZ> &result) {
		IndexSetType &oSet = this->m_set;
		IndexListType &indexes = this->m_index;
		while (!oSet.empty()) {
			IndexVectorType oRes;
			CriteriaType ccur = 0;
			size_t np = this->next_best_moves(fdist, oRes, ccur);
			while ((np == 1) && (!oSet.empty())) {
				IndexType nCur = oRes[0];
				oSet.erase(nCur);
				indexes.push_back(nCur);
				np = this->next_best_moves(fdist, oRes, ccur);
			}	// one move
			if ((np < 1) || oSet.empty()) {
				break;
			}
			bool hasmin = false;
			bool bfirst = true;
			IndexType nbest = 0;
			CriteriaType cbest = 0;
			auto oend = oRes.end();
			for (auto it = oRes.begin(); it != oend; ++it) {
				IndexType nCur = *it;
				IndexSetType xset(oSet);
				xset.erase(nCur);
				IndexVectorType xres;
				CriteriaType xcrit = 0;
				size_t nx = this->next_best_moves(fdist, xset, nCur, xres,
						xcrit);
				if (nx > 0) {
					if (bfirst) {
						bfirst = false;
						nbest = nCur;
						cbest = xcrit;
					} else if (xcrit < cbest) {
						hasmin = true;
						nbest = nCur;
						cbest = xcrit;
					} else if (xcrit > cbest) {
						hasmin = true;
					}
				}	// nx
			}	// it
			if (hasmin) {
				oSet.erase(nbest);
				indexes.push_back(nbest);
			} else {
				for (auto it = oRes.begin(); it != oend; ++it) {
					IndexType nCur = *it;
					ArrangeStructType oWork(*this);
					oWork.m_set.erase(nCur);
					oWork.m_index.push_back(nCur);
					oWork.arrange_impl(fdist, result);
				}	// it
				return;
			}
		}	// oSet
		CriteriaType xxcrit = 0;
		auto iend = indexes.end();
		for (auto it = indexes.begin(); it != iend; ++it) {
			auto jt = it;
			++jt;
			if (jt != iend) {
				xxcrit = (CriteriaType) (xxcrit + fdist(*it, *jt));
			}
		}
		this->m_crit = xxcrit;
		result.add(xxcrit, indexes);
	}	// arrange_impl
	template<class DISTFUNC, class ALLOCVECTORZ>
	static void arrange(const DISTFUNC &fdist, size_t n, IndexType iStart,
			std::vector<IndexListType, ALLOCVECTORZ> &oList,
			CriteriaType &crit) {
		ArrangeStructType oAr(n, iStart);
		oAr.arrange_impl(fdist, oList, crit);
	}	// arrange
public:
	template<class DISTFUNC, class ALLOCVECTORZ>
	static void arrange(const DISTFUNC &fdist, size_t n,
			std::vector<IndexListType, ALLOCVECTORZ> &oList,
			CriteriaType &crit) {
		ArrangeResult<T, Z, ALLOCZ, ALLOCVECTORZ> result;
		for (int i = 0; i < (int) n; ++i) {
			ArrangeStructType oAr(n, (IndexType) i);
			oAr.arrange_impl(fdist, result);
		}	// i
		crit = result.criteria();
		oList = result.indexes();
	}	// arrange
private:
	CriteriaType m_crit;
	IndexListType m_index;
	IndexSetType m_set;
};
// class ArrangeStruct<T,Z,ALLOCZ>
////////////////////////////////////////
template<typename T, typename Z, class DISTFUNC, class ALLOCZ,
		class ALLOCVECTORZ>
void MinimalPath(size_t n, const DISTFUNC &fdist, T & crit,
		std::vector<std::list<Z, ALLOCZ>, ALLOCVECTORZ> &indexes) {
	ArrangeStruct<T, Z, ALLOCZ>::arrange(fdist, n, indexes, crit);
}	// MinimalPath
template<typename T, typename Z, class ALLOCT, class ALLOCZ, class ALLOCVECTORZ>
void MinimalPath(size_t n, const std::vector<T, ALLOCT> &dist, T & crit,
		std::vector<std::list<Z, ALLOCZ>, ALLOCVECTORZ> &indexes) {
	auto fdist = [dist,n](Z i1, Z i2)->T {
		return (dist[i1 * n + i2]);
	};
	ArrangeStruct<T, Z, ALLOCZ>::arrange(fdist, n, indexes, crit);
}	// MinimalPath
}	// namespace statdata
/////////////////////////////////////////
#endif /* ARRANGEDESC_H_ */
