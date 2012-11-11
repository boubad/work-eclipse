#ifndef NO_GTEST
/////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////
#include <treeitem.h>
#include <csvreader.h>
#include <netcdfstatdataset.h>
///////////////////////////////////////////
#include <sstream>
#include <iostream>
#include <fstream>
//////////////////////////////
#include "../DataFixture.h"
////////////////////////////////
using namespace statdata;
/////////////////////////////

class TreeItemVisitorTest : public ::testing::Test {
public:
    typedef short IndexType;
    typedef float CriteriaType;
    //
    typedef statdata::TreeItem<CriteriaType, IndexType> TreeItemType;
    //
    typedef TreeItemType::IndexVectorType IndexVectorType;
    typedef TreeItemType::IndexArrayType IndexArrayType;
public:

    TreeItemVisitorTest() {
    }

    virtual ~TreeItemVisitorTest() {
    }
protected:

    static void TearDownTestCase() {

    }

    static void SetUpTestCase() {
        bool bExists = false;
        {
            std::ifstream in(m_ncfilename.c_str());
            if ((bExists = in.is_open()) == true) {
                in.close();
            }
        }
        if (!bExists) {
            NetCDFStatDataSetFile oFile;
            bool bRet = oFile.open(m_ncfilename, statdata::mode_replace);
            ASSERT_TRUE(bRet);
            std::map<std::string, Values > oMap;
            bRet = statdata::CSVReader::read_csv_file(m_sfilename, oMap);
            ASSERT_TRUE(bRet);
            std::string idsname(m_idsvarname);
            for (auto it = oMap.begin(); it != oMap.end(); ++it) {
                const std::string & varname = (*it).first;
                Values &values = (*it).second;
                bRet = oFile.write_indiv_data(varname, values);
                ASSERT_TRUE(bRet);
                if (varname == idsname) {
                    bRet = oFile.set_indivsids_variable(varname);
                    ASSERT_TRUE(bRet);
                }
            } // it
        } // filldata
        NetCDFStatDataSetFile oFile;
        bool bRet = oFile.open(m_ncfilename);
        ASSERT_TRUE(bRet);
        NetCDFStatDataSetFile *pFile = &oFile;
        std::string idsvarname(m_idsvarname);
        size_t nStart = 0, nCount = 0;
        bRet = pFile->read_indiv_data(idsvarname, m_names, nStart, nCount);
        ASSERT_TRUE(bRet);
        m_n = nCount;
        //
        std::string grpsvarname(m_grpvarname);
        nStart = 0;
        nCount = 0;
        bRet = pFile->read_indiv_data(grpsvarname, m_groupes, nStart, nCount);
        ASSERT_TRUE(bRet);
        //
        m_colorsmap.clear();
        size_t nColors = 0;
        while (TEST_COLORS_NAMES[nColors] != nullptr) {
            ++nColors;
        }
        size_t iCurrent = 0;
        for (size_t i = 0; i < nCount; ++i) {
            const std::string &grp = m_groupes[i];
            if (m_colorsmap.find(grp) == m_colorsmap.end()) {
                if (iCurrent >= nColors) {
                    iCurrent = 0;
                }
                const char *px = TEST_COLORS_NAMES[iCurrent];
                std::string scolor = px;
                m_colorsmap[grp] = scolor;
                iCurrent++;
            } // addColor
        } // i
        //
        NetCDFVariables vars;
        bRet = pFile->get_indivs_num_variables(vars);
        ASSERT_TRUE(bRet);
        const size_t nVars = vars.size();
        m_nvars = nVars;
        std::vector<std::string> oNumVars(nVars);
        for (size_t i = 0; i < nVars; ++i) {
            oNumVars[i] = (vars[i]).name;
        }
        std::map<std::string, std::map<size_t, boost::any> > oDict;
        bRet = pFile->get_variables_dictionary(oNumVars, oDict);
        ASSERT_TRUE(bRet);
        std::set<size_t> oSet;
        bRet = pFile->get_variables_not_missing_indexes(oNumVars, oSet);
        ASSERT_TRUE(bRet);
        const size_t nRows = oSet.size();
        m_n = nRows;
        std::vector<std::string> oCurnames(nRows);
        size_t jj = 0;
        for (auto it = oSet.begin(); it != oSet.end(); ++it) {
            size_t index = (size_t) (*it);
            oCurnames[jj++] = m_names[index];
        } // it
        const size_t nTotal = (size_t) (nVars * nRows);
        m_data = std::vector<float>(nTotal);
        for (size_t icol = 0; icol < nVars; ++icol) {
            const std::string & varname = oNumVars[icol];
            const std::map<size_t, boost::any> & vv = oDict[varname];
            size_t irow = 0;
            for (auto it = oSet.begin(); it != oSet.end(); ++it) {
                size_t index = (size_t) (*it);
                auto jt = vv.find(index);
                const boost::any &v = (*jt).second;
                float fval;
                ASSERT_TRUE(Value::get_value(v, fval));
                m_data[irow * nVars + icol] = fval;
                irow++;
            } // it
        } // icol
        m_distances = std::vector<float>(nRows * nRows);
        for (size_t i = 0; i < nRows; ++i) {
            m_distances[i * nRows + i] = 0;
            size_t igap = i * nVars;
            for (size_t j = 0; j < i; ++j) {
                size_t jgap = j * nVars;
                float sum = 0.0f;
                for (size_t k = 0; k < nVars; ++k) {
                    float x = (float) (m_data[igap + k]
                            - m_data[jgap + k]);
                    sum = (float) (sum + (x * x));
                } // k
                m_distances[i * nRows + j] = sum;
                m_distances[j * nRows + i] = sum;
            } // j
        } // i
    } // init


protected:
    static size_t m_n;
    static size_t m_nvars;
    static std::string m_grpvarname;
    static std::string m_idsvarname;
    static std::string m_sfilename;
    static std::string m_ncfilename;
    static std::string m_minfilename;
    static std::string m_meanfilename;
    static std::string m_maxfilename;
    static std::vector<float> m_data;
    static std::map<std::string, std::string> m_colorsmap;
    static std::vector<std::string> m_names;
    static std::vector<std::string> m_groupes;
    static std::vector<float> m_distances;
    static const char *TEST_COLORS_NAMES[];
};
size_t TreeItemVisitorTest::m_n = 0;
size_t TreeItemVisitorTest::m_nvars = 0;
std::string TreeItemVisitorTest::m_grpvarname("GR");
std::string TreeItemVisitorTest::m_idsvarname("NOM");
std::string TreeItemVisitorTest::m_sfilename("./data/Groupes_GTE1.tsv");
std::string TreeItemVisitorTest::m_ncfilename("./testresults/my_importdataset_1.nc");
std::string TreeItemVisitorTest::m_minfilename("./testresults/my_grp_min.dotg");
std::string TreeItemVisitorTest::m_meanfilename("./testresults/my_grp_mean.dotg");
std::string TreeItemVisitorTest::m_maxfilename("./testresults/my_grp_max.dotg");
std::vector<float> TreeItemVisitorTest::m_data;
std::map<std::string, std::string> TreeItemVisitorTest::m_colorsmap;
std::vector<std::string> TreeItemVisitorTest::m_names;
std::vector<std::string> TreeItemVisitorTest::m_groupes;
std::vector<float> TreeItemVisitorTest::m_distances;
const char *TreeItemVisitorTest::TEST_COLORS_NAMES[] = {"green", "cyan",
    "yellow", "orange", "red", nullptr};
