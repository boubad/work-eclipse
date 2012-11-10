/* 
 * File:   genrouts.h
 * Author: boubad
 *
 * Created on 9 novembre 2012, 12:53
 */

#ifndef GENROUTS_H
#define	GENROUTS_H
//////////////////////////////////////
#include "dataport.h"
////////////////////////////
#include <cassert>
#include <vector>
#include <valarray>
#include <memory>
#include <algorithm>
#include <string>
#include <iostream>
//////////////////////////////////////////
namespace statdata {
    ///////////////////

    template<typename T, typename Z>
    class IndexedValue {
    public:

        IndexedValue() :
        m_value(0), m_index(0) {
        }

        IndexedValue(Z ind, T v) :
        m_value(v), m_index(ind) {
        }

        IndexedValue(const IndexedValue<T, Z> &other) :
        m_value(other.m_value), m_index(other.m_index) {
        }

        IndexedValue<T, Z> & operator=(const IndexedValue<T, Z> &other) {
            if (this != &other) {
                this->m_value = other.m_value;
                this->m_index = other.m_index;
            }
            return (*this);
        }

        virtual ~IndexedValue() {
        }
    public:

        T value(void) const {
            return (this->m_value);
        }

        void value(Z v) {
            this->m_value = v;
        }

        Z index(void) const {
            return (this->m_index);
        }

        void index(Z n) {
            this->m_index = n;
        }

        bool operator==(const IndexedValue<T, Z> &other) const {
            return (this->m_value == other.m_value);
        }

        bool operator!=(const IndexedValue<T, Z> &other) const {
            return (this->m_value != other.m_value);
        }

        bool operator<(const IndexedValue<T, Z> &other) const {
            return (this->m_value < other.m_value);
        }

        bool operator>(const IndexedValue<T, Z> &other) const {
            return (this->m_value > other.m_value);
        }

        bool operator>=(const IndexedValue<T, Z> &other) const {
            return (this->m_value >= other.m_value);
        }

        bool operator<=(const IndexedValue<T, Z> &other) const {
            return (this->m_value <= other.m_value);
        }
    private:
        T m_value;
        Z m_index;
    };
    // class IndexedValue<T,Z>
    /////////////////////////////////////////

    template<typename X, typename T>
    void Correlations(size_t nr, size_t nv, const X *pSrc, T *pMeans, T *pStds,
            T *pCorr) {
        //
        assert(nv > 0);
        assert(nr > nv);
        assert(pSrc != nullptr);
        assert(pMeans != nullptr);
        assert(pStds != nullptr);
        assert(pCorr != nullptr);
        //
        const T den = (T) ((nr > 1) ? nr - 1 : nr);
        std::unique_ptr<T> oTemp(new T[nr * nv]);
        T *pTemp = oTemp.get();
        assert(pTemp != nullptr);
        //
        for (size_t ivar1 = 0; ivar1 < nv; ++ivar1) {
            pCorr[ivar1 * nv + ivar1] = (T) 1.0;
            T somme = 0;
            for (size_t i = 0; i < nr; ++i) {
                somme = (T) (somme + pSrc[i * nv + ivar1]);
            } // i
            const T moy1 = (T) (somme / nr);
            pMeans[ivar1] = moy1;
            somme = 0;
            for (size_t i = 0; i < nr; ++i) {
                T x = (T) (pSrc[i] - moy1);
                pTemp[i * nv + ivar1] = x;
                somme = (T) (somme + x * x);
            } // i
            somme = (T) (somme / den);
            const T ec1 = (T) std::sqrt(somme);
            pStds[ivar1] = ec1;
            if (ec1 > 0.0) {
                for (size_t ivar2 = 0; ivar2 < ivar1; ++ivar2) {
                    somme = 0;
                    for (size_t i = 0; i < nr; ++i) {
                        size_t gap = i * nv;
                        somme =
                                (T) (somme + pTemp[gap + ivar1] * pTemp[gap + ivar2]);
                    } // i
                    somme = (T) (somme / den);
                    T ec2 = pStds[ivar2];
                    if (ec2 > 0.0) {
                        T c = (T) (somme / (ec1 * ec2));
                        pCorr[ivar1 * nv + ivar2] = c;
                        pCorr[ivar2 * nv + ivar1] = c;
                    } // ec2
                } // ivar2
            } // ec1
        } // ivar1
    } // Correlations

