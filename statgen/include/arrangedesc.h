/*
 * arrangedesc.h
 *
 *  Created on: Oct 30, 2012
 *      Author: boubad
 */

#ifndef ARRANGEDESC_H_
#define ARRANGEDESC_H_
///////////////////////////////////
#include "dataport.h"
///////////////////////////////////
#include <cassert>
#include <memory>
#include <vector>
#include <valarray>
#include <string.h>
/////////////////////////////////
namespace statdata {
    /////////////////////////////////////

    template<typename T = float, typename W = short>
            class ArrangeDesc {
    public:

        ArrangeDesc() :
        m_ntotal(0), m_ncur(0), m_nleft(0), m_crit(0), m_pdist(nullptr) {
        }
    public:

        ArrangeDesc(size_t nTotal, const T *pDist, W iStart = 0) :
        m_ntotal(0), m_ncur(0), m_nleft(0), m_crit(0), m_pdist(pDist) {
            assert(nTotal > 0);
            assert(this->m_pdist != nullptr);
            this->m_ntotal = nTotal;
            this->m_ocur.reset(new W[nTotal]);
            assert(this->m_ocur.get() != nullptr);
            this->m_oleft.reset(new W[nTotal]);
            W *p = this->m_oleft.get();
            assert(p != nullptr);
            size_t j = 0;
            for (size_t i = 0; i < nTotal; ++i) {
                if ((W) i != iStart) {
                    p[j++] = (W) i;
                }
            } // i
            this->m_nleft = j;
            this->m_ncur = 1;
            W *px = this->m_ocur.get();
            assert(px != nullptr);
            *px = iStart;
        }

        ArrangeDesc(const ArrangeDesc<T, W> &other) :
        m_ntotal(0), m_ncur(0), m_nleft(0), m_crit(other.m_crit), m_pdist(
        other.m_pdist) {
            size_t nTotal = other.m_ntotal;
            if (nTotal > 0) {
                assert(this->m_pdist != nullptr);
                this->m_ocur.reset(new W[nTotal]);
                assert(this->m_ocur.get() != nullptr);
                this->m_oleft.reset(new W[nTotal]);
                assert(this->m_oleft.get() != nullptr);
                size_t n = other.m_ncur;
                this->m_ncur = n;
                assert(other.m_ocur.get() != nullptr);
                ::memcpy(this->m_ocur.get(), other.m_ocur.get(), n * sizeof (W));
                n = other.m_nleft;
                this->m_nleft = n;
                assert(other.m_oleft.get() != nullptr);
                ::memcpy(this->m_oleft.get(), other.m_oleft.get(), n * sizeof (W));
                this->m_ntotal = nTotal;
            } // nTotal
        }

        ArrangeDesc<T, W> & operator=(const ArrangeDesc<T, W> &other) {
            if (this != &other) {
                this->m_ocur.reset();
                this->m_oleft.reset();
                this->m_ntotal = other.m_ntotal;
                this->m_ncur = other.m_ncur;
                this->m_nleft = other.m_nleft;
                this->m_crit = other.m_crit;
                size_t nTotal = other.m_ntotal;
                if (nTotal > 0) {
                    this->m_pdist = other.m_pdist;
                    assert(this->m_pdist != nullptr);
                    this->m_ocur.reset(new W[nTotal]);
                    assert(this->m_ocur.get() != nullptr);
                    this->m_oleft.reset(new W[nTotal]);
                    assert(this->m_oleft.get() != nullptr);
                    size_t n = other.m_ncur;
                    assert(other.m_ocur.get() != nullptr);
                    ::memcpy(this->m_ocur.get(), other.m_ocur.get(), n * sizeof (W));
                    n = other.m_nleft;
                    assert(other.m_oleft.get() != nullptr);
                    ::memcpy(this->m_oleft.get(), other.m_oleft.get(),
                            n * sizeof (W));
                    this->m_ntotal = nTotal;
                } // nTotal
            }
            return (*this);
        }

        ~ArrangeDesc() {
            this->m_ocur.reset();
            this->m_oleft.reset();
        }
    public:

        size_t total_size(void) const {
            return (this->m_ntotal);
        }

        size_t current_size(void) const {
            return (this->m_ncur);
        }

        size_t left_size(void) const {
            return (this->m_nleft);
        }

        const W *current_indexes(void) const {
            return (this->m_ocur.get());
        }

        W *current_indexes(void) {
            return (this->m_ocur.get());
        }

        const W *left_indexes(void) const {
            return (this->m_oleft.get());
        }

        W *left_indexes(void) {
            return (this->m_oleft.get());
        }

        const T *distances(void) const {
            return (this->m_pdist);
        }

        T criteria(void) const {
            return (this->m_crit);
        }
    public:

        void arrange(void) {
            const size_t n = this->m_ntotal;
            if (n < 3) {
                return;
            }
            const T * pDist = this->m_pdist;
            assert(pDist != nullptr);
            std::unique_ptr<W> oBuf(new W[n]);
            W *pIndex = oBuf.get();
            assert(pIndex != nullptr);
            bool bDone = false;
            while (!bDone) {
                bDone = true;
                size_t nCur = this->m_ncur;
                size_t nLeft = this->m_nleft;
                if ((nCur < 1) || (nCur == n) || (nLeft < 1)) {
                    break;
                }
                W *pCur = this->m_ocur.get();
                assert(pCur != nullptr);
                W iLast = pCur[nCur - 1];
                const size_t gap = (size_t) (iLast * n);
                W *pLeft = this->m_oleft.get();
                assert(pLeft != nullptr);
                T cBest = 0;
                size_t nx = 0;
                for (size_t i = 0; i < nLeft; ++i) {
                    W cur = pLeft[i];
                    T crit = pDist[gap + cur];
                    if ((nx < 1) || (crit < cBest)) {
                        pIndex[0] = cur;
                        cBest = crit;
                        nx = 1;
                    } else if (crit == cBest) {
                        pIndex[nx++] = cur;
                    }
                } // i
                if (nx < 1) {
                    break;
                }
                bDone = false;
                if (nx == 1) {
                    W iNext = pIndex[0];
                    pCur[nCur] = iNext;
                    this->m_ncur++;
                    this->m_crit = (T) (this->m_crit + pDist[gap + iNext]);
                    for (size_t i = 0; i < nLeft; ++i) {
                        if (pLeft[i] == iNext) {
                            size_t nm = nLeft - 1 - i;
                            if (nm > 0) {
                                W *pDest = &(pLeft[i]);
                                W *pSrc = (W *) (pDest + 1);
                                ::memmove(pDest, pSrc, nm * sizeof (W));
                            }
                            this->m_nleft--;
                            break;
                        } // found
                    } // i
                } else {
                    ArrangeDesc<T, W> oRes;
                    bool bFirst = true;
                    for (size_t i = 0; i < nx; ++i) {
                        W iNext = pIndex[i];
                        ArrangeDesc<T, W> oWork(*this);
                        W *px = oWork.m_ocur.get();
                        px[nCur] = iNext;
                        oWork.m_ncur++;
                        oWork.m_crit = (T) (oWork.m_crit + pDist[gap + iNext]);
                        W *py = oWork.m_oleft.get();
                        for (size_t k = 0; k < nLeft; ++k) {
                            if (py[k] == iNext) {
                                size_t nm = nLeft - 1 - k;
                                if (nm > 0) {
                                    W *pDest = &(py[k]);
                                    W *pSrc = (W *) (pDest + 1);
                                    ::memmove(pDest, pSrc, nm * sizeof (W));
                                }
                                oWork.m_nleft--;
                                break;
                            } // found
                        } // k
                        oWork.arrange();
                        if (bFirst) {
                            oRes = oWork;
                            bFirst = false;
                        } else if (oWork.m_crit < oRes.m_crit) {
                            oRes = oWork;
                        }
                    } // i
                    *this = oRes;
                }
            } // not done
        } // arrange
    protected:
        size_t m_ntotal;
        size_t m_ncur;
        size_t m_nleft;
        T m_crit;
        const T *m_pdist;
        std::unique_ptr<W> m_ocur;
        std::unique_ptr<W> m_oleft;
    };
    // class ArrangeDesc<T,W>
    //////////////////////////////////////////

    template<typename T, typename W>
    void SingleMinPath(size_t n, const T *pDist, W iStart, W* pIndex, T &crit) {
        assert(n > 0);
        assert(pDist != nullptr);
        assert(pIndex != nullptr);
        assert((size_t) iStart < n);
        //
        ArrangeDesc<T, W> oDesc(n, pDist, iStart);
        oDesc.arrange();
        assert(oDesc.current_size() == n);
        crit = oDesc.criteria();
        const W *p = oDesc.current_indexes();
        assert(p != nullptr);
        ::memcpy(pIndex, p, n * sizeof (W));
    } //SingleMinPath

