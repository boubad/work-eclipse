/*
 * treeitem.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef TREEITEM_H_
#define TREEITEM_H_
/////////////////////////////////
#include "dataport.h"
///////////////////////////////
#include <cassert>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <valarray>
//////////////////////////////////
namespace statdata
{
///////////////////////////////////

typedef enum
{
    linkmean, linkmax, linkmin
} LinkType;
////////////////////////////////

template<typename W = float, typename Z = short,
class ALLOCW = std::allocator<W>,
class ALLOCZ = std::allocator<Z> >
class TreeItem
{
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
    CriteriaType m_level;
    TreeItems m_children;
    CriteriaVectorType m_curdist;
public:

    TreeItem() : m_parent(nullptr),
        m_link(linkmean), m_id((IndexType) - 1), m_var(0), m_level(
            0)
    {
    } // TreeItem

    virtual ~TreeItem()
    {
    }
protected:

    TreeItem(size_t n, const LinkType t = linkmean) : m_parent(nullptr),
        m_link(t), m_id((IndexType) - 1), m_var(0), m_level(0)
    {
        for (size_t i = 0; i < n; ++i)
        {
            TreeItemPtr pp = std::make_shared<TreeItemType > ();
            TreeItemType *p = pp.get();
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

    const TreeItemType *parent(void) const
    {
        return (this->m_parent);
    }

    const CriteriaVectorType & distances(void) const
    {
        return (this->m_curdist);
    }

    LinkType type(void) const
    {
        return (this->m_link);
    }

    IndexType id(void) const
    {
        return (this->m_id);
    }

    size_t size(void) const
    {
        return (this->m_children.size());
    }

    bool is_leaf(void) const
    {
        return (this->m_children.empty());
    }

    const TreeItemType *child_at(size_t i) const
    {
        assert(i < this->m_children.size());
        return (this->m_children[i].get());
    }

    const TreeItems & children(void) const
    {
        return (this->m_children);
    }

    TreeItems & children(void)
    {
        return (this->m_children);
    }

    CriteriaType variance(void) const
    {
        return (this->m_var);
    }

    CriteriaType level(void) const
    {
        return (this->m_level);
    }
public:

    template<class DISTFUNC>
    CriteriaType distance(const TreeItemType &other, const DISTFUNC & f) const
    {
        const TreeItems &items1 = this->m_children;
        const size_t n1 = items1.size();
        const TreeItems &items2 = other.m_children;
        const size_t n2 = items2.size();
        if ((n1 == 0) && (n2 == 0))
        {
            return ((CriteriaType) f(this->m_id, other.m_id));
        }
        if ((n2 == 0) && (n1 > 0))
        {
            return (other.distance(*this, f));
        }
        const LinkType t = this->m_link;
        CriteriaType res = (CriteriaType) 0;
        if ((n1 == 0) && (n2 > 0))
        {
            for (size_t i = 0; i < n2; ++i)
            {
                const TreeItemPtr & px = items2[i];
                const TreeItemType *p = px.get();
                CriteriaType x = this->distance(*p, f);
                switch (t)
                {
                case linkmean:
                    res = (CriteriaType) (res + x);
                    break;
                case linkmin:
                    if (i == 0)
                    {
                        res = x;
                    }
                    else if (x < res)
                    {
                        res = x;
                    }
                    break;
                case linkmax:
                    if (i == 0)
                    {
                        res = x;
                    }
                    else if (x > res)
                    {
                        res = x;
                    }
                    break;
                default:
                    break;
                } // t
            } // i
            if (t == linkmean)
            {
                res = (CriteriaType) (res / (double) n2);
            }
        }
        else if ((n1 > 0) && (n2 > 0))
        {
            for (size_t i1 = 0; i1 < n1; ++i1)
            {
                const TreeItemPtr & px1 = items1[i1];
                const TreeItemType *p1 = px1.get();
                for (size_t i2 = 0; i2 < n2; ++i2)
                {
                    const TreeItemPtr &px2 = items2[i2];
                    const TreeItemType *p2 = px2.get();
                    CriteriaType x = p1->distance(*p2, f);
                    switch (t)
                    {
                    case linkmean:
                        res = (CriteriaType) (res + x);
                        break;
                    case linkmin:
                        if ((i1 == 0) && (i2 == 0))
                        {
                            res = x;
                        }
                        else if (x < res)
                        {
                            res = x;
                        }
                        break;
                    case linkmax:
                        if ((i1 == 0) && (i2 == 0))
                        {
                            res = x;
                        }
                        else if (x > res)
                        {
                            res = x;
                        }
                        break;
                    default:
                        break;
                    } // t
                } // i2
            } // i1
            if (t == linkmean)
            {
                res = (CriteriaType) ((double) res / (n2 * n1));
            }
        }
        return (res);
    } // distance

    bool findNextAggreg(CriteriaType &crit, IndexSetType &oSet) const
    {
        //
        oSet.clear();
        const CriteriaVectorType &dist = this->m_curdist;
        const size_t n = this->m_children.size();
        for (size_t i = 0; i < n; ++i)
        {
            const size_t gap = i * n;
            for (size_t j = 0; j < i; ++j)
            {
                CriteriaType x = dist[gap + j];
                if (oSet.empty())
                {
                    crit = x;
                    oSet.insert((IndexType) i);
                    oSet.insert((IndexType) j);
                }
                else if (x < crit)
                {
                    crit = x;
                    oSet.clear();
                    oSet.insert((IndexType) i);
                    oSet.insert((IndexType) j);
                }
                else if (x == crit)
                {
                    auto iend = oSet.end();
                    if ((oSet.find((IndexType) i) != iend)
                            && (oSet.find((IndexType) j) == iend))
                    {
                        oSet.insert((IndexType) j);
                    }
                    else if ((oSet.find((IndexType) i) == iend)
                             && (oSet.find((IndexType) j) != iend))
                    {
                        oSet.insert((IndexType) i);
                    }
                }
            } // j
        } // i
        return (oSet.size() > 1);
    } // findNextAggreg

    template<class DISTFUNC>
    void compute_distances(const DISTFUNC &f)
    {
        TreeItems &items = this->m_children;
        const size_t n = items.size();
        if (n > 0)
        {
            this->m_curdist.resize(n * n);
            CriteriaVectorType & v = this->m_curdist;
            for (size_t i = 0; i < n; ++i)
            {
                const size_t igap = i * n;
                v[igap + i] = (W) 0;
                TreeItemPtr px1 = items[i];
                TreeItemType *p1 = px1.get();
                assert(p1 != nullptr);
                for (size_t j = 0; j < i; ++j)
                {
                    TreeItemPtr px2 = items[j];
                    TreeItemType *p2 = px2.get();
                    W x = p1->distance(*p2, f);
                    v[igap + j] = x;
                    v[j * n + i] = x;
                } // j
            } // i
        } // n
    } // compute_distances

    template<class DISTFUNC>
    bool next_step(const DISTFUNC &f, IndexType &nNodeId)
    {
        TreeItems & items = this->m_children;
        const size_t n = items.size();
        if (n < 2)
        {
            return (false);
        }
        IndexSetType oSet;
        CriteriaType dval = 0;
        if (!this->findNextAggreg(dval, oSet))
        {
            return (false);
        }
        TreeItems temp;
        auto iend1 = oSet.end();
        for (auto it = oSet.begin(); it != iend1; ++it)
        {
            IndexType ii = *it;
            TreeItemPtr p = items[ii];
            temp.push_back(p);
        } // it
        TreeItemPtr pp = std::make_shared<TreeItemType > ();
        TreeItemType *px = pp.get();
        px->m_parent = this;
        px->m_link = this->m_link;
        px->m_level = dval;
        px->m_id = ++nNodeId;
        auto iend2 = temp.end();
        for (auto it = temp.begin(); it != iend2; ++it)
        {
            TreeItemPtr p = *it;
            TreeItemType *pk = p.get();
            auto jt = std::find_if(items.begin(), items.end(),
                                   [ = ](TreeItemPtr & px)->bool
            {
                return (px.get() == pk);
            });
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
    void update_desc(DISTFUNC &f)
    {
        CriteriaType svar = 0;
        size_t vn = 0;
        TreeItems items;
        this->split(items);
        const size_t nx = items.size();
        if (nx > 1)
        {
            for (size_t i = 0; i < nx; ++i)
            {
                TreeItemPtr &px1 = items[i];
                TreeItemType *p1 = px1.get();
                const size_t ii = p1->m_id;
                for (size_t j = 0; j < i; ++j)
                {
                    TreeItemPtr &px2 = items[j];
                    TreeItemType *p2 = px2.get();
                    const size_t jj = p2->m_id;
                    if (ii != jj)
                    {
                        svar = (CriteriaType) (svar + f(ii, jj));
                        ++vn;
                    }
                } // j
            } // i
            if (vn > 1)
            {
                svar = (CriteriaType) (svar / (double) vn);
            }
        } // nx
        this->m_var = svar;
    } // update_desc

    template<class AllocS>
    std::ostream & internalWriteTo(std::ostream &out,
                                   const std::vector<std::string, AllocS> &names) const
    {
        const TreeItems &items = this->m_children;
        const size_t n = items.size();
        const size_t nId = (size_t) this->m_id;
        if (n == 0)
        {
            std::stringstream os;
            os << " [" << nId << "] ";
            if (nId < names.size())
            {
                std::string ss = names[nId];
                if (!ss.empty())
                {
                    os << ss << " ";
                }
            }
            std::string slabel = os.str();
            out << nId << " [shape = box,style=filled,color=yellow, label=\""
            << slabel << "\"];" << std::endl;
        }
        else
        {
            out << nId << " [label=\"S" << nId << "\\nVar = "
            << this->variance() << "\"];" << std::endl;
            for (size_t i = 0; i < n; ++i)
            {
                const TreeItemPtr &pp = items[i];
                const TreeItemType *p = pp.get();
                assert(p != nullptr);
                p->internalWriteTo(out, names);
                out << nId << " -> " << p->id() << ";" << std::endl;
            } // i
        }
        return (out);
    } // internalWriteTo

    template<class AllocS>
    std::wostream & internalWriteTo(std::wostream &out,
                                    const std::vector<std::wstring, AllocS> &names) const
    {
        const TreeItems &items = this->m_children;
        const size_t n = items.size();
        const size_t nId = (size_t) this->m_id;
        if (n == 0)
        {
            std::wstringstream os;
            os << L" [" << nId << L"] ";
            if (nId < names.size())
            {
                std::wstring ss = names[nId];
                if (!ss.empty())
                {
                    os << ss << L" ";
                }
            }
            std::wstring slabel = os.str();
            out << nId << L" [shape = box,style=filled,color=yellow, label=\""
            << slabel << L"\"];" << std::endl;
        }
        else
        {
            out << nId << L" [label=\"S" << nId << L"\\nVar = "
            << this->variance() << L"\"];" << std::endl;
            for (size_t i = 0; i < n; ++i)
            {
                const TreeItemPtr &pp = items[i];
                const TreeItemType *p = pp.get();
                p->internalWriteTo(out, names);
                out << nId << L" -> " << p->id() << L";" << std::endl;
            } // i
        }
        return (out);
    } // internalWriteTo
public:

    template<class AllocS>
    std::ostream & writeTo(std::ostream &out,
                           const std::vector<std::string, AllocS> &names) const
    {
        out << std::endl;
        out << "digraph G { " << std::endl;
        out << "size = \"4,4\";" << std::endl;
        const TreeItems &items = this->children();
        const size_t n = items.size();
        if (n == 0)
        {
            IndexType nId = this->id();
            std::stringstream os;
            os << " [" << nId << "] ";
            if ((size_t) nId < names.size())
            {
                std::string ss = names[nId];
                if (!ss.empty())
                {
                    os << ss << " ";
                }
            }
            std::string slabel = os.str();
            out << nId << " [shape = box,style=filled,color=yellow, label=\""
            << slabel << "\"];" << std::endl;
        }
        else
        {
            for (size_t i = 0; i < n; ++i)
            {
                const TreeItemPtr &pp = items[i];
                const TreeItemType *p = pp.get();
                assert(p != nullptr);
                p->internalWriteTo(out, names);
            } // i
        }
        out << "}" << std::endl;
        return (out);
    } // writeTo
    template<class AllocS>
    std::wostream & writeTo(std::wostream &out,
                            const std::vector<std::wstring, AllocS> &names) const
    {
        out << std::endl;
        out << L"digraph G { " << std::endl;
        out << L"size = \"4,4\";" << std::endl;
        const TreeItems &items = this->children();
        const size_t n = items.size();
        if (n == 0)
        {
            IndexType nId = this->id();
            std::wstringstream os;
            os << L" [" << nId << L"] ";
            if ((size_t) nId < names.size())
            {
                std::wstring ss = names[nId];
                if (!ss.empty())
                {
                    os << ss << L" ";
                }
            }
            std::wstring slabel = os.str();
            out << nId << L" [shape = box,style=filled,color=yellow, label=\""
            << slabel << L"\"];" << std::endl;
        }
        else
        {
            for (size_t i = 0; i < n; ++i)
            {
                const TreeItemPtr &pp = items[i];
                const TreeItemType *p = pp.get();
                p->internalWriteTo(out, names);
            } // i
        }
        out << L"}" << std::endl;
        return (out);
    } // writeTo
    template<typename X, class ALLOCX>
    void split(std::vector<X, ALLOCX> &oVec) const
    {
        if (this->m_children.empty())
        {
            oVec.push_back((X) this->m_id);
            return;
        }
        const TreeItems &items = this->m_children;
        const size_t n = items.size();
        for (size_t i = 0; i < n; ++i)
        {
            const TreeItemPtr & px = items[i];
            const TreeItemType *p = px.get();
            p->split(oVec);
        } // i
    } // split

    void split(TreeItems &elems) const
    {
        const TreeItems &items = this->m_children;
        const size_t n = items.size();
        if (n < 1)
        {
            return;
        }
        for (size_t i = 0; i < n; ++i)
        {
            const TreeItemPtr & px = items[i];
            const TreeItemType *p = px.get();
            if (p->m_children.empty())
            {
                elems.push_back(px);
            }
            else
            {
                p->split(elems);
            }
        } // i
    } // split

    template <class VISITORFUNC>
    void visit(const VISITORFUNC &func) const
    {
        const TreeItems &items = this->m_children;
        const size_t n = items.size();
        for (size_t i = 0; i < n; ++i)
        {
            const TreeItemPtr & px = items[i];
            const TreeItemType *p = px.get();
            p->visit(func);
        }// i
        func(*this);
    }// visit

    template<class DISTFUNC>
    void clusterize(const DISTFUNC &f, const size_t nClasses = 1)
    {
        this->compute_distances(f);
        this->update_desc(f);
        TreeItems & items = this->m_children;
        IndexType nNodeId = (IndexType) (items.size() + 1000);
        while (items.size() > nClasses)
        {
            if (!this->next_step(f, nNodeId))
            {
                break;
            }
        } // nClasses
    } // clusterize

    template<class DISTFUNC>
    void aggreg_leaves(const DISTFUNC &f, const size_t nClasses = 1)
    {
        this->compute_distances(f);
        TreeItems & items = this->m_children;
        IndexType nNodeId = (IndexType) (items.size() + 1000);
        while (items.size() > nClasses)
        {
            if (!this->next_step(f, nNodeId))
            {
                break;
            }
        } // nClasses
        size_t n = items.size();
        TreeItems elems;
        for (size_t i = 0; i < n; ++i)
        {
            TreeItemPtr & pp = items[i];
            TreeItemType *p = pp.get();
            if (!p->m_children.empty())
            {
                TreeItems elemsx;
                p->split(elemsx);
                for (auto it = elemsx.begin(); it != elemsx.end(); ++it)
                {
                    TreeItemPtr &p0 = *it;
                    TreeItemType *pp0 = p0.get();
                    pp0->m_parent = p;
                }// it
                p->m_children = elemsx;
                p->compute_distances(f);
                p->update_desc(f);
            }
            p->m_parent = this;
            elems.push_back(pp);
        } // i
        this->m_children = elems;
        this->compute_distances(f);
        this->update_desc(f);
    } // aggreg_leaves
public:

    template<class DISTFUNC>
    static void clusterize(size_t n, const DISTFUNC &fdist, TreeItemType &oTree,
                           const size_t nClasses = 1, const LinkType t = linkmean)
    {
        TreeItemType oCur(n, t);
        oCur.clusterize(fdist, nClasses);
        oTree.m_link = oCur.m_link;
        oTree.m_id = oCur.m_id;
        oTree.m_var = oCur.m_var;
        oTree.m_level = oCur.m_level;
        oTree.m_children = oCur.m_children;
        oTree.m_curdist = oCur.m_curdist;
    } // clusterize

    template<typename X, class AllocX>
    static void clusterize(size_t n, const std::vector<X, AllocX> &distances,
                           TreeItemType &oTree, const size_t nClasses = 1, const LinkType t =
                               linkmean)
    {
        auto fdist = [ = ](IndexType i1, IndexType i2)->CriteriaType
        {
            return ((CriteriaType) distances[(size_t) (i1 * n + i2)]);
        };
        TreeItemType::clusterize(n, fdist, oTree, nClasses, t);
    } // clusterize

    template<typename X>
    static void clusterize(size_t n, const std::valarray<X> &distances,
                           TreeItemType &oTree, const size_t nClasses = 1, const LinkType t =
                               linkmean)
    {
        auto fdist = [ = ](IndexType i1, IndexType i2)->CriteriaType
        {
            return ((CriteriaType) distances[(size_t) (i1 * n + i2)]);
        };
        TreeItemType::clusterize(n, fdist, oTree, nClasses, t);
    } // Clusterize

    template<typename X>
    static void clusterize(size_t n, const X * pdistances,
                           TreeItemType &oTree, const size_t nClasses = 1, const LinkType t =
                               linkmean)
    {
        auto fdist = [ = ](IndexType i1, IndexType i2)->CriteriaType
        {
            return ((CriteriaType) pdistances[(size_t) (i1 * n + i2)]);
        };
        TreeItemType::clusterize(n, fdist, oTree, nClasses, t);
    } // Clusterize
    //

    template<class DISTFUNC>
    static void clusterize_leaves(size_t n, const DISTFUNC &fdist,
                                  TreeItemType &oTree, const size_t nClasses = 1, const LinkType t =
                                      linkmean)
    {
        TreeItemType oCur(n, t);
        oCur.aggreg_leaves(fdist, nClasses);
        oTree.m_link = oCur.m_link;
        oTree.m_id = oCur.m_id;
        oTree.m_var = oCur.m_var;
        oTree.m_level = oCur.m_level;
        oTree.m_children = oCur.m_children;
        oTree.m_curdist = oCur.m_curdist;
    } // clusterize_leaves

    template<typename X, class AllocX>
    static void clusterize_leaves(size_t n,
                                  const std::vector<X, AllocX> &distances, TreeItemType &oTree,
                                  const size_t nClasses = 1, const LinkType t = linkmean)
    {
        assert(distances.size() >= (size_t) (n * n));
        auto fdist = [ = ](IndexType i1, IndexType i2)->CriteriaType
        {
            return ((CriteriaType) distances[(size_t) (i1 * n + i2)]);
        };
        TreeItemType::clusterize_leaves(n, fdist, oTree, nClasses, t);
    } // clusterize_leaves

    template<typename X>
    static void clusterize_leaves(size_t n,
                                  const std::valarray<X> &distances, TreeItemType &oTree,
                                  const size_t nClasses = 1, const LinkType t = linkmean)
    {
        auto fdist = [ = ](IndexType i1, IndexType i2)->CriteriaType
        {
            return ((CriteriaType) distances[(size_t) (i1 * n + i2)]);
        };
        TreeItemType::clusterize_leaves(n, fdist, oTree, nClasses, t);
    } // Clusterize_Leaves

    template<typename X>
    static void clusterize_leaves(size_t n,
                                  const X *pdistances, TreeItemType &oTree,
                                  const size_t nClasses = 1, const LinkType t = linkmean)
    {
        auto fdist = [ = ](IndexType i1, IndexType i2)->CriteriaType
        {
            return ((CriteriaType) pdistances[(size_t) (i1 * n + i2)]);
        };
        TreeItemType::clusterize_leaves(n, fdist, oTree, nClasses, t);
    } // Clusterize_Leaves
}; // class TreeItem<W,Z,ALLOCW, ALLOCZ>
///////////////////////////////

template<typename W, typename Z, class ALLOCW, class ALLOCZ, class ALLOCVEC>
void ClusterizeData(size_t n, const std::vector<W, ALLOCW> &distances,
                    std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC> &indexes,
                    size_t nClasses = 5, statdata::LinkType ltype = statdata::linkmean)
{
    typedef TreeItem<W, Z, ALLOCW, ALLOCZ> TreeType;
    typedef typename TreeType::TreeItemType TreeItemType;
    //
    TreeType oTree;
    TreeType::clusterize_leaves(n, distances, oTree, nClasses, ltype);
    const size_t nx = oTree.size();
    indexes = std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC > (nx);
    for (size_t i = 0; i < nx; ++i)
    {
        const TreeItemType *p = oTree.child_at(i);
        std::vector<Z, ALLOCZ> oVec;
        p->split(oVec);
        indexes[i] = oVec;
    } // i
} // ClusterizeData
///////////////////////////

template<typename W, typename Z, class ALLOCZ, class ALLOCVEC>
void ClusterizeData(size_t n, const std::valarray<W> &distances,
                    std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC> &indexes,
                    size_t nClasses = 5, statdata::LinkType ltype = statdata::linkmean)
{
    typedef TreeItem<W, Z, std::allocator<W>, ALLOCZ> TreeType;
    typedef typename TreeType::TreeItemType TreeItemType;
    size_t nn = distances.size();
    //
    std::vector<W> xdist(nn);
    for (size_t i = 0; i < nn; ++i)
    {
        xdist[i] = distances[i];
    }
    TreeType oTree;
    TreeType::clusterize_leaves(n, xdist, oTree, nClasses, ltype);
    const size_t nx = oTree.size();
    indexes = std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC > (nx);
    for (size_t i = 0; i < nx; ++i)
    {
        const TreeItemType *p = oTree.child_at(i);
        assert(p != nullptr);
        std::vector<Z, ALLOCZ> oVec;
        p->split(oVec);
        indexes[i] = oVec;
    } // i
} // ClusterizeData
///////////////////////////

template<typename W, typename Z, class ALLOCZ, class ALLOCVEC>
void ClusterizeData(size_t n, const W *pdistances,
                    std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC> &indexes,
                    size_t nClasses = 5, statdata::LinkType ltype = statdata::linkmean)
{
    typedef TreeItem<W, Z, std::allocator<W>, ALLOCZ> TreeType;
    typedef typename TreeType::TreeItemType TreeItemType;
    size_t nn = (size_t) (n * n);
    std::vector<W> xdist(nn);
    for (size_t i = 0; i < nn; ++i)
    {
        xdist[i] = pdistances[i];
    }
    TreeType oTree;
    TreeType::clusterize_leaves(n, xdist, oTree, nClasses, ltype);
    const size_t nx = oTree.size();
    indexes = std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC > (nx);
    for (size_t i = 0; i < nx; ++i)
    {
        const TreeItemType *p = oTree.child_at(i);
        std::vector<Z, ALLOCZ> oVec;
        p->split(oVec);
        indexes[i] = oVec;
    } // i
} // ClusterizeData
///////////////////////////

template<class DISTFUNC, typename Z, class ALLOCZ, class ALLOCVEC>
void ClusterizeData(size_t n, const DISTFUNC &fdist,
                    std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC> &indexes,
                    size_t nClasses = 5, statdata::LinkType ltype = statdata::linkmean)
{
    typedef float W;
    typedef std::allocator<W> ALLOCW;
    typedef TreeItem<W, Z, ALLOCW, ALLOCZ> TreeType;
    typedef typename TreeType::TreeItemType TreeItemType;
    TreeType oTree;
    TreeType::clusterize_leaves(n, fdist, oTree, nClasses, ltype);
    const size_t nx = oTree.size();
    indexes = std::vector<std::vector<Z, ALLOCZ>, ALLOCVEC > (nx);
    for (size_t i = 0; i < nx; ++i)
    {
        const TreeItemType *p = oTree.child_at(i);
        std::vector<Z, ALLOCZ> oVec;
        p->split(oVec);
        indexes[i] = oVec;
    } // i
} // ClusterizeData
///////////////////////////////////////
} // namespace statdata
///////////////////////////////////////////

template<typename W, typename Z, class AllocW, class AllocZ>
std::ostream & operator<<(std::ostream &os,
                          const statdata::TreeItem<W, Z, AllocW, AllocZ> &item)
{
    std::vector<std::string> names;
    return (item.writeTo(os, names));
} // operator<<
template<typename W, typename Z, class AllocW, class AllocZ>
std::wostream & operator<<(std::wostream &os,
                           const statdata::TreeItem<W, Z, AllocW, AllocZ> &item)
{
    std::vector<std::wstring> names;
    return (item.writeTo(os, names));
} // operator<<
/////////////////////////////////////
#endif /* TREEITEM_H_ */