    template<typename X, typename T, class AllocX, class AllocT>
    void Correlations(size_t nr, size_t nv,
    const std::vector<X, AllocX> &oSrc, std::vector<T, AllocT> &oMeans,
    std::vector<T, AllocT> &oStds, std::vector<T, AllocT> &oCorr) {
        //
        assert(nv > 0);
        assert(nr > nv);
        assert(oSrc.size() >= (size_t) (nr * nv));
        //
        oMeans = std::vector<T, AllocT > (nv);
        assert(oMeans.size() == nv);
        oStds = std::vector<T, AllocT > (nv);
        assert(oStds.size() == nv);
        oCorr = std::vector<T, AllocT > (nv * nv);
        assert(oCorr.size() == (size_t) (nv * nv));
        //
        const T den = (T) ((nr > 1) ? nr - 1 : nr);
        std::vector<T, AllocT> oTemp(nr * nv);
        assert(oTemp.size() == (size_t) (nr * nv));
        //
        for (size_t ivar1 = 0; ivar1 < nv; ++ivar1) {
            oCorr[ivar1 * nv + ivar1] = (T) 1.0;
            T somme = 0;
            for (size_t i = 0; i < nr; ++i) {
                somme = (T) (somme + oSrc[i * nv + ivar1]);
            } // i
            const T moy1 = (T) (somme / nr);
            oMeans[ivar1] = moy1;
            somme = 0;
            for (size_t i = 0; i < nr; ++i) {
                T x = (T) (oSrc[i] - moy1);
                oTemp[i * nv + ivar1] = x;
                somme = (T) (somme + x * x);
            } // i
            somme = (T) (somme / den);
            const T ec1 = (T) std::sqrt(somme);
            oStds[ivar1] = ec1;
            if (ec1 > 0.0) {
                for (size_t ivar2 = 0; ivar2 < ivar1; ++ivar2) {
                    somme = 0;
                    for (size_t i = 0; i < nr; ++i) {
                        size_t gap = i * nv;
                        somme =
                                (T) (somme + oTemp[gap + ivar1] * oTemp[gap + ivar2]);
                    } // i
                    somme = (T) (somme / den);
                    T ec2 = oStds[ivar2];
                    if (ec2 > 0.0) {
                        T c = (T) (somme / (ec1 * ec2));
                        oCorr[ivar1 * nv + ivar2] = c;
                        oCorr[ivar2 * nv + ivar1] = c;
                    } // ec2
                } // ivar2
            } // ec1
        } // ivar1
    } // Correlations

    template<typename X, typename T>
    void Correlations(size_t nr, size_t nv,
            const std::valarray<X> &oSrc, std::valarray<T> &oMeans,
            std::valarray<T> &oStds, std::valarray<T> &oCorr) {
        //
        assert(nv > 0);
        assert(nr > nv);
        assert(oSrc.size() >= (size_t) (nr * nv));
        //
        const T zero = (T) 0;
        oMeans.resize(nv, zero);
        oStds.resize(nv, zero);
        oCorr.resize(nv * nv, zero);
        //
        const T den = (T) ((nr > 1) ? nr - 1 : nr);
        std::valarray<T> oTemp(nr * nv);
        for (size_t ivar = 0; ivar < nv; ++ivar) {
            std::valarray<X> v0 = oSrc[std::slice(ivar, nr, nv)];
            std::valarray<T> vi(nr);
            for (size_t i = 0; i < nr; ++i) {
                vi[i] = (T) v0[i];
            } // i
            const T moy = (T) (vi.sum() / nr);
            oMeans[ivar] = moy;
            std::valarray<T> ti = vi - moy;
            oTemp[std::slice(ivar, nr, nv)] = ti;
            std::valarray<T> tii = ti * ti;
            const T eci = (T) std::sqrt(tii.sum() / den);
            if (eci > 0) {
                oStds[ivar] = eci;
                for (size_t jvar = 0; jvar <= ivar; ++jvar) {
                    const T ecj = oStds[jvar];
                    if (ecj > 0) {
                        std::valarray<T> tj = oTemp[std::slice(jvar, nr, nv)];
                        std::valarray<T> tij = ti * tj;
                        T corr = (T) ((tij.sum() / den) / (eci * ecj));
                        oCorr[ivar * nv + jvar] = corr;
                        if (ivar != jvar) {
                            oCorr[jvar * nv + ivar] = corr;
                        }
                    } // ecj
                } // jvar
            } // eci
        } // ivar
    } // Correlations

    ////////////////////////////////////////////

    template<typename T, typename W>
    void Inverse(size_t n, const T *psrc, W *pres) {
        assert(n > 0);
        assert(psrc != nullptr);
        assert(pres != nullptr);
        for (size_t i = 0; i < n; ++i) {
            W x = (W) psrc[i];
            if (x != (W) 0) {
                pres[i] = (W) (1.0 / x);
            } else {
                pres[i] = x;
            }
        } // i
    } // Inverse

