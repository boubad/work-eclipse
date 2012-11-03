/*
 * treeitem.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef TREEITEM_H_
#define TREEITEM_H_
////////////////////////////
#include "arrangedesc.h"
////////////////////////////
#include <cassert>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
//////////////////////////////
namespace statdata {
///////////////////////////////////
typedef enum {
	linkmean, linkmax, linkmin
} LinkType;
////////////////////////////////
template <typename Z = size_t, typename S = std::string>
class TreeElemInfo {
public:
	TreeElemInfo():m_index(0),m_grpindex(0){}
	TreeElemInfo(Z iIndex, const S &sname, Z grpindex = 0):
		m_index(iIndex),m_grpindex(grpindex),m_name(sname){}
	TreeElemInfo(const TreeElemInfo<Z,S> &other):m_index(other.m_index),
			m_grpindex(other.m_grpindex),m_name(other.m_name){}
	TreeElemInfo<Z,S> & operator=(const TreeElemInfo<Z,S> &other){
		if (this != &other){
			this->m_index = other.m_index;
			this->m_grpindex = other.m_grpindex;
			this->m_name = other.m_name;
		}
		return (*this);
	}
	virtual ~TreeElemInfo(){}
public:
	const S & name(void) const {
		return (this->m_name);
	}
	void name(const S &s){
		this->m_name = s;
	}
	Z data_index(void) const {
		return (this->m_index);
	}
	void data_index(Z i){
		this->m_index = i;
	}
	Z group_index(void) const {
		return (this->m_grpindex);
	}
	void group_index(Z i){
		this->m_grpindex = i;
	}
private:
	Z  m_index;
	Z  m_grpindex;
	S m_name;
}; // class TreeElemInfo<Z,S>
/////////////////////////
template<typename W = float, typename Z = size_t,
		class ALLOCW = std::allocator<W>,
		class ALLOCZ = std::allocator<Z> >
class TreeItem {
public:
	typedef W CriteriaType;
	typedef Z IndexType;
	typedef ALLOCW CriteriaAllocator;
	typedef ALLOCZ IndexAllocator;
	typedef std::vector<W, ALLOCW> CriteriaVectorType;
	typedef std::vector<Z, ALLOCZ> IndexVectorType;
	typedef std::set<Z, std::less<Z>, ALLOCZ> IndexSetType;
	typedef TreeItem<W, Z, ALLOCW, ALLOCZ> TreeItemType;
	typedef std::shared_ptr<TreeItemType> TreeItemPtr;
	typedef std::vector<TreeItemPtr > TreeItems;
	typedef std::vector<IndexVectorType> IndexArrayType;
private:
	TreeItemType *m_parent;
	LinkType m_link;
	IndexType m_id;
	CriteriaType m_var;
	CriteriaType m_crit;
	CriteriaType m_level;
	TreeItems m_children;
	CriteriaVectorType m_curdist;
public:
	TreeItem() :m_parent(nullptr),
			m_link(linkmean), m_id((IndexType) -1), m_var(0), m_crit(0), m_level(
					0) {
	} // TreeItem
	virtual ~TreeItem() {
	}
protected:
	TreeItem(size_t n, const LinkType t = linkmean) : m_parent(nullptr),
			m_link(t), m_id((IndexType) -1), m_var(0), m_crit(0), m_level(0) {
		assert(n > 0);
		for (size_t i = 0; i < n; ++i) {
			TreeItemPtr pp(new TreeItemType());
			TreeItemType *p = pp.get();
			assert(p != nullptr);
			p->m_parent = this;
			p->m_link = t;
			p->m_id = (IndexType) i;
			this->m_children.push_back(pp);
		} // i
	} // TreeItem
private:
	// no implementation
	TreeItem(const TreeItemType &/*other*/);
	TreeItemType & operator=(const TreeItemType & /*other*/);
