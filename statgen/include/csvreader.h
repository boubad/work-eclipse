/*
 * csvreader.h
 *
 *  Created on: 18 oct. 2012
 *      Author: boubad
 */

#ifndef CSVREADER_H_
#define CSVREADER_H_
//////////////////////////////
#include "value.h"
//////////////////////////////
#include <cassert>
#include <memory>
#include <string>
#include <fstream>
#include <cctype>
#include <limits>
#include <vector>
#include <set>
#include <list>
#include <map>
/////////////////////////////
#include <boost/algorithm/string.hpp>
//////////////////////////////
namespace statdata {
    ///////////////////////////////

    class CSVReader {
    public:

        static void convert_string(const std::string &s, boost::any &v) {
            v = boost::any();
            std::string ss = boost::trim_copy(s);
            boost::erase_all(ss, "\"");
            boost::erase_all(ss, " ");
            if (ss.empty()) {
                return;
            }
            std::string sx = boost::to_upper_copy(ss);
            if ((sx == "N/A") || (sx == "?") || (sx == "NAN") ||
                    (sx == "EMPTY")) {
                return;
            }
            bool bPoint = false;
            for (auto it = ss.begin(); it != ss.end(); ++it) {
                auto c = *it;
                if (std::isalpha(c)) {
                    if ((sx == "TRUE") || (sx == "VRAI") ||
                            (sx == "OUI")) {
                        v = true;
                        return;
                    } else if ((sx == "FALSE") || (sx == "FAUX") ||
                            (sx == "NON")) {
                        v = false;
                        return;
                    }
                    v = ss;
                    return;
                } else if (!std::isdigit(c)) {
                    if (c == '.') {
                        bPoint = true;
                    } else if ((c != '-') && (c != '+')) {
                        v = ss;
                        return;
                    }
                }
            } // it
            if (bPoint) {
                double fval = std::numeric_limits<double>::min();
                std::stringstream in(ss);
                in >> fval;
                v = fval;
            } else {
                int ival = std::numeric_limits<int>::min();
                std::stringstream in(ss);
                in >> ival;
                v = ival;
            }
        } // convert_string

        static void convert_string(const std::wstring &s, boost::any &v) {
            v = boost::any();
            std::wstring ss = boost::trim_copy(s);
            boost::erase_all(ss, L"\"");
            boost::erase_all(ss, L" ");
            if (ss.empty()) {
                return;
            }
            std::wstring sx = boost::to_upper_copy(ss);
            if ((sx == L"N/A") || (sx == L"?") || (sx == L"NAN") ||
                    (sx == L"EMPTY")) {
                return;
            }
            bool bPoint = false;
            for (auto it = ss.begin(); it != ss.end(); ++it) {
                auto c = *it;
                if (std::isalpha(c)) {
                    if ((sx == L"TRUE") || (sx == L"VRAI") ||
                            (sx == L"OUI")) {
                        v = true;
                        return;
                    } else if ((sx == L"FALSE") || (sx == L"FAUX") ||
                            (sx == L"NON")) {
                        v = false;
                        return;
                    }
                    v = ss;
                    return;
                } else if (!std::isdigit(c)) {
                    if (c == L'.') {
                        bPoint = true;
                    } else if ((c != L'-') && (c != L'+')) {
                        v = ss;
                        return;
                    }
                }
            } // it
            if (bPoint) {
                double fval = std::numeric_limits<double>::min();
                std::wstringstream in(ss);
                in >> fval;
                v = fval;
            } else {
                int ival = std::numeric_limits<int>::min();
                std::wstringstream in(ss);
                in >> ival;
                v = ival;
            }
        } // convert_string

