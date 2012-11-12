/*
 * univarstatdesc.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef UNIVARSTATDESC_H_
#define UNIVARSTATDESC_H_
/////////////////////////////////
#include <cassert>
#include <cmath>
#include <string>
#include <map>
#include <valarray>
#include <memory>
/////////////////////////////
#include "dataport.h"
///////////////////////////////
namespace statdata {
    ////////////////////////////////////////

    class UnivarStatDesc {
    public:

        UnivarStatDesc() :
        m_n(0), m_min(0), m_max(0), m_mean(0), m_median(0), m_var(0), m_std(
        0), m_skew(0), m_flat(0), m_quant05(0), m_quant10(0), m_quant25(
        0), m_quant75(0), m_quant90(0), m_quant95(0) {
        }

        template<typename T>
        UnivarStatDesc(size_t n, const T *pData) :
        m_n(0), m_min(0), m_max(0), m_mean(0), m_median(0), m_var(0), m_std(
        0), m_skew(0), m_flat(0), m_quant05(0), m_quant10(0), m_quant25(
        0), m_quant75(0), m_quant90(0), m_quant95(0) {
            this->compute(n, pData);
        }

        template<typename T>
        UnivarStatDesc(const std::valarray<T> &odata) :
        m_n(0), m_min(0), m_max(0), m_mean(0), m_median(0), m_var(0), m_std(
        0), m_skew(0), m_flat(0), m_quant05(0), m_quant10(0), m_quant25(
        0), m_quant75(0), m_quant90(0), m_quant95(0) {
            this->compute(odata);
        }

        template<typename T, class ALLOCT>
        UnivarStatDesc(const std::vector<T, ALLOCT> &odata) :
        m_n(0), m_min(0), m_max(0), m_mean(0), m_median(0), m_var(0), m_std(
        0), m_skew(0), m_flat(0), m_quant05(0), m_quant10(0), m_quant25(
        0), m_quant75(0), m_quant90(0), m_quant95(0) {
            this->compute(odata);
        }

        UnivarStatDesc(const UnivarStatDesc &other) :
        m_n(other.m_n), m_min(other.m_min), m_max(other.m_max), m_mean(
        other.m_mean), m_median(other.m_median), m_var(other.m_var), m_std(
        other.m_std), m_skew(other.m_skew), m_flat(other.m_flat), m_quant05(
        other.m_quant05), m_quant10(other.m_quant10), m_quant25(
        other.m_quant25), m_quant75(other.m_quant75), m_quant90(
        other.m_quant90), m_quant95(other.m_quant95), m_name(
        other.m_name) {

        }

        UnivarStatDesc & operator=(const UnivarStatDesc &other) {
            if (this != &other) {
                this->m_n = other.m_n;
                this->m_min = other.m_min;
                this->m_max = other.m_max;
                this->m_mean = other.m_mean;
                this->m_median = other.m_median;
                this->m_var = other.m_var;
                this->m_std = other.m_std;
                this->m_skew = other.m_skew;
                this->m_flat = other.m_flat;
                this->m_quant05 = other.m_quant05;
                this->m_quant10 = other.m_quant10;
                this->m_quant25 = other.m_quant25;
                this->m_quant75 = other.m_quant75;
                this->m_quant90 = other.m_quant90;
                this->m_quant95 = other.m_quant95;
                this->m_name = other.m_name;
            }
            return *this;
        }

        virtual ~UnivarStatDesc() {
        }
    public:

        bool operator==(const UnivarStatDesc &other) const {
            return (this->m_name == other.m_name);
        }

        bool operator<(const UnivarStatDesc &other) const {
            return (this->m_name < other.m_name);
        }

        const std::string & key(void) const {
            return (this->m_name);
        }

        const std::string & operator()(void) const {
            return (this->m_name);
        }
    public:
        void clear(void);

        const std::string &name(void) const {
            return (this->m_name);
        }

        void name(const std::string &s) {
            this->m_name = s;
        }

        size_t size(void) const {
            return (this->m_n);
        }

        double min_value(void) const {
            return (this->m_min);
        }

        double max_value(void) const {
            return (this->m_max);
        }

        double mean(void) const {
            return (this->m_mean);
        }

        double median(void) const {
            return (this->m_median);
        }

        double variance(void) const {
            return (this->m_var);
        }

        double standard_deviation(void) const {
            return (this->m_std);
        }

        double skewness(void) const {
            return (this->m_skew);
        }

        double kurtosis(void) const {
            return (this->m_flat);
        }

        double quant05(void) const {
            return (this->m_quant05);
        }

        double quant10(void) const {
            return (this->m_quant10);
        }

        double quant25(void) const {
            return (this->m_quant25);
        }

        double quant75(void) const {
            return (this->m_quant75);
        }

        double quant90(void) const {
            return (this->m_quant90);
        }

        double quant95(void) const {
            return (this->m_quant95);
        }
    public:
        bool compute(const size_t n, const double *pData);

        template<typename T>
        bool compute(size_t n, const T *pData) {
            if ((n < 1) || (pData == nullptr)) {
                return (false);
            }
            std::unique_ptr<double> oTemp(new double[n]);
            double *p = oTemp.get();
            assert(p != nullptr);
            for (size_t i = 0; i < n; ++i) {
                p[i] = (double) pData[i];
            } // i
            bool bRet = this->compute(n, p);
            return bRet;
        } // compute

        template<typename T, class ALLOCT>
        bool compute(const std::vector<T, ALLOCT> &odata) {
            const size_t n = odata.size();
            assert(n > 0);
            std::unique_ptr<double> oTemp(new double[n]);
            double *p = oTemp.get();
            assert(p != nullptr);
            for (size_t i = 0; i < n; ++i) {
                p[i] = (double) odata[i];
            } // i
            bool bRet = this->compute(n, p);
            return bRet;
        } // compute

        template<typename T>
        bool compute(const std::valarray<T> &odata) {
            const size_t n = odata.size();
            assert(n > 0);
            std::unique_ptr<double> oTemp(new double[n]);
            double *p = oTemp.get();
            assert(p != nullptr);
            for (size_t i = 0; i < n; ++i) {
                p[i] = (double) odata[i];
            } // i
            bool bRet = this->compute(n, p);
            return bRet;
        } // compute
        bool mean_confidence_interval(double &vMin, double &vMax,
                double prob = 0.05);
    public:

        template<typename T, typename W>
        static bool compute_value(size_t n, const T *px, const T *py, T x, W &res) {
            assert(n > 1);
            assert(px != nullptr);
            assert(py != nullptr);
            if (x <= px[0]) {
                res = py[0];
                return (true);
            } else if (x >= px[n - 1]) {
                res = py[n - 1];
                return (true);
            }
            size_t nm1 = (size_t) (n - 1);
            for (size_t i = 0; i < n; ++i) {
                T xi = px[i];
                W yi = py[i];
                if (xi == x) {
                    res = yi;
                    return (true);
                } else if ((x < xi) && (i < nm1)) {
                    size_t i1 = (size_t)(i - 1);
                    size_t i2 = i;
                    double x1 = (double)px[i1];
                    double x2 = (double)px[i2];
                    double dx0 = x2 - x1;
                    if (dx0 <= 0.0){
                        return (false);
                    }
                    W y1 = py[i1];
                    W y2 = py[i2];
                    double dx = (double) (x - x1);
                    res = (W) (y1 + (dx / dx0) * (y2 - y1));
                    return (true);
                }
            } // i
            return (false);
        } // compute_value

        template<typename T, typename W, typename Z>
        static bool compute_value(size_t nx, size_t ny, const T *px, const W *py,
                const Z *pz, T x, W y, Z &res) {
            assert(nx > 1);
            assert(ny > 1);
            assert(px != nullptr);
            assert(py != nullptr);
            assert(pz != nullptr);
            //
            size_t nxm1 = (size_t) (nx - 1);
            size_t nym1 = (size_t) (ny - 1);
            size_t i1 = 0;
            size_t i2 = 0;
            if (x >= px[nxm1]) {
                i1 = nxm1;
                i2 = nxm1;
            } else if (x > px[0]) {
                for (size_t i = 0; i < nx; ++i) {
                    T xi = px[i];
                    if (x == xi) {
                        i1 = i;
                        i2 = i;
                        break;
                    } else if (x < xi) {
                        if (i > 0) {
                            i1 = (size_t) (i - 1);
                            i2 = i;
                        } else {
                            i1 = i;
                            i2 = i;
                        }
                        break;
                    }
                } // i
            }
            size_t j1 = 0;
            size_t j2 = 0;
            if (y >= py[nym1]) {
                j1 = nym1;
                j2 = nym1;
            } else if (y > py[0]) {
                for (size_t i = 0; i < ny; ++i) {
                    W yi = py[i];
                    if (y == yi) {
                        j1 = i;
                        j2 = i;
                        break;
                    } else if (y < yi) {
                        if (i > 0){
                            j1 = (size_t)(i - 1);
                            j2 = i;
                        } else {
                            j1 = i;
                            j2 = i;
                        }
                        break;
                    }
                } // i
            }
            if ((i1 == i2) && (j1 == j2)) {
                res = pz[i1 * ny + j1];
                return (true);
            } else if ((i1 == i2) && (j1 != j2)) {
                double y1 = (double) py[j1];
                double dy0 = (double) (py[j2] - y1);
                if (dy0 <= 0.0) {
                    return (false);
                }
                double z1 = (double) pz[i1 * ny + j1];
                double dz0 = (double) (pz[i1 * ny + j2] - z1);
                res = (Z) (z1 + (y - y1) * (dz0 / dy0));
                return (true);
            } else if ((i1 != i2) && (j1 == j2)) {
                double x1 = (double) px[i1];
                double dx0 = (double) (px[i2] - x1);
                if (dx0 <= 0.0) {
                    return (false);
                }
                double z1 = (double) pz[i1 * ny + j1];
                double dz0 = (double) (pz[i2 * ny + j1] - z1);
                res = (Z) (z1 + (x - x1) * (dz0 / dx0));
                return (true);
            } else {
                double x1 = (double) px[i1];
                double dx0 = (double) (px[i2] - px[i1]);
                if (dx0 <= 0.0) {
                    return (false);
                }
                double y1 = (double) py[j1];
                double dy0 = (double) (py[j2] - y1);
                if (dy0 <= 0.0) {
                    return (false);
                }
                double z1x = (double) pz[i1 * ny + j1];
                double dz0x = (double) (pz[i2 * ny + j1] - z1x);
                double r1 = (Z) (z1x + (x - x1) * (dz0x / dx0));
                z1x = (double) pz[i1 * ny + j2];
                dz0x = (double) (pz[i2 * ny + j2] - z1x);
                double r2 = (Z) (z1x + (x - x1) * (dz0x / dx0));
                double dz0 = r2 - r1;
                res = (Z) (r1 + (y - y1) * dz0 / dy0);
                return (true);
            }
        } // compute_value
        static bool get_vmax(size_t n, double &res, double alpha = 0.05);

        template<typename T>
        static bool must_reject(size_t n, const T *pData, T val,
                double alpha = 0.05) {
            assert(n > 1);
            assert(pData != nullptr);
            assert(alpha > 0.0);
            assert(alpha < 1.0);
            //
            double vcrit = 0;
            if (!UnivarStatDesc::get_vmax(n, vcrit, alpha)) {
                return (false);
            }
            double s = 0.0;
            for (size_t i = 0; i < n; ++i) {
                s += (double) pData[i];
            } // i
            const double xbarre = s / n;
            s = 0.0;
            for (size_t i = 0; i < n; ++i) {
                double x = (double) pData[i] - xbarre;
                s += x * x;
            }
            s /= (double) (n - 1);
            if (s <= 0.0) {
                return (false);
            }
            double vx = ((double) val - xbarre) / std::sqrt(s);
            if (vx < 0.0) {
                vx = -vx;
            }
            return (vx >= vcrit);
        } // must_reject

        template<typename T>
        static bool must_reject(std::valarray<T> &data, T val,
                double alpha = 0.05) {
            assert(alpha > 0.0);
            assert(alpha < 1.0);
            //
            size_t n = data.size();
            assert(n > 1);
            double vcrit = 0;
            if (!UnivarStatDesc::get_vmax(n, vcrit, alpha)) {
                return (false);
            }
            double s = 0.0;
            for (size_t i = 0; i < n; ++i) {
                s += (double) data[i];
            } // i
            const double xbarre = s / n;
            s = 0.0;
            for (size_t i = 0; i < n; ++i) {
                double x = (double) data[i] - xbarre;
                s += x * x;
            }
            s /= (double) (n - 1);
            if (s <= 0.0) {
                return (false);
            }
            double vx = ((double) val - xbarre) / std::sqrt(s);
            if (vx < 0.0) {
                vx = -vx;
            }
            return (vx >= vcrit);
        } // must_reject

        template<typename T, class ALLOCT>
        static bool must_reject(std::vector<T, ALLOCT> &data, T val, double alpha =
        0.05) {
            assert(alpha > 0.0);
            assert(alpha < 1.0);
            //
            size_t n = data.size();
            assert(n > 1);
            double vcrit = 0;
            if (!UnivarStatDesc::get_vmax(n, vcrit, alpha)) {
                return (false);
            }
            double s = 0.0;
            for (size_t i = 0; i < n; ++i) {
                s += (double) data[i];
            } // i
            const double xbarre = s / n;
            s = 0.0;
            for (size_t i = 0; i < n; ++i) {
                double x = (double) data[i] - xbarre;
                s += x * x;
            }
            s /= (double) (n - 1);
            if (s <= 0.0) {
                return (false);
            }
            double vx = ((double) val - xbarre) / std::sqrt(s);
            if (vx < 0.0) {
                vx = -vx;
            }
            return (vx >= vcrit);
        } // must_reject

        template<typename T, class ALLOCT, class ALLOCP>
        static void clear_values(const std::vector<T, ALLOCT> &initialData,
        std::map<size_t, T, std::less<size_t>, ALLOCP> &oMap, double alpha =
        0.05) {
            oMap.clear();
            const size_t n0 = initialData.size();
            bool bDone = false;
            while (!bDone) {
                bDone = true;
                size_t n = 0;
                double s = 0.0;
                T vmin = 0;
                T vmax = 0;
                for (size_t i = 0; i < n0; ++i) {
                    if (oMap.find(i) == oMap.end()) {
                        T x = initialData[i];
                        s += x;
                        ++n;
                        if (n == 1) {
                            vmin = x;
                            vmax = x;
                        } else if (x < vmin) {
                            vmin = x;
                        } else if (x > vmax) {
                            vmax = x;
                        }
                    } // ok
                } // i
                if ((n < 2) || (vmin >= vmax)) {
                    break;
                }
                const double xbarre = s / n;
                s = 0.0;
                for (size_t i = 0; i < n0; ++i) {
                    if (oMap.find(i) == oMap.end()) {
                        double x = (double) initialData[i] - xbarre;
                        s += x * x;
                    } // ok
                } // i
                s /= (double) (n - 1);
                if (s <= 0.0) {
                    break;
                }
                s = std::sqrt(s);
                double vcrit = 0.0;
                if (!UnivarStatDesc::get_vmax(n, vcrit, alpha)) {
                    break;
                }
                double crit = ((double) (vmin) - xbarre) / s;
                if (crit < 0.0) {
                    crit = -crit;
                }
                if (crit >= vcrit) {
                    bDone = false;
                    for (size_t i = 0; i < n0; ++i) {
                        if (oMap.find(i) == oMap.end()) {
                            if (initialData[i] == vmin) {
                                oMap[i] = vmin;
                            }
                        }
                    } // i
                } // crit
                crit = ((double) (vmax) - xbarre) / s;
                if (crit < 0.0) {
                    crit = -crit;
                }
                if (crit >= vcrit) {
                    bDone = false;
                    for (size_t i = 0; i < n0; ++i) {
                        if (oMap.find(i) == oMap.end()) {
                            if (initialData[i] == vmax) {
                                oMap[i] = vmax;
                            }
                        }
                    } // i
                } // crit
            } // not done
        } // clear_values

        template<typename T, class ALLOCP>
        static void clear_values(const std::valarray<T> &initialData,
        std::map<size_t, T, std::less<size_t>, ALLOCP> &oMap, double alpha =
        0.05) {
            oMap.clear();
            const size_t n0 = initialData.size();
            bool bDone = false;
            while (!bDone) {
                bDone = true;
                size_t n = 0;
                double s = 0.0;
                T vmin = 0;
                T vmax = 0;
                for (size_t i = 0; i < n0; ++i) {
                    if (oMap.find(i) == oMap.end()) {
                        T x = initialData[i];
                        s += x;
                        ++n;
                        if (n == 1) {
                            vmin = x;
                            vmax = x;
                        } else if (x < vmin) {
                            vmin = x;
                        } else if (x > vmax) {
                            vmax = x;
                        }
                    } // ok
                } // i
                if ((n < 2) || (vmin >= vmax)) {
                    break;
                }
                const double xbarre = s / n;
                s = 0.0;
                for (size_t i = 0; i < n0; ++i) {
                    if (oMap.find(i) == oMap.end()) {
                        double x = (double) initialData[i] - xbarre;
                        s += x * x;
                    } // ok
                } // i
                s /= (double) (n - 1);
                if (s <= 0.0) {
                    break;
                }
                s = std::sqrt(s);
                double vcrit = 0.0;
                if (!UnivarStatDesc::get_vmax(n, vcrit, alpha)) {
                    break;
                }
                double crit = ((double) (vmin) - xbarre) / s;
                if (crit < 0.0) {
                    crit = -crit;
                }
                if (crit >= vcrit) {
                    bDone = false;
                    for (size_t i = 0; i < n0; ++i) {
                        if (oMap.find(i) == oMap.end()) {
                            if (initialData[i] == vmin) {
                                oMap[i] = vmin;
                            }
                        }
                    } // i
                } // crit
                crit = ((double) (vmax) - xbarre) / s;
                if (crit < 0.0) {
                    crit = -crit;
                }
                if (crit >= vcrit) {
                    bDone = false;
                    for (size_t i = 0; i < n0; ++i) {
                        if (oMap.find(i) == oMap.end()) {
                            if (initialData[i] == vmax) {
                                oMap[i] = vmax;
                            }
                        }
                    } // i
                } // crit
            } // not done
        } // clear_values

        template<typename T, class ALLOCP>
        static void clear_values(size_t n0, const T *initialData,
        std::map<size_t, T, std::less<size_t>, ALLOCP> &oMap, double alpha =
        0.05) {
            assert(initialData != nullptr);
            oMap.clear();
            bool bDone = false;
            while (!bDone) {
                bDone = true;
                size_t n = 0;
                double s = 0.0;
                T vmin = 0;
                T vmax = 0;
                for (size_t i = 0; i < n0; ++i) {
                    if (oMap.find(i) == oMap.end()) {
                        T x = initialData[i];
                        s += x;
                        ++n;
                        if (n == 1) {
                            vmin = x;
                            vmax = x;
                        } else if (x < vmin) {
                            vmin = x;
                        } else if (x > vmax) {
                            vmax = x;
                        }
                    } // ok
                } // i
                if ((n < 2) || (vmin >= vmax)) {
                    break;
                }
                const double xbarre = s / n;
                s = 0.0;
                for (size_t i = 0; i < n0; ++i) {
                    if (oMap.find(i) == oMap.end()) {
                        double x = (double) initialData[i] - xbarre;
                        s += x * x;
                    } // ok
                } // i
                s /= (double) (n - 1);
                if (s <= 0.0) {
                    break;
                }
                s = std::sqrt(s);
                double vcrit = 0.0;
                if (!UnivarStatDesc::get_vmax(n, vcrit, alpha)) {
                    break;
                }
                double crit = ((double) (vmin) - xbarre) / s;
                if (crit < 0.0) {
                    crit = -crit;
                }
                if (crit >= vcrit) {
                    bDone = false;
                    for (size_t i = 0; i < n0; ++i) {
                        if (oMap.find(i) == oMap.end()) {
                            if (initialData[i] == vmin) {
                                oMap[i] = vmin;
                            }
                        }
                    } // i
                } // crit
                crit = ((double) (vmax) - xbarre) / s;
                if (crit < 0.0) {
                    crit = -crit;
                }
                if (crit >= vcrit) {
                    bDone = false;
                    for (size_t i = 0; i < n0; ++i) {
                        if (oMap.find(i) == oMap.end()) {
                            if (initialData[i] == vmax) {
                                oMap[i] = vmax;
                            }
                        }
                    } // i
                } // crit
            } // not done
        } // clear_values
    protected:
        void compute_quants(const size_t n, const double *pData);
        void compute_var(const size_t n, const double *pData);
        void compute_kurtosis(const size_t n, const double *pData);
    private:
        size_t m_n;
        double m_min;
        double m_max;
        double m_mean;
        double m_median;
        double m_var;
        double m_std;
        double m_skew;
        double m_flat;
        double m_quant05;
        double m_quant10;
        double m_quant25;
        double m_quant75;
        double m_quant90;
        double m_quant95;
        std::string m_name;
    private:
        static size_t TAB_XVALS[];
        static double TAB_YVALS[];
        static double TAB_ZVALS[];
    };
    // class UnivarStatDesc
    //////////////////////////////////////////
}// namespace statdata
///////////////////////////////////

#endif /* UNIVARSTATDESC_H_ */