    template<typename T, typename W>
    void Inverse(const std::valarray<T> &src, std::valarray<W> &res) {
        const size_t n = src.size();
        res.resize(n);
        for (size_t i = 0; i < n; ++i) {
            W x = (W) src[i];
            if (x != (W) 0) {
                res[i] = (W) (1.0 / x);
            } else {
                res[i] = x;
            }
        } // i
    } // Inverse

    template<typename T, typename W, class AllocT, class AllocW>
    void Inverse(const std::vector<T, AllocT> &src, std::vector<W, AllocW> &res) {
        const size_t n = src.size();
        res = std::vector<W, AllocW > (n);
        for (size_t i = 0; i < n; ++i) {
            W x = (W) src[i];
            if (x != (W) 0) {
                res[i] = (W) (1.0 / x);
            } else {
                res[i] = x;
            }
        } // i
    } // Inverse
    ///////////////////////////////////////////////////

    template<typename T, typename W>
    bool MeanStd(size_t n, const T *pdata, W & vMean, W &vStd) {
        assert(n > 0);
        assert(pdata != nullptr);
        bool bRet = false;
        W somme = (W) 0;
        for (size_t i = 0; i < n; ++i) {
            somme = (W) (somme + pdata[i]);
        } // i
        if (n > 0) {
            const W m = (W) (somme / n);
            vMean = m;
            if (n > 1) {
                somme = (W) 0;
                for (size_t i = 0; i < n; ++i) {
                    W x = (W) (pdata[i] - m);
                    somme = (W) (somme + x * x);
                } // i
                vStd = (W) std::sqrt(somme / (n - 1));
                bRet = true;
            } // n
        } //
        return bRet;
    } // MeanStd

    template<typename T, typename W>
    bool MeanStd(const std::valarray<T> &data, W & vMean, W &vStd) {
        const size_t n = data.size();
        bool bRet = false;
        W somme = (W) 0;
        for (size_t i = 0; i < n; ++i) {
            somme = (W) (somme + data[i]);
        } // i
        if (n > 0) {
            const W m = (W) (somme / n);
            vMean = m;
            if (n > 1) {
                somme = (W) 0;
                for (size_t i = 0; i < n; ++i) {
                    W x = (W) (data[i] - m);
                    somme = (W) (somme + x * x);
                } // i
                vStd = (W) std::sqrt(somme / (n - 1));
                bRet = true;
            } // n
        } //
        return bRet;
    } // MeanStd

    template<typename T, typename W, class AllocT>
    bool MeanStd(const std::vector<T, AllocT> &data, W & vMean, W &vStd) {
        const size_t n = data.size();
        bool bRet = false;
        W somme = (W) 0;
        for (size_t i = 0; i < n; ++i) {
            somme = (W) (somme + data[i]);
        } // i
        if (n > 0) {
            const W m = (W) (somme / n);
            vMean = m;
            if (n > 1) {
                somme = (W) 0;
                for (size_t i = 0; i < n; ++i) {
                    W x = (W) (data[i] - m);
                    somme = (W) (somme + x * x);
                } // i
                vStd = (W) std::sqrt(somme / (n - 1));
                bRet = true;
            } // n
        } //
        return bRet;
    } // MeanStd
    ///////////////////////////////////////////////////////////////////

    template<typename T, typename W>
    bool Normalize_Array(size_t nRows, size_t nCols, const T *pdata, W *pDest,
            W* pWeights) {
        assert(nRows > 1);
        assert(nCols > 0);
        assert(pdata != nullptr);
        assert(pDest != nullptr);
        assert(pWeights != nullptr);
        //
        double den = (double) nRows - 1.0;
        double somvar = 0.0;
        bool bRet = true;
        for (size_t icol = 0; icol < nCols; ++icol) {
            double s1 = 0.0;
            for (size_t i = 0; i < nRows; ++i) {
                s1 += (double) pdata[i * nCols + icol];
            } // i
            const double moy = s1 / nRows;
            s1 = 0.0;
            for (size_t i = 0; i < nRows; ++i) {
                double x = (double) pdata[i * nCols + icol] - moy;
                s1 += x * x;
            } // i
            s1 /= den;
            const double vec = ::sqrt(s1);
            if (vec < 0.000001) {
                bRet = false;
            } else {
                double xa = 1.0 / vec;
                pWeights[icol] = (W) xa;
                somvar += xa;
                for (size_t i = 0; i < nRows; ++i) {
                    size_t gap = (size_t) (i * nCols + icol);
                    pDest[gap] = (W) ((pdata[gap] - moy) / vec);
                } // i
            }
        } // icol
        if (bRet) {
            assert(somvar > 0.0);
            for (size_t i = 0; i < nCols; ++i) {
                pWeights[i] = (W) (pWeights[i] / somvar);
            } // i
        }
        return bRet;
    } // Normalize_Array