        template<class ALLOCS, class ALLOCVEC>
        static bool read_csv_file(const std::string &filename,
        std::vector<std::vector<std::string, ALLOCS>, ALLOCVEC> &oArray) {
            oArray.clear();
            std::ifstream in(filename.c_str());
            if (!in.is_open()) {
                return (false);
            }
            const size_t T_BUF_SIZE = 4095;
            std::unique_ptr<char> oBuf(new char[T_BUF_SIZE + 1]);
            char *pBuf = oBuf.get();
            assert(pBuf != nullptr);
            size_t nCols = 0;
            while (!in.eof()) {
                pBuf[0] = (char) 0;
                in.getline(pBuf, T_BUF_SIZE);
                std::string s = pBuf;
                if (s.empty()) {
                    continue;
                }
                std::vector<std::string> vec;
                boost::split(vec, s, boost::is_any_of("\t ,;"));
                const size_t nx = vec.size();
                if (nCols == 0) {
                    nCols = nx;
                }
                size_t n = (nx < nCols) ? nx : nCols;
                std::vector<std::string, ALLOCS> oCur(n);
                for (size_t i = 0; i < n; ++i) {
                    std::string sx = vec[i];
                    boost::erase_all(sx, "\"");
                    boost::trim(sx);
                    boost::to_upper(sx);
                    if ((!sx.empty()) && (sx != "?") && (sx != "N/A")
                            && (sx != "EMPTY")) {
                        oCur[i] = sx;
                    }
                } // i
                oArray.push_back(oCur);
            } // in
            return (true);
        } // read_csv_file

        template<class ALLOCS, class ALLOCVEC>
        static bool read_csv_file(const std::wstring &filename,
        std::vector<std::vector<std::wstring, ALLOCS>, ALLOCVEC> &oArray) {
            oArray.clear();
            std::string sv(filename.length(), ' ');
            std::copy(filename.begin(), filename.end(), sv.begin());
            std::wifstream in(sv.c_str());
            if (!in.is_open()) {
                return (false);
            }
            const size_t T_BUF_SIZE = 4095;
            std::unique_ptr<wchar_t> oBuf(new wchar_t[T_BUF_SIZE + 1]);
            wchar_t *pBuf = oBuf.get();
            assert(pBuf != nullptr);
            size_t nCols = 0;
            while (!in.eof()) {
                pBuf[0] = (wchar_t) 0;
                in.getline(pBuf, T_BUF_SIZE);
                std::wstring s = pBuf;
                if (s.empty()) {
                    continue;
                }
                std::vector<std::wstring> vec;
                boost::split(vec, s, boost::is_any_of(L"\t ,;"));
                const size_t nx = vec.size();
                if (nCols == 0) {
                    nCols = nx;
                }
                size_t n = (nx < nCols) ? nx : nCols;
                std::vector<std::wstring, ALLOCS> oCur(n);
                for (size_t i = 0; i < n; ++i) {
                    std::wstring sx = vec[i];
                    boost::erase_all(sx, L"\"");
                    boost::trim(sx);
                    boost::to_upper(sx);
                    if ((!sx.empty()) && (sx != L"?") && (sx != L"N/A")
                            && (sx != L"EMPTY")) {
                        oCur[i] = sx;
                    }
                } // i
                oArray.push_back(oCur);
            } // in
            return (true);
        } // read_csv_file

        template<class ALLOCV, class ALLOCPAIR>
        static bool read_csv_file(const std::string &filename,
        std::map<std::string, std::vector<boost::any, ALLOCV>,
        std::less<std::string>, ALLOCPAIR> &oMap) {
            oMap.clear();
            std::vector<std::vector<std::string> > oVec;
            if (!CSVReader::read_csv_file(filename, oVec)) {
                return (false);
            }
            size_t nr = oVec.size();
            if (nr < 2) {
                return (false);
            }
            const std::vector<std::string> &varnames = oVec[0];
            const size_t nCols = varnames.size();
            if (nCols < 1) {
                return (false);
            }
            const size_t nRows = (size_t) (nr - 1);
            for (size_t icol = 0; icol < nCols; ++icol) {
                std::string varname = varnames[icol];
                if (varname.empty()) {
                    std::stringstream os;
                    os << "VAR" << (icol + 1);
                    varname = os.str();
                }
                std::vector<boost::any, ALLOCV> data(nRows);
                bool bFloat = false;
                bool bInt = false;
                bool bAlpha = false;
                for (size_t i = 1; i < nr; ++i) {
                    const std::vector<std::string> &vx = oVec[i];
                    const std::string &s = vx[icol];
                    boost::any v;
                    CSVReader::convert_string(s, v);
                    if (!v.empty()) {
                        const std::type_info &tt = v.type();
                        if (tt == typeid (std::string)) {
                            bAlpha = true;
                        } else if (tt == typeid (double)) {
                            bFloat = true;
                        } else if (tt == typeid (int)) {
                            bInt = true;
                        }
                    }
                    size_t irow = (size_t) (i - 1);
                    data[irow] = v;
                } // i
                if (bAlpha) {
                    for (size_t i = 0; i < nRows; ++i) {
                        const boost::any &v = data[i];
                        if (!v.empty()) {
                            const std::type_info &tt = v.type();
                            if (tt != typeid (std::string)) {
                                std::string s;
                                Value::get_value(v, s);
                                data[i] = s;
                            }
                        }
                    } // i
                } else if (bFloat) {
                    for (size_t i = 0; i < nRows; ++i) {
                        const boost::any &v = data[i];
                        if (!v.empty()) {
                            const std::type_info &tt = v.type();
                            if (tt != typeid (double)) {
                                double s;
                                Value::get_value(v, s);
                                data[i] = s;
                            }
                        }
                    } // i
                } else if (bInt) {
                    for (size_t i = 0; i < nRows; ++i) {
                        const boost::any &v = data[i];
                        if (!v.empty()) {
                            const std::type_info &tt = v.type();
                            if (tt != typeid (int)) {
                                int s;
                                Value::get_value(v, s);
                                data[i] = s;
                            }
                        }
                    } // i
                }
                oMap[varname] = data;
            } // icol
            return (true);
        } // read_csv_file

