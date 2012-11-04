/* 
 * File:   omp_arrange.h
 * Author: boubad
 *
 * Created on 4 novembre 2012, 13:04
 */

#ifndef OMP_ARRANGE_H
#define	OMP_ARRANGE_H
/////////////////////////////////
#include "arrangedesc.h"
/////////////////////////////////
namespace statdata
{
/////////////////////////////////////

template<typename T, typename W>
void OMPMinPath(size_t n, const T *pDist, W *pIndexes, T &crit)
{
    assert(n > 0);
    assert(pDist != nullptr);
    assert(pIndexes != nullptr);
    //
    bool bFirst = true;
    T cBest = 0;
    ArrangeDesc<T, W> oRes;
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        W iStart = (W) i;
        ArrangeDesc<T, W> oDesc(n, pDist, iStart);
        oDesc.arrange();
        T x = oDesc.criteria();
#pragma omp critical
        {
            if (bFirst)
            {
                bFirst = false;
                cBest = x;
                oRes = oDesc;
            }
            else if (x < cBest)
            {
                cBest = x;
                oRes = oDesc;
            }
        }// critical
    } // i
    assert(oRes.current_size() == n);
    crit = oRes.criteria();
    const W *p = oRes.current_indexes();
    assert(p != nullptr);
    ::memcpy(pIndexes, p, n * sizeof (W));
} //OMPMinPath
////////////////////////////////////////

template<typename T, typename W, class DISTFUNC>
void OMPMinPath(const DISTFUNC &func, size_t n, W* pIndex, T &crit)
{
    assert(n > 0);
    assert(pIndex != nullptr);
    //
    std::unique_ptr<T> oTemp(new T[n * n]);
    T *pDist = oTemp.get();
    assert(pDist != nullptr);
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        size_t gap = (size_t) (i * n);
        pDist[gap + i] = (T) 0;
        for (size_t j = 0; j < i; ++j)
        {
            T v = func(i, j);
            pDist[gap + j] = v;
            pDist[j * n + i] = v;
        } // j
    } // i
    statdata::OMPMinPath(n, pDist, pIndex, crit);
} //SerialMinPath

template<typename T, typename W, class ALLOCT, class ALLOCW>
void OMPMinPath(size_t n, const std::vector<T, ALLOCT> &oDist,
                std::vector<W, ALLOCW> &oIndexes, T &crit)
{
    assert(n > 0);
    size_t nn = (size_t) (n * n);
    assert(oDist.size() >= nn);
    //
    std::unique_ptr<T> oTemp(new T[nn]);
    T *pDist = oTemp.get();
    assert(pDist != nullptr);
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        size_t gap = (size_t) (i * n);
        pDist[gap + i] = (T) 0;
        for (size_t j = 0; j < i; ++j)
        {
            size_t k1 = (size_t) (gap + j);
            T v = oDist[k1];
            pDist[k1] = v;
            pDist[j * n + i] = v;
        } // j
    } // i
    bool bFirst = true;
    T cBest = 0;
    ArrangeDesc<T, W> oRes;
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        W iStart = (W) i;
        ArrangeDesc<T, W> oDesc(n, pDist, iStart);
        oDesc.arrange();
        T x = oDesc.criteria();
#pragma omp critical
        {
            if (bFirst)
            {
                bFirst = false;
                cBest = x;
                oRes = oDesc;
            }
            else if (x < cBest)
            {
                cBest = x;
                oRes = oDesc;
            }
        }
    } // i
    assert(oRes.current_size() == n);
    oIndexes.resize(n);
    crit = oRes.criteria();
    const W *pIndex = oRes.current_indexes();
    for (size_t i = 0; i < n; ++i)
    {
        oIndexes[i] = pIndex[i];
    } // i
} //OMPMinPath

template<typename T, typename W, class DISTFUNC, class ALLOCW>
void OMPMinPath(const DISTFUNC &func, size_t n,
                std::vector<W, ALLOCW> &oIndexes, T &crit)
{
    assert(n > 0);
    size_t nn = (size_t) (n * n);
    //
    std::vector<T> oDist(nn);
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        size_t gap = (size_t) (i * n);
        oDist[gap + i] = (T) 0;
        for (size_t j = 0; j < i; ++j)
        {
            T v = func(i, j);
            oDist[gap + i] = v;
            oDist[j * n + i] = v;
        } // j
    } // i
    statdata::OMPMinPath(n, oDist, oIndexes, crit);
} //OMPMinPath

template<typename T, typename W>
void OMPMinPath(size_t n, const std::valarray<T> &oDist,
                std::valarray<W> &oIndexes, T &crit)
{
    assert(n > 0);
    size_t nn = (size_t) (n * n);
    assert(oDist.size() >= nn);
    //
    std::unique_ptr<T> oTemp(new T[nn]);
    T *pDist = oTemp.get();
    assert(pDist != nullptr);
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        size_t gap = (size_t) (i * n);
        pDist[gap + i] = (T) 0;
        for (size_t j = 0; j < i; ++j)
        {
            size_t k1 = (size_t) (gap + j);
            T v = oDist[k1];
            pDist[k1] = v;
            pDist[j * n + i] = v;
        } // j
    } // i
    bool bFirst = true;
    T cBest = 0;
    ArrangeDesc<T, W> oRes;
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        W iStart = (W) i;
        ArrangeDesc<T, W> oDesc(n, pDist, iStart);
        oDesc.arrange();
        T x = oDesc.criteria();
#pragma omp critical
        {
            if (bFirst)
            {
                bFirst = false;
                cBest = x;
                oRes = oDesc;
            }
            else if (x < cBest)
            {
                cBest = x;
                oRes = oDesc;
            }
        }
    } // i
    assert(oRes.current_size() == n);
    oIndexes.resize(n);
    crit = oRes.criteria();
    const W *pIndex = oRes.current_indexes();
    for (size_t i = 0; i < n; ++i)
    {
        oIndexes[i] = pIndex[i];
    } // i
} //SerialMinPath

template<typename T, typename W, class DISTFUNC>
void OMPMinPath(const DISTFUNC &func, size_t n, std::valarray<W> &oIndexes,
                T &crit)
{
    assert(n > 0);
    size_t nn = (size_t) (n * n);
    //
    std::valarray<T> oDist(nn);
#pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
    {
        size_t gap = (size_t) (i * n);
        oDist[gap + i] = (T) 0;
        for (size_t j = 0; j < i; ++j)
        {
            T v = func(i, j);
            oDist[gap + i] = v;
            oDist[j * n + i] = v;
        } // j
    } // i
    statdata::SerialMinPath(n, oDist, oIndexes, crit);
} //OMPMinPath
///////////////////////////////
}// namespace statdata
///////////////////////////////////
#endif	/* OMP_ARRANGE_H */