    template<typename T, class AllocT, typename W, class AllocW>
    bool Normalize_Array(size_t nRows, size_t nCols,
    const std::vector<T, AllocT> &data, std::vector<W, AllocW> &oDest,
    std::vector<W, AllocW> &oWeights) {
        assert(nRows > 1);
        assert(nCols > 0);
        assert(data.size() >= (size_t) (nCols * nRows));
        //
        double den = (double) nRows - 1.0;
        oDest = std::vector<W, AllocW > (nCols * nRows);
        assert(oDest.size() == (size_t) (nCols * nRows));
        oWeights = std::vector<W, AllocW > (nCols);
        assert(oWeights.size() == nCols);
        double somvar = 0.0;
        bool bRet = true;
        for (size_t icol = 0; icol < nCols; ++icol) {
            double s1 = 0.0;
            for (size_t i = 0; i < nRows; ++i) {
                s1 += (double) data[i * nCols + icol];
            } // i
            const double moy = s1 / nRows;
            s1 = 0.0;
            for (size_t i = 0; i < nRows; ++i) {
                double x = (double) data[i * nCols + icol] - moy;
                s1 += x * x;
            } // i
            s1 /= den;
            const double vec = ::sqrt(s1);
            if (vec < 0.000001) {
                bRet = false;
            } else {
                double xa = 1.0 / vec;
                oWeights[icol] = (W) xa;
                somvar += xa;
                for (size_t i = 0; i < nRows; ++i) {
                    size_t gap = (size_t) (i * nCols + icol);
                    oDest[gap] = (W) ((data[gap] - moy) / vec);
                } // i
            }
        } // icol
        if (bRet) {
            assert(somvar > 0.0);
            for (size_t i = 0; i < nCols; ++i) {
                oWeights[i] = (W) (oWeights[i] / somvar);
            } // i
        }
        return bRet;
    } // Normalize_Array

    template<typename T, typename W>
    bool Normalize_Array(size_t nRows, size_t nCols,
            const std::valarray<T> &data, std::valarray<W> &oDest,
            std::valarray<W> &oWeights) {
        assert(nRows > 1);
        assert(nCols > 0);
        assert(data.size() >= (size_t) (nCols * nRows));
        //
        double den = (double) nRows - 1.0;
        oDest.resize(nCols * nRows);
        assert(oDest.size() == (size_t) (nCols * nRows));
        oWeights.resize(nCols);
        assert(oWeights.size() == nCols);
        double somvar = 0.0;
        bool bRet = true;
        for (size_t icol = 0; icol < nCols; ++icol) {
            double s1 = 0.0;
            for (size_t i = 0; i < nRows; ++i) {
                s1 += (double) data[i * nCols + icol];
            } // i
            const double moy = s1 / nRows;
            s1 = 0.0;
            for (size_t i = 0; i < nRows; ++i) {
                double x = (double) data[i * nCols + icol] - moy;
                s1 += x * x;
            } // i
            s1 /= den;
            const double vec = ::sqrt(s1);
            if (vec < 0.000001) {
                bRet = false;
            } else {
                double xa = 1.0 / vec;
                oWeights[icol] = (W) xa;
                somvar += xa;
                for (size_t i = 0; i < nRows; ++i) {
                    size_t gap = (size_t) (i * nCols + icol);
                    oDest[gap] = (W) ((data[gap] - moy) / vec);
                } // i
            }
        } // icol
        if (bRet) {
            assert(somvar > 0.0);
            for (size_t i = 0; i < nCols; ++i) {
                oWeights[i] = (W) (oWeights[i] / somvar);
            } // i
        }
        return bRet;
    } // Normalize_Array
    ////////////////////////////////////////////////////////////////

    template<typename T, typename W>
    bool Compute_Distances(size_t nRows, size_t nCols, const T *pdata, W *pDest,
            W *pRowDist, W *pColDist) {
        assert(nRows > 1);
        assert(nCols > 0);
        assert(pdata != nullptr);
        assert(pDest != nullptr);
        assert(pRowDist != nullptr);
        assert(pColDist != nullptr);
        //
        std::unique_ptr<W> oWeights(new W[nCols]);
        W *pWeights = oWeights.get();
        assert(pWeights != nullptr);
        if (!statdata::Normalize_Array(nRows, nCols, pdata, pDest, pWeights)) {
            return false;
        }
        for (size_t irow1 = 0; irow1 < nRows; ++irow1) {
            pRowDist[irow1 * nRows + irow1] = (W) 0;
            const size_t gap1 = (size_t) (irow1 * nCols);
            for (size_t irow2 = 0; irow2 < irow1; ++irow2) {
                const size_t gap2 = (size_t) (irow2 * nCols);
                double s = 0;
                for (size_t icol = 0; icol < nCols; ++icol) {
                    double x = (double) pDest[gap1 + icol]
                            - (double) pDest[gap2 + icol];
                    s += (double) pWeights[icol] * x * x;
                } // icol
                pRowDist[irow1 * nRows + irow2] = (W) s;
                pRowDist[irow2 * nRows + irow1] = (W) s;
            } // irow2
        } // irow1
        //
        for (size_t icol1 = 0; icol1 < nCols; ++icol1) {
            pColDist[icol1 * nCols + icol1] = (W) 0;
            for (size_t icol2 = 0; icol2 < icol1; ++icol2) {
                double s = 0;
                for (size_t irow = 0; irow < nRows; ++irow) {
                    size_t gap = (size_t) (irow * nCols);
                    double x = (double) pDest[gap + icol1]
                            - (double) pDest[gap + icol2];
                    s += x * x;
                } // irow
                pColDist[icol1 * nCols + icol2] = (W) s;
                pColDist[icol2 * nCols + icol1] = (W) s;
            } // icol2
        } // icol1
        return true;
    } // Compute_Distances