    template<typename T, typename W, class DISTFUNC>
    void SingleMinPath(const DISTFUNC &func, size_t n, W iStart, W* pIndex,
    T &crit) {
        assert(n > 0);
        assert(pIndex != nullptr);
        assert((size_t) iStart < n);
        //
        std::unique_ptr<T> oTemp(new T[n * n]);
        T *pDist = oTemp.get();
        assert(pDist != nullptr);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            pDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                T v = func(i, j);
                pDist[gap + j] = v;
                pDist[j * n + i] = v;
            } // j
        } // i
        statdata::SingleMinPath(n, pDist, iStart, pIndex, crit);
    } //SingleMinPath

    template<typename T, typename W>
    void SerialMinPath(size_t n, const T *pDist, W *pIndexes, T &crit) {
        assert(n > 0);
        assert(pDist != nullptr);
        assert(pIndexes != nullptr);
        //
        bool bFirst = true;
        T cBest = 0;
        ArrangeDesc<T, W> oRes;
        for (size_t i = 0; i < n; ++i) {
            W iStart = (W) i;
            ArrangeDesc<T, W> oDesc(n, pDist, iStart);
            oDesc.arrange();
            T x = oDesc.criteria();
            if (bFirst) {
                bFirst = false;
                cBest = x;
                oRes = oDesc;
            } else if (x < cBest) {
                cBest = x;
                oRes = oDesc;
            }
        } // i
        assert(oRes.current_size() == n);
        crit = oRes.criteria();
        const W *p = oRes.current_indexes();
        assert(p != nullptr);
        ::memcpy(pIndexes, p, n * sizeof (W));
    } //SerialMinPath

    template<typename T, typename W, class DISTFUNC>
    void SerialMinPath(const DISTFUNC &func, size_t n, W* pIndex, T &crit) {
        assert(n > 0);
        assert(pIndex != nullptr);
        //
        std::unique_ptr<T> oTemp(new T[n * n]);
        T *pDist = oTemp.get();
        assert(pDist != nullptr);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            pDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                T v = func(i, j);
                pDist[gap + j] = v;
                pDist[j * n + i] = v;
            } // j
        } // i
        statdata::SerialMinPath(n, pDist, pIndex, crit);
    } //SerialMinPath

    template<typename T, typename W, class ALLOCT, class ALLOCW>
    void SingleMinPath(size_t n, const std::vector<T, ALLOCT> &oDist, W iStart,
    std::vector<W, ALLOCW> &oIndexes, T &crit) {
        assert(n > 0);
        size_t nn = (size_t) (n * n);
        assert(oDist.size() >= nn);
        assert((size_t) iStart < n);
        //
        std::unique_ptr<T> oTemp(new T[nn]);
        T *pDist = oTemp.get();
        assert(pDist != nullptr);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            pDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                size_t k1 = (size_t) (gap + j);
                T v = oDist[k1];
                pDist[k1] = v;
                pDist[j * n + i] = v;
            } // j
        } // i
        ArrangeDesc<T, W> oDesc(n, pDist, iStart);
        oDesc.arrange();
        assert(oDesc.current_size() == n);
        oIndexes.resize(n);
        crit = oDesc.criteria();
        const W *pIndex = oDesc.current_indexes();
        for (size_t i = 0; i < n; ++i) {
            oIndexes[i] = pIndex[i];
        } // i
    } //SingleMinPath

    template<typename T, typename W, class DISTFUNC, class ALLOCW>
    void SingleMinPath(const DISTFUNC &func, size_t n, W iStart,
    std::vector<W, ALLOCW> &oIndexes, T &crit) {
        assert(n > 0);
        size_t nn = (size_t) (n * n);
        assert((size_t) iStart < n);
        //
        std::vector<T> oDist(nn);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            oDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                T v = func(i, j);
                oDist[gap + i] = v;
                oDist[j * n + i] = v;
            } // j
        } // i
        statdata::SingleMinPath(n, oDist, iStart, oIndexes, crit);
    } //SingleMinPath

    template<typename T, typename W, class ALLOCT, class ALLOCW>
    void SerialMinPath(size_t n, const std::vector<T, ALLOCT> &oDist,
    std::vector<W, ALLOCW> &oIndexes, T &crit) {
        assert(n > 0);
        size_t nn = (size_t) (n * n);
        assert(oDist.size() >= nn);
        //
        std::unique_ptr<T> oTemp(new T[nn]);
        T *pDist = oTemp.get();
        assert(pDist != nullptr);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            pDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                size_t k1 = (size_t) (gap + j);
                T v = oDist[k1];
                pDist[k1] = v;
                pDist[j * n + i] = v;
            } // j
        } // i
        bool bFirst = true;
        T cBest = 0;
        ArrangeDesc<T, W> oRes;
        for (size_t i = 0; i < n; ++i) {
            W iStart = (W) i;
            ArrangeDesc<T, W> oDesc(n, pDist, iStart);
            oDesc.arrange();
            T x = oDesc.criteria();
            if (bFirst) {
                bFirst = false;
                cBest = x;
                oRes = oDesc;
            } else if (x < cBest) {
                cBest = x;
                oRes = oDesc;
            }
        } // i
        assert(oRes.current_size() == n);
        oIndexes.resize(n);
        crit = oRes.criteria();
        const W *pIndex = oRes.current_indexes();
        for (size_t i = 0; i < n; ++i) {
            oIndexes[i] = pIndex[i];
        } // i
    } //SerialMinPath

    template<typename T, typename W, class DISTFUNC, class ALLOCW>
    void SerialMinPath(const DISTFUNC &func, size_t n,
    std::vector<W, ALLOCW> &oIndexes, T &crit) {
        assert(n > 0);
        size_t nn = (size_t) (n * n);
        //
        std::vector<T> oDist(nn);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            oDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                T v = func(i, j);
                oDist[gap + i] = v;
                oDist[j * n + i] = v;
            } // j
        } // i
        statdata::SerialMinPath(n, oDist, oIndexes, crit);
    } //SerialMinPath

    template<typename T, typename W>
    void SingleMinPath(size_t n, const std::valarray<T> &oDist, W iStart,
            std::valarray<W> &oIndexes, T &crit) {
        assert(n > 0);
        size_t nn = (size_t) (n * n);
        assert(oDist.size() >= nn);
        assert((size_t) iStart < n);
        //
        std::unique_ptr<T> oTemp(new T[nn]);
        T *pDist = oTemp.get();
        assert(pDist != nullptr);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            pDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                size_t k1 = (size_t) (gap + j);
                T v = oDist[k1];
                pDist[k1] = v;
                pDist[j * n + i] = v;
            } // j
        } // i
        ArrangeDesc<T, W> oDesc(n, pDist, iStart);
        oDesc.arrange();
        assert(oDesc.current_size() == n);
        oIndexes.resize(n);
        crit = oDesc.criteria();
        const W *pIndex = oDesc.current_indexes();
        for (size_t i = 0; i < n; ++i) {
            oIndexes[i] = pIndex[i];
        } // i
    } //SingleMinPath

    template<typename T, typename W, class DISTFUNC>
    void SingleMinPath(const DISTFUNC &func, size_t n, W iStart,
    std::valarray<W> &oIndexes, T &crit) {
        assert(n > 0);
        size_t nn = (size_t) (n * n);
        assert((size_t) iStart < n);
        //
        std::valarray<T> oDist(nn);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            oDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                T v = func(i, j);
                oDist[gap + i] = v;
                oDist[j * n + i] = v;
            } // j
        } // i
        statdata::SingleMinPath(n, oDist, iStart, oIndexes, crit);
    } //SingleMinPath

    template<typename T, typename W>
    void SerialMinPath(size_t n, const std::valarray<T> &oDist,
            std::valarray<W> &oIndexes, T &crit) {
        assert(n > 0);
        size_t nn = (size_t) (n * n);
        assert(oDist.size() >= nn);
        //
        std::unique_ptr<T> oTemp(new T[nn]);
        T *pDist = oTemp.get();
        assert(pDist != nullptr);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            pDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                size_t k1 = (size_t) (gap + j);
                T v = oDist[k1];
                pDist[k1] = v;
                pDist[j * n + i] = v;
            } // j
        } // i
        bool bFirst = true;
        T cBest = 0;
        ArrangeDesc<T, W> oRes;
        for (size_t i = 0; i < n; ++i) {
            W iStart = (W) i;
            ArrangeDesc<T, W> oDesc(n, pDist, iStart);
            oDesc.arrange();
            T x = oDesc.criteria();
            if (bFirst) {
                bFirst = false;
                cBest = x;
                oRes = oDesc;
            } else if (x < cBest) {
                cBest = x;
                oRes = oDesc;
            }
        } // i
        assert(oRes.current_size() == n);
        oIndexes.resize(n);
        crit = oRes.criteria();
        const W *pIndex = oRes.current_indexes();
        for (size_t i = 0; i < n; ++i) {
            oIndexes[i] = pIndex[i];
        } // i
    } //SerialMinPath

    template<typename T, typename W, class DISTFUNC>
    void SerialMinPath(const DISTFUNC &func, size_t n, std::valarray<W> &oIndexes,
    T &crit) {
        assert(n > 0);
        size_t nn = (size_t) (n * n);
        //
        std::valarray<T> oDist(nn);
        for (size_t i = 0; i < n; ++i) {
            size_t gap = (size_t) (i * n);
            oDist[gap + i] = (T) 0;
            for (size_t j = 0; j < i; ++j) {
                T v = func(i, j);
                oDist[gap + i] = v;
                oDist[j * n + i] = v;
            } // j
        } // i
        statdata::SerialMinPath(n, oDist, oIndexes, crit);
    } //SerialMinPath
    ////////////////////////////////////////
} // namespace statdata
///////////////////////////////

#endif /* ARRANGEDESC_H_ */
