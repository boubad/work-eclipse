/* 
 * File:   statdistance.h
 * Author: boubad
 *
 * Created on 9 novembre 2012, 12:09
 */

#ifndef STATDISTANCE_H
#define	STATDISTANCE_H
////////////////////////////////
#include "dataport.h"
#include "genrouts.h"
//////////////////////////////////////
#include <cassert>
#include <vector>
#include <valarray>
#include <memory>
//////////////////////////////////////////
namespace statdata {
    /////////////////////////////////////////

    typedef enum {
        q_squared,
        q_module,
        q_maximum,
        q_divergence,
        q_khi2,
        b_russel_rao,
        b_jaccard,
        b_dice,
        b_sokal_sneath_2,
        b_kulczinsky_1,
        b_kulczinsky_2,
        b_ochiai,
        b_simpson,
        b_kochen_wong,
        b_sokal_michener,
        b_sokal_sneath_1,
        b_rogers_tanimoto,
        b_sokal_sneath_3,
        b_sokal_sneath_4,
        b_sokal_sneath_5,
        b_roux_1,
        b_roux_2,
        b_hamann,
        b_yule,
        b_phi_de_pearson
    } DistanceType;
    //////////////////////////////////////////////////

    template<typename T = float, typename W = float>
            class ArrayDistFunc : public std::binary_function<size_t, size_t, W> {
    private:
        static std::valarray<T> m_def;
    protected:

        ArrayDistFunc() :
        m_nrows(0), m_ncols(0), m_type(q_squared), m_data(m_def) {
        }
    public:

        ArrayDistFunc(size_t nRows, size_t nCols, const std::valarray<T> &data,
                DistanceType type = q_squared) :
        m_nrows(nRows), m_ncols(nCols), m_type(type), m_data(data) {
            assert(this->m_nrows > 0);
            assert(this->m_ncols > 0);
            assert(this->m_data.size() >= (size_t) (this->m_nrows * this->m_ncols));
        }

        ArrayDistFunc(const ArrayDistFunc<T, W> &other) :
        m_nrows(other.m_nrows), m_ncols(other.m_ncols), m_type(
        other.m_type), m_data(other.m_data) {
        }

        ArrayDistFunc<T, W> & operator=(const ArrayDistFunc<T, W> &other) {
            if (this != &other) {
                this->m_nrows = other.m_nrows;
                this->m_ncols = other.m_ncols;
                this->m_type = other.m_type;
                this->m_data = other.m_data;
            }
            return *this;
        }

        virtual ~ArrayDistFunc() {
        }
    public:

        size_t rows(void) const {
            return this->m_nrows;
        }

        DistanceType type(void) const {
            return this->m_type;
        }

        virtual void type(DistanceType t) {
            this->m_type = t;
        }

        size_t cols(void) const {
            return this->m_ncols;
        }

        const std::valarray<T> & data(void) const {
            return this->m_data;
        }

        void get(size_t irow, std::valarray<T> &row) const {
            assert(irow < this->m_nrows);
            const size_t nv = this->m_ncols;
            assert(nv > 0);
            row = this->m_data[std::slice(irow * nv, nv, 1)];
        } // get

        W operator()(const size_t &i1, const size_t &i2) const {
            const size_t j1 = i1;
            const size_t j2 = i2;
            return (j1 == j2) ? (W) 0 : this->compute(j1, j2);
        } // operator()
    protected:
        virtual W compute(const size_t &i1, const size_t &i2) const = 0;
    protected:
        size_t m_nrows;
        size_t m_ncols;
        DistanceType m_type;
        const std::valarray<T> &m_data;
    protected:
        static const double EPSILON;
    };
    // class ArrayDistFunc<T,W>
    template<typename T, typename W>
            const double ArrayDistFunc<T, W>::EPSILON = 0.000001;
    template<typename T, typename W>
    std::valarray<T> ArrayDistFunc<T, W>::m_def;
    //////////////////////////////////////////////////

    template<typename T = float, class AllocT = std::allocator<T>, typename W = T >
            class VectorArrayDistFunc : public ArrayDistFunc<T, W> {
    public:

        VectorArrayDistFunc(size_t n, const std::vector<T, AllocT> &dist) : m_dist(dist) {
            assert(this->m_dist.size() >= (size_t) (n * n));
            this->m_nrows = n;
        }

        VectorArrayDistFunc(const VectorArrayDistFunc<T, AllocT, W> &other) : ArrayDistFunc<T, W>(other),
        m_dist(other.m_dist) {
        }

        VectorArrayDistFunc<T, AllocT, W> & operator=(const VectorArrayDistFunc<T, AllocT, W> &other) {
            if (this != &other) {
                ArrayDistFunc<T, W>::operator=(other);
                this->m_dist = other.m_dist;
            }
            return *this;
        }

        virtual ~VectorArrayDistFunc() {
        }
    protected:

        virtual W compute(const size_t &i1, const size_t &i2) const {
            return (W)this->m_dist[i1 * this->m_nrows + i2];
        }// compute
    private:
        const std::vector<T, AllocT> &m_dist;
    }; // class  VectorArrayDistFunc<T,AllocT,W>
    ///////////////////////////////////////////////////

    template<typename T = float, typename W = long>
            class ArrayAdapterDistFunc : public ArrayDistFunc<T, W> {
    public:

        template<typename X>
        ArrayAdapterDistFunc(const ArrayDistFunc<T, X> &func, W wmax = 1000,
                W wmin = 0) {
            const size_t n = func.rows();
            assert(n > 0);
            this->m_nrows = n;
            std::valarray<X> temp(n * n);
            for (size_t i = 0; i < n; ++i) {
                temp[i * n + i] = (X) 0;
                for (size_t j = 0; j < i; ++j) {
                    X vv = func(i, j);
                    temp[i * n + j] = vv;
                    temp[j * n + i] = vv;
                } // j
            } // i
            statdata::Recode(temp, this->m_dist, wmax, wmin);
            assert(this->m_dist.size() == (size_t) (n * n));
        }

        ArrayAdapterDistFunc(const ArrayAdapterDistFunc<T, W> &other) :
        ArrayDistFunc<T, W>(other), m_dist(other.m_dist) {
        }

        ArrayAdapterDistFunc<T, W> & operator=(
                const ArrayAdapterDistFunc<T, W> &other) {
            if (this != &other) {
                ArrayDistFunc<T, W>::operator=(other);
                this->m_dist = other.m_dist;
            }
            return *this;
        }

        virtual ~ArrayAdapterDistFunc() {
        }
    protected:

        virtual W compute(const size_t &i1, const size_t &i2) const {
            return this->m_dist[i1 * this->m_nrows + i2];
        } // compute
    protected:
        std::valarray<W> m_dist;
    };
    //////////////////////////////////////////////////

    template<typename T = float, typename W = float>
            class QuantArrayDistFunc : public ArrayDistFunc<T, W> {
    public:

        QuantArrayDistFunc(size_t nRows, size_t nCols, const std::valarray<T> &data,
                DistanceType type = q_squared) :
        ArrayDistFunc<T, W>(nRows, nCols, data, type) {
            this->adjust();
        }

        QuantArrayDistFunc(const QuantArrayDistFunc<T, W> &other) :
        ArrayDistFunc<T, W>(other), m_rowweights(other.m_rowweights), m_colweights(
        other.m_colweights) {
        }

        QuantArrayDistFunc<T, W> & operator=(
                const QuantArrayDistFunc<T, W> &other) {
            if (this != &other) {
                ArrayDistFunc<T, W>::operator=(other);
                this->m_rowweights = other.m_rowweights;
                this->m_colweights = other.m_colweights;
            }
            return *this;
        }

        virtual ~QuantArrayDistFunc() {
        }
    public:

        virtual void type(DistanceType t) {
            ArrayDistFunc<T, W>::type(t);
            this->adjust();
        }
    protected:

        virtual W compute(const size_t &i1, const size_t &i2) const {
            const size_t n = this->cols();
            assert(i1 < this->rows());
            assert(i2 < this->rows());
            assert(n > 0);
            W res = (W) 0;
            const std::valarray<T> &xdata = this->data();
            const size_t gap1 = i1 * n;
            const size_t gap2 = i2 * n;
            switch (this->m_type) {
                case q_khi2:
                {
                    const std::valarray<double> &vrow = this->m_rowweights;
                    const std::valarray<double> &vcol = this->m_colweights;
                    const double s1 = vrow[i1];
                    const double s2 = vrow[i2];
                    if ((s1 > 0.0) && (s2 > 0.0)) {
                        for (size_t i = 0; i < n; ++i) {
                            const double sj = vcol[i];
                            if (sj > 0.0) {
                                W x = (W) ((xdata[gap1 + i] / s1)
                                        - (xdata[gap2 + i] / s2));
                                double xx = (x * x) / sj;
                                if (std::abs(xx) > ArrayDistFunc<T, W>::EPSILON) {
                                    res = (W) (res + xx);
                                }
                            } // sj
                        } // i
                    } // s1 && s2
                }
                    break;
                case q_divergence:
                {
                    size_t nCur = 0;
                    for (size_t i = 0; i < n; ++i) {
                        const T x1 = xdata[gap1 + i];
                        const T x2 = xdata[gap2 + i];
                        double xx1 = std::abs((double) x1);
                        double xx2 = std::abs((double) x2);
                        double d = xx1 + xx2;
                        if (d > ArrayDistFunc<T, W>::EPSILON) {
                            double xx = std::abs((xx1 - xx2) / d);
                            res = (W) (res + xx);
                            ++nCur;
                        }
                    } // i
                    if (nCur != 0) {
                        res = (W) (res / nCur);
                    }
                }
                    break;
                case q_maximum:
                    for (size_t i = 0; i < n; ++i) {
                        W x = (W) std::abs(xdata[gap1 + i] - xdata[gap2 + i]);
                        if (x > res) {
                            res = x;
                        }
                    } // i
                    break;
                case q_module:
                    for (size_t i = 0; i < n; ++i) {
                        W x = (W) std::abs(xdata[gap1 + i] - xdata[gap2 + i]);
                        res = (W) (res + x);
                    } // i
                    break;
                default:
                    for (size_t i = 0; i < n; ++i) {
                        W x = (W) (xdata[gap1 + i] - xdata[gap2 + i]);
                        res = (W) (res + (x * x));
                    } // i
                    res = (W) (res / n);
                    break;
            } // type
            return res;
        } // compute
    private:

        void adjust(void) {
            if (this->m_type == q_khi2) {
                const size_t nCols = this->cols();
                const size_t nRows = this->cols();
                assert(nCols > 0);
                assert(nRows > 0);
                this->m_rowweights.resize(nRows);
                const std::valarray<T> & xdata = this->data();
                for (size_t i = 0; i < nRows; ++i) {
                    double s = 0.0;
                    for (size_t j = 0; j < nCols; ++j) {
                        s = (double) (s + xdata[i * nCols + j]);
                    } // j
                    this->m_rowweights[i] = s;
                } // i
                this->m_colweights.resize(nCols);
                for (size_t i = 0; i < nCols; ++i) {
                    double s = 0.0;
                    for (size_t j = 0; j < nRows; ++j) {
                        s = (double) (s + xdata[j * nCols + i]);
                    } // j
                    this->m_colweights[i] = s;
                } // i
            } // khi2
        } // adjust
    protected:
        std::valarray<double> m_rowweights;
        std::valarray<double> m_colweights;
    };
    // class QuantArrayDistFunc
    //////////////////////////////////////////

    template<typename T = float, typename W = float>
            class WeightQuantArrayDistFunc : public QuantArrayDistFunc<T, W> {
    public:

        template<typename Z>
        WeightQuantArrayDistFunc(size_t nRows, size_t nCols,
                const std::valarray<T> &data, const std::valarray<Z> &p,
                DistanceType type = q_squared) :
        QuantArrayDistFunc<T, W>(nRows, nCols, data, type) {
            const size_t n = p.size();
            assert(n > 0);
            double somme = 0;
            this->m_weights.resize(n);
            for (size_t i = 0; i < n; ++i) {
                double x = std::abs((double) p[i]);
                this->m_weights[i] = x;
                somme += x;
            } // i
            assert(somme > 0.0);
            for (size_t i = 0; i < n; ++i) {
                this->m_weights[i] = this->m_weights[i] / somme;
            } // i
        }