    template<typename T, class AllocT, typename W, class AllocW>
    bool Compute_Distances(size_t nRows, size_t nCols,
    const std::vector<T, AllocT> &data, std::vector<W, AllocW> &oDest,
    std::vector<W, AllocW> &oRowDist, std::vector<W, AllocW> &oColDist) {
        std::vector<W, AllocW> oWeights;
        if (!statdata::Normalize_Array(nRows, nCols, data, oDest, oWeights)) {
            return false;
        }
        oRowDist = std::vector<W, AllocW > (nRows * nRows);
        assert(oRowDist.size() == (size_t) (nRows * nRows));
        for (size_t irow1 = 0; irow1 < nRows; ++irow1) {
            oRowDist[irow1 * nRows + irow1] = (W) 0;
            const size_t gap1 = (size_t) (irow1 * nCols);
            for (size_t irow2 = 0; irow2 < irow1; ++irow2) {
                const size_t gap2 = (size_t) (irow2 * nCols);
                double s = 0;
                for (size_t icol = 0; icol < nCols; ++icol) {
                    double x = (double) oDest[gap1 + icol]
                            - (double) oDest[gap2 + icol];
                    s += (double) oWeights[icol] * x * x;
                } // icol
                oRowDist[irow1 * nRows + irow2] = (W) s;
                oRowDist[irow2 * nRows + irow1] = (W) s;
            } // irow2
        } // irow1
        //
        oColDist = std::vector<W, AllocW > (nCols * nCols);
        assert(oColDist.size() == (size_t) (nCols * nCols));
        for (size_t icol1 = 0; icol1 < nCols; ++icol1) {
            oColDist[icol1 * nCols + icol1] = (W) 0;
            for (size_t icol2 = 0; icol2 < icol1; ++icol2) {
                double s = 0;
                for (size_t irow = 0; irow < nRows; ++irow) {
                    size_t gap = (size_t) (irow * nCols);
                    double x = (double) oDest[gap + icol1]
                            - (double) oDest[gap + icol2];
                    s += x * x;
                } // irow
                oColDist[icol1 * nCols + icol2] = (W) s;
                oColDist[icol2 * nCols + icol1] = (W) s;
            } // icol2
        } // icol1
        return true;
    } // Compute_Distances

    template<typename T, typename W>
    bool Compute_Distances(size_t nRows, size_t nCols,
            const std::valarray<T> &data, std::valarray<W> &oDest,
            std::valarray<W> &oRowDist, std::valarray<W> &oColDist) {
        std::valarray<W> oWeights;
        if (!statdata::Normalize_Array(nRows, nCols, data, oDest, oWeights)) {
            return false;
        }
        oRowDist.resize(nRows * nRows);
        assert(oRowDist.size() == (size_t) (nRows * nRows));
        for (size_t irow1 = 0; irow1 < nRows; ++irow1) {
            oRowDist[irow1 * nRows + irow1] = (W) 0;
            const size_t gap1 = (size_t) (irow1 * nCols);
            for (size_t irow2 = 0; irow2 < irow1; ++irow2) {
                const size_t gap2 = (size_t) (irow2 * nCols);
                double s = 0;
                for (size_t icol = 0; icol < nCols; ++icol) {
                    double x = (double) oDest[gap1 + icol]
                            - (double) oDest[gap2 + icol];
                    s += (double) oWeights[icol] * x * x;
                } // icol
                oRowDist[irow1 * nRows + irow2] = (W) s;
                oRowDist[irow2 * nRows + irow1] = (W) s;
            } // irow2
        } // irow1
        //
        oColDist.resize(nCols * nCols);
        assert(oColDist.size() == (size_t) (nCols * nCols));
        for (size_t icol1 = 0; icol1 < nCols; ++icol1) {
            oColDist[icol1 * nCols + icol1] = (W) 0;
            for (size_t icol2 = 0; icol2 < icol1; ++icol2) {
                double s = 0;
                for (size_t irow = 0; irow < nRows; ++irow) {
                    size_t gap = (size_t) (irow * nCols);
                    double x = (double) oDest[gap + icol1]
                            - (double) oDest[gap + icol2];
                    s += x * x;
                } // irow
                oColDist[icol1 * nCols + icol2] = (W) s;
                oColDist[icol2 * nCols + icol1] = (W) s;
            } // icol2
        } // icol1
        return true;
    } // Compute_Distances
    ///////////////////////////////////////////////////////////