public:
	const TreeItemType *parent(void) const {
		return (this->m_parent);
	}
	const CriteriaVectorType & distances(void) const {
		return (this->m_curdist);
	}
	LinkType type(void) const {
		return (this->m_link);
	}
	IndexType id(void) const {
		return (this->m_id);
	}
	size_t size(void) const {
		return (this->m_children.size());
	}
	bool is_leaf(void) const {
		return (this->m_children.empty());
	}
	const TreeItemType *child_at(size_t i) const {
		assert(i < this->m_children.size());
		return (this->m_children[i].get());
	}
	const TreeItems & children(void) const {
		return (this->m_children);
	}
	TreeItems & children(void) {
		return (this->m_children);
	}
	CriteriaType variance(void) const {
		return (this->m_var);
	}
	CriteriaType criteria(void) const {
		return (this->m_crit);
	}
	int quality(void) const {
		if (this->m_var > 0) {
			return ((int) (1000.0 * this->m_crit / (double) this->m_var));
		} else {
			return (0);
		}
	}
	CriteriaType level(void) const {
		return (this->m_level);
	}
public:
private:
	void arrange(void){
		TreeItems &items = this->m_children;
		const size_t n = items.size();
		if (n < 3){
			return;
		}
		std::vector<std::list<IndexType,IndexAllocator> > oVec;
		CriteriaType crit = 0;
		statdata::MinimalPath(n,this->m_curdist, crit,oVec);
		if (oVec.empty()){
			return;
		}
		const std::list<IndexType,IndexAllocator> &indexes = oVec[0];
		if (indexes.size() != n){
			return;
		}
		TreeItems itemsSave(n);
		size_t icur = 0;
		for (auto it = indexes.begin(); it != indexes.end(); ++it){
			IndexType ind = *it;
			assert((size_t)ind < n);
			itemsSave[icur++] = items[ind];
		}// it
		this->m_children = itemsSave;
	}// arrange
	template<class DISTFUNC>
	CriteriaType distance(const TreeItemType &other, const DISTFUNC & f) const {
		const TreeItems &items1 = this->children();
		const size_t n1 = items1.size();
		const TreeItems &items2 = other.children();
		const size_t n2 = items2.size();
		if ((n1 == 0) && (n2 == 0)) {
			return ((CriteriaType) f(this->id(), other.id()));
		}
		if ((n2 == 0) && (n1 > 0)) {
			return (other.distance(*this, f));
		}
		const LinkType t = this->type();
		CriteriaType res = (CriteriaType) 0;
		if ((n1 == 0) && (n2 > 0)) {
			for (size_t i = 0; i < n2; ++i) {
				const TreeItemPtr & px = items2[i];
				const TreeItemType *p = px.get();
				assert(p != nullptr);
				CriteriaType x = this->distance(*p, f);
				switch (t) {
				case linkmean:
					res = (CriteriaType) (res + x);
					break;
				case linkmin:
					if (i == 0) {
						res = x;
					} else if (x < res) {
						res = x;
					}
					break;
				case linkmax:
					if (i == 0) {
						res = x;
					} else if (x > res) {
						res = x;
					}
					break;
				default:
					break;
				} // t
			} // i
			if (t == linkmean) {
				res = (CriteriaType) (res / (double) n2);
			}
		} else if ((n1 > 0) && (n2 > 0)) {
			for (size_t i1 = 0; i1 < n1; ++i1) {
				const TreeItemPtr & px1 = items1[i1];
				const TreeItemType *p1 = px1.get();
				assert(p1 != nullptr);
				for (size_t i2 = 0; i2 < n2; ++i2) {
					const TreeItemPtr &px2 = items2[i2];
					const TreeItemType *p2 = px2.get();
					assert(p2 != nullptr);
					CriteriaType x = p1->distance(*p2, f);
					switch (t) {
					case linkmean:
						res = (CriteriaType) (res + x);
						break;
					case linkmin:
						if ((i1 == 0) && (i2 == 0)) {
							res = x;
						} else if (x < res) {
							res = x;
						}
						break;
					case linkmax:
						if ((i1 == 0) && (i2 == 0)) {
							res = x;
						} else if (x > res) {
							res = x;
						}
						break;
					default:
						break;
					} // t
				} // i2
			} // i1
			if (t == linkmean) {
				res = (CriteriaType) ((double) res / (n2 * n1));
			}
		}
		return (res);
	} // distance
	bool findNextAggreg(CriteriaType &crit, IndexSetType &oSet) const {
		//
		oSet.clear();
		const CriteriaVectorType &dist = this->m_curdist;
		const size_t n = this->m_children.size();
		for (size_t i = 0; i < n; ++i) {
			const size_t gap = i * n;
			for (size_t j = 0; j < i; ++j) {
				CriteriaType x = dist[gap + j];
				if (oSet.empty()) {
					crit = x;
					oSet.insert((IndexType) i);
					oSet.insert((IndexType) j);
				} else if (x < crit) {
					crit = x;
					oSet.clear();
					oSet.insert((IndexType) i);
					oSet.insert((IndexType) j);
				} else if (x == crit) {
					auto iend = oSet.end();
					if ((oSet.find((IndexType) i) != iend)
							&& (oSet.find((IndexType) j) == iend)) {
						oSet.insert((IndexType) j);
					} else if ((oSet.find((IndexType) i) == iend)
							&& (oSet.find((IndexType) j) != iend)) {
						oSet.insert((IndexType) i);
					}
				}
			} // j
		} // i
		return (oSet.size() > 1);
	} // findNextAggreg
	template<class DISTFUNC>
	void compute_distances(const DISTFUNC &f) {
		TreeItems &items = this->children();
		const size_t n = items.size();
		if (n > 0) {
			this->m_curdist = CriteriaVectorType(n * n);
			CriteriaVectorType & v = this->m_curdist;
			for (size_t i = 0; i < n; ++i) {
				const size_t igap = i * n;
				v[igap + i] = (W) 0;
				TreeItemPtr px1 = items[i];
				TreeItemType *p1 = px1.get();
				assert(p1 != nullptr);
				for (size_t j = 0; j < i; ++j) {
					TreeItemPtr px2 = items[j];
					TreeItemType *p2 = px2.get();
					assert(p2 != nullptr);
					W x = p1->distance(*p2, f);
					v[igap + j] = x;
					v[j * n + i] = x;
				} // j
			} // i
		} // n
	} // compute_distances
	template<class DISTFUNC>
	bool next_step(const DISTFUNC &f, IndexType &nNodeId) {
		TreeItems & items = this->children();
		const size_t n = items.size();
		if (n < 2) {
			return (false);
		}
		IndexSetType oSet;
		CriteriaType dval = 0;
		if (!this->findNextAggreg(dval, oSet)) {
			return (false);
		}
		TreeItems temp;
		auto iend1 = oSet.end();
		for (auto it = oSet.begin(); it != iend1; ++it) {
			IndexType ii = *it;
			assert((size_t)ii < n);
			TreeItemPtr p = items[ii];
			assert(p.get() != nullptr);
			temp.push_back(p);
		} // it
		TreeItemPtr pp(new TreeItemType());
		TreeItemType *px = pp.get();
		assert(px != nullptr);
		px->m_parent = this;
		px->m_link = this->m_link;
		px->m_level = dval;
		px->m_id = ++nNodeId;
		auto iend2 = temp.end();
		for (auto it = temp.begin(); it != iend2; ++it) {
			TreeItemPtr p = *it;
			TreeItemType *pk = p.get();
			assert(pk != nullptr);
			auto jt = std::find_if(items.begin(), items.end(),
					[=](TreeItemPtr &px)->bool {
						return (px.get() == pk);
					});
			assert(jt != items.end());
			items.erase(jt);
			pk->m_parent = px;
			px->m_children.push_back(p);
		} // it
		px->compute_distances(f);
		px->update_desc(f);
		this->m_children.push_back(pp);
		this->compute_distances(f);
		this->update_desc(f);
		return (true);
	} // next_step
	template<class DISTFUNC>
	void update_desc(DISTFUNC &f) {
		CriteriaType svar = 0;
		CriteriaType scrit = 0;
		size_t vn = 0;
		TreeItems items;
		this->split(items);
		const size_t nx = items.size();
		if (nx > 1) {
			for (size_t i = 0; i < nx; ++i) {
				TreeItemPtr &px1 = items[i];
				TreeItemType *p1 = px1.get();
				assert(p1 != nullptr);
				assert(p1->is_leaf());
				const size_t ii = p1->id();
				if (i > 0) {
					TreeItemPtr &px0 = items[i - 1];
					TreeItemType *p0 = px0.get();
					assert(p0 != nullptr);
					const size_t jj = p0->id();
					scrit = (CriteriaType) (scrit + f(ii, jj));
				}
				for (size_t j = 0; j < i; ++j) {
					TreeItemPtr &px2 = items[j];
					TreeItemType *p2 = px2.get();
					assert(p2 != nullptr);
					const size_t jj = p2->id();
					svar = (CriteriaType) (svar + f(ii, jj));
					++vn;
				} // j
			} // i
			scrit = (CriteriaType) (scrit / nx);
			if (vn > 1) {
				svar = (CriteriaType) (svar / (double) vn);
			}
		} // nx
		this->m_var = svar;
		this->m_crit = scrit;
	} // update_desc
	template<class AllocS>
	std::ostream & internalWriteTo(std::ostream &out,
			const std::vector<std::string, AllocS> &names) const {
		const TreeItems &items = this->children();
		const size_t n = items.size();
		const size_t nId = (size_t) this->id();
		if (n == 0) {
			std::stringstream os;
			os << " [" << nId << "] ";
			if (nId < names.size()) {
				std::string ss = names[nId];
				if (!ss.empty()) {
					os << ss << " ";
				}
			}
			std::string slabel = os.str();
			out << nId << " [shape = box,style=filled,color=yellow, label=\""
					<< slabel << "\"];" << std::endl;
		} else {
			out << nId << " [label=\"S" << nId << "\\nVar = "
					<< this->variance() << "\\nQ = " << this->quality()
					<< "\"];" << std::endl;
			for (size_t i = 0; i < n; ++i) {
				const TreeItemPtr &pp = items[i];
				const TreeItemType *p = pp.get();
				assert(p != nullptr);
				p->internalWriteTo(out, names);
				out << nId << " -> " << p->id() << ";" << std::endl;
			} // i
		}
		return (out);
	} // internalWriteTo