        WeightQuantArrayDistFunc(const WeightQuantArrayDistFunc<T, W> &other) :
        QuantArrayDistFunc<T, W>(other), m_weights(other.m_weights) {
        }

        WeightQuantArrayDistFunc<T, W> & operator=(
                const WeightQuantArrayDistFunc<T, W> &other) {
            if (this != &other) {
                QuantArrayDistFunc<T, W>::operator=(other);
                this->m_weights = other.m_weights;
            }
            return *this;
        }

        virtual ~WeightQuantArrayDistFunc() {
        }
    protected:

        virtual W compute(const size_t &i1, const size_t &i2) const {
            switch (this->m_type) {
                case q_khi2:
                    return QuantArrayDistFunc<T, W>::compute(i1, i2);
                default:
                    break;
            }// type
            const size_t n = this->cols();
            assert(i1 < this->rows());
            assert(i2 < this->rows());
            assert(n > 0);
            W res = (W) 0;
            const std::valarray<T> &xdata = this->data();
            const size_t gap1 = i1 * n;
            const size_t gap2 = i2 * n;
            const std::valarray<double> &ww = this->m_weights;
            switch (this->m_type) {
                case q_divergence:
                {
                    size_t nCur = 0;
                    for (size_t i = 0; i < n; ++i) {
                        const T x1 = xdata[gap1 + i];
                        const T x2 = xdata[gap2 + i];
                        double xx1 = std::abs((double) x1);
                        double xx2 = std::abs((double) x2);
                        double d = xx1 + xx2;
                        if (d > ArrayDistFunc<T, W>::EPSILON) {
                            double xx = std::abs((xx1 - xx2) / d);
                            res = (W) (res + xx * ww[i]);
                            ++nCur;
                        }
                    } // i
                    if (nCur != 0) {
                        res = (W) (res / nCur);
                    }
                }
                    break;
                case q_maximum:
                    for (size_t i = 0; i < n; ++i) {
                        double w = ww[i];
                        W x = (W) (w * std::abs(xdata[gap1 + i] - xdata[gap2 + i]));
                        if (x > res) {
                            res = x;
                        }
                    } // i
                    break;
                case q_module:
                    for (size_t i = 0; i < n; ++i) {
                        double w = ww[i];
                        W x = (W) (w * std::abs(xdata[gap1 + i] - xdata[gap2 + i]));
                        res = (W) (res + x);
                    } // i
                    break;
                default:
                    for (size_t i = 0; i < n; ++i) {
                        double w = ww[i];
                        W x = (W) (w * (xdata[gap1 + i] - xdata[gap2 + i]));
                        res = (W) (res + (x * x));
                    } // i
                    res = (W) (res / n);
                    break;
            } // type
            return res;
        } // compute
    private:
        std::valarray<double> m_weights;
    };
    // class WeightQuantArrayDistanceFunc<T,W>
    ////////////////////////////////////////

    template<typename W = float>
            class BoolArrayDistFunc : public ArrayDistFunc<bool, W> {
    public:

        BoolArrayDistFunc(size_t nRows, size_t nCols,
                const std::valarray<bool> &data, DistanceType type = b_russel_rao) :
        ArrayDistFunc<bool, W>(nRows, nCols, data, type) {
        }

        BoolArrayDistFunc(const BoolArrayDistFunc<W> &other) :
        ArrayDistFunc<bool, W>(other) {
        }

        BoolArrayDistFunc<W> & operator=(const BoolArrayDistFunc<W> &other) {
            if (this != &other) {
                ArrayDistFunc<bool, W>::operator=(other);
            }
            return *this;
        }

        virtual ~BoolArrayDistFunc() {
        }
    protected:

        virtual W compute(const size_t &i1, const size_t &i2) const {
            const size_t n = this->cols();
            assert(i1 < this->rows());
            assert(i2 < this->rows());
            assert(n > 0);
            W res = (W) 0;
            const std::valarray<bool> &xdata = this->data();
            const size_t gap1 = i1 * n;
            const size_t gap2 = i2 * n;
            size_t p = 0;
            size_t q = 0;
            size_t c = 0;
            for (size_t i = 0; i < n; ++i) {
                bool b1 = xdata[gap1 + i];
                bool b2 = xdata[gap2 + i];
                if (b1) {
                    ++p;
                }
                if (b2) {
                    ++q;
                }
                if (b1 && b2) {
                    ++c;
                }
            } // i
            size_t dd = n + c - (p + q);
            switch (this->m_type) {
                case b_russel_rao:
                    res = (W) ((double) c / n);
                    break;
                case b_jaccard:
                {
                    double d = (double) (p + q - c);
                    if (d != 0.0) {
                        res = (W) (c / d);
                    }
                }
                    break;
                case b_dice:
                {
                    double d = (double) (p + q);
                    if (d != 0.0) {
                        res = (W) (2.0 * c / d);
                    }
                }
                    break;
                case b_sokal_sneath_2:
                {
                    double d = (double) (2.0 * (p + q) - 3.0 * c);
                    if (d != 0.0) {
                        res = (W) (c / d);
                    }
                }
                    break;
                case b_kulczinsky_1:
                {
                    double d = (double) (p + q - 2.0 * c);
                    if (d != 0.0) {
                        res = (W) (c / d);
                    }
                }
                    break;
                case b_kulczinsky_2:
                    if ((p > 0) && (q > 0)) {
                        res = (W) (((double) c / (double) p + (double) c / (double) q)
                                / 2.0);
                    }
                    break;
                case b_ochiai:
                    if ((p > 0) && (q > 0)) {
                        res = (W) ((double) c / std::sqrt((double) p * q));
                    }
                    break;
                case b_simpson:
                {
                    size_t m = (p < q) ? p : q;
                    if (m != 0) {
                        res = (W) ((double) m);
                    }
                }
                    break;
                case b_kochen_wong:
                    if ((p > 0) && (q > 0)) {
                        res = (W) ((double) c * n / (double) (p * q));
                    }
                    break;
                case b_sokal_michener:
                    res = (W) (((double) c + dd) / n);
                    break;
                case b_sokal_sneath_1:
                    res = (W) (2.0 * (c + dd) / (n + c + dd));
                    break;
                case b_rogers_tanimoto:
                    res = (W) (1.0 * (c + dd) / (2.0 * n - (c + dd)));
                    break;
                case b_sokal_sneath_3:
                {
                    double d = (double) (p + q - 2.0 * c);
                    if (d != 0.0) {
                        res = (W) ((c + dd) / d);
                    }
                }
                    break;
                case b_sokal_sneath_4:
                {
                    if ((p > 0) && (q > 0) && (n != q)) {
                        double nn = ((double) c / p) + ((double) c / q)
                                + ((double) dd / (n - p)) + ((double) dd / (n - q));
                        res = (W) (nn / 4.0);
                    }
                }
                    break;
                case b_sokal_sneath_5:
                    res = (W) ((double) c * dd) / ((double) p * q * (n - p) * (n - q));
                    break;
                case b_roux_1:
                {
                    double d1 = (double) ((p < q) ? p : q);
                    double d2 = (double) (((n - p) < (n - q)) ? n - p : n - q);
                    if ((d1 + d2) != 0.0) {
                        res = (W) ((c + dd) / (d1 + d2));
                    }
                }
                    break;
                case b_roux_2:
                    res = (W) (((double) n - c * dd) / (p * q * (n - p) * (n - q)));
                    break;
                case b_hamann:
                    res = (W) ((((double) c + dd) - (p - c) - (q - c)) / n);
                    break;
                case b_yule:
                {
                    double nn = (double) c * dd - (p - c) * (q - c);
                    double dx = (double) c * dd + (p - c) * (q - c);
                    if (dx != 0.0) {
                        res = (W) (nn / dx);
                    }
                }
                    break;
                case b_phi_de_pearson:
                {
                    double nn = (double) c * dd - (p - c) * (q - c);
                    double dx = (double) p * q * (n - p) * (n - q);
                    if (dx != 0.0) {
                        res = (W) (nn / dx);
                    }
                }
                    break;
                default:
                    break;
            } // type
            return res;
        } // compute
    };
    // class BoolDistanceFunc<W>
    ////////////////////////////////////////
}// namespace statdata
//////////////////////////////////////////////


#endif	/* STATDISTANCE_H */