    template<typename T, typename Z>
    void Sort(size_t n, const T *pdata, Z *pindex) {
        assert(n > 1);
        assert(pdata != nullptr);
        assert(pindex != nullptr);
        typedef IndexedValue<T, Z> xPair;
        std::vector<xPair> vec(n);
        for (size_t i = 0; i < n; ++i) {
             vec[i] = xPair((Z) i, pdata[i]);
        } // i
        if (n > 1) {
            std::sort(vec.begin(), vec.end());
        } // n
        for (size_t i = 0; i < n; ++i) {
            const xPair &oPair = vec[i];
            Z val = oPair.index();
            pindex[i] = val;
        } // i
    } // sort

    template<typename T, typename Z>
    void Sort(const std::valarray<T> &data, std::valarray<Z> &index) {
        typedef IndexedValue<T, Z> xPair;
        const size_t n = data.size();
        std::vector<xPair> vec(n);
        for (size_t i = 0; i < n; ++i) {
            vec[i] = xPair((Z) i, data[i]);
        } // i
        if (n > 1) {
            std::sort(vec.begin(), vec.end());
        } // n
        index.resize(n);
        for (size_t i = 0; i < n; ++i) {
            index[i] = (vec[i]).index();
        } // i
    } // sort

    template<typename T, typename Z, class AllocT, class AllocZ>
    void Sort(const std::vector<T, AllocT> &data, std::vector<Z, AllocZ> &index) {
        typedef IndexedValue<T, Z> xPair;
        const size_t n = data.size();
        std::vector<xPair> vec(n);
        for (size_t i = 0; i < n; ++i) {
            vec[i] = xPair((Z) i, data[i]);
        } // i
        if (n > 1) {
            std::sort(vec.begin(), vec.end());
        } // n
        index = std::vector<Z, AllocZ > (n);
        for (size_t i = 0; i < n; ++i) {
            index[i] = (vec[i]).index();
        } // i
    } // sort
    ///////////////////////////////////////

    template<typename T>
    void Sort(size_t n, T *pdata) {
        assert(n > 1);
        assert(pdata != nullptr);
        std::unique_ptr<T> vec(new T[n]);
        T *pvec = vec.get();
        assert(pvec != nullptr);
        for (size_t i = 0; i < n; ++i) {
            pvec[i] = pdata[i];
        } // i
        if (n > 1) {
            std::sort(pvec, pvec + n);
        }
        for (size_t i = 0; i < n; ++i) {
            pdata[i] = pvec[i];
        } // i
    } // sort

    template<typename T>
    void Sort(std::valarray<T> &data) {
        const size_t n = data.size();
        std::vector<T> vec(n);
        for (size_t i = 0; i < n; ++i) {
            vec[i] = data[i];
        } // i
        if (n > 1) {
            std::sort(vec.begin(), vec.end());
        }
        for (size_t i = 0; i < n; ++i) {
            data[i] = vec[i];
        } // i
    } // sort

    template<typename T, class ALLOCT>
    void Sort(std::vector<T, ALLOCT> &data) {
        const size_t n = data.size();
        std::vector<T, ALLOCT> vec(n);
        for (size_t i = 0; i < n; ++i) {
            vec[i] = data[i];
        } // i
        if (n > 1) {
            std::sort(vec.begin(), vec.end());
        }
        for (size_t i = 0; i < n; ++i) {
            data[i] = vec[i];
        } // i
    } // sort
    /////////////////////////////////

    template<typename T, typename Z>
    void Rank(size_t n, const T *pdata, Z *pRank) {
        assert(n > 1);
        assert(pdata != nullptr);
        assert(pRank != nullptr);
        //
        std::unique_ptr<Z> indexes(new Z[n]);
        Z *pindexes = indexes.get();
        assert(pindexes != nullptr);
        
        statdata::Sort(n,pdata, pindexes);
        //
        T prevalue = 0;
        T curvalue = 0;
        Z nRank = 1;
        for (size_t i = 0; i < n; ++i) {
            Z k = pindexes[i];
            curvalue = pdata[k];
            if (i > 0) {
                if (curvalue > prevalue) {
                    ++nRank;
                }
            }
            pRank[k] = nRank;
            prevalue = curvalue;
        } // i
    } // rank

