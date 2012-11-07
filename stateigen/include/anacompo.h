/*
 * anacompo.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef ANACOMPO_H_
#define ANACOMPO_H_
/////////////////////////////
#include "dataport.h"
////////////////////////
#include <cassert>
#include <vector>
#include <valarray>
#include <algorithm>
#include <cmath>
#include <memory>
///////////////////////////////////
#include "eigen.h"
////////////////////////////////////
namespace statdata {
    ///////////////////////////////////////////////

    template<typename T = double>
            class IntraEigenSolver {
    public:
        template<typename X, typename Z>
        static bool compute_eigen(const size_t nr,
                const X *pSrc, size_t &nFacts,Z *pa, Z *pv) {
            //
            assert(nr > 1);
            assert(pSrc != nullptr);
            assert(pa != nullptr);
            assert(pv != nullptr);
            //
            const size_t nn = (size_t) (nr * nr);
            //
            std::unique_ptr<double> xData(new double[nn]);
            double *pData = xData.get();
            assert(pData != nullptr);
            std::unique_ptr<double> xVals(new double[nr]);
            double *pVals = xVals.get();
            assert(pVals != nullptr);
            std::unique_ptr<double> xVecs(new double[nn]);
            double *pVecs = xVecs.get();
            assert(pVecs != nullptr);
            nFacts = 0;
            for (size_t i = 0; i < nr; ++i) {
                for (size_t j = 0; j <= i; ++j) {
                    const size_t k = (size_t) (i * nr + j);
                    double v = (double) pSrc[k];
                    pData[k] = v;
                    if (j != i) {
                        pData[j * nr + i] = v;
                    }
                }
            }
            if (!statdata::compute_eigen_impl(nr, pData, pVals, pVecs, &nFacts)) {
                return false;
            }
            for (size_t i = 0; i < nFacts; ++i) {
                pa[i] = (Z) pVals[i];
                for (size_t j = 0; j < nr; ++j) {
                    const size_t k = (size_t) (j * nFacts + i);
                    pv[k] = (Z) pVecs[k];
                }
            }
            return (true);
        }// compute_eigen
        template<typename X, typename Z>
        static bool compute_eigen(const size_t nr,
                const std::valarray<X> &oSrc, size_t &nFacts,
                std::valarray<Z> &oVals, std::valarray<Z> &oVecs) {
            //
            assert(nr > 1);
            const size_t nn = (size_t) (nr * nr);
            assert(oSrc.size() >= nn);
            //
            std::unique_ptr<double> xData(new double[nn]);
            double *pData = xData.get();
            assert(pData != nullptr);
            std::unique_ptr<double> xVals(new double[nr]);
            double *pVals = xVals.get();
            assert(pVals != nullptr);
            std::unique_ptr<double> xVecs(new double[nn]);
            double *pVecs = xVecs.get();
            assert(pVecs != nullptr);
            nFacts = 0;
            for (size_t i = 0; i < nr; ++i) {
                for (size_t j = 0; j <= i; ++j) {
                    const size_t k = (size_t) (i * nr + j);
                    double v = (double) oSrc[k];
                    pData[k] = v;
                    if (j != i) {
                        pData[j * nr + i] = v;
                    }
                }
            }
            if (!statdata::compute_eigen_impl(nr, pData, pVals, pVecs, &nFacts)) {
                return false;
            }
            oVals.resize(nFacts);
            oVecs.resize(nFacts * nr);
            for (size_t i = 0; i < nFacts; ++i) {
                oVals[i] = (Z) pVals[i];
                for (size_t j = 0; j < nr; ++j) {
                    const size_t k = (size_t) (j * nFacts + i);
                    oVecs[k] = (Z) pVecs[k];
                }
            }
            return (true);
        }// compute_eigen

        template<typename X, typename Z, class ALLOCX, class ALLOCZ>
        static bool compute_eigen(const size_t nr,
        const std::vector<X, ALLOCX> &oSrc, size_t &nFacts,
        std::vector<Z, ALLOCZ> &oVals, std::vector<Z, ALLOCZ> &oVecs) {
            //
            assert(nr > 1);
            const size_t nn = (size_t) (nr * nr);
            assert(oSrc.size() >= nn);
            //
            std::unique_ptr<double> xData(new double[nn]);
            double *pData = xData.get();
            assert(pData != nullptr);
            std::unique_ptr<double> xVals(new double[nr]);
            double *pVals = xVals.get();
            assert(pVals != nullptr);
            std::unique_ptr<double> xVecs(new double[nn]);
            double *pVecs = xVecs.get();
            assert(pVecs != nullptr);
            nFacts = 0;
            for (size_t i = 0; i < nr; ++i) {
                for (size_t j = 0; j <= i; ++j) {
                    const size_t k = (size_t) (i * nr + j);
                    double v = (double) oSrc[k];
                    pData[k] = v;
                    if (j != i) {
                        pData[j * nr + i] = v;
                    }
                }
            }
            if (!statdata::compute_eigen_impl(nr, pData, pVals, pVecs, &nFacts)) {
                return false;
            }
            oVals.resize(nFacts);
            oVecs.resize(nFacts * nr);
            for (size_t i = 0; i < nFacts; ++i) {
                oVals[i] = (Z)pVals[i];
                for (size_t j = 0; j < nr; ++j) {
                    const size_t k = (size_t) (j * nFacts + i);
                    oVecs[k] = (Z)pVecs[k];
                }
            }
            return (true);
        }

        template<typename X, class ALLOCX, class ALLOCT>
        static bool compute_anacompo(const size_t nr, const size_t nv,
        const std::vector<X, ALLOCX> &oSrc, std::vector<T, ALLOCT> &oMeans,
        std::vector<T, ALLOCT> &oStds, std::vector<T, ALLOCT> &oCorr,
        size_t &nFacts, std::vector<T, ALLOCT> &oFreq,
        std::vector<T, ALLOCT> &oVals, std::vector<T, ALLOCT> &oVecs,
        std::vector<T, ALLOCT> &oVars, std::vector<T, ALLOCT> &oInds) {
            //
            assert(nv > 1);
            assert(nr > nv);
            //
            const size_t nnv = nv * nv;
            const size_t nnr = nr * nv;
            //
            assert(oSrc.size() >= nnr);
            //
            oMeans = std::vector<T, ALLOCT > (nv);
            oStds = std::vector<T, ALLOCT > (nv);
            oCorr = std::vector<T, ALLOCT > (nnv);
            //
            std::vector<double> oTemp(nnr);
            std::vector<double> xCorr(nnv);
            std::vector<double> xStds(nv);
            bool bRet = true;
            double dnr = (double) nr;
            double dnr1 = (double) (nr - 1);
            for (size_t ivar = 0; ivar < nv; ++ivar) {
                xCorr[ivar * nv + ivar] = 1.0;
                oCorr[ivar * nv + ivar] = (T) 1.0;
                double s = 0.0;
                for (size_t i = 0; i < nr; ++i) {
                    const size_t k = i * nv + ivar;
                    double x = (double) oSrc[k];
                    oTemp[k] = x;
                    s += x;
                } // i
                double moy = s / dnr;
                oMeans[ivar] = (T) moy;
                s = 0.0;
                for (size_t i = 0; i < nr; ++i) {
                    const size_t k = i * nv + ivar;
                    double x = oTemp[k] - moy;
                    oTemp[k] = x;
                    s += x * x;
                } // i
                double dvar = s / dnr1;
                if (dvar > 0.0) {
                    double ec1 = std::sqrt(dvar);
                    xStds[ivar] = ec1;
                    oStds[ivar] = (T) ec1;
                    for (size_t ivar1 = 0; ivar1 < ivar; ++ivar1) {
                        double s = 0.0;
                        for (size_t i = 0; i < nr; ++i) {
                            double x = oTemp[i * nv + ivar1]
                                    * oTemp[i * nv + ivar1];
                            s += x;
                        }
                        s /= dnr1;
                        double r = s / (ec1 * xStds[ivar1]);
                        T rt = (T) r;
                        xCorr[ivar * nv + ivar1] = r;
                        oCorr[ivar * nv + ivar1] = rt;
                        xCorr[ivar1 * nv + ivar] = r;
                        oCorr[ivar1 * nv + ivar] = rt;
                    }
                } else {
                    bRet = false;
                }
            }
            if (bRet) {
                for (size_t ivar = 0; ivar < nv; ++ivar) {
                    double vec = xStds[ivar];
                    for (size_t i = 0; i < nr; ++i) {
                        const size_t k = i * nv + ivar;
                        double x = oTemp[k] / vec;
                        oTemp[k] = x;
                    }
                }
            }

            nFacts = 0;
            std::vector<double> xVals, xVecs;
            double stotal = 0.0;
            bRet = bRet
                    && IntraEigenSolver<T>::compute_eigen(nv, xCorr, nFacts, xVals,
                    xVecs);
            if (bRet) {
                oVals = std::vector<T, ALLOCT > (nFacts);
                oVecs = std::vector<T, ALLOCT > (nFacts * nv);
                for (size_t i = 0; i < nFacts; ++i) {
                    double x = xVals[i];
                    oVals[i] = (T) x;
                    stotal += x;
                }
                if (stotal <= 0) {
                    bRet = false;
                }
            }
            if (bRet) {
                oFreq = std::vector<T, ALLOCT > (nFacts);
                oVecs = std::vector<T, ALLOCT > (nFacts * nv);
                oVars = std::vector<T, ALLOCT > (nFacts * nv);
                oInds = std::vector<T, ALLOCT > (nFacts * nr);
                double act2 = std::sqrt((double) nv);
                for (size_t iFact = 0; iFact < nFacts; ++iFact) {
                    double val = xVals[iFact];
                    oFreq[iFact] = (T) (val / stotal);
                    double fval = std::sqrt(val);
                    for (size_t ivar = 0; ivar < nv; ++ivar) {
                        const size_t k = ivar * nFacts + iFact;
                        double vx = xVecs[k];
                        oVecs[k] = (T) vx;
                        oVars[k] = (T) (fval * xVecs[k]);
                    }
                    for (size_t irow = 0; irow < nr; ++irow) {
                        double s = 0.0;
                        for (size_t ivar = 0; ivar < nv; ++ivar) {
                            double x = oTemp[irow * nv + ivar]
                                    * xVecs[ivar * nFacts + iFact];
                            s += x;
                        }
                        oInds[irow * nFacts + iFact] = (T) (s / act2);
                    }
                }
            }
            return (bRet);
        }

        template<typename X, class ALLOCX, class ALLOCT>
        static bool compute_anacompo(const size_t nr, const size_t nv,
        const std::vector<X, ALLOCX> &oSrc, size_t &nFacts,
        std::vector<T, ALLOCT> &oVars, std::vector<T, ALLOCT> &oInds) {
            std::vector<T, ALLOCT> oMeans, oStds, oCorr, oFreq, oVals, oVecs;
            return (IntraEigenSolver<T>::compute_anacompo(nr, nv, oSrc, oMeans,
                    oStds, oCorr, nFacts, oFreq, oVals, oVecs, oVars, oInds));
        }
    };
    // class IntraEigenSolver<T>
    //////////////////////////////////////////
}
////////////////////////////////////////

#endif /* ANACOMPO_H_ */
