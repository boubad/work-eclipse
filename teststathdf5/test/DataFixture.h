/* 
 * File:   DataFixture.h
 * Author: boubad
 *
 * Created on 4 novembre 2012, 13:07
 */

#ifndef DATAFIXTURE_H
#define	DATAFIXTURE_H
////////////////////////////////////
#include <string>
#include <vector>
#include <valarray>
#include <algorithm>
////////////////////////////////////
class DataFixture
{
public:
    DataFixture();
    virtual ~DataFixture();
    size_t cols(void) const
    {
        return this->m_ncols;
    }
    size_t rows(void) const
    {
        return this->m_nrows;
    }
    const std::vector<std::string> &colnames(void) const
    {
        return this->m_varnames;
    }
    const std::vector<std::string> &rownames(void) const
    {
        return this->m_indnames;
    }
    const std::vector<float> & initial_data(void) const
    {
        return this->m_initialdata;
    }
    const std::vector<float> & norm_data(void) const
    {
        return this->m_normdata;
    }
    const std::vector<float> & col_distances(void) const
    {
        return this->m_coldist;
    }
    const std::vector<float> & row_distances(void) const
    {
        return this->m_rowdist;
    }
    template <typename X, typename Y, class ALLOCX, class ALLOCY>
    static bool Recode(const std::vector<X,ALLOCX> &data,
                       std::vector<Y,ALLOCY> &oRes, Y rmax = (Y)1000, Y rmin = 0)
    {
        if (rmax <= rmin)
        {
            return false;
        }
        X vmin = *std::min_element(data.begin(),data.end());
        X vmax = *std::max_element(data.begin(),data.end());
        if (vmax <= vmin)
        {
            return false;
        }
        double delta = ((double)rmax - rmin) /(vmax - vmin);
        oRes = std::vector<Y,ALLOCY>(data.size());
        std::transform(data.begin(),data.end(),oRes.begin(),[&](X value)->Y
        {
            return (Y)((value - vmin)* delta + (double)rmin);
        });
        return true;
    }// Recode
    template <typename W>
    static void Recode(const std::vector<float> &data,std::valarray<W> &oRes)
    {
        size_t n = data.size();
        oRes.resize(n);
        for (size_t i = 0; i < n; ++i)
        {
            oRes[i] = (W)data[i];
        }// i
    }// Recode
    const std::vector<std::string> & col_longnames(void) const
    {
        return this->m_longnames;
    }
    const std::vector<std::string> & col_units(void) const
    {
        return this->m_units;
    }
    const std::vector<std::string> & col_cformat(void) const
    {
        return this->m_cformats;
    }
    const std::string & rowsnames_var(void) const
    {
        return this->m_varindname;
    }
    const std::string & rownames_varlongname(void) const
    {
        return this->m_longvarindname;
    }
private:
    size_t m_ncols;
    size_t m_nrows;
    std::vector<std::string> m_varnames;
    std::vector<std::string> m_indnames;
    std::vector<float> m_initialdata;
    std::vector<float> m_normdata;
    std::vector<float> m_coldist;
    std::vector<float> m_rowdist;
    std::vector<std::string> m_longnames;
    std::vector<std::string> m_units;
    std::vector<std::string> m_cformats;
    std::string m_varindname;
    std::string m_longvarindname;
public:
    static const char *TEST_VARNAMES[];
    static const char *TEST_INDNAMES[];
    static double TEST_DATA[];
    static const char *TEST_LONGNAMES[];
    static const char *TEST_UNITS[];
    static const char *TEST_VAR_INDNAMES;
    static const char *TEST_VAR_LONGINDNAMES;
    static const char *TEST_CFORMAT[];
};


#endif	/* DATAFIXTURE_H */