    template<typename T, typename Z>
    void Rank(const std::valarray<T> &data, std::valarray<Z> &oRank) {
        std::valarray<Z> indexes;
        Sort(data, indexes);
        const size_t n = data.size();
        assert(indexes.size() == n);
        oRank.resize(n);
        T prevalue = 0;
        T curvalue = 0;
        Z nRank = 1;
        for (size_t i = 0; i < n; ++i) {
            Z k = indexes[i];
            curvalue = data[k];
            if (i > 0) {
                if (curvalue > prevalue) {
                    ++nRank;
                }
            }
            oRank[k] = nRank;
            prevalue = curvalue;
        } // i
    } // rank

    template<typename T, typename Z, class AllocT, class AllocZ>
    void Rank(const std::vector<T, AllocT> &data, std::vector<Z, AllocZ> &oRank) {
        std::vector<Z, AllocZ> indexes;
        Sort(data, indexes);
        const size_t n = data.size();
        assert(indexes.size() == n);
        oRank = std::vector<Z, AllocZ > (n);
        T prevalue = 0;
        T curvalue = 0;
        Z nRank = 1;
        for (size_t i = 0; i < n; ++i) {
            Z k = indexes[i];
            curvalue = data[k];
            if (i > 0) {
                if (curvalue > prevalue) {
                    ++nRank;
                }
            }
            oRank[k] = nRank;
            prevalue = curvalue;
        } // i
    } // rank
    ////////////////////////////////////////////////

    template<typename T, typename Z>
    bool Recode(size_t n, const T *pSrc, Z *pDest, const Z rMax = (Z) 1000,
            const Z rMin = (Z) 0) {
        if (rMax <= rMin) {
            return false;
        }
        if (n < 2) {
            return false;
        }
        T xmin = 0;
        T xmax = 0;
        for (size_t i = 0; i < n; ++i) {
            T x = pSrc[i];
            if (i == 0) {
                xmax = x;
                xmin = x;
            } else if (x < xmin) {
                xmin = x;
            } else if (x > xmax) {
                xmax = x;
            }
        } // i
        if (xmin >= xmax) {
            return false;
        }
        assert(pDest != nullptr);
        double delta = ((double) rMax - rMin) / ((double) xmax - xmin);
        for (size_t i = 0; i < n; ++i) {
            double x = ((double) pSrc[i] - (double) xmin) * delta + (double) rMin;
            pDest[i] = (Z) x;
        } // i
        return true;
    } // Recode

    template<typename T, typename Z>
    bool Recode(const std::valarray<T> &oSrc, std::valarray<Z> &oDest,
            const Z rMax = (Z) 1000, const Z rMin = (Z) 0) {
        if (rMax <= rMin) {
            return false;
        }
        const size_t n = oSrc.size();
        if (n < 2) {
            return false;
        }
        T xmin = 0;
        T xmax = 0;
        for (size_t i = 0; i < n; ++i) {
            T x = oSrc[i];
            if (i == 0) {
                xmax = x;
                xmin = x;
            } else if (x < xmin) {
                xmin = x;
            } else if (x > xmax) {
                xmax = x;
            }
        } // i
        if (xmin >= xmax) {
            return false;
        }
        oDest.resize(n);
        double delta = ((double) rMax - rMin) / ((double) xmax - xmin);
        assert(oDest.size() == n);
        for (size_t i = 0; i < n; ++i) {
            double x = ((double) oSrc[i] - (double) xmin) * delta + (double) rMin;
            oDest[i] = (Z) x;
        } // i
        return true;
    } // Recode

    template<typename T, typename Z, class AllocT, class AllocZ>
    bool Recode(const std::vector<T, AllocT> &oSrc, std::vector<Z, AllocZ> &oDest,
    const Z rMax = (Z) 1000, const Z rMin = (Z) 0) {
        if (rMax <= rMin) {
            return false;
        }
        const size_t n = oSrc.size();
        if (n < 2) {
            return false;
        }
        T xmin = 0;
        T xmax = 0;
        for (size_t i = 0; i < n; ++i) {
            T x = oSrc[i];
            if (i == 0) {
                xmax = x;
                xmin = x;
            } else if (x < xmin) {
                xmin = x;
            } else if (x > xmax) {
                xmax = x;
            }
        } // i
        if (xmin >= xmax) {
            return false;
        }
        oDest = std::vector<Z, AllocZ > (n);
        double delta = ((double) rMax - rMin) / ((double) xmax - xmin);
        assert(oDest.size() == n);
        for (size_t i = 0; i < n; ++i) {
            double x = ((double) oSrc[i] - (double) xmin) * delta + (double) rMin;
            oDest[i] = (Z) x;
        } // i
        return true;
    } // Recode
    //////////////////////////////////////////