///////////////////////////////////////

TEST_F(TreeItemVisitorTest, test_clusterize_min) {
    const size_t n = m_n;
    const std::vector<std::string> &names = m_names;
    const std::vector<float> &distances = m_distances;
    const std::vector<std::string> &groupes = m_groupes;
    const std::map<std::string, std::string> &colorsMap = m_colorsmap;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    TreeItemType::clusterize(n, distances, oTree, nClasses, linkmin);
    //
    std::ofstream out(m_minfilename.c_str());
    //
    out << "digraph G { " << std::endl;
    out << "size = \"4,4\";" << std::endl;
    //
    auto visitorFunc =
            [&](const TreeItemType & oTree){
        IndexType nIndex = oTree.id();
        if (oTree.is_leaf()) {
            const std::string & name = names[nIndex];
            std::string color("white");
            const std::string &grp = groupes[nIndex];
            std::string slabel = "[" + grp + "] " + name;
            auto jt = colorsMap.find(grp);
            if (jt != colorsMap.end()) {
                color = (*jt).second;
            }
            out << nIndex << " [shape = box,style=filled,color=" << color << ", label=\""
                    << slabel << "\"];" << std::endl;
        } else {
            out << nIndex << std::endl;
            size_t n = oTree.size();
            for (size_t i = 0; i < n; ++i) {
                const TreeItemType *p = oTree.child_at(i);
                out << nIndex << " -> " << p->id() << ";" << std::endl;
            } // i
        }
    };
    //
    oTree.visit(visitorFunc);
    out << "}" << std::endl;
} // test_clusterize_min