public:
	template<class AllocS>
	std::ostream & writeTo(std::ostream &out,
			const std::vector<std::string, AllocS> &names) const {
		out << std::endl;
		out << "digraph G { " << std::endl;
		out << "size = \"4,4\";" << std::endl;
		const TreeItems &items = this->children();
		const size_t n = items.size();
		if (n == 0) {
			IndexType nId = this->id();
			std::stringstream os;
			os << " [" << nId << "] ";
			if ((size_t)nId < names.size()) {
				std::string ss = names[nId];
				if (!ss.empty()) {
					os << ss << " ";
				}
			}
			std::string slabel = os.str();
			out << nId << " [shape = box,style=filled,color=yellow, label=\""
					<< slabel << "\"];" << std::endl;
		} else {
			for (size_t i = 0; i < n; ++i) {
				const TreeItemPtr &pp = items[i];
				const TreeItemType *p = pp.get();
				assert(p != nullptr);
				p->internalWriteTo(out, names);
			} // i
		}
		out << "}" << std::endl;
		return (out);
	} // writeTo
	template<typename X, class ALLOCX>
	void split(std::vector<X, ALLOCX> &oVec) const {
		if (this->m_children.empty()) {
			oVec.push_back((X) this->id());
			return;
		}
		const TreeItems &items = this->m_children;
		const size_t n = items.size();
		for (size_t i = 0; i < n; ++i) {
			const TreeItemPtr & px = items[i];
			const TreeItemType *p = px.get();
			assert(p != nullptr);
			p->split(oVec);
		} // i
	} // split
	void split(TreeItems &elems) const {
		const TreeItems &items = this->m_children;
		const size_t n = items.size();
		if (n < 1) {
			return;
		}
		for (size_t i = 0; i < n; ++i) {
			const TreeItemPtr & px = items[i];
			const TreeItemType *p = px.get();
			assert(p != nullptr);
			if (p->m_children.empty()) {
				elems.push_back(px);
			} else {
				p->split(elems);
			}
		} // i
	} // split
	template <class VISITORFUNC>
	void visit(const VISITORFUNC &func) const {
		const TreeItems &items = this->m_children;
		const size_t n = items.size();
		for (size_t i = 0; i < n; ++i){
			const TreeItemPtr & px = items[i];
			const TreeItemType *p = px.get();
			assert(p != nullptr);
			p->visit(func);
		}// i
		func(*this);
	}// visit
	template<class DISTFUNC>
	void clusterize(const DISTFUNC &f, const size_t nClasses = 1) {
		this->compute_distances(f);
		this->update_desc(f);
		TreeItems & items = this->m_children;
		IndexType nNodeId = (IndexType) (this->m_children.size() + 1000);
		while (items.size() > nClasses) {
			if (!this->next_step(f, nNodeId)) {
				break;
			}
		} // nClasses
	} // clusterize
	template<class DISTFUNC>
	void aggreg_leaves(const DISTFUNC &f, const size_t nClasses = 1) {
		this->compute_distances(f);
		TreeItems & items = this->m_children;
		IndexType nNodeId = (IndexType) (this->m_children.size() + 1000);
		while (items.size() > nClasses) {
			if (!this->next_step(f, nNodeId)) {
				break;
			}
		} // nClasses
		size_t n = items.size();
		TreeItems elems;
		for (size_t i = 0; i < n; ++i) {
			TreeItemPtr & pp = items[i];
			TreeItemType *p = pp.get();
			assert(p != nullptr);
			if (!p->m_children.empty()) {
				TreeItems elemsx;
				p->split(elemsx);
				for (auto it = elemsx.begin(); it != elemsx.end(); ++it){
					TreeItemPtr &p0 = *it;
					TreeItemType *pp0 = p0.get();
					assert(pp0 != nullptr);
					pp0->m_parent = p;
				}// it
				p->m_children = elemsx;
				p->compute_distances(f);
				p->update_desc(f);
				p->arrange();
			}
			p->m_parent = this;
			elems.push_back(pp);
		} // i
		this->m_children = elems;
		this->compute_distances(f);
		this->update_desc(f);
		this->arrange();
	} // aggreg_leaves