    template<typename T, typename Z, class AllocS>
    std::ostream & WriteIndexedArray(std::ostream &os, size_t nRows, size_t nCols,
    const std::valarray<T> &data,
    const std::vector<std::string, AllocS> &oIndNames,
    const std::vector<std::string, AllocS> &oVarNames,
    const std::valarray<Z> &oRowIndexes,
    const std::valarray<Z> &oVarIndexes) {
        os << "\"ITEM\"";
        for (size_t i = 0; i < nCols; ++i) {
            size_t jj = oVarIndexes[i];
            os << "\t\"" << oVarNames[jj] << "\"";
        } // i
        os << std::endl;
        for (size_t i = 0; i < nRows; ++i) {
            size_t ii = oRowIndexes[i];
            os << "\"" << oIndNames[ii] << "\"";
            for (size_t j = 0; j < nCols; ++j) {
                size_t jj = oVarIndexes[j];
                os << "\t" << data[ii * nCols + jj];
            } // j
            os << std::endl;
        } // i
        return os;
    } // WriteIndexedArray

    template<typename T, class AllocT, typename Z, class AllocZ, class AllocS>
    std::ostream & WriteIndexedArray(std::ostream &os, size_t nRows, size_t nCols,
    const std::vector<T, AllocT> &data,
    const std::vector<std::string, AllocS> &oIndNames,
    const std::vector<std::string, AllocS> &oVarNames,
    const std::vector<Z, AllocZ> &oRowIndexes,
    const std::vector<Z, AllocZ> &oVarIndexes) {
        os << "\"ITEM\"";
        for (size_t i = 0; i < nCols; ++i) {
            size_t jj = oVarIndexes[i];
            os << "\t\"" << oVarNames[jj] << "\"";
        } // i
        os << std::endl;
        for (size_t i = 0; i < nRows; ++i) {
            size_t ii = oRowIndexes[i];
            os << "\"" << oIndNames[ii] << "\"";
            for (size_t j = 0; j < nCols; ++j) {
                size_t jj = oVarIndexes[j];
                os << "\t" << data[ii * nCols + jj];
            } // j
            os << std::endl;
        } // i
        return os;
    } // WriteIndexedArray

    template<typename T, class AllocT, typename Z, class AllocZ, class AllocS>
    std::wostream & WriteIndexedArray(std::wostream &os, size_t nRows, size_t nCols,
    const std::vector<T, AllocT> &data,
    const std::vector<std::wstring, AllocS> &oIndNames,
    const std::vector<std::wstring, AllocS> &oVarNames,
    const std::vector<Z, AllocZ> &oRowIndexes,
    const std::vector<Z, AllocZ> &oVarIndexes) {
        os << L"\"ITEM\"";
        for (size_t i = 0; i < nCols; ++i) {
            size_t jj = oVarIndexes[i];
            os << L"\t\"" << oVarNames[jj] << L"\"";
        } // i
        os << std::endl;
        for (size_t i = 0; i < nRows; ++i) {
            size_t ii = oRowIndexes[i];
            os << L"\"" << oIndNames[ii] << L"\"";
            for (size_t j = 0; j < nCols; ++j) {
                size_t jj = oVarIndexes[j];
                os << L"\t" << data[ii * nCols + jj];
            } // j
            os << std::endl;
        } // i
        return os;
    } // WriteIndexedArray

    template<typename T, typename Z, class AllocS>
    std::wostream & WriteIndexedArray(std::wostream &os, size_t nRows, size_t nCols,
    const std::valarray<T> &data,
    const std::vector<std::wstring, AllocS> &oIndNames,
    const std::vector<std::wstring, AllocS> &oVarNames,
    const std::valarray<Z> &oRowIndexes,
    const std::valarray<Z> &oVarIndexes) {
        os << L"\"ITEM\"";
        for (size_t i = 0; i < nCols; ++i) {
            size_t jj = oVarIndexes[i];
            os << L"\t\"" << oVarNames[jj] << L"\"";
        } // i
        os << std::endl;
        for (size_t i = 0; i < nRows; ++i) {
            size_t ii = oRowIndexes[i];
            os << L"\"" << oIndNames[ii] << L"\"";
            for (size_t j = 0; j < nCols; ++j) {
                size_t jj = oVarIndexes[j];
                os << L"\t" << data[ii * nCols + jj];
            } // j
            os << std::endl;
        } // i
        return os;
    } // WriteIndexedArray
    //////////////////////////////////
} // namespace statdata
///////////////////////////////////////
#endif	/* GENROUTS_H */