        template<class ALLOCV, class ALLOCPAIR>
        static bool read_csv_file(const std::wstring &filename,
        std::map<std::wstring, std::vector<boost::any, ALLOCV>,
        std::less<std::wstring>, ALLOCPAIR> &oMap) {
            oMap.clear();
            std::vector<std::vector<std::wstring> > oVec;
            if (!CSVReader::read_csv_file(filename, oVec)) {
                return (false);
            }
            size_t nr = oVec.size();
            if (nr < 2) {
                return (false);
            }
            const std::vector<std::wstring> &varnames = oVec[0];
            const size_t nCols = varnames.size();
            if (nCols < 1) {
                return (false);
            }
            const size_t nRows = (size_t) (nr - 1);
            for (size_t icol = 0; icol < nCols; ++icol) {
                std::wstring varname = varnames[icol];
                if (varname.empty()) {
                    std::wstringstream os;
                    os << L"VAR" << (icol + 1);
                    varname = os.str();
                }
                std::vector<boost::any, ALLOCV> data(nRows);
                bool bFloat = false;
                bool bInt = false;
                bool bAlpha = false;
                for (size_t i = 1; i < nr; ++i) {
                    const std::vector<std::wstring> &vx = oVec[i];
                    const std::wstring &s = vx[icol];
                    boost::any v;
                    CSVReader::convert_string(s, v);
                    if (!v.empty()) {
                        const std::type_info &tt = v.type();
                        if (tt == typeid (std::wstring)) {
                            bAlpha = true;
                        } else if (tt == typeid (double)) {
                            bFloat = true;
                        } else if (tt == typeid (int)) {
                            bInt = true;
                        }
                    }
                    size_t irow = (size_t) (i - 1);
                    data[irow] = v;
                } // i
                if (bAlpha) {
                    for (size_t i = 0; i < nRows; ++i) {
                        const boost::any &v = data[i];
                        if (!v.empty()) {
                            const std::type_info &tt = v.type();
                            if (tt != typeid (std::wstring)) {
                                std::wstring s;
                                Value::get_value(v, s);
                                data[i] = s;
                            }
                        }
                    } // i
                } else if (bFloat) {
                    for (size_t i = 0; i < nRows; ++i) {
                        const boost::any &v = data[i];
                        if (!v.empty()) {
                            const std::type_info &tt = v.type();
                            if (tt != typeid (double)) {
                                double s;
                                Value::get_value(v, s);
                                data[i] = s;
                            }
                        }
                    } // i
                } else if (bInt) {
                    for (size_t i = 0; i < nRows; ++i) {
                        const boost::any &v = data[i];
                        if (!v.empty()) {
                            const std::type_info &tt = v.type();
                            if (tt != typeid (int)) {
                                int s;
                                Value::get_value(v, s);
                                data[i] = s;
                            }
                        }
                    } // i
                }
                oMap[varname] = data;
            } // icol
            return (true);
        } // read_csv_file
    };
    // class CSVReadear
    /////////////////////////////////
}// namespace statdata
///////////////////////////////////

#endif /* CSVREADER_H_ */