public:
	template<class DISTFUNC>
	static void Clusterize(size_t n, const DISTFUNC &fdist, TreeItemType &oTree,
			const size_t nClasses = 1, const LinkType t = linkmean) {
		assert(n > 0);
		TreeItemType oCur(n, t);
		oCur.clusterize(fdist, nClasses);
		oTree.m_link = oCur.m_link;
		oTree.m_id = oCur.m_id;
		oTree.m_var = oCur.m_var;
		oTree.m_crit = oCur.m_crit;
		oTree.m_level = oCur.m_level;
		oTree.m_children = oCur.m_children;
		oTree.m_curdist = oCur.m_curdist;
	} // Clusterize
	template<typename X, class AllocX>
	static void Clusterize(size_t n, const std::vector<X, AllocX> &distances,
			TreeItemType &oTree, const size_t nClasses = 1, const LinkType t =
					linkmean) {
		assert(n > 0);
		assert(distances.size() >= (size_t)(n * n));
		auto fdist = [=](IndexType i1, IndexType i2)->CriteriaType {
			return ((CriteriaType)distances[(size_t)(i1 * n + i2)]);
		};
		TreeItemType::Clusterize(n, fdist, oTree, nClasses, t);
	} // Clusterize
	  //
	template<class DISTFUNC>
	static void Clusterize_Leaves(size_t n, const DISTFUNC &fdist,
			TreeItemType &oTree, const size_t nClasses = 1, const LinkType t =
					linkmean) {
		assert(n > 0);
		TreeItemType oCur(n, t);
		oCur.aggreg_leaves(fdist, nClasses);
		oTree.m_link = oCur.m_link;
		oTree.m_id = oCur.m_id;
		oTree.m_var = oCur.m_var;
		oTree.m_crit = oCur.m_crit;
		oTree.m_level = oCur.m_level;
		oTree.m_children = oCur.m_children;
		oTree.m_curdist = oCur.m_curdist;
	} // Clusterize
	template<typename X, class AllocX>
	static void Clusterize_Leaves(size_t n,
			const std::vector<X, AllocX> &distances, TreeItemType &oTree,
			const size_t nClasses = 1, const LinkType t = linkmean) {
		assert(n > 0);
		assert(distances.size() >= (size_t)(n * n));
		auto fdist = [=](IndexType i1, IndexType i2)->CriteriaType {
			return ((CriteriaType)distances[(size_t)(i1 * n + i2)]);
		};
		TreeItemType::Clusterize_Leaves(n, fdist, oTree, nClasses, t);
	} // Clusterize
};
// class TreeItem<W,Z,ALLOCW,ALLOCZ>
////////////////////////////////////////
template<typename W, typename Z, class ALLOCW, class ALLOCZ, class ALLOCVEC>
void ClusterizeData(size_t n, const std::vector<W, ALLOCW> &distances,
		std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC> &indexes,
		size_t nClasses = 5, statdata::LinkType ltype = statdata::linkmean) {
	typedef TreeItem<W, Z, ALLOCW, ALLOCZ> TreeType;
	typedef typename TreeType::TreeItemType TreeItemType;
	TreeType oTree;
	TreeType::Clusterize_Leaves(n, distances, oTree, nClasses, ltype);
	const size_t nx = oTree.size();
	indexes = std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC>(nx);
	for (size_t i = 0; i < nx; ++i) {
		const TreeItemType *p = oTree.child_at(i);
		assert(p != nullptr);
		std::vector<Z, ALLOCZ> oVec;
		p->split(oVec);
		indexes[i] = oVec;
	} // i
} // ClusterizeData
///////////////////////////
template<class DISTFUNC, typename Z, class ALLOCZ, class ALLOCVEC>
void ClusterizeData(size_t n, const DISTFUNC &fdist,
		std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC> &indexes,
		size_t nClasses = 5, statdata::LinkType ltype = statdata::linkmean) {
	typedef float W;
	typedef std::allocator<W> ALLOCW;
	typedef TreeItem<W, Z, ALLOCW, ALLOCZ> TreeType;
	typedef typename TreeType::TreeItemType TreeItemType;
	TreeType oTree;
	TreeType::Clusterize_Leaves(n, fdist, oTree, nClasses, ltype);
	const size_t nx = oTree.size();
	indexes = std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC>(nx);
	for (size_t i = 0; i < nx; ++i) {
		const TreeItemType *p = oTree.child_at(i);
		assert(p != nullptr);
		std::vector<Z, ALLOCZ> oVec;
		p->split(oVec);
		indexes[i] = oVec;
	} // i
} // ClusterizeData
///////////////////////////////////////
} // namespace statdata
///////////////////////
template<typename W, typename Z, class AllocW, class AllocZ>
std::ostream & operator<<(std::ostream &os,
		const statdata::TreeItem<W, Z, AllocW, AllocZ> &item) {
	std::vector<std::string> names;
	return (item.writeTo(os, names));
} // operator<<
////////////////
#endif /* TREEITEM_H_ */