TEST_F(TreeItemVisitorTest, test_clusterize_mean) {
    const size_t n = m_n;
    const std::vector<std::string> &names = m_names;
    const std::vector<float> &distances = m_distances;
    const std::vector<std::string> &groupes = m_groupes;
    const std::map<std::string, std::string> &colorsMap = m_colorsmap;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    TreeItemType::clusterize(n, distances, oTree, nClasses, linkmean);
    //
    std::ofstream out(m_meanfilename.c_str());
    //
    out << "digraph G { " << std::endl;
    out << "size = \"4,4\";" << std::endl;
    //
    auto visitorFunc =
            [&](const TreeItemType & oTree){
        IndexType nIndex = oTree.id();
        if (oTree.is_leaf()) {
            const std::string & name = names[nIndex];
            std::string color("white");
            const std::string &grp = groupes[nIndex];
            std::string slabel = "[" + grp + "] " + name;
            auto jt = colorsMap.find(grp);
            if (jt != colorsMap.end()) {
                color = (*jt).second;
            }
            out << nIndex << " [shape = box,style=filled,color=" << color << ", label=\""
                    << slabel << "\"];" << std::endl;
        } else {
            out << nIndex << std::endl;
            size_t n = oTree.size();
            for (size_t i = 0; i < n; ++i) {
                const TreeItemType *p = oTree.child_at(i);
                out << nIndex << " -> " << p->id() << ";" << std::endl;
            } // i
        }
    };
    //
    oTree.visit(visitorFunc);
    out << "}" << std::endl;
} // test_clusterize_mean

TEST_F(TreeItemVisitorTest, test_clusterize_max) {
    const size_t n = m_n;
    const std::vector<std::string> &names = m_names;
    const std::vector<float> &distances = m_distances;
    const std::vector<std::string> &groupes = m_groupes;
    const std::map<std::string, std::string> &colorsMap = m_colorsmap;
    //
    TreeItemType oTree;
    const size_t nClasses = 1;
    TreeItemType::clusterize(n, distances, oTree, nClasses, linkmax);
    //
    std::ofstream out(m_maxfilename.c_str());
    //
    out << "digraph G { " << std::endl;
    out << "size = \"4,4\";" << std::endl;
    //
    auto visitorFunc =
            [&](const TreeItemType & oTree){
        IndexType nIndex = oTree.id();
        if (oTree.is_leaf()) {
            const std::string & name = names[nIndex];
            std::string color("white");
            const std::string &grp = groupes[nIndex];
            std::string slabel = "[" + grp + "] " + name;
            auto jt = colorsMap.find(grp);
            if (jt != colorsMap.end()) {
                color = (*jt).second;
            }
            out << nIndex << " [shape = box,style=filled,color=" << color << ", label=\""
                    << slabel << "\"];" << std::endl;
        } else {
            out << nIndex << std::endl;
            size_t n = oTree.size();
            for (size_t i = 0; i < n; ++i) {
                const TreeItemType *p = oTree.child_at(i);
                out << nIndex << " -> " << p->id() << ";" << std::endl;
            } // i
        }
    };
    //
    oTree.visit(visitorFunc);
    out << "}" << std::endl;
} // test_clusterize_max
#else

#endif // NO_GTEST

