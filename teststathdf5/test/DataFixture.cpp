/* 
 * File:   DataFixture.cpp
 * Author: boubad
 * 
 * Created on 4 novembre 2012, 13:07
 */

#include "DataFixture.h"
///////////////////////////
#include <cassert>
/////////////////////////
const char *DataFixture::TEST_CFORMAT[] =
{
    "%.4g","%.4g","%.4g","%.4g","%.4g","%.4g"
};
const char *DataFixture::TEST_VAR_INDNAMES = "Noms";
const char *DataFixture::TEST_VAR_LONGINDNAMES = "Noms des individus";
const char *DataFixture::TEST_LONGNAMES[] =
{
    "Moyenne Sem 1 UE1","Moyenne Sem 1 UE2","Moyenne Sem 1 UE3",
    "Moyenne Sem 2 UE1","Moyenne Sem 2 UE2","Moyenne Sem 2 UE3"
};
const char *DataFixture::TEST_UNITS[] = {"Note/20","Note/20","Note/20","Note/20","Note/20","Note/20"};
const char *DataFixture::TEST_VARNAMES[] = { "UE11", "UE12", "UE13", "UE21", "UE22", "UE23" };
const char *DataFixture::TEST_INDNAMES[] =
{
    "LHEUREUX",
    "ARCHERAY",
    "BLAISOT",
    "THUEUX",
    "SPIESSER",
    "PICHON",
    "LEPORCHER",
    "DEFRUTOS",
    "ARSALANE",
    "QUESNEL",
    "HOMO",
    "MONGOCK",
    "BERTHOU",
    "BALUEL",
    "LEGRAND",
    "KIMITETE",
    "PRETERRE",
    "TILLE",
    "HECKER",
    "HEDOUIN",
    "ARTUS",
    "LESOMPTIER",
    "GOSSE_J",
    "CIROU",
    "VASSARD",
    "FIGUER",
    "DEFRESNE",
    "LANGE",
    "BARIL",
    "CAMBOUNET",
    "LAHBIB",
    "SIGNOL",
    "BELLOUTI",
    "EVANNO",
    "BOURDAIS",
    "PITTE",
    "LALLEMAND",
    "DUBOIS",
    "BOUGUET",
    "RONGET",
    "NAGUEZ",
    "MALOU",
    "DUPONQ",
    "DEVAUX",
    "COCQUELIN",
    "MOROY",
    "BELMADANI",
    "ROGER",
    "LEGENDRE",
    "MARDARGENT",
    "CHERKAOUI",
    "LANDOULSI",
    "HAZET",
    "COURRIER",
    "LEDUC",
    "ASSELIN",
    "MAUBERT",
    "KADDOURA",
    "GROUT",
    "DOUYERE",
    "DHOUIB",
    "PRIEUX",
    "HENNEQUEZ",
    "GOMEZ",
    "QUEDEVILLE",
    "VINTAM",
    "LECOURS",
    "LERALLIC",
    "COLLEY",
    "VIRON",
    "JOLY",
    "GOSSE_S",
    "PINEL",
    "PAUMIER",
    "KASAWAT",
    "YOUSFI"
};
double DataFixture::TEST_DATA[] =
{
    15.1,14.9,15.6,13.6,15.7,18.3,
    14.5,14.9,15.5,14.1,15.0,15.8,
    14.6,	15.0,	15.4,	13.8,	13.3,	15.8,
    12.6,	14.5,	12.4,	13.3,	12.9,	14.9,
    12.8	,12.4,	15.1,	13.7,	14.1,	12.7,
    11.4,	10.7,	14.1,	13.7,	13.8,	13.0,
    11.5,	14.2,	14.0,	13.3,	13.0,	14.3,
    11.2,	15.9,	14.8,	12.0,	12.9,	14.6,
    12.9,	15.4,	13.4,	13.6,	11.8,	13.3,
    11.5,	10.3,	12.8,	10.7,	12.1,	17.0,
    12.1,	13.5,	13.9,	11.9,	12.3,	13.8,
    9.5,	9.2,	8.8,	11.0,	13.8,	13.0,
    13.8,	13.5,	13.2,	11.9,	12.5,	13.1,
    11.4,	10.6,	13.3,	12.3,	10.8,	14.1,
    10.7,	14.3,	13.7,	11.8,	12.0,	12.6,
    9.9,	11.2,	14.1,	12.0,	12.8,	11.3,
    9.4,	12.9,	13.2,	11.0,	12.3,	13.1,
    9.8,	10.8,	12.9,	11.2,	12.2,	12.7,
    11.7,	10.0,	12.3,	11.5,	10.6,	13.9,
    9.4,	13.6,	15.5,	10.6,	12.1,	13.3,
    8.0,	9.2,	10.8,	10.3,	11.7,	13.8,
    8.7,	11.0,	11.3,	11.8,	11.5,	11.8,
    10.1,	11.1,	13.8,	11.1,	11.7,	12.6,
    10.2,	10.3,	12.5,	10.6,	11.9,	12.9,
    10.7,	11.6,	13.8,	10.7,	11.6,	13.0,
    10.3,	11.9,	13.9,	10.1,	11.4,	14.1,
    10.8,	11.9,	13.1,	10.6,	11.4,	13.0,
    9.3,	13.0,	13.5,	9.9,	11.3,	13.5,
    10.1,	9.7,	12.1,	12.4,	11.0,	9.8,
    9.8,	10.1,	13.6,	10.8,	12.2,	10.7,
    9.0,	11.5,	13.5,	11.3,	10.3,	11.9,
    8.5,	10.8,	12.6,	9.4,	10.9,	13.3,
    9.1,	7.9,	9.5,	10.6,	10.3,	12.1,
    11.4,	11.1,	12.5,	11.5,	11.0,	9.6,
    9.6,	10.9,	13.3,	8.2,	11.7,	13.6,
    8.5,	13.2,	11.7,	10.5,	11.2,	10.8,
    9.8,	12.3,	12.0,	9.9,	10.4,	12.7,
    8.1,	10.9,	13.3,	8.2,	11.7,	13.6,
    10.5,	9.5,	12.1,	10.5,	10.7,	11.1,
    8.8,	10.2,	11.5,	9.1,	10.8,	13.0,
    8.1,	11.4,	13.2,	10.3,	11.6,	10.2,
    10.8,	11.6,	10.9,	9.9,	10.2,	11.6,
    9.3,	11.0,	12.5,	10.7,	10.1,	10.5,
    10.1,	10.8,	11.3,	9.8,	10.6,	11.0,
    9.8,	11.5,	11.5,	10.4,	10.4,	10.3,
    8.3,	11.7,	12.5,	10.7,	9.9,	10.0,
    11.2,	10.0,	11.6,	11.0,	9.2,	10.4,
    9.7,	11.5,	11.5,	9.6,	9.7,	11.6,
    9.2,	9.2,	13.5,	10.4,	8.9,	11.4,
    10.9,	9.5,	11.9,	9.6,	10.6,	10.2,
    7.8,	8.3,	13.3,	9.0,	10.6,	10.6,
    8.6,	10.0,	11.5,	9.3,	9.3,	10.6,
    9.6,	6.7,	11.0,	10.1,	8.2,	10.5,
    7.4,	10.1,	10.7,	9.0,	9.0,	10.6,
    7.3,	9.5,	10.9,	9.5,	8.0,	10.8,
    10.4,	10.0,	13.5,	9.3,	9.7,	9.0,
    9.4,	8.1,	12.4,	9.0,	9.8,	8.3,
    11.0,	7.2,	11.0,	10.1,	8.7,	7.4,
    10.6,	8.3,	12.1,	9.6,	7.5,	8.4,
    8.0,	6.9,	9.2,	8.5,	9.0,	8.0,
    9.1,	9.7,	10.3,	8.1,	8.6,	8.3,
    9.9,	10.9,	10.6,	8.4,	8.3,	7.2,
    9.4,	9.9,	12.7,	8.7,	7.2,	6.4,
    7.5,	5.7,	9.7,	7.3,	8.8,	5.9,
    5.5,	9.5,	10.3,	7.2,	7.0,	7.0,
    7.9,	9.8,	10.2,	6.9,	6.3,	8.0,
    6.5,	8.9,	9.4,	7.0,	5.3,	8.8,
    6.8,	6.0,	10.7,	6.8,	6.2,	5.2,
    7.6,	6.8,	8.3,	2.0,	2.6,	1.9,
    6.0,	6.7,	7.3,	2.7,	2.6,	0.4,
    5.4,	9.0,	9.8,	2.5,	0.7,	2.0,
    6.3,	5.9,	10.6,	1.4,	1.8,	1.3,
    11.1,	9.6,	11.1,	0.4,	0.6,	1.4,
    2.7,	4.0,	6.7,	0.0,	1.5,	0.0,
    6.4,	7.6,	10.2,	0.4,	0.0,	0.4,
    4.0,	5.8,	10.2,	0.0,	0.1,	0.0
};
DataFixture::DataFixture():m_ncols(0),m_nrows(0)
{
    m_ncols = sizeof(TEST_VARNAMES)/sizeof(const char *);
    assert(m_ncols > 0);
    m_nrows = sizeof(TEST_INDNAMES) / sizeof(const char *);
    assert(m_nrows > 0);
    size_t nTotal = sizeof(TEST_DATA) / sizeof(double);
    assert(nTotal >= (size_t)(m_ncols * m_nrows));
    //
    m_varindname = TEST_VAR_INDNAMES;
    m_longvarindname = TEST_VAR_LONGINDNAMES;
    m_longnames = std::vector<std::string>(m_ncols);
    m_units = std::vector<std::string>(m_ncols);
    m_cformats = std::vector<std::string>(m_ncols);
    for (size_t i = 0; i < m_ncols; ++i)
    {
        m_longnames[i] = std::string(TEST_LONGNAMES[i]);
        m_units[i] = std::string(TEST_UNITS[i]);
        m_cformats[i] = std::string(TEST_CFORMAT[i]);
    }// i
    //
    m_varnames = std::vector<std::string>(m_ncols);
    for (size_t i = 0; i < m_ncols; ++i)
    {
        m_varnames[i] = std::string(TEST_VARNAMES[i]);
    }// i
    m_indnames = std::vector<std::string>(m_nrows);
    m_initialdata = std::vector<float>(nTotal);
    for (size_t i = 0; i < m_nrows; ++i)
    {
        m_indnames[i] = std::string(TEST_INDNAMES[i]);
        for (size_t j = 0; j < m_ncols; ++j)
        {
            size_t k = i * m_ncols + j;
            m_initialdata[k] = (float)TEST_DATA[k];
        }// i
    }// i
    m_normdata = std::vector<float>(nTotal);
    for (size_t i = 0; i < m_ncols; ++i)
    {
        double fmin=0, fmax=0;
        for (size_t j = 0; j < m_nrows; ++j)
        {
            double x = TEST_DATA[j * m_ncols + i];
            if (j == 0)
            {
                fmin = x;
                fmax = x;
            }
            else if (x < fmin)
            {
                fmin = x;
            }
            else if (x > fmax)
            {
                fmax = x;
            }
        }// j
        assert(fmax > fmin);
        double delta = 1.0 / (fmax - fmin);
        for (size_t j = 0; j < m_nrows; ++j)
        {
            size_t k = j * m_ncols + i;
            m_normdata[k] = (float)((TEST_DATA[k] - fmin) * delta);
        }// j
    }// i
    m_coldist = std::vector<float>(m_ncols * m_ncols);
    for (size_t i = 0; i < m_ncols; ++i)
    {
        m_coldist[i * m_ncols + i] = 0;
        for (size_t j = 0; j < i; ++j)
        {
            float d = (float)0;
            for (size_t k = 0; k < m_nrows; ++k)
            {
                double x = m_normdata[k * m_ncols + i] - m_normdata[k * m_ncols + j];
                d = (float)(d + x * x);
            }// k
            m_coldist[i * m_ncols + j] = d;
            m_coldist[j * m_ncols + i] = d;
        }// j
    }// i
    m_rowdist = std::vector<float>(m_nrows * m_nrows);
    for (size_t i = 0; i < m_nrows; ++i)
    {
        m_rowdist[i * m_nrows + i] = 0;
        for (size_t j = 0; j < i; ++j)
        {
            float d = (float)0;
            for (size_t k = 0; k < m_ncols; ++k)
            {
                double x = m_normdata[i * m_ncols + k] - m_normdata[j * m_ncols + k];
                d = (float)(d + x * x);
            }// k
            m_rowdist[i * m_nrows + j] = d;
            m_rowdist[j * m_nrows + i] = d;
        }// j
    }// i
}// DataFixture


DataFixture::~DataFixture(void)
{
}
